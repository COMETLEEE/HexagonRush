#include "pch.h"
#include "ParticleProcessor.h"

#include "BlendState.h"
#include "DepthStencilState.h"

#include "ResourceManager.h"
#include "ShaderManager.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"

#include "Texture.h"

#include "Pass_Deferred.h"

namespace GraphicsEngine
{
	uint64 GraphicsEngine::ParticleSystem::_nextID = 0;

	void ParticleProcessor::Initialize()
	{
		_deviceContext = Renderer::Get()->GetDeviceContext();

		_depthStencilState = Renderer::Get()->GetDepthStencilState();

		_blendState = Renderer::Get()->GetBlendState();

		_gsStreamOutParticleCircle = ShaderManager::Get()->GetGS(TEXT("GS_Particle_StreamOut_Circle"));

		_gsStreamOutParticleSphere = ShaderManager::Get()->GetGS(TEXT("GS_Particle_StreamOut_Sphere"));

		_gsStreamOutOnlyDelete = ShaderManager::Get()->GetGS(TEXT("GS_Particle_StreamOut_OnlyDelete"));

		_gsDraw = ShaderManager::Get()->GetGS(TEXT("GS_Particle_Draw"));

		_vsStreamOut = ShaderManager::Get()->GetVS(TEXT("VS_Particle_StreamOut"));

		_vsDraw = ShaderManager::Get()->GetVS(TEXT("VS_Particle_Draw"));

		_psDraw = ShaderManager::Get()->GetPS(TEXT("PS_Particle_Draw"));
	
		// 참조용 Pass
		_deferredPassRef = Renderer::Get()->GetDeferredPass();

		// Key : ID => Value : 그래픽스 엔진에서 회전시키는 ParticleSyatem 객체
		_systemsIDMap = std::map<uint64, std::shared_ptr<ParticleSystem>>();

		// 쉐이더 코드 내에서 난수로 사용할 텍스쳐
		CreateRandomTextureSRV();
	}

	void ParticleProcessor::Release()
	{
		_deviceContext.Reset();

		_gsStreamOutParticleCircle.reset();

		_gsStreamOutParticleSphere.reset();

		_gsStreamOutOnlyDelete.reset();

		_gsDraw.reset();

		_vsStreamOut.reset();

		_vsDraw.reset();

		_psDraw.reset();

		// Pass Ref Reset
		_deferredPassRef.reset();

		_randVecTexSRV.ReleaseAndGetAddressOf();

		// 남아있는 시스템을 릴리즈한다.
		for (auto& system : _systemsIDMap)
			system.second->Release();
	}

