#pragma once

namespace GraphicsEngine
{
	class RasterizerState
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device);

		void Release();

		void SetSolidRenderState(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetWireFrameRenderState(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetSolidRenderStateNoneCull(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetShadowSolidRenderState(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetShadowWireFrameRenderState(ComPtr<ID3D11DeviceContext> deviceContext);

		ComPtr<ID3D11RasterizerState> GetSolidRenderState() { return _solidRenderState; }

		ComPtr<ID3D11RasterizerState> GetWireFrameRenderState() { return _wireFrameRenderState; }

		ComPtr<ID3D11RasterizerState> GetSolidRenderStateNoneCull() { return _solideRenderStateNoneCull; }

		ComPtr<ID3D11RasterizerState> GetShadowSolidRenderState() { return _shadowSolidRenderState; }

		ComPtr<ID3D11RasterizerState> GetShadowWireFrameRenderState() { return _shadowWireFrameRenderState; }

	private:
		ComPtr<ID3D11RasterizerState> _solidRenderState;

		ComPtr<ID3D11RasterizerState> _wireFrameRenderState;

		ComPtr<ID3D11RasterizerState> _solideRenderStateNoneCull;

		// Shader Map에 편향치를 준다.
		ComPtr<ID3D11RasterizerState> _shadowSolidRenderState;

		ComPtr<ID3D11RasterizerState> _shadowWireFrameRenderState;
	};
}