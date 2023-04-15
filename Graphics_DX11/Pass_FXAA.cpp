#include "pch.h"
#include "Pass_FXAA.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

namespace GraphicsEngine
{
	void Pass_FXAA::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_screenViewPort = std::make_shared<ViewPort>();

		_screenViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_fxaaOutput = std::make_shared<RenderTarget>();

		_fxaaOutput->Initialize(_device, screenWidth, screenHeight);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psFXAA = ShaderManager::Get()->GetPS(TEXT("PS_FXAA"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);
	}

	void Pass_FXAA::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_fxaaOutput->OnResize(_device, screenWidth, screenHeight);

		_screenViewPort->OnResize(screenWidth, screenHeight);
	}

	void Pass_FXAA::Release()
	{
		_screenViewPort->Release();

		_fxaaOutput->Release();

		_vsQuad.reset();

		_psFXAA.reset();

		_meshScreen.reset();

		__super::Release();
	}

	void Pass_FXAA::Render(std::shared_ptr<RenderTarget> targetScreen)
	{
		_fxaaOutput->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _fxaaOutput->GetRTV().GetAddressOf(), nullptr);

		_screenViewPort->SetViewPort(_deviceContext);

		_vsQuad->Update();
		
		CB_PERTEXTURE cbTexture;

		cbTexture._textureInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(_screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		_psFXAA->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t0);

		_psFXAA->UpdateConstantBuffer(&cbTexture, CBV_REGISTER::b1);

		_psFXAA->Update();

		_meshScreen->Update();
	}
}