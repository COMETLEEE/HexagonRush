#include "pch.h"
#include "Pass_Bloom.h"

#include "Renderer.h"
#include "PostProcessor.h"
#include "Pass_Sampling.h"
#include "Pass_GaussianBlur.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

namespace GraphicsEngine
{
	void Pass_Bloom::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_samplingPass = Renderer::Get()->_samplingPass;

		_screenSizeViewPort = std::make_shared<ViewPort>();
		_screenSizeViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_bloomSampled = std::make_shared<RenderTarget>();
		_bloomSampled->Initialize(_device, screenWidth, screenHeight);

		_bloomOutput = std::make_shared<RenderTarget>();
		_bloomOutput->Initialize(_device, screenWidth, screenHeight);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));
		_psBloomSampling1 = ShaderManager::Get()->GetPS(TEXT("PS_BloomSampling_1"));
		_psBloomSampling2 = ShaderManager::Get()->GetPS(TEXT("PS_BloomSampling_2"));
		_psBloomSampling3 = ShaderManager::Get()->GetPS(TEXT("PS_BloomSampling_3"));
		_psBloomAddUp = ShaderManager::Get()->GetPS(TEXT("PS_BloomAddUp"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);

		// Accumulating을 위한 녀석들
		_octarViewPort = std::make_shared<ViewPort>();
		_octarViewPort->Initialize(Vector2::Zero, screenWidth / 8, screenHeight / 8);

		_octarTarget = std::make_shared<RenderTarget>();
		_octarTarget->Initialize(_device, screenWidth / 8, screenHeight / 8);

		_quarterViewPort = std::make_shared<ViewPort>();
		_quarterViewPort->Initialize(Vector2::Zero, screenWidth / 4, screenHeight / 4);

		_quarterTarget = std::make_shared<RenderTarget>();
		_quarterTarget->Initialize(_device, screenWidth / 4, screenHeight / 4);

		_halfViewPort = std::make_shared<ViewPort>();
		_halfViewPort->Initialize(Vector2::Zero, screenWidth / 2, screenHeight / 2);

		_halfTarget = std::make_shared<RenderTarget>();
		_halfTarget->Initialize(_device, screenWidth / 2, screenHeight / 2);
	}

	void Pass_Bloom::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_screenSizeViewPort->OnResize(screenWidth, screenHeight);

		_bloomSampled->OnResize(_device, screenWidth, screenHeight);

		_bloomOutput->OnResize(_device, screenWidth, screenHeight);

		// Accumulating을 위한 녀석들
		auto&& octarWidth = std::move(screenWidth / 8);
		auto&& octarHeight = std::move(screenHeight / 8);

		_octarViewPort->OnResize(octarWidth, octarHeight);
		_octarTarget->OnResize(_device, octarWidth, octarHeight);

		auto&& quarterWidth = std::move(screenWidth / 4);
		auto&& quarterHeight = std::move(screenHeight / 4);

		_quarterViewPort->OnResize(quarterWidth, quarterHeight);
		_quarterTarget->OnResize(_device, quarterWidth, quarterHeight);

		auto&& halfWidth = std::move(screenWidth / 2);
		auto&& halfHeight = std::move(screenHeight / 2);

		_halfViewPort->OnResize(halfWidth, halfHeight);
		_halfTarget->OnResize(_device, halfWidth, halfHeight);
	}

	void Pass_Bloom::Release()
	{
		_samplingPass.reset();
		
		_blurPass.reset();

		_screenSizeViewPort->Release();

		_bloomSampled->Release();

		_bloomOutput->Release();

		_vsQuad.reset();

		_psBloomSampling1.reset();

		_psBloomSampling2.reset();

		_psBloomSampling3.reset();

		_psBloomAddUp.reset();

		_meshScreen.reset();

		// Accumulating을 위한 녀석들
		_octarViewPort->Release();
		_octarTarget->Release();

		_quarterViewPort->Release();
		_quarterTarget->Release();

		_halfViewPort->Release();
		_halfTarget->Release();

		__super::Release();
	}

	void Pass_Bloom::Render(std::shared_ptr<RenderTarget> targetScreen)
	{
		BloomSampling(targetScreen);

		_samplingPass->Render(_bloomSampled, SAMPLING_OPTION::ALL);

		Blur16XAddUp8X();

		Blur8XAddUp4X();

		Blur4XAddUp2X();

		Blur2XAddUpOrigin(targetScreen);

		EndRender();
	}

	void Pass_Bloom::BloomSampling(std::shared_ptr<RenderTarget> targetScreen)
	{
		_screenSizeViewPort->SetViewPort(_deviceContext);

		_bloomSampled->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _bloomSampled->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		// 변수로 컨트롤할 수 있도록
		_psBloomSampling1->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t0);
		_psBloomSampling1->Update();

		/*_psBloomSampling2->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t0);
		_psBloomSampling2->Update();*/

		//_psBloomSampling3->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t0);
		//_psBloomSampling3->Update();

		_meshScreen->Update();
	}

	void Pass_Bloom::Blur16XAddUp8X()
	{
		_blurPass->Render(_samplingPass->_sampleSixteen);

		_octarViewPort->SetViewPort(_deviceContext);

		_octarTarget->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _octarTarget->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psBloomAddUp->UpdateShaderResource(_blurPass->_blurOutput->GetSRV().Get(), SRV_REGISTER::t0);
		_psBloomAddUp->UpdateShaderResource(_samplingPass->_sampleOctar->GetSRV().Get(), SRV_REGISTER::t1);
		_psBloomAddUp->Update();

		_meshScreen->Update();
	}

	void Pass_Bloom::Blur8XAddUp4X()
	{
		_blurPass->Render(_octarTarget);

		_quarterViewPort->SetViewPort(_deviceContext);

		_quarterTarget->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _quarterTarget->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psBloomAddUp->UpdateShaderResource(_blurPass->_blurOutput->GetSRV().Get(), SRV_REGISTER::t0);
		_psBloomAddUp->UpdateShaderResource(_samplingPass->_sampleQuarter->GetSRV().Get(), SRV_REGISTER::t1);
		_psBloomAddUp->Update();

		_meshScreen->Update();
	}

	void Pass_Bloom::Blur4XAddUp2X()
	{
		_blurPass->Render(_quarterTarget);

		_halfViewPort->SetViewPort(_deviceContext);

		_halfTarget->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _halfTarget->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psBloomAddUp->UpdateShaderResource(_blurPass->_blurOutput->GetSRV().Get(), SRV_REGISTER::t0);
		_psBloomAddUp->UpdateShaderResource(_samplingPass->_sampleHalf->GetSRV().Get(), SRV_REGISTER::t1);
		_psBloomAddUp->Update();

		_meshScreen->Update();
	}

	void Pass_Bloom::Blur2XAddUpOrigin(std::shared_ptr<RenderTarget> targetScreen)
	{
		_blurPass->Render(_halfTarget);

		_screenSizeViewPort->SetViewPort(_deviceContext);

		_bloomOutput->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _bloomOutput->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psBloomAddUp->UpdateShaderResource(_blurPass->_blurOutput->GetSRV().Get(), SRV_REGISTER::t0);
		_psBloomAddUp->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t1);
		_psBloomAddUp->Update();

		_meshScreen->Update();
	}

	void Pass_Bloom::EndRender()
	{
		// UnBind
		ID3D11ShaderResourceView* nullSRV[3] = { nullptr };

		_deviceContext->PSSetShaderResources(0, 3, nullSRV);
	}
}