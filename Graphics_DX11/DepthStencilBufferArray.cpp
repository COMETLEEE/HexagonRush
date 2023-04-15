#include "pch.h"
#include "DepthStencilBufferArray.h"

#include "CB_Structure.h"

namespace GraphicsEngine
{
	void DepthStencilBufferArray::Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight)
	{
		uint32 arrayCount = FRUSTUM_CASCADE_COUNT;

		D3D11_TEXTURE2D_DESC texDesc =
		{
			textureWidth, textureHeight, 1, arrayCount, DXGI_FORMAT_R32_TYPELESS, 1, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 0, 0
		};

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc =
		{
			DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2DARRAY, 0
		};

		dsvDesc.Texture2DArray.FirstArraySlice = 0;

		dsvDesc.Texture2DArray.ArraySize = arrayCount;

		dsvDesc.Texture2DArray.MipSlice = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc =
		{
			DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2DARRAY, 0, 0
		};

		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = arrayCount;
		srvDesc.Texture2DArray.MipLevels = 1;
		srvDesc.Texture2DArray.MostDetailedMip = 0;

		HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, _depthStencilBuffer.GetAddressOf());

		if (FAILED(hr))
			assert(false);

		hr = device->CreateDepthStencilView(_depthStencilBuffer.Get(), &dsvDesc, _depthStencilView.GetAddressOf());

		if (FAILED(hr))
			assert(false);

		hr = device->CreateShaderResourceView(_depthStencilBuffer.Get(), &srvDesc, _shaderResourceView.GetAddressOf());

		if (FAILED(hr))
			assert(false);
	}

	void DepthStencilBufferArray::Release()
	{
		_depthStencilBuffer.ReleaseAndGetAddressOf();

		_depthStencilView.ReleaseAndGetAddressOf();

		_shaderResourceView.ReleaseAndGetAddressOf();
	}

	void DepthStencilBufferArray::OnResize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight)
	{
		this->Release();

		Initialize(device, textureWidth, textureHeight);
	}

	void DepthStencilBufferArray::ClearDepthStencilView(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}