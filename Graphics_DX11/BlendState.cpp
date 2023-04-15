#include "pch.h"
#include "BlendState.h"

namespace GraphicsEngine
{
	void BlendState::Initialize(ComPtr<ID3D11Device> device)
	{
		D3D11_BLEND_DESC desc1;

		ZeroMemory(&desc1, sizeof(D3D11_BLEND_DESC));

		desc1.RenderTarget[0].BlendEnable = TRUE;
		desc1.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc1.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
		desc1.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		// 그린 이미지의 Alpha 값을 지정하는 필드. 보통 변동이 없게 하기 위해 Source에 1, Dest에 Zero, OP에 Add를 넣어서 1로 만들어줌.
		desc1.RenderTarget[0].SrcBlendAlpha= D3D11_BLEND_ONE;
		desc1.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
		desc1.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc1.RenderTarget[0].RenderTargetWriteMask  = 0x0f;

		device->CreateBlendState(&desc1, _srcDestAlphaBlendState.GetAddressOf());

		D3D11_BLEND_DESC desc2;

		ZeroMemory(&desc2, sizeof(D3D11_BLEND_DESC));

		desc2.RenderTarget[0].BlendEnable = TRUE;
		desc2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc2.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&desc2, _srcAdditiveBlendState.GetAddressOf());

		D3D11_BLEND_DESC desc3;

		ZeroMemory(&desc3, sizeof(D3D11_BLEND_DESC));

		desc3.RenderTarget[0].BlendEnable = TRUE;
		desc3.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc3.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc3.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc3.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc3.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc3.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc3.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&desc3, _uiBlendState.GetAddressOf());
	}

	void BlendState::Release()
	{
		_srcDestAlphaBlendState.ReleaseAndGetAddressOf();

		_srcAdditiveBlendState.ReleaseAndGetAddressOf();

		_uiBlendState.ReleaseAndGetAddressOf();
	}

	void BlendState::SetBlendStateOff(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void BlendState::SetSrcDestAlphaBlendState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->OMSetBlendState(_srcDestAlphaBlendState.Get(), nullptr, 0xffffffff);
	}

	void BlendState::SetSrcAdditiveBlendState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->OMSetBlendState(_srcAdditiveBlendState.Get(), nullptr, 0xffffffff);
	}

	void BlendState::SetUIBlendState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->OMSetBlendState(_uiBlendState.Get(), nullptr, 0xffffffff);
	}
}