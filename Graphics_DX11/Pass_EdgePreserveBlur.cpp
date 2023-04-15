#include "pch.h"
#include "Pass_EdgePreserveBlur.h"

#include "ResourceManager.h"
#include "ShaderManager.h"

#include "ViewPort.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace GraphicsEngine
{
	void Pass_EdgePreserveBlur::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_blurViewPort = std::make_shared<ViewPort>();

		_blurViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_blurHorizontal = std::make_shared<RenderTarget>();

		_blurHorizontal->Initialize(_device, screenWidth, screenHeight);

		for (size_t i = 0; i < BLUR_MAX_COUNT; i++)
		{
			_blurOutput[i] = std::make_shared<RenderTarget>();

			_blurOutput[i]->Initialize(_device, screenWidth, screenHeight);
		}

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);
		
		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psEdgePreserveBlurX = ShaderManager::Get()->GetPS(TEXT("PS_EdgePreserveBlur_X"));

		_psEdgePreserveBlurY = ShaderManager::Get()->GetPS(TEXT("PS_EdgePreserveBlur_Y"));
	}

	void Pass_EdgePreserveBlur::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_blurHorizontal->OnResize(_device, screenWidth, screenHeight);

		for (size_t i = 0; i < BLUR_MAX_COUNT; i++)
			_blurOutput[i]->OnResize(_device, screenWidth, screenHeight);

		_blurViewPort->OnResize(screenWidth, screenHeight);
	}

	void Pass_EdgePreserveBlur::Release()
	{
		_blurHorizontal->Release();

		for (size_t i = 0; i < BLUR_MAX_COUNT; i++)
			_blurOutput[i]->Release();

		_blurViewPort->Release();

		_meshScreen.reset();

		_vsQuad.reset();

		_psEdgePreserveBlurX.reset();

		_psEdgePreserveBlurY.reset();

		__super::Release();
	}

	void Pass_EdgePreserveBlur::Render(std::shared_ptr<RenderTarget> depth, std::shared_ptr<RenderTarget> normal, 
		std::shared_ptr<RenderTarget> targetScreen, uint32 blurCount)
	{
		BeginRender(targetScreen);

		CB_PERTEXTURE cbTexture;

		cbTexture._screenInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(_screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		cbTexture._textureInfo = Vector4(static_cast<float>(_screenWidth), static_cast<float>(_screenHeight),
			1.f / _screenWidth, 1.f / _screenHeight);

		for (size_t i = 0; i < blurCount; i++)
		{
			BeginRenderToHorizontal();

			_psEdgePreserveBlurX->UpdateConstantBuffer(&cbTexture, CBV_REGISTER::b1);
			_psEdgePreserveBlurX->UpdateShaderResource(depth->GetSRV().Get(), SRV_REGISTER::t0);
			_psEdgePreserveBlurX->UpdateShaderResource(normal->GetSRV().Get(), SRV_REGISTER::t1);

			if (i != 0)
				_psEdgePreserveBlurX->UpdateShaderResource(_blurOutput[i - 1]->GetSRV().Get(), SRV_REGISTER::t2);
			else
				_psEdgePreserveBlurX->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t2);
			
			_vsQuad->Update();
			_psEdgePreserveBlurX->Update();
			_meshScreen->Update();

			BeginRenderToOutput(i);

			_psEdgePreserveBlurY->UpdateConstantBuffer(&cbTexture, CBV_REGISTER::b1);
			_psEdgePreserveBlurY->UpdateShaderResource(depth->GetSRV().Get(), SRV_REGISTER::t0);
			_psEdgePreserveBlurY->UpdateShaderResource(normal->GetSRV().Get(), SRV_REGISTER::t1);
			_psEdgePreserveBlurY->UpdateShaderResource(_blurHorizontal->GetSRV().Get(), SRV_REGISTER::t2);

			_vsQuad->Update();
			_psEdgePreserveBlurY->Update();
			_meshScreen->Update();

			EndRender();
		}
	}

	void Pass_EdgePreserveBlur::BeginRender(std::shared_ptr<RenderTarget> targetScreen)
	{
		// 일단 사이즈부터 맞춰준다.
		if ((targetScreen->GetTextureWidth() != _screenWidth) || (targetScreen->GetTextureHeight()))
			OnResize(targetScreen->GetTextureWidth(), targetScreen->GetTextureHeight());
	}

	void Pass_EdgePreserveBlur::BeginRenderToHorizontal()
	{
		_blurHorizontal->ClearRenderTargetView(_deviceContext);

		_blurViewPort->SetViewPort(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _blurHorizontal->GetRTV().GetAddressOf(), nullptr);
	}

	void Pass_EdgePreserveBlur::BeginRenderToOutput(const uint32& rounds)
	{
		_blurOutput[rounds]->ClearRenderTargetView(_deviceContext);

		_blurViewPort->SetViewPort(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _blurOutput[rounds]->GetRTV().GetAddressOf(), nullptr);
	}

	void Pass_EdgePreserveBlur::EndRender()
	{
		// SRV UnBind
		ID3D11ShaderResourceView* nullSRV[5] = { nullptr };

		_deviceContext->PSSetShaderResources(0, 5, nullSRV);
	}
}