	void ParticleProcessor::Render(std::shared_ptr<RenderTarget> targetScreen,
		std::vector<std::shared_ptr<RenderingData_Particle>> renderQueueParticles)
	{
		// 1. 받은 정보의 Particle들에 대하여 그린다.
		// (오브젝트가 존재하는 한 계속 발생하는 Loop이거나, 처음 온 데이터)
		for (auto& particleData : renderQueueParticles)
		{
			// 이번 프레임에 시작한 녀석이다.
			if (particleData->_initInfo->_firstRun)
			{
				std::shared_ptr<ParticleSystem> newSys = std::make_shared<ParticleSystem>();

				// IsLoop, Duration, TotalTime 등 .. 
				newSys->Initialize(particleData);

				UpdateConstantBufferOfSystem(particleData, newSys);

				UpdateShaderResourceOfSystem(particleData, newSys);

				UpdateBufferOfShader(newSys);

				FirstRunOfSystem(newSys);

				particleData->_initInfo->_firstRun = false;

				particleData->_initInfo->_particlePlayID = newSys->_systemPlayID;

				_systemsIDMap.insert({ newSys->_systemPlayID , newSys });

				newSys->_totalTime += _currFramedeltaTime;

				newSys->_isThisFrameUpdated = true;
			}
			else
			{
				std::shared_ptr<ParticleSystem> system = GetParticleSystem(particleData->_initInfo->_particlePlayID);

				if (system == nullptr)
					continue;

				UpdateConstantBufferOfSystem(particleData, system);

				UpdateShaderResourceOfSystem(particleData, system);

				UpdateBufferOfShader(system);

				ContinuousRunOfSystem(system);

				system->_totalTime += _currFramedeltaTime;

				system->_isThisFrameUpdated = true;
			}
		}

#pragma region 구버전
		//// 2. System 中 Loop가 아니면 여기 들어오고 끝남. Duration과 Total Time으로 컨트롤해서 넘었으면 삭제
		//// 3. System 中 Loop인데 만약 이번 프레임에 정보가 안왔다면 ? => Stop 호출했다는 것. 삭제해주자.
		//for (auto iter = _systemsIDMap.begin() ;  iter != _systemsIDMap.end() ;)
		//{
		//	std::shared_ptr<ParticleSystem> system = iter->second;

		//	// ㅋㅋㅋㅋ
		//	if ((!system->_isLoop) && (system->_duration <= system->_totalTime))
		//	{
		//		system->Release();

		//		_systemsIDMap.erase(iter++);
		//	}
		//	// 루프 아닌데 시간은 아직 덜 됨. => 업데이트
		//	else if (!system->_isLoop)
		//	{
		//		// 헤응 ..
		//		UpdateBufferOfShader(system);

		//		ContinuousRunOfSystem(system);

		//		system->_totalTime += _currFramedeltaTime;

		//		system->_isThisFrameUpdated = true;

		//		++iter;
		//	}
		//	// 기존에 루프 시스템이였지만 1프레임 전에 꺼진 녀석
		//	/*else if ((!system->_isLoop) && (system->_isLoopedSystem))
		//	{
		//		system->_totalTime += _currFramedeltaTime;

		//		system->_isThisFrameUpdated = true;

		//		++iter;
		//	}*/
		//	// Loop System인데 이번 프레임에 업데이트 되지 않았다면 ? => Stop => 삭제
		//	// 가 아닌 .. LifeSpan만큼 삭제는 하지 않지만 .. 더 생성은 하지 말자. (유니티 어떻게 되어있는지는 모-름)
		//	else if ((system->_isLoop) && (!system->_isThisFrameUpdated))
		//	{
		//		/*system->_isLoop = false;

		//		system->_duration = system->_totalTime + system->_cbStreamOut._lifeSpan;
		//		
		//		++iter;*/

		//		// system->_isLoopedSystem = true;

		//		// => 위의 기능을 원하는 체크 옵션이 필요할 것 같다.

		//		system->Release();

		//		_systemsIDMap.erase(iter++);
		//	}
		//	// 아무 문제 없다면 모두 프레임 업데이트 여부 초기화해줍시다.
		//	else
		//	{
		//		system->_isThisFrameUpdated = false;

		//		++iter;
		//	}
		//}
#pragma endregion

#pragma region 신버전
		// 2. System 中 Loop가 아니면 여기 들어오고 끝남. Duration과 Total Time으로 컨트롤해서 넘었으면 삭제
		// 3. System 中 Loop인데 만약 이번 프레임에 정보가 안왔다면 ? => Stop 호출했다는 것. 삭제해주자.
		for (auto iter = _systemsIDMap.begin(); iter != _systemsIDMap.end();)
		{
			std::shared_ptr<ParticleSystem> system = iter->second;

			if ((system->_isFinalPhaseOfSystem) && (system->_duration > system->_totalTime))
			{
				UpdateBufferOfShader(system);

				OnlyDeleteAndDrawRunOfSystem(system);

				system->_totalTime += _currFramedeltaTime;

				system->_isThisFrameUpdated = true;

				++iter;

				continue;
			}
			else if ((system->_isFinalPhaseOfSystem) && (system->_duration <= system->_totalTime))
			{
				// 지우기 
				system->Release();

				_systemsIDMap.erase(iter++);

				continue;
			}

			// 루프 시스템 아닌데 시간이 다 되었다 ! => 파이널 페이즈로 진입하자. (라이프 스팬만큼만 더 시뮬레이션, 노 생성)
			if ((!system->_isLoop) && (system->_duration <= system->_totalTime))
			{
				system->_isFinalPhaseOfSystem = true;

				// 파티클의 수명보다 좀 더 업데이트를 돌린다.
				system->_duration = system->_totalTime + system->_cbStreamOut._lifeSpan;

				++iter;
			}
			// 루프 아닌데 시간은 아직 덜 됨. => 업데이트
			else if (!system->_isLoop)
			{
				// 헤응 ..
				UpdateBufferOfShader(system);

				ContinuousRunOfSystem(system);

				system->_totalTime += _currFramedeltaTime;

				system->_isThisFrameUpdated = true;

				++iter;
			}
			
			// 루프인데 이제 메시지가 안옴. 중단 => Final Phase로 들어간다.
			else if ((system->_isLoop) && (!system->_isThisFrameUpdated))
			{
				UpdateBufferOfShader(system);

				OnlyDeleteAndDrawRunOfSystem(system);

				system->_isFinalPhaseOfSystem = true;

				system->_duration = system->_totalTime + system->_cbStreamOut._lifeSpan;

				++iter;

				// => 위의 기능을 원하는 체크 옵션이 필요할 것 같다.
			}
			// 아무 문제 없다면 모두 프레임 업데이트 여부 초기화해줍시다.
			else
			{
				system->_isThisFrameUpdated = false;

				++iter;
			}
		}
#pragma endregion
		// NULL To Un-Binding
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

		_deviceContext->GSSetShaderResources(0, 1, nullSRV);
	}

