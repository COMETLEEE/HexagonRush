#include "pch.h"
#include "DepthStencilState.h"

namespace GraphicsEngine
{
	void DepthStencilState::Initialize(ComPtr<ID3D11Device> device)
	{
		// Z-버퍼링을 키는 뎁스 스텐실 스테이트를 생성해놓는다.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		// Stencil operations if pixel is front-facing. 
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing. 
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		device->CreateDepthStencilState(&depthStencilDesc, _depthEnableState.GetAddressOf());

		// Z-버퍼링을 끄는 뎁스 스텐실 스테이트를 생성해놓자.
		D3D11_DEPTH_STENCIL_DESC depthDisableStencilDesc;
		ZeroMemory(&depthDisableStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthDisableStencilDesc.DepthEnable = false;
		depthDisableStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDisableStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthDisableStencilDesc.StencilEnable = false;
		depthDisableStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthDisableStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		depthDisableStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisableStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisableStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisableStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthDisableStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisableStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisableStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisableStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		device->CreateDepthStencilState(&depthDisableStencilDesc, _depthDisableState.GetAddressOf());
	}

	void DepthStencilState::Release()
	{
		_depthEnableState.ReleaseAndGetAddressOf();

		_depthDisableState.ReleaseAndGetAddressOf();
	}

	void DepthStencilState::SetDepthEnableState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->OMSetDepthStencilState(_depthEnableState.Get(), NULL);
	}

	void DepthStencilState::SetDepthDisableState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->OMSetDepthStencilState(_depthDisableState.Get(), NULL);
	}
}