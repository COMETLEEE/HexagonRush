#include "pch.h"
#include "ResourceManager.h"
#include <fstream>

// Resources Header Files
#include "Texture.h"
#include "Mesh.h"
#include "Font.h"

#include "ShaderManager.h"

#include "ComputeShader.h"

std::shared_ptr<GraphicsEngine::ResourceManager> GraphicsEngine::ResourceManager::_resourceManager = nullptr;

namespace GraphicsEngine
{
	std::shared_ptr<ResourceManager> ResourceManager::Get()
	{
		if (_resourceManager == nullptr)
			_resourceManager = std::make_shared<GraphicsEngine::ResourceManager>();

		return _resourceManager;
	}

	void ResourceManager::Initialize()
	{
		CreateBasicMeshes();
	}

	void ResourceManager::Update()
	{
		// 모든 리소스를 순회하며 시간을 재면서 .. Ref Count가 0인 녀석들의 시간을 올려준다.
		// 만약 정해둔 지나면 메모리 상에서 내린다.
	}

	void ResourceManager::Release()
	{
		for (auto& texture : _textures)
			if (texture.second != nullptr)
				texture.second->Release();

		for (auto& mesh : _meshes)
			if (mesh.second != nullptr)
				mesh.second->Release();

		for (auto& font : _fonts)
			if (font.second != nullptr)
				font.second->Release();

		for (auto& iblSources : _IBLSourceForEnvMap)
			iblSources.second->Release();

	}

	const uint64& ResourceManager::LoadTexture(const tstring& textureName, const tstring& path)
	{
		std::shared_ptr<Texture> newTex = std::make_shared<Texture>();

		newTex->Initialize(textureName, path);

		_textures.insert({ newTex->GetID(), newTex });

		return newTex->GetID();
	}

	void ResourceManager::CreateIBLSource(const uint64& envMapID, bool fullBuild,
		const uint64& HDRIMapID)
	{
		// Create Specular Pre-Filtered Map, Specular BRDF LUT, Irradiance Map
		std::shared_ptr<Texture> envMap = GetTexture(envMapID);

		// For UAV UnBind
		ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		std::shared_ptr<Texture> specularPreMap; 

		// 나의 실력의 부재로 인한 .. Full Build 따로 놔둔다.
		if (fullBuild)
		{
			// deviceContext->GenerateMips(envMap->GetSRV().Get());

			// HDRI니까 .. 뽑을 수 있음
			// specularPreMap = envMap;

			// Compute Shader
			specularPreMap = Texture::CreateEmptyTextureCube(envMap->_width, envMap->_height, envMap->_format);

			{
				std::shared_ptr<ComputeShader> spmapCom = ShaderManager::Get()->GetCS(TEXT("CS_IBL_Specular_Pre_Filtered"));

				// Copy 0th mipmap level into Dest Env Map
				for (int i = 0; i < 6; i++)
				{
					uint32 subresourceIndex = D3D11CalcSubresource(0, i, specularPreMap->_levels);

					deviceContext->CopySubresourceRegion(specularPreMap->_texture2D.Get(), subresourceIndex, 0, 0, 0,
						envMap->_texture2D.Get(), subresourceIndex, nullptr);
				}

				deviceContext->GenerateMips(specularPreMap->GetSRV().Get());

				// Pre-Filter Rest of the Mip Chain
				const float deltaRoughness = 1.f / std::max<float>(float(specularPreMap->_levels - 1), 1.f);

				for (uint32 level = 1, size = (envMap->_width  / 2) ; level < specularPreMap->_levels; level++, size /= 2)
				{
					const uint32 numGroups = std::max<uint32>(1, size / 32);

					specularPreMap->CreateTextureUAV(level);

					CB_SPECMAPFILTER spMapConst = { level * deltaRoughness };

					spmapCom->UpdateConstantBuffer(&spMapConst, CBV_REGISTER::b1);
					spmapCom->UpdateShaderResource(envMap->GetSRV().Get(), SRV_REGISTER::t0);

					deviceContext->CSSetUnorderedAccessViews(0, 1, specularPreMap->GetUAV().GetAddressOf(), nullptr);

					spmapCom->Update();

					deviceContext->Dispatch(numGroups, numGroups, 6);

					specularPreMap->_unorderedAccessView.ReleaseAndGetAddressOf();
				}
			}
		}
		else
			// 이거 언 오더드 엑세스 뷰 문제 때문에 안 되는 경우가 많으니 .. 픽셀 쉐이더로라도 뽑아줘야함.
			specularPreMap = envMap;
	

		std::shared_ptr<Texture> irradianceMap = Texture::CreateEmptyTextureCube(32, 32, envMap->_format, D3D11_BIND_UNORDERED_ACCESS, 1);

		irradianceMap->CreateTextureUAV(0);

		// Compute diffuse irradiance cubemap.
		{
			std::shared_ptr<ComputeShader> irradianceCom = ShaderManager::Get()->GetCS(TEXT("CS_IBL_Irradiance"));

			irradianceCom->UpdateShaderResource(specularPreMap->GetSRV().Get(), SRV_REGISTER::t0);
			deviceContext->CSSetUnorderedAccessViews(0, 1, irradianceMap->GetUAV().GetAddressOf(), nullptr);
			irradianceCom->Update();

			deviceContext->Dispatch(irradianceMap->_width / 32, irradianceMap->_height / 32, 6);

			deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		}

		std::shared_ptr<Texture> brdf2DLUT = Texture::CreateEmptyTexture2D(256, 256, DXGI_FORMAT_R16G16_FLOAT, D3D11_BIND_UNORDERED_ACCESS, 1);

		brdf2DLUT->CreateTextureUAV(0);

		// Compute Cook-Torrance BRDF 2D LUT for split-sum approximation.
		{
			std::shared_ptr<ComputeShader> brdf2DLUTCom = ShaderManager::Get()->GetCS(TEXT("CS_IBL_Specular_BRDF_LUT"));

			deviceContext->CSSetUnorderedAccessViews(0, 1, brdf2DLUT->GetUAV().GetAddressOf(), nullptr);
			brdf2DLUTCom->Update();

			deviceContext->Dispatch(brdf2DLUT->_width / 32, brdf2DLUT->_height / 32, 1);

			deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		}

		// IBL Source들을 정해진 순서대로 넣는다 !
		std::shared_ptr<IBLSource> iblSource = std::make_shared<IBLSource>();

		iblSource->Initialize(irradianceMap, specularPreMap, brdf2DLUT);

		if (HDRIMapID != ULLONG_MAX)
		{
			_IBLSourceForEnvMap.insert({ HDRIMapID, iblSource });
		}
		else
		{
			// 해당 환경맵이 가지는 IBL Source들의 묶음을 저장한다.
			_IBLSourceForEnvMap.insert({ envMap->GetID(), iblSource });
		}
	}

