#include "pch.h"
#include "Pass_Vignetting.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ViewPort.h"

#include "Mesh.h"
#include "PixelShader.h"
#include "VertexShader.h"

namespace GraphicsEngine
{
	void Pass_Vignetting::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);
		
		_vignettingOutput = std::make_shared<RenderTarget>();

		_vignettingOutput->Initialize(_device, screenWidth, screenHeight);

		_screenViewPort = std::make_shared<ViewPort>();

		_screenViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_screenMesh = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psVignetting = ShaderManager::Get()->GetPS(TEXT("PS_Vignetting"));
	}

	void Pass_Vignetting::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_vignettingOutput->OnResize(_device, screenWidth, screenHeight);

		_screenViewPort->OnResize(screenWidth, screenHeight);
	}

	void Pass_Vignetting::Release()
	{
		_vignettingOutput->Release();

		_screenMesh.reset();

		_vsQuad.reset();

		_psVignetting.reset();

		__super::Release();
	}

	void Pass_Vignetting::Render(std::shared_ptr<RenderTarget> targetScreen)
	{
		_vignettingOutput->ClearRenderTargetView(_deviceContext);

		_screenViewPort->SetViewPort(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _vignettingOutput->GetRTV().GetAddressOf(), nullptr);

		_vsQuad->Update();

		_psVignetting->UpdateShaderResource(targetScreen->GetSRV().Get(), SRV_REGISTER::t0);

		_psVignetting->Update();

		_screenMesh->Update();

		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

		_deviceContext->PSSetShaderResources(0, 1, nullSRV);
	}
}