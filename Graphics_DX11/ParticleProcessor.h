#pragma once
#include "Graphics_RenderingData_Particle.h"
#include "CB_Structure.h"

namespace GraphicsEngine
{
	constexpr uint32 PARTICLE_EMITTER = 0;

	constexpr uint32 PARTICLE_FLARE = 1;

	class Pass_Deferred;

	class RenderTarget;

	class DepthStencilState;

	class BlendState;

	class VertexShader;

	class PixelShader;
	
	class GeometryShader;
	
	class Texture;

	constexpr uint32 MAX_PARTICLE_COUNT = 1000;

	// 화면에 출력되고 있는 하나의 파티클 시스템
	struct ParticleSystem						// TODO : System Pool 만들자
	{
		static uint64 _nextID;

		ParticleSystem() : _effectType(PARTICLE_EFFECT_TYPE::CIRCLE), _systemPlayID(0), _totalTime(0.f), 
			_duration(0.f), _initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr), _isFinalPhaseOfSystem(false)
		{}

		void Initialize(std::shared_ptr<RenderingData_Particle> targetData);

		void InitVBs(std::shared_ptr<RenderingData_Particle> targetData);

		void Release();

		PARTICLE_EFFECT_TYPE _effectType;

		// 게임 엔진과의 소통을 위한 ID.
		uint64 _systemPlayID;

		// 파티클 시스템이 돌아가고 걸린 시간. 이걸로 제한 시간을 재서 자동 종료도 만들자.
		float _totalTime;

		// 지속 시간
		float _duration;

		// 루프인가요 ?
		bool _isLoop;

		// Loop Particle System인데 .. 이번 프레임에 업데이트 신호가 왔는가 ?
		bool _isThisFrameUpdated;

		// XY 표준 편차를 똑같게 사용하는가 ? => 이미지 보존 목적
		bool _useXYSameDeviation;

		bool _isFinalPhaseOfSystem;

		// 편차에 의한 Particle Size Variation을 주기 위함
		std::normal_distribution<float> _norDistX;

		std::normal_distribution<float> _norDistY;
		
		ComPtr<ID3D11Buffer> _initVB;

		ComPtr<ID3D11Buffer> _drawVB;

		ComPtr<ID3D11Buffer> _streamOutVB;

		CB_PARTICLE_TIMER _cbTimer;

		CB_PARTICLE_STREAMOUT _cbStreamOut;

		CB_PARTICLE_DRAW _cbDraw;

		CB_PARTICLE_COLOR _cbColor;

		uint64 _refTextureID;
	};

	// 입자의 출력을 담당한다.
	// 라이팅 패스 후 라이팅 패스 결과물을 렌더 타겟으로
	// 파티클들이 무조건 투명이라는 편견은 버리셈. 나비, 벌레 등 불투명 물체도 파티클로 사용할 수 있음.
	// 빠른 구현을 위해 머릿 속에 생각나는 것 필터링 없이 코드를 적었다 ..
	class ParticleProcessor
	{
	public:
		void Initialize();

		void Release();

		// Pass_Lighting의 Target Screen을 받아서 그것을 렌더 타겟으로 설정 후 파티클 그리면 될 듯 ..? 몰 루
		void Render(std::shared_ptr<RenderTarget> targetScreen, 
			std::vector<std::shared_ptr<RenderingData_Particle>> renderQueueParticle);

	private:
		void CreateRandomTextureSRV();

	public:
		void SetCurrentFrameDeltaTime(float deltaTime) { _currFramedeltaTime = deltaTime; }

	private:
		float _currFramedeltaTime;

		std::shared_ptr<Pass_Deferred> _deferredPassRef;

		ComPtr<ID3D11DeviceContext> _deviceContext;

		// 파티클 시스템 렌더링 특성 상 Depth 상태를 끄고 키는 경우가 많다. (Stream Out)
		std::shared_ptr<DepthStencilState> _depthStencilState;

		// 파티클 시스템 렌더링 특성 상 Blend State의 조절도 많다.
		std::shared_ptr<BlendState> _blendState;

		// 무작위성을 가진 정점 버퍼를 만들어낸다.
		std::shared_ptr<GeometryShader> _gsStreamOutParticleCircle;				// 원형 (2D) 파티클

		std::shared_ptr<GeometryShader> _gsStreamOutParticleSphere;				// 구형 (3D) 파티클

		std::shared_ptr<GeometryShader> _gsStreamOutOnlyDelete;					// 생성 X, 지우기만 한다.

		// 위의 정점 버퍼를 텍스처를 입힐 수 있는 쿼드로 기하 구조 변경
		std::shared_ptr<GeometryShader> _gsDraw;
		
		std::shared_ptr<VertexShader> _vsStreamOut;

		std::shared_ptr<VertexShader> _vsDraw;

		std::shared_ptr<PixelShader> _psDraw;

		std::map<uint64, std::shared_ptr<ParticleSystem>> _systemsIDMap;

	private:
		ComPtr<ID3D11ShaderResourceView> _randVecTexSRV;
		// 난수 발생을 사용하고 싶을 때 쉐이더에서 사용한다.

	private:
		std::shared_ptr<ParticleSystem> GetParticleSystem(const uint64& particleDataID);

		void UpdateConstantBufferOfSystem(std::shared_ptr<RenderingData_Particle>& renderingData, std::shared_ptr<ParticleSystem> system);

		void UpdateShaderResourceOfSystem(std::shared_ptr<RenderingData_Particle>& renderingData, std::shared_ptr<ParticleSystem> system);

		// System에 담긴 정보들로 쉐이더를 업데이트 !
		void UpdateBufferOfShader(std::shared_ptr<ParticleSystem>& system);

		// First Run
		void FirstRunOfSystem(std::shared_ptr<ParticleSystem>& system);

		// Continuous
		void ContinuousRunOfSystem(std::shared_ptr<ParticleSystem>& system);

		// Only Delete And Draw Run => Loop Particle Last Phase
		void OnlyDeleteAndDrawRunOfSystem(std::shared_ptr<ParticleSystem>& system);
	};
}