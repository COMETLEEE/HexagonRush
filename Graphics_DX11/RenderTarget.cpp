#include "pch.h"
#include "RenderTarget.h"

#include "Graphics_DX11.h"

namespace GraphicsEngine
{
	void RenderTarget::Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, 
		bool usingMSAA, DXGI_FORMAT textureFormat)
	{
		_textureFormat = textureFormat;

		_usingMSAA = usingMSAA;

		_textureWidth = textureWidth;

		_textureHeight = textureHeight;

		CreateAllViews(device, textureWidth, textureHeight, _usingMSAA, _textureFormat);

		// MSAA 적용된 렌더 타겟의 경우에는 텍스쳐를 Resolve해서 Shader Resource로 사용하기 위함. 
		if (usingMSAA)
			InitializeForResolvedTexture(device, textureWidth, textureHeight, _textureFormat);
	}

	void RenderTarget::OnResize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight)
	{
		Release();

		_textureWidth = textureWidth;

		_textureHeight = textureHeight;

		CreateAllViews(device, textureWidth, textureHeight, _usingMSAA, _textureFormat);

		if (_usingMSAA)
			InitializeForResolvedTexture(device, textureWidth, textureHeight, _textureFormat);
	}

	void RenderTarget::Release()
	{
		_renderTargetTexture.ReleaseAndGetAddressOf();

		_renderTargetView.ReleaseAndGetAddressOf();

		_shaderResourceView.ReleaseAndGetAddressOf();

		_unorderedAccessView.ReleaseAndGetAddressOf();

		_renderTargetTextureResolved.ReleaseAndGetAddressOf();

		_shaderResourceViewResolved.ReleaseAndGetAddressOf();
	}

	void RenderTarget::ClearRenderTargetView(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->ClearRenderTargetView(_renderTargetView.Get(), DirectX::Colors::Black);
	}

	void RenderTarget::InitializeForResolvedTexture(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, DXGI_FORMAT textureFormat)
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// RTT Desc 초기화
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		// RTT Desc 세팅
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = textureFormat;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		hr = device->CreateTexture2D(&textureDesc, NULL, _renderTargetTextureResolved.GetAddressOf());

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Create Render Target Failed !", nullptr, MB_OK);

		// SRV Desc 세팅
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(_renderTargetTextureResolved.Get(), &shaderResourceViewDesc, _shaderResourceViewResolved.GetAddressOf());

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Create Render Target Failed !", nullptr, MB_OK);
	}

	void RenderTarget::CreateResolveTexture(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		// deviceContext->ResolveSubresource(_renderTargetTextureResolved.Get(), 0, _renderTargetTexture.Get(), 0, _textureFormat);
	}

	void RenderTarget::CreateAllViews(ComPtr<ID3D11Device> device, uint32& textureWidth, uint32& textureHeight, bool usingMSAA, DXGI_FORMAT textureFormat)
	{
		// 1번 : 렌더 타겟으로 사용할 텍스쳐 자원을 만든다.
		HRESULT hr;

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		// RTT Desc 세팅
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = textureFormat;

		if (usingMSAA)
		{
			// 멀티 샘플링
			textureDesc.SampleDesc.Count = 4;
			textureDesc.SampleDesc.Quality = 0;
		}
		else
		{
			// 단일 샘플링 
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
		}

		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		hr = device->CreateTexture2D(&textureDesc, NULL, _renderTargetTexture.GetAddressOf());

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Create Render Target Failed !", nullptr, MB_OK);

		// RTV Desc 세팅
		rtvDesc.Format = textureDesc.Format;

		if (usingMSAA)
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		else
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		rtvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateRenderTargetView(_renderTargetTexture.Get(), &rtvDesc, _renderTargetView.GetAddressOf());

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Create Render Target Failed !", nullptr, MB_OK);

		// SRV Desc 세팅
		shaderResourceViewDesc.Format = textureDesc.Format;

		if (usingMSAA)
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		else
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(_renderTargetTexture.Get(), &shaderResourceViewDesc, _shaderResourceView.GetAddressOf());

		// TO DO : UAV
	}
}