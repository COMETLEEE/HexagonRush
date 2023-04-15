#include "pch.h"
#include "SamplerState.h"

namespace GraphicsEngine
{
	void SamplerState::Initialize(ComPtr<ID3D11Device> device)
	{
		D3D11_SAMPLER_DESC samDesc;

		samDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		samDesc.MipLODBias = 0.f;
		samDesc.MaxAnisotropy = 2;
		samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc.BorderColor[0] = 0.f;
		samDesc.BorderColor[1] = 0.f;
		samDesc.BorderColor[2] = 0.f;
		samDesc.BorderColor[3] = 0.f;

		samDesc.MinLOD = -FLT_MAX;
		samDesc.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&samDesc, _wrapSamplerState.GetAddressOf());

		D3D11_SAMPLER_DESC samDesc2;

		samDesc2.Filter = D3D11_FILTER_ANISOTROPIC;
		samDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		samDesc2.MipLODBias = 0.f;
		samDesc2.MaxAnisotropy = 2;
		samDesc2.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc2.BorderColor[0] = 0.f;
		samDesc2.BorderColor[1] = 0.f;
		samDesc2.BorderColor[2] = 0.f;
		samDesc2.BorderColor[3] = 0.f;

		samDesc2.MinLOD = -FLT_MAX;
		samDesc2.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&samDesc2, _clampSamplerState.GetAddressOf());

		D3D11_SAMPLER_DESC samDesc3;

		samDesc3.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samDesc3.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc3.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samDesc3.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		samDesc3.MipLODBias = 0.f;
		samDesc3.MaxAnisotropy = 2;
		samDesc3.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samDesc3.BorderColor[0] = 0.f;
		samDesc3.BorderColor[1] = 0.f;
		samDesc3.BorderColor[2] = 0.f;
		samDesc3.BorderColor[3] = 0.f;

		samDesc3.MinLOD = -FLT_MAX;
		samDesc3.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&samDesc3, _pointSamplerState.GetAddressOf());

		D3D11_SAMPLER_DESC samDesc4;

		samDesc4.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		samDesc4.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samDesc4.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samDesc4.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

		samDesc4.MipLODBias = 0.f;
		samDesc4.MaxAnisotropy = 2;
		samDesc4.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		samDesc4.BorderColor[0] = 0.f;
		samDesc4.BorderColor[1] = 0.f;
		samDesc4.BorderColor[2] = 0.f;
		samDesc4.BorderColor[3] = 0.f;

		samDesc4.MinLOD = -FLT_MAX;
		samDesc4.MaxLOD = FLT_MAX;

		device->CreateSamplerState(&samDesc4, _comparisonSamplerState.GetAddressOf());
	}

	void SamplerState::Release()
	{
		_wrapSamplerState.ReleaseAndGetAddressOf();

		_clampSamplerState.ReleaseAndGetAddressOf();

		_pointSamplerState.ReleaseAndGetAddressOf();

		_comparisonSamplerState.ReleaseAndGetAddressOf();
	}
}