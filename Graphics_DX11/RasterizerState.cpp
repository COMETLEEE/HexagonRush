#include "pch.h"
#include "RasterizerState.h"

namespace GraphicsEngine
{
	void RasterizerState::Initialize(ComPtr<ID3D11Device> device)
	{
		D3D11_RASTERIZER_DESC solidDesc;

		// Desc의 다른 원소들의 값은 기본값이 0일 때 보통의 기능을 냅니다. 따라서 Zeromemory로 초기화하고 실시합니다.
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;
		solidDesc.AntialiasedLineEnable = true;
		solidDesc.MultisampleEnable = true;
		solidDesc.DepthBias = 0.f;
		solidDesc.DepthBiasClamp = 0.0f;
		solidDesc.DepthClipEnable = true;
		solidDesc.ScissorEnable = false;
		solidDesc.SlopeScaledDepthBias = 0.f;

		HRESULT hr = device->CreateRasterizerState(&solidDesc, _solidRenderState.GetAddressOf());

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;
		wireframeDesc.AntialiasedLineEnable = true;
		wireframeDesc.MultisampleEnable = true;
		wireframeDesc.DepthBias = 0.f;
		wireframeDesc.DepthBiasClamp = 0.0f;
		wireframeDesc.DepthClipEnable = true;
		wireframeDesc.ScissorEnable = false;
		wireframeDesc.SlopeScaledDepthBias = 0.f;

		hr = device->CreateRasterizerState(&wireframeDesc, _wireFrameRenderState.GetAddressOf());

		D3D11_RASTERIZER_DESC solidNoneCullDesc;

		ZeroMemory(&solidNoneCullDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidNoneCullDesc.FillMode = D3D11_FILL_SOLID;
		solidNoneCullDesc.CullMode = D3D11_CULL_NONE;
		solidNoneCullDesc.FrontCounterClockwise = false;
		solidNoneCullDesc.DepthClipEnable = true;
		solidNoneCullDesc.AntialiasedLineEnable = true;
		solidNoneCullDesc.MultisampleEnable = true;
		solidNoneCullDesc.DepthBias = 0;
		solidNoneCullDesc.DepthBiasClamp = 0.0f;
		solidNoneCullDesc.DepthClipEnable = true;
		solidNoneCullDesc.ScissorEnable = false;
		solidNoneCullDesc.SlopeScaledDepthBias = 0.0f;

		hr = device->CreateRasterizerState(&solidNoneCullDesc, _solideRenderStateNoneCull.GetAddressOf());

		D3D11_RASTERIZER_DESC solidDesc2;

		// Desc의 다른 원소들의 값은 기본값이 0일 때 보통의 기능을 냅니다. 따라서 Zeromemory로 초기화하고 실시합니다.
		ZeroMemory(&solidDesc2, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc2.FillMode = D3D11_FILL_SOLID;
		solidDesc2.CullMode = D3D11_CULL_BACK;
		solidDesc2.FrontCounterClockwise = false;
		solidDesc2.DepthClipEnable = true;
		solidDesc2.AntialiasedLineEnable = true;
		solidDesc2.MultisampleEnable = true;
		solidDesc2.DepthBias = 100000;
		solidDesc2.DepthBiasClamp = 0.0f;
		solidDesc2.DepthClipEnable = true;
		solidDesc2.ScissorEnable = false;
		solidDesc2.SlopeScaledDepthBias = 1.0f;

		hr = device->CreateRasterizerState(&solidDesc2, _shadowSolidRenderState.GetAddressOf());

		D3D11_RASTERIZER_DESC wireframeDesc2;
		ZeroMemory(&wireframeDesc2, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc2.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc2.CullMode = D3D11_CULL_BACK;
		wireframeDesc2.FrontCounterClockwise = false;
		wireframeDesc2.DepthClipEnable = true;
		wireframeDesc2.AntialiasedLineEnable = true;
		wireframeDesc2.MultisampleEnable = true;
		wireframeDesc2.DepthBias = 100000;
		wireframeDesc2.DepthBiasClamp = 0.0f;
		wireframeDesc2.DepthClipEnable = true;
		wireframeDesc2.ScissorEnable = false;
		wireframeDesc2.SlopeScaledDepthBias = 1.0f;

		hr = device->CreateRasterizerState(&wireframeDesc2, _shadowWireFrameRenderState.GetAddressOf());
	}

	void RasterizerState::Release()
	{
		_solidRenderState.ReleaseAndGetAddressOf();

		_wireFrameRenderState.ReleaseAndGetAddressOf();

		_solideRenderStateNoneCull.ReleaseAndGetAddressOf();

		_shadowSolidRenderState.ReleaseAndGetAddressOf();

		_shadowWireFrameRenderState.ReleaseAndGetAddressOf();
	}

	void RasterizerState::SetSolidRenderState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->RSSetState(_solidRenderState.Get());
	}

	void RasterizerState::SetWireFrameRenderState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->RSSetState(_wireFrameRenderState.Get());
	}

	void RasterizerState::SetSolidRenderStateNoneCull(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->RSSetState(_solideRenderStateNoneCull.Get());
	}

	void RasterizerState::SetShadowSolidRenderState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->RSSetState(_shadowSolidRenderState.Get());
	}

	void RasterizerState::SetShadowWireFrameRenderState(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		deviceContext->RSSetState(_shadowWireFrameRenderState.Get());
	}
}