	void ParticleProcessor::CreateRandomTextureSRV()
	{
		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		std::random_device rd;

		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> dist(-1.f, 1.f);

		// 
		// Create the random data.
		//
		Vector4 randomValues[1024];

		for (int i = 0; i < 1024; ++i)
		{
			randomValues[i].x = dist(gen);
			randomValues[i].y = dist(gen);
			randomValues[i].z = dist(gen);
			randomValues[i].w = dist(gen);
		}

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = randomValues;
		initData.SysMemPitch = 1024 * sizeof(Vector4);
		initData.SysMemSlicePitch = 0;

		//
		// Create the texture.
		//
		D3D11_TEXTURE1D_DESC texDesc;
		texDesc.Width = 1024;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.ArraySize = 1;

		ComPtr<ID3D11Texture1D> randomTex = 0;
		HR(device->CreateTexture1D(&texDesc, &initData, randomTex.GetAddressOf()));

		//
		// Create the resource view.
		//
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
		viewDesc.Texture1D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* randomTexSRV = 0;
		HR(device->CreateShaderResourceView(randomTex.Get(), &viewDesc, &_randVecTexSRV));

		randomTex.ReleaseAndGetAddressOf();
	}

	std::shared_ptr<GraphicsEngine::ParticleSystem> ParticleProcessor::GetParticleSystem(const uint64& particleDataID)
	{
		return _systemsIDMap.find(particleDataID) != _systemsIDMap.end() ? _systemsIDMap.at(particleDataID) : nullptr;
	}

	void ParticleProcessor::UpdateConstantBufferOfSystem(std::shared_ptr<RenderingData_Particle>& renderingData, std::shared_ptr<ParticleSystem> system)
	{
		// TEST CODE .. 방출기의 랜덤 넘버를 초기화해준다.
		static std::random_device rd;

		static std::mt19937 gen(rd());

		static std::uniform_real_distribution<float> dist(0.f, 1.f);

		// CB_Stream Output Update
		system->_cbStreamOut._emitVelocity = renderingData->_shapeInfo->_emitVelocity;
		system->_cbStreamOut._emitTime = renderingData->_shapeInfo->_emitTime;
		system->_cbStreamOut._transformMatrix = renderingData->_shapeInfo->_transformMatrix;
		system->_cbStreamOut._radius = renderingData->_shapeInfo->_radius;

		if (system->_useXYSameDeviation)
		{
			float dev = std::max<float>(system->_norDistX(gen), system->_norDistY(gen));

			system->_cbStreamOut._averageSize = renderingData->_shapeInfo->_particleAverageSize + Vector2(dev, dev);
		}
		else
			system->_cbStreamOut._averageSize = renderingData->_shapeInfo->_particleAverageSize + Vector2(system->_norDistX(gen), system->_norDistY(gen));

		system->_cbStreamOut._lifeSpan = renderingData->_shapeInfo->_lifeSpan;
		system->_cbStreamOut._period = renderingData->_shapeInfo->_period;
		system->_cbStreamOut._randNumber = dist(gen);							// 쉐이더에서 난수 발생을 위해 사용하는 것

		// CB_Draw Update
		system->_cbDraw._acceleration = renderingData->_shapeInfo->_acceleration;
		system->_cbDraw._lifeSpan = renderingData->_shapeInfo->_lifeSpan;

		// CB_Color Update
		system->_cbColor._color = renderingData->_shapeInfo->_color;
	}

