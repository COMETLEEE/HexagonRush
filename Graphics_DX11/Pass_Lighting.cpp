#include "pch.h"
#include "Pass_Lighting.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ViewPort.h"
#include "RenderTarget.h"
#include "DepthStencilBuffer.h"
#include "DepthStencilBufferArray.h"

#include "Pass_Deferred.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "Texture.h"

namespace GraphicsEngine
{

	void Pass_Lighting::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_cbPerTexture = std::make_shared<CB_PERTEXTURE>();

		_cbPerTexture->_screenInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		_cbPerTexture->_textureInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		_screenSizeViewPort = std::make_shared<ViewPort>();

		_screenSizeViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_lightingOutput = std::make_shared<RenderTarget>();

		_lightingOutput->Initialize(_device, _screenWidth, _screenHeight);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		// Legacy Lighting Model
		// _psLighting = ShaderManager::Get()->GetPS(TEXT("PS_Lighting"));

		// PBR Lighting Model
		_psLighting = ShaderManager::Get()->GetPS(TEXT("PS_Lighting_PBR"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);
	}

	void Pass_Lighting::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_cbPerTexture->_screenInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(screenHeight), 
			1.f / _screenWidth, 1.f / _screenHeight);
	
		_cbPerTexture->_textureInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		_screenSizeViewPort->OnResize(screenWidth, screenHeight);

		_lightingOutput->OnResize(_device, screenWidth, screenHeight);
	}

	void Pass_Lighting::Release()
	{
		_cbPerTexture.reset();

		_screenSizeViewPort->Release();

		_lightingOutput->Release();

		_vsQuad.reset();

		_psLighting.reset();

		_meshScreen.reset();

		__super::Release();
	}

	void Pass_Lighting::BeginRender()
	{
		_lightingOutput->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _lightingOutput->GetRTV().GetAddressOf(), nullptr);

		_screenSizeViewPort->SetViewPort(_deviceContext);
	}

	void Pass_Lighting::Render(const std::vector<std::shared_ptr<RenderTarget>>& gBuffers, std::shared_ptr<DepthStencilBuffer> shadowMap,
		std::shared_ptr<DepthStencilBufferArray> cascadeShadowMap,
		std::shared_ptr<RenderTarget> ssaoMap, CB_LIGHTVIEWPROJ* cbLightViewProj, CB_DIRLIGHTVIEWPROJ* cbDirLightViewProj, std::shared_ptr<RenderTarget> emissive)
	{
		BeginRender();

		// Vertex Shader
		_vsQuad->Update();

		_psLighting->UpdateConstantBuffer(_cbPerTexture.get(), CBV_REGISTER::b1);

		if (cbLightViewProj != nullptr)
			_psLighting->UpdateConstantBuffer(cbLightViewProj, CBV_REGISTER::b2);

		if (cbDirLightViewProj != nullptr)
			_psLighting->UpdateConstantBuffer(cbDirLightViewProj, CBV_REGISTER::b3);

		for (int i = 0; i < DEFERRED_RENDERTARGET_COUNT; i++)
			_psLighting->UpdateShaderResource(gBuffers[i]->GetSRV().Get(), static_cast<SRV_REGISTER>(i));

		// Irradiance, Specular Pre-Filtered, Specular_BRDF_LUT. TEST TEST TEST
		std::shared_ptr<IBLSource> iblSource =
			ResourceManager::Get()->_IBLSourceForEnvMap.find(Renderer::Get()->_currCubeMapID) != ResourceManager::Get()->_IBLSourceForEnvMap.end() ?
			ResourceManager::Get()->_IBLSourceForEnvMap.at(Renderer::Get()->_currCubeMapID) : nullptr;

		// TEST CODE - Emissive
		if (emissive != nullptr)
		{
			_psLighting->UpdateShaderResource(emissive->GetSRV(), SRV_REGISTER::t4);
		}

		// TEST CODE - IBL Source
		if (iblSource != nullptr)
		{
			_psLighting->UpdateShaderResource(iblSource->_iblSources[0]->GetSRV(), SRV_REGISTER::t6);
			_psLighting->UpdateShaderResource(iblSource->_iblSources[1]->GetSRV(), SRV_REGISTER::t7);
			_psLighting->UpdateShaderResource(iblSource->_iblSources[2]->GetSRV(), SRV_REGISTER::t8);
		}

		// TEST CODE - SSAO Map Bind
		if (ssaoMap != nullptr)
			_psLighting->UpdateShaderResource(ssaoMap->GetSRV(), SRV_REGISTER::t9);

		// TEST CODE - Shadow Map Bind
		if (shadowMap != nullptr)
			_psLighting->UpdateShaderResource(shadowMap->GetSRV(), SRV_REGISTER::t10);

		// TEST CODE - Cascade Shadow Map Bind
		if (cascadeShadowMap != nullptr)
			_psLighting->UpdateShaderResource(cascadeShadowMap->GetSRV(), SRV_REGISTER::t11);

		// Pixel Shader
		_psLighting->Update();

		// Draw Output
		_meshScreen->Update();

		EndRender();
	}

	void Pass_Lighting::EndRender()
	{
		ID3D11ShaderResourceView* nullSRV[3] = { nullptr };

		// Shadow Map Unbind
		_deviceContext->PSSetShaderResources(static_cast<uint32>(SRV_REGISTER::t9), 3, nullSRV);
	}
}