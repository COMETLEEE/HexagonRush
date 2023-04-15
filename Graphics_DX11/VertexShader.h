#pragma once
#include "ShaderBase.h"

namespace GraphicsEngine
{
	class VertexShader : public ShaderBase<SHADER_TYPE::VERTEX_SHADER>
	{
	public:
		// 경로, 진입점, 버젼 (VS_Main / vs_5_0)
		virtual void Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint = TEXT("VS_Main"),
			const tstring& version = TEXT("vs_5_0"), const D3D_SHADER_MACRO* defines = nullptr) override;

		virtual void Release() override;

		virtual void Update() override;

	public:
		ComPtr<ID3D11InputLayout> _inputLayout;

		ComPtr<ID3D11VertexShader> _vertexShader;

		ComPtr<ID3DBlob> _errorBlob;
	};
}