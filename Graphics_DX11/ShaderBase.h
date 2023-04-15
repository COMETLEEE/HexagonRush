#pragma once
#include "Renderer.h"
#include "ResourceBase.h"
#include "CB_Structure.h"

namespace GraphicsEngine
{
	// 슬롯을 사용해서 컨스턴트 버퍼들을 모두 지정해줄 수 있도록 설계하자
	struct ConstantBuffer
	{
		ComPtr<ID3D11Buffer> _cbGPU;

		CBV_REGISTER _slot;
	};

	struct ShaderResource
	{
		ComPtr<ID3D11ShaderResourceView> _srv;

		SRV_REGISTER _slot;
	};

	struct Sampler
	{
		ComPtr<ID3D11SamplerState> _samplerState;

		SAM_REGISTER _slot;
	};

	// 사용하는 Constant Buffer의 타입을 받는다.
	template <SHADER_TYPE ShaderType>
	class ShaderBase
	{
	public:
		virtual void Initialize(const tstring& path, const tstring& shaderName, const tstring& entryPoint,
			const tstring& version, const D3D_SHADER_MACRO* defines) abstract;

		virtual void Update() abstract;

		virtual void Release();

	public:
		inline tstring GetName() { return _shaderName; }

		inline SHADER_TYPE GetType() { return _shaderType; }

		// 이 쉐이더 파일에 필요한 컨스턴트 버퍼를 추가합니다.
		template <typename TConstantBuffer>
		void CreateConstantBuffer(CBV_REGISTER slot)
		{
			std::shared_ptr<ConstantBuffer> cb = std::make_shared<ConstantBuffer>();

			cb->_slot = slot;

			std::shared_ptr<TConstantBuffer> temp = std::make_shared<TConstantBuffer>();

			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(TConstantBuffer);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = NULL;
			cbDesc.StructureByteStride = NULL;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = temp.get();
			initData.SysMemPitch = NULL;
			initData.SysMemSlicePitch = NULL;

			Renderer::Get()->GetDevice()->CreateBuffer(&cbDesc, &initData, cb->_cbGPU.GetAddressOf());

			temp.reset();

			_constantBuffers.push_back(cb);
		}

		// 이 쉐이더 파일에 있는 컨스턴트 버퍼의 내용을 업데이트합니다.
		template <typename TConstantBuffer>
		void UpdateConstantBuffer(TConstantBuffer* srcbuffer, CBV_REGISTER slot)
		{
			// 0번 레지스터는 Per-Frame 버퍼로 고정 => 따로 쉐이더에서 관리해주지 않는다.
			std::shared_ptr<ConstantBuffer> cb = _constantBuffers[static_cast<uint32>(slot) - 1];

			D3D11_MAPPED_SUBRESOURCE mappedResource;

			Renderer::Get()->GetDeviceContext()->Map(cb->_cbGPU.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			TConstantBuffer* dataPtr = (TConstantBuffer*)mappedResource.pData;

			memcpy(dataPtr, srcbuffer, sizeof(TConstantBuffer));

			Renderer::Get()->GetDeviceContext()->Unmap(cb->_cbGPU.Get(), 0);
		}

		// 쉐이더 리소스를 연결할 수 있도록 공간을 생성합니다. (수동 기어)
		void CreateShaderResource(SRV_REGISTER slot)
		{
			std::shared_ptr<ShaderResource> newSR = std::make_shared<ShaderResource>();

			newSR->_slot = slot;

			_shaderResources.push_back(newSR);
		}

		// 이 쉐이더 코드에서 사용할 쉐이더 리소스 뷰를 해당 슬롯에 업데이트할 수 있도록 설정합니다.
		void UpdateShaderResource(ComPtr<ID3D11ShaderResourceView> shaderResourceView, SRV_REGISTER slot)
		{
			// 같은 슬롯인 녀석을 업데이트합니다.
			for (auto& sr : _shaderResources)
				if (static_cast<uint32>(sr->_slot) == static_cast<uint32>(slot))
					sr->_srv = shaderResourceView;
		}

		void CreateSamplerState(SAM_REGISTER slot)
		{
			std::shared_ptr<Sampler> newSS = std::make_shared<Sampler>();

			newSS->_slot = slot;

			_samplerStates.push_back(newSS);
		}

		// 근데 이건 특수한 경우 아니면 쉐이더 로드 후 지정하는 것 외에는 사용할 일이 없을듯
		void UpdateSamplerState(ComPtr<ID3D11SamplerState> samplerState, SAM_REGISTER slot)
		{
			for (auto& sam: _samplerStates)
				if (static_cast<uint32>(sam->_slot) == static_cast<uint32>(slot))
					sam->_samplerState = samplerState;
		}

	protected:
		std::vector<std::shared_ptr<ConstantBuffer>> _constantBuffers;

		std::vector<std::shared_ptr<ShaderResource>> _shaderResources;

		std::vector<std::shared_ptr<Sampler>> _samplerStates;

		SHADER_TYPE _shaderType;

		tstring _shaderName;
	};

	template <SHADER_TYPE ShaderType>
	void GraphicsEngine::ShaderBase<ShaderType>::Release()
	{
		for (auto& constantBuffer : _constantBuffers)
		{
			constantBuffer->_cbGPU.Reset();
		}
		
		for (auto& shaderResource : _shaderResources)
		{
			shaderResource->_srv.Reset();
		}

		for (auto& samplerState : _samplerStates)
		{
			samplerState->_samplerState.Reset();
		}
	}
}