	void ResourceManager::RegisterTexture(std::shared_ptr<Texture> texture)
	{
		_textures.insert({ texture->GetID(), texture });
	}

	const uint64& ResourceManager::LoadFont(const tstring& fontName, const tstring& path)
	{
		std::shared_ptr<Font> newFont = std::make_shared<Font>();

		newFont->Initialize(fontName, path);

		_fonts.insert({ newFont->GetID(), newFont });

		return newFont->GetID();
	}

	std::shared_ptr<Texture> ResourceManager::GetTexture(const uint64& resourceID)
	{
		return _textures.find(resourceID) != _textures.end() ? _textures.at(resourceID) : nullptr;
	}

	std::shared_ptr<Mesh> ResourceManager::GetMesh(const uint64& resourceID)
	{
		return _meshes.find(resourceID) != _meshes.end() ? _meshes.at(resourceID) : nullptr;
	}

	std::shared_ptr<GraphicsEngine::Font> ResourceManager::GetFont(const uint64& resourceID)
	{
		return _fonts.find(resourceID) != _fonts.end() ? _fonts.at(resourceID) : nullptr;
	}

	const uint64 ResourceManager::GetIDByResourceName(const tstring& name)
	{
		for (auto& texture : _textures)
			if (texture.second->GetName() == name)
				return texture.second->GetID();

		for (auto& mesh : _meshes)
			if (mesh.second->GetName() == name)
				return mesh.second->GetID();

		for (auto& font : _fonts)
			if (font.second->GetName() == name)
				return font.second->GetID();

		return ULLONG_MAX;
	}

	void ResourceManager::ResetTexture()
	{
		for (auto& texture : _textures)
			if (texture.second != nullptr)
				texture.second->Release();
		for (auto& texture : _IBLSourceForEnvMap)
			if (texture.second != nullptr)
				texture.second->Release();

		_textures;
		_meshes;
		_fonts;
		_IBLSourceForEnvMap;

		_textures.clear();
		_IBLSourceForEnvMap.clear();



	}

