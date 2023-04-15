#include "pch.h"
#include "Pass_GaussianBlur.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace GraphicsEngine
{
	void Pass_GaussianBlur::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_blurHorizontal = std::make_shared<RenderTarget>();

		_blurHorizontal->Initialize(_device, screenWidth, screenHeight);

		_blurOutput = std::make_shared<RenderTarget>();

		_blurOutput->Initialize(_device, screenWidth, screenHeight);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psBlurX = ShaderManager::Get()->GetPS(TEXT("PS_GaussianBlur_X"));

		_psBlurY = ShaderManager::Get()->GetPS(TEXT("PS_GaussianBlur_Y"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);
	}

	void Pass_GaussianBlur::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_blurHorizontal->OnResize(_device, screenWidth, screenHeight);

		_blurOutput->OnResize(_device, screenWidth, screenHeight);
	}

	void Pass_GaussianBlur::Release()
	{
		_blurHorizontal->Release();

		_blurOutput->Release();
		
		_vsQuad.reset();

		_psBlurX.reset();

		_psBlurY.reset();

		_meshScreen.reset();

		__super::Release();
	}

	void Pass_GaussianBlur::Render(std::shared_ptr<RenderTarget>& targetScreen)
	{
		BeginRender(targetScreen);

		CB_PERTEXTURE cbTexture;

		cbTexture._textureInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(_screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		BeginRenderToHorizontal();

		_psBlurX->UpdateConstantBuffer(&cbTexture, CBV_REGISTER::b1);
		_psBlurX->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t0);

		_vsQuad->Update();
		_psBlurX->Update();
		_meshScreen->Update();

		BeginRenderToOutput();

		_psBlurY->UpdateConstantBuffer(&cbTexture, CBV_REGISTER::b1);
		_psBlurY->UpdateShaderResource(_blurHorizontal->GetSRV().Get(), SRV_REGISTER::t0);

		_vsQuad->Update();
		_psBlurY->Update();
		_meshScreen->Update();

		EndRender();
	}

	void Pass_GaussianBlur::BeginRender(std::shared_ptr<RenderTarget>& targetScreen)
	{
		// 일단 사이즈부터 맞춰준다.
		if (targetScreen->GetTextureWidth() != _screenWidth || targetScreen->GetTextureHeight() != _screenHeight)
			OnResize(targetScreen->GetTextureWidth(), targetScreen->GetTextureHeight());
	}

	void Pass_GaussianBlur::EndRender()
	{
		// Target Screen SRV UnBind
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

		_deviceContext->PSSetShaderResources(0, 1, nullSRV);
	}

	void Pass_GaussianBlur::BeginRenderToHorizontal()
	{
		_blurHorizontal->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _blurHorizontal->GetRTV().GetAddressOf(), nullptr);
	}

	void Pass_GaussianBlur::BeginRenderToOutput()
	{
		_blurOutput->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _blurOutput->GetRTV().GetAddressOf(), nullptr);
	}
}