	void ParticleProcessor::UpdateShaderResourceOfSystem(std::shared_ptr<RenderingData_Particle>& renderingData, std::shared_ptr<ParticleSystem> system)
	{
		system->_refTextureID = renderingData->_shapeInfo->_refTextureID;
	}

	void ParticleProcessor::UpdateBufferOfShader(std::shared_ptr<ParticleSystem>& system)
	{
		// --------------------------------- Constant Buffer Resource ---------------------------------
		// 쉐이더들에게 컨스턴트 버퍼를 업데이트 시킬 준비를 해준다.

		// CB_Timer Update (루프가 아닌 친구에게도 시간은 실시간 업데이트가 필요하다.)
		system->_cbTimer._deltaTime = _currFramedeltaTime;
		system->_cbTimer._totalTime += _currFramedeltaTime;						// 이건 진짜 총 토탈 타임으로 관리해도 문제 없다.

		_vsDraw->UpdateConstantBuffer(&system->_cbDraw, CBV_REGISTER::b1);
		_psDraw->UpdateConstantBuffer(&system->_cbTimer, CBV_REGISTER::b1);
		_psDraw->UpdateConstantBuffer(&system->_cbColor, CBV_REGISTER::b2);

		_gsStreamOutParticleCircle->UpdateConstantBuffer(&system->_cbStreamOut, CBV_REGISTER::b1);
		_gsStreamOutParticleCircle->UpdateConstantBuffer(&system->_cbTimer, CBV_REGISTER::b2);

		_gsStreamOutParticleSphere->UpdateConstantBuffer(&system->_cbStreamOut, CBV_REGISTER::b1);
		_gsStreamOutParticleSphere->UpdateConstantBuffer(&system->_cbTimer, CBV_REGISTER::b2);

		_gsStreamOutOnlyDelete->UpdateConstantBuffer(&system->_cbStreamOut, CBV_REGISTER::b1);
		_gsStreamOutOnlyDelete->UpdateConstantBuffer(&system->_cbTimer, CBV_REGISTER::b2);


		// --------------------------------- Shader Resource ---------------------------------
		// 파티클을 그리는데 사용하는 레퍼런스 텍스쳐
		std::shared_ptr<Texture> tex = ResourceManager::Get()->GetTexture(system->_refTextureID);

		if (tex != nullptr)
			_psDraw->UpdateShaderResource(tex->GetSRV(), SRV_REGISTER::t0);

		// 난투 발생
		_gsStreamOutParticleCircle->UpdateShaderResource(_randVecTexSRV, SRV_REGISTER::t0);
		_gsStreamOutParticleSphere->UpdateShaderResource(_randVecTexSRV, SRV_REGISTER::t0);
		_gsStreamOutOnlyDelete->UpdateShaderResource(_randVecTexSRV, SRV_REGISTER::t0);

		// Z - Culling
		_gsDraw->UpdateShaderResource(_deferredPassRef->GetDepth()->GetSRV(), SRV_REGISTER::t0);
	}

