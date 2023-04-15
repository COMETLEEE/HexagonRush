#include "pch.h"
#include "ComputeShader.h"

namespace GraphicsEngine
{
	void ComputeShader::Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint /*= TEXT("CS_Main")*/, const tstring& version /*= TEXT("cs_5_0")*/, const D3D_SHADER_MACRO* defines /*= nullptr*/)
	{
		_shaderType = SHADER_TYPE::COMPUTE_SHADER;

		_shaderName = shaderName;

#if defined(DEBUG) || defined(_DEBUG)
		uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
		uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

		ComPtr<ID3DBlob> blob = nullptr;

		if (FAILED(::D3DCompileFromFile(StringHelper::ToWString(path).c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, StringHelper::ToString(entryPoint).c_str(), StringHelper::ToString(version).c_str(), compileFlag, 0, blob.GetAddressOf(), _errorBlob.GetAddressOf())))
		{
			::MessageBoxA(nullptr, "Compute Shader Create Failed !", nullptr, MB_OK);
		}

		Renderer::Get()->GetDevice()->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, _computeShader.GetAddressOf());
	}

	void ComputeShader::Release()
	{
		_computeShader.ReleaseAndGetAddressOf();

		_errorBlob.ReleaseAndGetAddressOf();

		__super::Release();
	}

	void ComputeShader::Update()
	{
		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		deviceContext->CSSetShader(_computeShader.Get(), nullptr, NULL);

		for (auto& constantBuffer : _constantBuffers)
			deviceContext->CSSetConstantBuffers(static_cast<uint32>(constantBuffer->_slot), 1, constantBuffer->_cbGPU.GetAddressOf());

		for (auto& shaderResource : _shaderResources)
			deviceContext->CSSetShaderResources(static_cast<uint32>(shaderResource->_slot), 1, shaderResource->_srv.GetAddressOf());

		for (auto& sampler : _samplerStates)
			deviceContext->CSSetSamplers(static_cast<uint32>(sampler->_slot), 1, sampler->_samplerState.GetAddressOf());

		deviceContext.Reset();
	}
}