#include "pch.h"
#include "Pass_Sampling.h"

#include "ViewPort.h"
#include "RenderTarget.h"
#include "DepthStencilBuffer.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

namespace GraphicsEngine
{
	void Pass_Sampling::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_workList = std::vector<std::function<void(Pass_Sampling&, std::shared_ptr<RenderTarget>)>>(SAMPLING_OPTION_COUNT);

		_workList[0] = &Pass_Sampling::DrawHalf;
		_workList[1] = &Pass_Sampling::DrawQuarter;
		_workList[2] = &Pass_Sampling::DrawOctar;
		_workList[3] = &Pass_Sampling::DrawSixteen;

		_halfViewPort = std::make_shared<ViewPort>();
		_halfViewPort->Initialize(Vector2::Zero, screenWidth / 2, screenHeight / 2);

		_sampleHalf = std::make_shared<RenderTarget>();
		_sampleHalf->Initialize(_device, screenWidth / 2, screenHeight / 2);

		_quarterViewPort = std::make_shared<ViewPort>();
		_quarterViewPort->Initialize(Vector2::Zero, screenWidth / 4, screenHeight / 4);

		_sampleQuarter = std::make_shared<RenderTarget>();	
		_sampleQuarter->Initialize(_device, screenWidth / 4, screenHeight / 4);

		_octarViewPort = std::make_shared<ViewPort>();
		_octarViewPort->Initialize(Vector2::Zero, screenWidth / 8, screenHeight / 8);

		_sampleOctar = std::make_shared<RenderTarget>();
		_sampleOctar->Initialize(_device, screenWidth / 8, screenHeight / 8);

		_sixteenViewPort = std::make_shared<ViewPort>();
		_sixteenViewPort->Initialize(Vector2::Zero, screenWidth / 16, screenHeight / 16);

		_sampleSixteen = std::make_shared<RenderTarget>();
		_sampleSixteen->Initialize(_device, screenWidth / 16, screenHeight / 16);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psSample = ShaderManager::Get()->GetPS(TEXT("PS_Sample"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);
	}

	void Pass_Sampling::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		auto&& halfWidth = std::move(screenWidth / 2);
		auto&& halfHeight = std::move(screenHeight / 2);

		_halfViewPort->OnResize(halfWidth, halfHeight);
		_sampleHalf->OnResize(_device, halfWidth, halfHeight);

		auto&& quarterWidth = std::move(screenWidth / 4);
		auto&& quarterHeight = std::move(screenHeight / 4);

		_quarterViewPort->OnResize(quarterWidth, quarterHeight);
		_sampleQuarter->OnResize(_device, quarterWidth, quarterHeight);

		auto&& octarWidth = std::move(screenWidth / 8);
		auto&& octarHeight = std::move(screenHeight / 8);

		_octarViewPort->OnResize(octarWidth, octarHeight);
		_sampleOctar->OnResize(_device, octarWidth, octarHeight);

		auto&& sixteenWidth = std::move(screenWidth / 16);
		auto&& sixteenHeight = std::move(screenHeight / 16);

		_sixteenViewPort->OnResize(sixteenWidth, sixteenHeight);
		_sampleSixteen->OnResize(_device, sixteenWidth, sixteenHeight);
	}

	void Pass_Sampling::Release()
	{
		_halfViewPort->Release();

		_sampleHalf->Release();

		_quarterViewPort->Release();

		_sampleQuarter->Release();

		_octarViewPort->Release();

		_sampleOctar->Release();

		_sixteenViewPort->Release();

		_sampleSixteen->Release();

		__super::Release();
	}

	void Pass_Sampling::Render(std::shared_ptr<RenderTarget> target, SAMPLING_OPTION option /*= SAMPLING_OPTION::ALL*/)
	{
		if ((_screenWidth != target->GetTextureWidth()) || (_screenHeight != target->GetTextureHeight()))
			OnResize(target->GetTextureWidth(), target->GetTextureHeight());

		auto twoPow = [](size_t i) -> uint32&& { uint32 ret = 1; for (size_t j = 0; j < i; j++) ret *= 2; return std::move(ret);  };

		for (size_t i = 0; i < _workList.size(); i++)
		{
			auto&& iSquare = twoPow(i);

			if (((static_cast<uint32>(option) & iSquare)) == iSquare)
				_workList[i](*this, target);
		}
	}

	void Pass_Sampling::DrawHalf(std::shared_ptr<RenderTarget> target)
	{
		_halfViewPort->SetViewPort(_deviceContext);

		_sampleHalf->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _sampleHalf->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psSample->UpdateShaderResource(target->GetSRV().Get(), SRV_REGISTER::t0);

		_psSample->Update();

		_meshScreen->Update();
	}

	void Pass_Sampling::DrawQuarter(std::shared_ptr<RenderTarget> target)
	{
		_quarterViewPort->SetViewPort(_deviceContext);

		_sampleQuarter->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _sampleQuarter->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psSample->UpdateShaderResource(target->GetSRV().Get(), SRV_REGISTER::t0);

		_psSample->Update();

		_meshScreen->Update();
	}

	void Pass_Sampling::DrawOctar(std::shared_ptr<RenderTarget> target)
	{
		_octarViewPort->SetViewPort(_deviceContext);

		_sampleOctar->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _sampleOctar->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psSample->UpdateShaderResource(target->GetSRV().Get(), SRV_REGISTER::t0);

		_psSample->Update();

		_meshScreen->Update();
	}

	void Pass_Sampling::DrawSixteen(std::shared_ptr<RenderTarget> target)
	{
		_sixteenViewPort->SetViewPort(_deviceContext);

		_sampleSixteen->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _sampleSixteen->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psSample->UpdateShaderResource(target->GetSRV().Get(), SRV_REGISTER::t0);

		_psSample->Update();

		_meshScreen->Update();
	}
}