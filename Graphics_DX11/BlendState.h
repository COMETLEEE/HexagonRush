#pragma once

namespace GraphicsEngine
{
	class BlendState
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device);

		void Release();

		void SetBlendStateOff(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetSrcDestAlphaBlendState(ComPtr<ID3D11DeviceContext> deviceContext);

		void SetSrcAdditiveBlendState(ComPtr<ID3D11DeviceContext> deviceContext);
		
		void SetUIBlendState(ComPtr<ID3D11DeviceContext> deviceContext);

	public:
		// 각각의 알파에 맞는 가산 혼합
		ComPtr<ID3D11BlendState> _srcDestAlphaBlendState;

		// Dest에 Source를 해당 Alpha에 맞게 칠하는 가산혼합
		ComPtr<ID3D11BlendState> _srcAdditiveBlendState;

		// UI Blend State
		ComPtr<ID3D11BlendState> _uiBlendState;
	};
}