#include "pch.h"
#include "Renderer.h"

#include "Graphics_RenderingData_3D.h"
#include "CB_Structure.h"

#include "Device.h"
#include "SwapChain.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

#include "ShaderManager.h"
#include "ResourceManager.h"

#include "Texture.h"
#include "Mesh.h"
#include "Font.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include "Pass_Shadow.h"
#include "Pass_Deferred.h"
#include "Pass_SSAO.h"
#include "Pass_GaussianBlur.h"
#include "Pass_EdgePreserveBlur.h"
#include "Pass_Sampling.h"
#include "Pass_Lighting.h"
#include "Pass_CameraMotionBlur.h"
#include "Pass_Bloom.h"
#include "Pass_FXAA.h"
#include "Pass_Final.h"

#include "ParticleProcessor.h"
#include "PostProcessor.h"

std::shared_ptr<GraphicsEngine::Renderer> GraphicsEngine::Renderer::_renderer = nullptr;

namespace GraphicsEngine
{
	std::shared_ptr<GraphicsEngine::Renderer> Renderer::Get()
	{
		if (_renderer == nullptr)
			_renderer = std::make_shared<GraphicsEngine::Renderer>();

		return _renderer;
	}

	void Renderer::Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		_info._screenWidth = screenWidth;
		_info._screenHeight = screenHeight;
		_info._hWnd = hWnd;
		_info._enable4xMSAA = true;

		_device = std::make_shared<Device>();
		_swapChain = std::make_shared<SwapChain>();
		_depthStencilState = std::make_shared<DepthStencilState>();
		_rasterizerState = std::make_shared<RasterizerState>();
		_samplerState = std::make_shared<SamplerState>();
		_blendState = std::make_shared<BlendState>();
		_spriteBatcher = std::make_shared<SpriteBatcher>();

		_device->Initialize(_info);
		_swapChain->Initialize(_info, _device, screenWidth, screenHeight);
		_depthStencilState->Initialize(_device->GetDevice());
		_rasterizerState->Initialize(_device->GetDevice());
		_samplerState->Initialize(_device->GetDevice());
		_blendState->Initialize(_device->GetDevice());
		_spriteBatcher->Initialize(_device->GetDeviceContext());

		// 디버그 개체 이름을 설정합니다.
		D3D11SetDebugObjectName(_device->GetDeviceContext().Get(), "ImmediateContext");
		DXGISetDebugObjectName(_swapChain->GetSwapChain().Get(), "SwapChain");

