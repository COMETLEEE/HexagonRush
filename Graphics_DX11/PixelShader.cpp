#include "pch.h"
#include "PixelShader.h"

#include "Renderer.h"

namespace GraphicsEngine
{
	void PixelShader::Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint, const tstring& version, const D3D_SHADER_MACRO* defines)
	{
		_shaderType = SHADER_TYPE::PIXEL_SHADER;

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
			::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
		}

		Renderer::Get()->GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, _pixelShader.GetAddressOf());
	}

	void PixelShader::Release()
	{
		__super::Release();

		_pixelShader.ReleaseAndGetAddressOf();

		_errorBlob.ReleaseAndGetAddressOf();
	}

	void PixelShader::Update()
	{
		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		deviceContext->PSSetShader(_pixelShader.Get(), nullptr, NULL);

		for (auto& constantBuffer : _constantBuffers)
			deviceContext->PSSetConstantBuffers(static_cast<uint32>(constantBuffer->_slot), 1, constantBuffer->_cbGPU.GetAddressOf());

		for (auto& shaderResource : _shaderResources)
			deviceContext->PSSetShaderResources(static_cast<uint32>(shaderResource->_slot), 1, shaderResource->_srv.GetAddressOf());

		for (auto& sampler : _samplerStates)
			deviceContext->PSSetSamplers(static_cast<uint32>(sampler->_slot), 1, sampler->_samplerState.GetAddressOf());

		deviceContext.Reset();
	}
}