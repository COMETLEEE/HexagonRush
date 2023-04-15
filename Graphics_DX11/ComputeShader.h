#pragma once
#include "ShaderBase.h"

namespace GraphicsEngine
{
	class ComputeShader : public ShaderBase<SHADER_TYPE::COMPUTE_SHADER>
	{
	public:
		// 경로, 진입점, 버젼
		virtual void Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint = TEXT("CS_Main"),
			const tstring& version = TEXT("cs_5_0"), const D3D_SHADER_MACRO* defines = nullptr) override;

		virtual void Release() override;

		virtual void Update() override;

	public:
		ComPtr<ID3D11ComputeShader> _computeShader;

		ComPtr<ID3DBlob> _errorBlob;
	};
}