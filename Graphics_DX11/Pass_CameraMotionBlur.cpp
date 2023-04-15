#include "pch.h"
#include "Pass_CameraMotionBlur.h"

#include "ResourceManager.h"
#include "ShaderManager.h"

#include "ViewPort.h"
#include "RenderTarget.h"
#include "DepthStencilBuffer.h"

#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace GraphicsEngine
{
	void Pass_CameraMotionBlur::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_blurOutput = std::make_shared<RenderTarget>();

		_blurOutput->Initialize(_device, screenWidth, screenHeight);
		
		_screenSizeViewPort = std::make_shared<ViewPort>();

		_screenSizeViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psCameraMotionBlur = ShaderManager::Get()->GetPS(TEXT("PS_CameraMotionBlur"));
	}

	void Pass_CameraMotionBlur::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_blurOutput->OnResize(_device, screenWidth, screenHeight);

		_screenSizeViewPort->OnResize(screenWidth, screenHeight);
	}

	void Pass_CameraMotionBlur::Release()
	{
		_meshScreen.reset();

		_vsQuad.reset();

		_psCameraMotionBlur.reset();

		_blurOutput->Release();

		_screenSizeViewPort->Release();

		__super::Release();
	}

	void Pass_CameraMotionBlur::Render(std::shared_ptr<RenderTarget> position, std::shared_ptr<RenderTarget> target)
	{
		_screenSizeViewPort->SetViewPort(_deviceContext);

		_blurOutput->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _blurOutput->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psCameraMotionBlur->UpdateShaderResource(position->GetSRV().Get(), SRV_REGISTER::t0);

		_psCameraMotionBlur->UpdateShaderResource(target->GetSRV().Get(), SRV_REGISTER::t1);

		_psCameraMotionBlur->Update();

		_meshScreen->Update();

		EndRender();
	}
	
	void Pass_CameraMotionBlur::EndRender()
	{
		// UnBind
		ID3D11ShaderResourceView* nullSRV[3] = { nullptr };

		_deviceContext->PSSetShaderResources(0, 3, nullSRV);
	}
}