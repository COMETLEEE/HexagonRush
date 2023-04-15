#include "pch.h"
#include "Pass_SSAO.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ViewPort.h"
#include "RenderTarget.h"
#include "DepthStencilBuffer.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

namespace GraphicsEngine
{
	void Pass_SSAO::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_screenViewPort = std::make_shared<ViewPort>();

		_screenViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_ssaoOutput = std::make_shared<RenderTarget>();

		_ssaoOutput->Initialize(_device, screenWidth, screenHeight);

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);

		_vsSSAO = ShaderManager::Get()->GetVS(TEXT("VS_SSAO"));

		_psSSAO = ShaderManager::Get()->GetPS(TEXT("PS_SSAO"));

		BuildRandomVectorMap();
	}

	void Pass_SSAO::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_ssaoOutput->OnResize(_device, screenWidth, screenHeight);

		_screenViewPort->OnResize(screenWidth, screenHeight);
	}

	void Pass_SSAO::Release()
	{
		_randomVectorMap.ReleaseAndGetAddressOf();

		_screenViewPort->Release();

		_ssaoOutput->Release();

		_meshScreen.reset();

		_vsSSAO.reset();

		_psSSAO.reset();

		__super::Release();
	}

	void Pass_SSAO::Render(std::shared_ptr<RenderTarget> depth, std::shared_ptr<RenderTarget> normal)
	{
		BeginRender();

		_vsSSAO->Update();

		_psSSAO->UpdateShaderResource(depth->GetSRV().Get(), SRV_REGISTER::t0);

		_psSSAO->UpdateShaderResource(normal->GetSRV().Get(), SRV_REGISTER::t1);

		_psSSAO->UpdateShaderResource(_randomVectorMap.Get(), SRV_REGISTER::t2);

		_psSSAO->Update();

		_meshScreen->Update();

		EndRender();
	}

	void Pass_SSAO::BeginRender()
	{
		_screenViewPort->SetViewPort(_deviceContext);

		_ssaoOutput->ClearRenderTargetView(_deviceContext);

		_deviceContext->OMSetRenderTargets(1, _ssaoOutput->GetRTV().GetAddressOf(), nullptr);
	}

	void Pass_SSAO::BuildRandomVectorMap()
	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = 256;
		texDesc.Height = 256;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.SysMemPitch = 256 * sizeof(XMCOLOR);

		XMCOLOR* color = new XMCOLOR[256 * 256];

		std::random_device rd;

		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> dist(0.f, 1.f);

		for (int i = 0; i < 256; ++i)
		{
			for (int j = 0; j < 256; ++j)
			{
				Vector3 v(dist(gen), dist(gen), dist(gen));

				color[i * 256 + j] = XMCOLOR(v.x, v.y, v.z, 0.0f);
			}
		}

		initData.pSysMem = color;

		ID3D11Texture2D* tex;

		_device->CreateTexture2D(&texDesc, &initData, &tex);

		if (tex != nullptr)
			_device->CreateShaderResourceView(tex, 0, &_randomVectorMap);

		tex->Release();

		delete[] color;
	}

	void Pass_SSAO::EndRender()
	{

	}
}