	void ResourceManager::CreateBasicMeshes()
	{
		CreateStaticQuadMesh();			// 0번
		
		CreateStaticDebugMesh();		// 1번

		CreateDynamicQuadMesh();		// 2번
	}

	void ResourceManager::CreateStaticQuadMesh()
	{
		std::vector<StaticMeshVertex> vertices(6);
		
		// 좌 하단
		vertices[0]._position = Vector3(-1.f, -1.f, 0.f);
		vertices[0]._uv = Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1]._position = Vector3(-1.f, 1.f, 0.f);
		vertices[1]._uv = Vector2(0.f, 0.f);

		// 우 상단
		vertices[2]._position = Vector3(1.f, 1.f, 0.f);
		vertices[2]._uv = Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3]._position = Vector3(-1.f, -1.f, 0.f);
		vertices[3]._uv = Vector2(0.f, 1.f);

		// 우 상단
		vertices[4]._position = Vector3(1.f, 1.f, 0.f);
		vertices[4]._uv = Vector2(1.f, 0.f);

		// 우 하단
		vertices[5]._position = Vector3(1.f, -1.f, 0.f);
		vertices[5]._uv = Vector2(1.f, 1.f);

		std::vector<uint32> indices(6);

		for (int i = 0; i < 6; i++)
			indices[i] = i;

		std::shared_ptr<Mesh> quad = std::make_shared<Mesh>();

		quad->Initialize(TEXT("Static_Quad"), vertices, indices);

		_meshes.insert({ quad->GetID(), quad });
	}

	void ResourceManager::CreateStaticDebugMesh()
	{
		std::vector<StaticMeshVertex> vertices(6);

		// 좌 하단
		vertices[0]._position = Vector3(-1.f, 0.6f, 0.f);
		vertices[0]._uv = Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1]._position = Vector3(-1.f, 1.f, 0.f);
		vertices[1]._uv = Vector2(0.f, 0.f);

		// 우 상단
		vertices[2]._position = Vector3(1.f, 1.f, 0.f);
		vertices[2]._uv = Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3]._position = Vector3(-1.f, 0.6f, 0.f);
		vertices[3]._uv = Vector2(0.f, 1.f);

		// 우 상단
		vertices[4]._position = Vector3(1.f, 1.f, 0.f);
		vertices[4]._uv = Vector2(1.f, 0.f);

		// 우 하단
		vertices[5]._position = Vector3(1.f, 0.6f, 0.f);
		vertices[5]._uv = Vector2(1.f, 1.f);

		std::vector<uint32> indices(6);

		for (int i = 0; i < 6; i++)
			indices[i] = i;

		std::shared_ptr<Mesh> quad = std::make_shared<Mesh>();

		quad->Initialize(TEXT("Static_Debug_Quad"), vertices, indices);

		_meshes.insert({ quad->GetID(), quad });
	}

	void ResourceManager::CreateDynamicQuadMesh()
	{
		std::vector<StaticMeshVertex> vertices(6);

		// 좌 하단
		vertices[0]._position = Vector3(-1.f, -1.f, 0.f);
		vertices[0]._uv = Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1]._position = Vector3(-1.f, 1.f, 0.f);
		vertices[1]._uv = Vector2(0.f, 0.f);

		// 우 상단
		vertices[2]._position = Vector3(1.f, 1.f, 0.f);
		vertices[2]._uv = Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3]._position = Vector3(-1.f, -1.f, 0.f);
		vertices[3]._uv = Vector2(0.f, 1.f);

		// 우 상단
		vertices[4]._position = Vector3(1.f, 1.f, 0.f);
		vertices[4]._uv = Vector2(1.f, 0.f);

		// 우 하단
		vertices[5]._position = Vector3(1.f, -1.f, 0.f);
		vertices[5]._uv = Vector2(1.f, 1.f);

		std::vector<uint32> indices(6);

		for (int i = 0; i < 6; i++)
			indices[i] = i;

		std::shared_ptr<Mesh> quad = std::make_shared<Mesh>();

		quad->Initialize(TEXT("Dynamic_Quad"), vertices, indices, true);

		_meshes.insert({ quad->GetID(), quad });
	}
}