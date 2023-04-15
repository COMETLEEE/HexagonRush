#pragma once

namespace GraphicsEngine
{
	class DepthStencilState
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device);

		void Release();

		void SetDepthEnableState(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetDepthDisableState(ComPtr<ID3D11DeviceContext> deviceContext);

	public:
		ComPtr<ID3D11DepthStencilState> _depthEnableState;

		ComPtr<ID3D11DepthStencilState> _depthDisableState;
	};
}