	void ParticleProcessor::FirstRunOfSystem(std::shared_ptr<ParticleSystem>& system)
	{
		// 1. Stream - Out Pass
		// 이번 연산에서 다루는 Primitive Topology는 PointList ..!
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		uint32 stride = sizeof(Particle);
		uint32 offset = 0;

		_deviceContext->IASetVertexBuffers(0, 1, system->_initVB.GetAddressOf(), &stride, &offset);

		// 현재 입자 목록을 스트림 출력 전용 기법으로 그려서 입자들을 갱신한다.
		// 갱신된 입자들은 스트림 출력을 통해서 Target으로 설정된 버퍼에 기록된다.
		_deviceContext->SOSetTargets(1, system->_streamOutVB.GetAddressOf(), &offset);

		// Pixel Shader 끈다.
		_deviceContext->PSSetShader(nullptr, NULL, NULL);

		// Depth도 끈다.
		_depthStencilState->SetDepthDisableState(_deviceContext);

		_vsStreamOut->Update();

		if (system->_effectType == PARTICLE_EFFECT_TYPE::CIRCLE)
			_gsStreamOutParticleCircle->Update();
		else if (system->_effectType == PARTICLE_EFFECT_TYPE::SPHERE)
			_gsStreamOutParticleSphere->Update();
			
		_deviceContext->Draw(1, 0);

		// Stream Out Target으로 사용한 VB UnBinding
		ID3D11Buffer* bufferArray[1] = { nullptr };

		_deviceContext->SOSetTargets(1, bufferArray, &offset);

		// 버텍스 버퍼 핑퐁 (그림 그리기 용과 스트림 아웃의 출력으로 사용할 녀석을 프레임 단위로
		// 바꿔가면서 사용한다. 일종의 더블 버퍼링)
		// 그럼 이제 스트림 된 녀석이 Draw !
		system->_streamOutVB.Swap(system->_drawVB);

		// 2. Draw Particles Pass

		// 뎁스를 다시 켜준다.
		_depthStencilState->SetDepthEnableState(_deviceContext);

		// Blend State On
		_blendState->SetSrcAdditiveBlendState(_deviceContext);

		_deviceContext->IASetVertexBuffers(0, 1, system->_drawVB.GetAddressOf(), &stride, &offset);

		// 쉐이더 세팅
		_vsDraw->Update();
		_gsDraw->Update();
		_psDraw->Update();

		// 그려 !
		_deviceContext->DrawAuto();

		// 3. 마무리 작업 (기하 쉐이더 끄기, 블렌드 스테이트 끄기)
		_deviceContext->GSSetShader(nullptr, NULL, NULL);

		_blendState->SetBlendStateOff(_deviceContext);
	}

	void ParticleProcessor::ContinuousRunOfSystem(std::shared_ptr<ParticleSystem>& system)
	{
		// 1. Stream - Out Pass
		// 이번 연산에서 다루는 Primitive Topology는 PointList ..!
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		uint32 stride = sizeof(Particle);
		uint32 offset = 0;

		_deviceContext->IASetVertexBuffers(0, 1, system->_drawVB.GetAddressOf(), &stride, &offset);

		// 현재 입자 목록을 스트림 출력 전용 기법으로 그려서 입자들을 갱신한다.
		// 갱신된 입자들은 스트림 출력을 통해서 Target으로 설정된 버퍼에 기록된다.
		_deviceContext->SOSetTargets(1, system->_streamOutVB.GetAddressOf(), &offset);

		// Pixel Shader 끈다.
		_deviceContext->PSSetShader(nullptr, NULL, NULL);

		// Depth도 끈다.
		_depthStencilState->SetDepthDisableState(_deviceContext);

		_vsStreamOut->Update();

		if (system->_effectType == PARTICLE_EFFECT_TYPE::CIRCLE)
			_gsStreamOutParticleCircle->Update();
		else if (system->_effectType == PARTICLE_EFFECT_TYPE::SPHERE)
			_gsStreamOutParticleSphere->Update();

		_deviceContext->DrawAuto();

		// Stream Out Target으로 사용한 VB UnBinding
		ID3D11Buffer* bufferArray[1] = { nullptr };

		_deviceContext->SOSetTargets(1, bufferArray, &offset);

		// 버텍스 버퍼 핑퐁 (그림 그리기 용과 스트림 아웃의 출력으로 사용할 녀석을 프레임 단위로
		// 바꿔가면서 사용한다. 일종의 더블 버퍼링)
		// 그럼 이제 스트림 된 녀석이 Draw !
		system->_streamOutVB.Swap(system->_drawVB);

		// 2. Draw Particles Pass
		// 뎁스를 다시 켜준다.
		_depthStencilState->SetDepthEnableState(_deviceContext);

		// Blend State On
		_blendState->SetSrcAdditiveBlendState(_deviceContext);

		_deviceContext->IASetVertexBuffers(0, 1, system->_drawVB.GetAddressOf(), &stride, &offset);

		// 쉐이더 세팅
		_vsDraw->Update();
		_gsDraw->Update();
		_psDraw->Update();

		// 그려 !
		_deviceContext->DrawAuto();

		// 3. 마무리 작업 (기하 쉐이더 끄기, 블렌드 스테이트 끄기)
		_deviceContext->GSSetShader(nullptr, NULL, NULL);

		_blendState->SetBlendStateOff(_deviceContext);
	}

