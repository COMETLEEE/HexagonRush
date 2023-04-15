#include "pch.h"
#include "GeometryShader.h"

namespace GraphicsEngine
{
	void GeometryShader::Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint, const tstring& version, const D3D_SHADER_MACRO* defines)
	{
		_shaderType = SHADER_TYPE::GEOMETRY_SHADER;

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

		// Stream Out Geometry Shader
		if (shaderName.find(TEXT("StreamOut")) != shaderName.npos)
		{
			//define the system output declaration entry, i.e. what will be written in the SO
			D3D11_SO_DECLARATION_ENTRY pDecl[5] =
			{
				//position, semantic name, semantic index, start component, component count, output slot
				{0,"POSITION", 0, 0, 3, 0 }, // output first 3 components of SPEED
				{0, "VELOCITY", 0, 0, 3, 0 }, // output first 3 components of "POSITION"
				{0, "SIZE", 0, 0, 2, 0 }, // output first 2 components of SIZE
				{0, "AGE", 0, 0, 1, 0 }, // output AGE
				{0, "TYPE",0, 0, 1, 0 } // output TYPE
			};

			UINT strides[1] = { sizeof(Particle) };

			Renderer::Get()->GetDevice()->CreateGeometryShaderWithStreamOutput(blob->GetBufferPointer(), blob->GetBufferSize(), pDecl,
				5, strides, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, _geometryShader.GetAddressOf());
		}
		else
		{
			Renderer::Get()->GetDevice()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, _geometryShader.GetAddressOf());
		}
	}

	void GeometryShader::Release()
	{
		_geometryShader.ReleaseAndGetAddressOf();

		_errorBlob.ReleaseAndGetAddressOf();

		__super::Release();
	}

	void GeometryShader::Update()
	{
		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		deviceContext->GSSetShader(_geometryShader.Get(), nullptr, NULL);

		for (auto& constantBuffer : _constantBuffers)
			deviceContext->GSSetConstantBuffers(static_cast<uint32>(constantBuffer->_slot), 1, constantBuffer->_cbGPU.GetAddressOf());

		for (auto& shaderResource : _shaderResources)
			deviceContext->GSSetShaderResources(static_cast<uint32>(shaderResource->_slot), 1, shaderResource->_srv.GetAddressOf());

		for (auto& sampler : _samplerStates)
			deviceContext->GSSetSamplers(static_cast<uint32>(sampler->_slot), 1, sampler->_samplerState.GetAddressOf());

		deviceContext.Reset();
	}
}