		// 그래픽 카드에 데이터를 꽂을 때 매개체로 사용할 녀석들
		_cbPerFrame = std::make_shared<PerFrameConstantBuffer>();
		_cbPerFrame->Initialize(_device->GetDevice());
	}

	void Renderer::InitializeForPass()
	{
		// ------------------ Pass ------------------
		_shadowPass = std::make_shared<Pass_Shadow>();
		_shadowPass->Initialize(_info._screenWidth, _info._screenHeight);

		_deferredPass = std::make_shared<Pass_Deferred>();
		_deferredPass->Initialize(_info._screenWidth, _info._screenHeight);

		_samplingPass = std::make_shared<Pass_Sampling>();
		_samplingPass->Initialize(_info._screenWidth, _info._screenHeight);		

		_lightingPass = std::make_shared<Pass_Lighting>();
		_lightingPass->Initialize(_info._screenWidth, _info._screenHeight);
	
		// Particle Processing
		_particleProcessor = std::make_shared<ParticleProcessor>();
		_particleProcessor->Initialize();

		// Post Processing
		_postProcessor = std::make_shared<PostProcessor>();
		_postProcessor->Initialize(_lightingPass, _deferredPass,
			_info._screenWidth, _info._screenHeight);



		// SSAO 봉인
		/*_edgePreserveBlurPass = std::make_shared<Pass_EdgePreserveBlur>();
		_edgePreserveBlurPass->Initialize(_info._screenWidth, _info._screenHeight);

		_ssaoPass = std::make_shared<Pass_SSAO>();
		_ssaoPass->Initialize(_info._screenWidth, _info._screenHeight);*/
	}

	void Renderer::Release()
	{
		_device->Release();

		_swapChain->Release();

		_depthStencilState->Release();

		_rasterizerState->Release();

		_samplerState->Release();

		_blendState->Release();

		_spriteBatcher->Release();

		for (int i = 0; i < _renderQueue.size() ; i++)
			_renderQueue.front().reset();

		_cbPerFrame->Release();

		// ------------------ Pass ------------------
		_shadowPass->Release();

		_deferredPass->Release();

		_samplingPass->Release();

		_lightingPass->Release();

		// ------------------ Processor ------------------
		_particleProcessor->Release();

		_postProcessor->Release();
		
		// SSAO 봉인
		/*_ssaoPass->Release();
		_edgePreserveBlurPass->Release();*/
	}

	void Renderer::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		_info._screenWidth = screenWidth;
		_info._screenHeight = screenHeight;

		// ------------------ Pass ------------------
		_shadowPass->OnResize(screenWidth * 4, screenHeight * 4);
		_deferredPass->OnResize(screenWidth, screenHeight);
		_samplingPass->OnResize(screenWidth, screenHeight);
		_lightingPass->OnResize(screenWidth, screenHeight);

		_postProcessor->OnResize(screenWidth, screenHeight);

		// SSAO 봉인 (아무리 해도 예쁘게 나오지를 않음 ..)
		/*_ssaoPass->OnResize(screenWidth, screenHeight);
		_edgePreserveBlurPass->OnResize(screenWidth, screenHeight);*/
	}

	void Renderer::PostRenderingData_UI(std::shared_ptr<::RenderingData_UI>& renderingData)
	{
		_renderQueueUI.push_back(renderingData);
	}

	void Renderer::PostRenderingData_3D(std::shared_ptr<::RenderingData_3D>& renderingData)
	{
		_renderQueue.push_back(renderingData);
	}

	void Renderer::PostRenderingData_Particle(std::shared_ptr<::RenderingData_Particle>& renderingData)
	{
		_renderQueueParticle.push_back(renderingData);
	}

	void Renderer::PostTextData(std::shared_ptr<::TextData> textData)
	{
		_textDataQueue.push_back(textData);
	}

	void Renderer::ExecuteRenderQueue()
	{
		// 그림자 맵을 그린다.
		_shadowPass->Render(_cbPerFrame, _renderQueue);
		
		// 현재 씬의 G-Buffer 및 기타 맵을 뽑아낸다.
		_deferredPass->Render(_renderQueue);

#pragma region SSAO 봉인소
		//// SSAO : 화면과 동일한 사이즈로 그리자 .. 최대한 품질을 높이자 ..!
		//_ssaoPass->Render(_deferredPass->GetDepth(), _deferredPass->GetNormal());

		//// 위의 SSAO Map의 사이즈를 절반으로 샘플링 !!
		//_samplingPass->Render(_ssaoPass->_ssaoOutput, SAMPLING_OPTION::Half);

		//// 위의 결과물을 Blur해서 노이즈 제거 (4번 정도는 해야 봐줄만 하다 ..)
		//_edgePreserveBlurPass->Render(_deferredPass->GetDepth(), _deferredPass->GetNormal(), _samplingPass->_sampleHalf, BLUR_MAX_COUNT);
#pragma endregion

		// Emissive Effect !
		_postProcessor->_bloomPass->Render(_deferredPass->GetEmissive());

		// 빛 연산을 실시한다.
		_lightingPass->Render(_deferredPass->_gBuffers, _shadowPass->_shadowDepth, _shadowPass->_cascadeShadowMap, nullptr,
			_shadowPass->_cbLightViewProjs.get(), _shadowPass->_cbDirLightViewProj.get(), _postProcessor->_bloomPass->GetOutput());

		// 파티클 렌더링
		_particleProcessor->Render(_lightingPass->GetOutput(), _renderQueueParticle);

		// 마지막 포스트 프로세싱, UI, Texture 렌더링 및 스왑체인 플리핑까지 ..!
		_postProcessor->Render(_cbPerFrame->_currPostProcessOption, _textDataQueue, _renderQueueUI);

		// 렌더 큐 및 UI 렌더큐, 텍스트 데이터 큐 초기화 ! => CPU 단에게 작업을 할 수 있도록 넘긴다.
		_renderQueue.clear();
		_renderQueueUI.clear();
		_renderQueueParticle.clear();
		_textDataQueue.clear();
	}

	void Renderer::ExecuteForPerFrameData(std::shared_ptr<::PerFrameData> perframeData)
	{
		// 엔진 내부에서 공용으로 사용할 PerFrame ConstantBuffer를 최신화합니다.
		_cbPerFrame->UpdatePerFrameConstantBuffer(_device->GetDeviceContext(), perframeData);

		// 파티클 프로세서에서 물리 시뮬레이션을 구현하기 위해 DeltaTime을 건내줍니다.
		_particleProcessor->SetCurrentFrameDeltaTime(perframeData->_deltaTime);
	}

	void PerFrameConstantBuffer::Initialize(ComPtr<ID3D11Device> device)
	{
		_cbPerFrameCPU = std::make_shared<CB_PERFRAME>();

		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(CB_PERFRAME);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = NULL;
		cbDesc.StructureByteStride = NULL;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = _cbPerFrameCPU.get();
		initData.SysMemPitch = NULL;
		initData.SysMemSlicePitch = NULL;

		Renderer::Get()->GetDevice()->CreateBuffer(&cbDesc, &initData, _cbPerFrameGPU.GetAddressOf());
	}

	void PerFrameConstantBuffer::UpdatePerFrameConstantBuffer(ComPtr<ID3D11DeviceContext> deviceContext, std::shared_ptr<::PerFrameData> perframeData)
	{
		// Current Frame Post Processing Option
		memcpy(&_currPostProcessOption, &perframeData->_postProcessOption, sizeof(POSTPROCESS_OPTION));

		// --------------- Main Camera Data ---------------
		// 일단 빨리 모션 블러를 보고 싶어서 여기에 놔둔다. 이 친구를 어디서 관리할지, CB를 따로 분리할지는 .. 나중의 나에게
		memcpy(&_cbPerFrameCPU->_cameraInfo, perframeData->_cameraInfo.get(), sizeof(CameraInfo));
		// --------------- Main Camera Data ---------------

		// Directional Light
		_cbPerFrameCPU->_directionalLightCount = static_cast<uint32>(perframeData->_directionalLightInfos.size());

		for (size_t i = 0; i < _cbPerFrameCPU->_directionalLightCount ; i++)
			memcpy(_cbPerFrameCPU->_directionalLightInfos + i, perframeData->_directionalLightInfos[i].get(), sizeof(DirectionalLightInfo));

		// Spot Light
		_cbPerFrameCPU->_spotLightCount = static_cast<uint32>(perframeData->_spotLightInfos.size());

		for (size_t i = 0; i < _cbPerFrameCPU->_spotLightCount; i++)
			memcpy(_cbPerFrameCPU->_spotLightInfos + i, perframeData->_spotLightInfos[i].get(), sizeof(SpotLightInfo));

		// Point Light
		_cbPerFrameCPU->_pointLightCount = static_cast<uint32>(perframeData->_pointLightInfos.size());

		for (size_t i = 0; i < _cbPerFrameCPU->_pointLightCount; i++)
			memcpy(_cbPerFrameCPU->_pointLightInfos + i, perframeData->_pointLightInfos[i].get(), sizeof(PointLightInfo));

		// SSAO
		memcpy(&_cbPerFrameCPU->_ssaoInfo, perframeData->_ssaoInfo.get(), sizeof(SSAOInfo));

		D3D11_MAPPED_SUBRESOURCE mappedResource;

		deviceContext->Map(_cbPerFrameGPU.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		CB_PERFRAME* dataPtr = (CB_PERFRAME*)mappedResource.pData;

		memcpy(dataPtr, _cbPerFrameCPU.get(), sizeof(CB_PERFRAME));

		deviceContext->Unmap(_cbPerFrameGPU.Get(), 0);

		// b0번 레지스터는 Per Frame Data용 Register이다.
		deviceContext->VSSetConstantBuffers(static_cast<uint32>(CBV_REGISTER::b0), 1, _cbPerFrameGPU.GetAddressOf());
		deviceContext->PSSetConstantBuffers(static_cast<uint32>(CBV_REGISTER::b0), 1, _cbPerFrameGPU.GetAddressOf());
		deviceContext->GSSetConstantBuffers(static_cast<uint32>(CBV_REGISTER::b0), 1, _cbPerFrameGPU.GetAddressOf());

		perframeData->_directionalLightInfos.clear();
		perframeData->_spotLightInfos.clear();
		perframeData->_pointLightInfos.clear();
	}
}