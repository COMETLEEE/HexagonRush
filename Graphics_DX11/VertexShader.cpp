#include "pch.h"
#include "VertexShader.h"

#include "Renderer.h"

namespace GraphicsEngine
{
	void VertexShader::Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint, const tstring& version, const D3D_SHADER_MACRO* defines)
	{
		_shaderType = SHADER_TYPE::VERTEX_SHADER;

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

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, _vertexShader.GetAddressOf());

		// ½ºÅ°´× ½¦ÀÌ´õ
		if (shaderName.find(TEXT("Skinning")) != shaderName.npos)
		{
			D3D11_INPUT_ELEMENT_DESC desc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"WEIGHTSS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 92, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 108, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEINDEXES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 124, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BONEINDEXESS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 140, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			// ÀÎÇ² ·¹ÀÌ¾Æ¿ô »ý¼º
			device->CreateInputLayout(desc, 11, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
		}
		// ÆÄÆ¼Å¬ ½¦ÀÌ´õ
		else if (shaderName.find(TEXT("Particle")) != shaderName.npos)
		{
			D3D11_INPUT_ELEMENT_DESC desc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TYPE",    0, DXGI_FORMAT_R32_UINT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			// ÀÎÇ² ·¹ÀÌ¾Æ¿ô »ý¼º
			device->CreateInputLayout(desc, 5, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
		}
		// ½ºÅÂÆ½ ¸Þ½¬ ½¦ÀÌ´õ
		else 
		{
			// ÀÏ´Ü ±âº» ÀÎÇ² ·¹ÀÌ¾Æ¿ôÀ¸·Î ¸¸µéÀÚ ..
			D3D11_INPUT_ELEMENT_DESC desc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			// ÀÎÇ² ·¹ÀÌ¾Æ¿ô »ý¼º
			device->CreateInputLayout(desc, 5, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
		}
	}

	void VertexShader::Release()
	{
		__super::Release();

		_inputLayout.ReleaseAndGetAddressOf();

		_vertexShader.ReleaseAndGetAddressOf();

		_errorBlob.ReleaseAndGetAddressOf();
	}

	void VertexShader::Update()
	{
		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		// ÀÎÇ² ·¹ÀÌ¾Æ¿ô Setting
		deviceContext->IASetInputLayout(_inputLayout.Get());

		// Vertex Shader Setting
		deviceContext->VSSetShader(_vertexShader.Get(), nullptr, NULL);

		for (auto& constantBuffer : _constantBuffers)
			deviceContext->VSSetConstantBuffers(static_cast<uint32>(constantBuffer->_slot), 1, constantBuffer->_cbGPU.GetAddressOf());

		for (auto& shaderResource : _shaderResources)
			deviceContext->VSSetShaderResources(static_cast<uint32>(shaderResource->_slot), 1, shaderResource->_srv.GetAddressOf());

		for (auto& sampler : _samplerStates)
			deviceContext->VSSetSamplers(static_cast<uint32>(sampler->_slot), 1, sampler->_samplerState.GetAddressOf());

		// COM Ref Count--
		deviceContext.Reset();
	}
}