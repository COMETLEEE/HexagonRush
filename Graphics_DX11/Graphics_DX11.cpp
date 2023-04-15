#include "pch.h"
#include "Graphics_DX11.h"

#include "Mesh.h"
#include "Texture.h"
#include "Font.h"

#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

// Managers
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

std::shared_ptr<GraphicsEngine::Graphics_DX11> GraphicsEngine::Graphics_DX11::_graphics_DX11 = 
	std::dynamic_pointer_cast<GraphicsEngine::Graphics_DX11>(Graphics_Interface::Get());

namespace GraphicsEngine
{
	void Graphics_DX11::Initialize(HWND hWnd, uint32  screenWidth, uint32 screenHeight)
	{
		CoInitialize(NULL);

		_renderer = Renderer::Get();
		_resourceManager = ResourceManager::Get();
		_shaderManager = ShaderManager::Get();
		
		// 본체 Initialize
		_renderer->Initialize(hWnd, screenWidth, screenHeight);
		_resourceManager->Initialize();
		_shaderManager->Initialize();

		// 기타 상호간의 참조가 필요한 Initialize
		_renderer->InitializeForPass();
	}

	// For Model Render
	void Graphics_DX11::PostRenderingData_3D(std::shared_ptr<::RenderingData_3D>& renderingData)
	{
		_renderer->PostRenderingData_3D(renderingData);
	}

	void Graphics_DX11::PostRenderingData_Particle(std::shared_ptr<::RenderingData_Particle>& renderingData)
	{
		_renderer->PostRenderingData_Particle(renderingData);
	}

	void Graphics_DX11::PostRenderingData_UI(std::shared_ptr<::RenderingData_UI>& renderingData)
	{
		_renderer->PostRenderingData_UI(renderingData);
	}

	// For Data Structure
	void Graphics_DX11::PostPerFrameData(std::shared_ptr<::PerFrameData>& perframeData)
	{
		_renderer->ExecuteForPerFrameData(perframeData);
	}

	void Graphics_DX11::PostTextData(std::shared_ptr<::TextData>& textData)
	{
		_renderer->PostTextData(textData);
	}

	void Graphics_DX11::ExecuteRender()
	{
		_renderer->ExecuteRenderQueue();
	}

	void Graphics_DX11::Release()
	{
		// Manager 객체들을 Reset한다.
		_shaderManager->Release();

		_resourceManager->Release();

		_renderer->Release();

		_resourceManager.reset();

		_shaderManager.reset();

		_renderer.reset();

		CoUninitialize();
	}

	void Graphics_DX11::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		_renderer->OnResize(screenWidth, screenHeight);
	}

	const uint64& Graphics_DX11::LoadTexture(const tstring& textureName, const tstring& path)
	{
		return _resourceManager->LoadTexture(textureName, path);
	}

	// 상당히 더러운 코드다 ..
	const uint64& Graphics_DX11::LoadEnvironmentCubeMap(const tstring& envMapName, const tstring& path)
	{
		// 큐브맵을 받는다. (Env Map)
		const uint64& retID = _resourceManager->LoadTexture(envMapName, path);

		_resourceManager->CreateIBLSource(retID, true, ULLONG_MAX);

		return retID;
	}

	// 상당히 더러운 코드다 .. 2222222
	const uint64& Graphics_DX11::LoadEnvironmentPanoramaMap(const tstring& envMapName, const tstring& path)
	{		
		// 예를 들면, HDR 이미지를 로드한다.
		const uint64& panaMapID = _resourceManager->LoadTexture(envMapName, path);

		std::shared_ptr<Texture> panaMap = _resourceManager->GetTexture(panaMapID);

		// HDR 이미지를 Cube로 변경 ! => IBL Source로 굽기 위한 작업 준비 ..
		std::shared_ptr<Texture> texCube = Texture::CreateEmptyTextureCube(1024, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
		
		texCube->CreateTextureUAV(0);

		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		ResourceManager::Get()->RegisterTexture(texCube);

		std::shared_ptr<ComputeShader> specCom = ShaderManager::Get()->GetCS(TEXT("CS_EquirectToCube"));

		// SRV, UAV Setting
		specCom->UpdateShaderResource(panaMap->GetSRV().Get(), SRV_REGISTER::t0);

		deviceContext->CSSetUnorderedAccessViews(0, 1, texCube->GetUAV().GetAddressOf(), nullptr);

		specCom->Update();
		
		deviceContext->Dispatch(texCube->GetWidth() / 32, texCube->GetHeight() / 32, 6);
		
		ID3D11UnorderedAccessView* const nullUAV[] = { nullptr };

		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		_resourceManager->CreateIBLSource(texCube->GetID(), true, panaMapID);

		texCube->Release();

		// Panoramic SkyBox 대응용 코드 작성 완료 ..!
		return panaMapID;
	}

	const uint64& Graphics_DX11::LoadMesh(const tstring& meshName, const std::vector<StaticMeshVertex>& vertexVector, const std::vector<uint32>& indexVector,
		bool isDynamic /*= false*/, bool isLineList)
	{
		return _resourceManager->LoadMesh(meshName, vertexVector, indexVector, isDynamic, isLineList);
	}

	const uint64& Graphics_DX11::LoadMesh(const tstring& meshName, const std::vector<SkinnedMeshVertex>& vertexVector, const std::vector<uint32>& indexVector, 
		bool isDynamic /*= false*/, bool isLineList)
	{
		return _resourceManager->LoadMesh(meshName, vertexVector, indexVector, isDynamic, isLineList);
	}

	const uint64& Graphics_DX11::LoadFont(const tstring& fontName, const tstring& path)
	{
		return _resourceManager->LoadFont(fontName, path);
	}

	const DirectX::SimpleMath::Vector4&& Graphics_DX11::MeasureDrawBounds(std::shared_ptr<::TextData> textData)
	{
		std::shared_ptr<Font> font = ResourceManager::Get()->GetFont(textData->_fontID);

		RECT rect = font->MeasureDrawBounds(textData->_outText, textData->_startCoord);

		return std::move(Vector4(static_cast<float>(rect.left), static_cast<float>(rect.top), 
			static_cast<float>(rect.right), static_cast<float>(rect.bottom)));
	}
	void Graphics_DX11::ReleaseTexture()
	{
		// 그냥 싹 밀어버리자~
		_resourceManager->ResetTexture();
	}
}