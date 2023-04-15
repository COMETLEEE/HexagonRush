#pragma once
#include "ShaderBase.h"

namespace GraphicsEngine
{
	class GeometryShader : public ShaderBase<SHADER_TYPE::GEOMETRY_SHADER>
	{
	public:
		// 경로, 진입점, 버젼
		virtual void Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint = TEXT("GS_Main"),
			const tstring& version = TEXT("gs_5_0"), const D3D_SHADER_MACRO* defines = nullptr) override;

		virtual void Release() override;

		virtual void Update() override;

	public:
		ComPtr<ID3D11GeometryShader> _geometryShader;

		ComPtr<ID3DBlob> _errorBlob;
	};
}