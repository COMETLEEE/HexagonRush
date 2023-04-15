#pragma once
#include "ShaderBase.h"

namespace GraphicsEngine
{
	class PixelShader : public ShaderBase<SHADER_TYPE::PIXEL_SHADER>
	{
	public:
		virtual void Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint = TEXT("PS_Main"),
			const tstring& version = TEXT("ps_5_0"), const D3D_SHADER_MACRO* defines = nullptr) override;

		virtual void Release() override;

		virtual void Update() override;

	public:
		ComPtr<ID3D11PixelShader> _pixelShader;

		ComPtr<ID3DBlob> _errorBlob;
	};
}