	void ParticleProcessor::OnlyDeleteAndDrawRunOfSystem(std::shared_ptr<ParticleSystem>& system)
	{
		// 1. Stream - Out Pass
		// 이번 연산에서 다루는 Primitive Topology는 PointList ..!
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		uint32 stride = sizeof(Particle);
		uint32 offset = 0;

		_deviceContext->IASetVertexBuffers(0, 1, system->_drawVB.GetAddressOf(), &stride, &offset);

		// 현재 입자 목록을 스트림 출력 전용 기법으로 그려서 입자들을 갱신한다.
		// 갱신된 입자들은 스트림 출력을 통해서 Target으로 설정된 버퍼에 기록된다.
		_deviceContext->SOSetTargets(1, system->_streamOutVB.GetAddressOf(), &offset);

		// Pixel Shader 끈다.
		_deviceContext->PSSetShader(nullptr, NULL, NULL);

		// Depth도 끈다.
		_depthStencilState->SetDepthDisableState(_deviceContext);

		_vsStreamOut->Update();

		// 이 Phase에서는 버텍스의 Delete Time Update와 지우기만 한다.
		_gsStreamOutOnlyDelete->Update();

		_deviceContext->DrawAuto();

		// Stream Out Target으로 사용한 VB UnBinding
		ID3D11Buffer* bufferArray[1] = { nullptr };

		_deviceContext->SOSetTargets(1, bufferArray, &offset);

		// 버텍스 버퍼 핑퐁 (그림 그리기 용과 스트림 아웃의 출력으로 사용할 녀석을 프레임 단위로
		// 바꿔가면서 사용한다. 일종의 더블 버퍼링)
		// 그럼 이제 스트림 된 녀석이 Draw !
		system->_streamOutVB.Swap(system->_drawVB);

		// 2. Draw Particles Pass
		// 뎁스를 다시 켜준다.
		_depthStencilState->SetDepthEnableState(_deviceContext);

		// Blend State On
		_blendState->SetSrcAdditiveBlendState(_deviceContext);

		_deviceContext->IASetVertexBuffers(0, 1, system->_drawVB.GetAddressOf(), &stride, &offset);

		// 쉐이더 세팅
		_vsDraw->Update();
		_gsDraw->Update();
		_psDraw->Update();

		// 그려 !
		_deviceContext->DrawAuto();

		// 3. 마무리 작업 (기하 쉐이더 끄기, 블렌드 스테이트 끄기)
		_deviceContext->GSSetShader(nullptr, NULL, NULL);

		_blendState->SetBlendStateOff(_deviceContext);
	}

	// --------------------- Particle System In Graphics Engine ---------------------
	void ParticleSystem::Initialize(std::shared_ptr<RenderingData_Particle> targetData)
	{
		// 사이즈에 다양성을 주기 위한 분포 형성
		_norDistX = std::normal_distribution<float>(targetData->_shapeInfo->_particleAverageSize.x, targetData->_shapeInfo->_deviation.x);

		_norDistY = std::normal_distribution<float>(targetData->_shapeInfo->_particleAverageSize.y, targetData->_shapeInfo->_deviation.y);

		_useXYSameDeviation = targetData->_shapeInfo->_useXYSameDeviation;

		_systemPlayID = _nextID++;

		_totalTime = 0.f;

		_duration = targetData->_initInfo->_duration;

		_isLoop = targetData->_initInfo->_isLoop;

		_effectType = targetData->_initInfo->_particleEffectType;

		InitVBs(targetData);
	}

	void ParticleSystem::InitVBs(std::shared_ptr<RenderingData_Particle> targetData)
	{
		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		D3D11_BUFFER_DESC vbDesc;

		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.ByteWidth = sizeof(Particle);
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		Particle initParticle;
		ZeroMemory(&initParticle, sizeof(Particle) * 1);

		initParticle._age = 0.f;

		// Emitting Particle Type
		initParticle._type = PARTICLE_EMITTER;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &initParticle;

		HRESULT hr = device->CreateBuffer(&vbDesc, &vinitData, _initVB.GetAddressOf());

		vbDesc.ByteWidth = sizeof(Particle) * targetData->_initInfo->_maxParticleCount;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

		hr = device->CreateBuffer(&vbDesc, NULL, _streamOutVB.GetAddressOf());
		hr = device->CreateBuffer(&vbDesc, NULL, _drawVB.GetAddressOf());
	}

	void ParticleSystem::Release()
	{
		_initVB.ReleaseAndGetAddressOf();

		_drawVB.ReleaseAndGetAddressOf();

		_streamOutVB.ReleaseAndGetAddressOf();
	}
}