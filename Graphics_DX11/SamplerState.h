#pragma once

namespace GraphicsEngine
{
	class SamplerState
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device);

		void Release();

		ComPtr<ID3D11SamplerState> GetWrapSamplerState() { return _wrapSamplerState; }

		ComPtr<ID3D11SamplerState> GetClampSamplerState() { return _clampSamplerState; }

		ComPtr<ID3D11SamplerState> GetPointSamplerState() { return _pointSamplerState; }

		ComPtr<ID3D11SamplerState> GetComparisonSamplerState() { return _comparisonSamplerState; }

	public:
		ComPtr<ID3D11SamplerState> _wrapSamplerState;

		ComPtr<ID3D11SamplerState> _clampSamplerState;

		ComPtr<ID3D11SamplerState> _pointSamplerState;

		ComPtr<ID3D11SamplerState> _comparisonSamplerState;
	};
}