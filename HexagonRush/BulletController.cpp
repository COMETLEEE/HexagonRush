#include "pch.h"
#include "BulletController.h"

#define EPSILON 1.5f
#define BULLET_SPEED 45.f

BulletController::BulletController(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _currBulletState(BULLET_STATE::LOAD), _burn(nullptr), _burnRenderer(nullptr),
	_afterBurn(nullptr), _afterBurnRenderer(nullptr), _boom(nullptr), _boomRenderer(nullptr), _destination(Vector3::Zero)
{

}

BulletController::~BulletController()
{
	_topTransform.reset();

	_burn.reset();

	_burnRenderer.reset();

	_afterBurn.reset();

	_afterBurnRenderer.reset();

	_boom.reset();

	_boomRenderer.reset();
}

void BulletController::SettingParticleOfBurn()
{
#pragma region EZREAL_ULT
//	std::shared_ptr<RenderingData_Particle> renderingData = _burnRenderer->GetParticleData();
//
//	std::shared_ptr<Particle_InitInfo> initInfo = renderingData->_initInfo;
//
//	std::shared_ptr<Particle_ShapeInfo> shapeInfo = renderingData->_shapeInfo;
//
//	// Init Info
//	initInfo->_firstRun = true;
//
//	initInfo->_isLoop = true;
//
//	initInfo->_duration = 1.5f;
//
//	initInfo->_maxParticleCount = static_cast<uint32>(_currBulletLevel) * 200;
//
//	// Shape Info
//	shapeInfo->_emitVelocity = _destination - _topTransform->GetWorldPosition();
//
//	shapeInfo->_emitVelocity.Normalize();
//
//	shapeInfo->_emitVelocity *= static_cast<uint32>(_currBulletLevel) * 1.5f;
//
//	shapeInfo->_radius = static_cast<uint32>(_currBulletLevel) * 1.f;
//
//	shapeInfo->_emitTime =  0.01f / static_cast<uint32>(_currBulletLevel);
//
//	shapeInfo->_period = 0.2f;
//
//	shapeInfo->_particleAverageSize = Vector2(1.f, 1.f);
//
//	shapeInfo->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
//
//	shapeInfo->_lifeSpan = 1.f;
#pragma endregion

	std::shared_ptr<RenderingData_Particle> renderingData = _burnRenderer->GetParticleData();

	std::shared_ptr<Particle_InitInfo> initInfo = renderingData->_initInfo;

	std::shared_ptr<Particle_ShapeInfo> shapeInfo = renderingData->_shapeInfo;

	// Init Info
	initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;

	initInfo->_firstRun = true;

	initInfo->_isLoop = true;

	initInfo->_maxParticleCount = static_cast<uint32>(_currBulletLevel) * 200;
	
	shapeInfo->_emitVelocity = Vector3::Zero;

	shapeInfo->_radius = 0.f;

	shapeInfo->_emitTime = 0.005f;

	shapeInfo->_period = 0.2f;

	shapeInfo->_particleAverageSize = Vector2(0.4f, 0.4f) * static_cast<uint32>(_currBulletLevel);

	if (_currBulletColor == BULLET_COLOR::RED)
		shapeInfo->_color = Vector4(1.f, 0.f, 0.f, 1.f);
	else if (_currBulletColor == BULLET_COLOR::GREEN)
		shapeInfo->_color = Vector4(0.f, 1.f, 0.f, 1.f);
	else if (_currBulletColor == BULLET_COLOR::BLUE)
		shapeInfo->_color = Vector4(0.f, 0.1f, 0.9f, 1.f);

	shapeInfo->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !

	shapeInfo->_lifeSpan = 0.5f;
}

void BulletController::SettingParticleOfAfterBurn()
{
	std::shared_ptr<RenderingData_Particle> renderingData = _afterBurnRenderer->GetParticleData();

	std::shared_ptr<Particle_InitInfo> initInfo = renderingData->_initInfo;

	std::shared_ptr<Particle_ShapeInfo> shapeInfo = renderingData->_shapeInfo;

	// Init Info
	initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;

	initInfo->_firstRun = true;

	initInfo->_isLoop = true;

	initInfo->_maxParticleCount = static_cast<uint32>(_currBulletLevel) * 40;

	// Shape Info
	shapeInfo->_emitVelocity = Vector3::Zero;

	shapeInfo->_radius = static_cast<uint32>(_currBulletLevel) * 0.5f;

	shapeInfo->_emitTime = 0.01f / static_cast<uint32>(_currBulletLevel);

	shapeInfo->_period = 0.4f;

	if (_currBulletColor == BULLET_COLOR::RED)
		shapeInfo->_color = Vector4(1.f, 0.f, 0.f, 1.f);
	else if (_currBulletColor == BULLET_COLOR::GREEN)
		shapeInfo->_color = Vector4(0.f, 1.f, 0.f, 1.f);
	else if (_currBulletColor == BULLET_COLOR::BLUE)
		shapeInfo->_color = Vector4(0.f, 0.f, 1.f, 1.f);

	shapeInfo->_particleAverageSize = Vector2(0.15f, 0.15f) * static_cast<uint32>(_currBulletLevel);

	shapeInfo->_acceleration = Vector3::Zero;			// 중력 한 번 해보자

	shapeInfo->_lifeSpan = 0.3f;
}

void BulletController::SettingParticleOfBoom()
{
	std::shared_ptr<RenderingData_Particle> renderingData = _boomRenderer->GetParticleData();

	std::shared_ptr<Particle_InitInfo> initInfo = renderingData->_initInfo;

	std::shared_ptr<Particle_ShapeInfo> shapeInfo = renderingData->_shapeInfo;

	// Init Info
	initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;

	initInfo->_firstRun = true;

	initInfo->_isLoop = false;

	initInfo->_duration = 2.0f;

	initInfo->_maxParticleCount = static_cast<uint32>(_currBulletLevel) * 600;

	// Shape Info
	shapeInfo->_emitVelocity = Vector3::Up;		// 어차피 구 모양의 쉐이더는 Speed Zero이면 제자리에 있는다

	shapeInfo->_radius = static_cast<uint32>(_currBulletLevel) * 0.5f;

	shapeInfo->_emitTime = 0.005f;

	shapeInfo->_period = 0.2f;

	if (_currBulletColor == BULLET_COLOR::RED)
		shapeInfo->_color = Vector4(1.f, 0.f, 0.f, 1.f);
	else if (_currBulletColor == BULLET_COLOR::GREEN)
		shapeInfo->_color = Vector4(0.f, 1.f, 0.f, 1.f);
	else if (_currBulletColor == BULLET_COLOR::BLUE)
		shapeInfo->_color = Vector4(0.f, 0.1f, 0.9f, 1.f);

	shapeInfo->_particleAverageSize = Vector2(0.1f, 0.1f) * static_cast<uint32>(_currBulletLevel);

	shapeInfo->_acceleration = Vector3::Down;

	shapeInfo->_lifeSpan = 1.f;
}

// 씬이 초기화될 때 호출되는 함수던데 .. 이 정도 역할만 할 수 있겠다.
void BulletController::Start()
{
	auto&& childrens = GetGameObject()->GetChildrens();

	_topTransform = GetGameObject()->GetTransform();

	// 각각 목적에 맞는 오브젝트들을 캐싱 및 인지 후 
	for (auto& children : childrens)
	{
		auto&& tag = children->GetTag();

		if (tag == "Burn")
		{
			_burn = children;
			
			_burnRenderer = children->GetComponent<Muscle::ParticleRenderer>();
		}
		else if (tag == "AfterBurn")
		{
			_afterBurn = children;

			_afterBurnRenderer = children->GetComponent<Muscle::ParticleRenderer>();
		}
		else if (tag == "Boom")
		{
			_boom = children;

			_boomRenderer = children->GetComponent<Muscle::ParticleRenderer>();
		}
	}
}

void BulletController::Update()
{
	if (GameProcess::Get()->GetIsPausePanelUp())
		return;
	
	switch (_currBulletState)
	{
		case BULLET_STATE::LOAD:
		{
			// 총알의 나아가는 각도를 Destination을 바라보도록 셋팅 !
			_topTransform->LookAt(_destination, _topTransform->GetWorldPosition());

			// 파티클 정보들 셋팅해주고 ..!
			SettingParticleOfBurn();

			SettingParticleOfAfterBurn();

			SettingParticleOfBoom();

			// 이 총알을 발사한 에이전트에게 너가 날린 총알의 정보를 알자.

			_currBulletState = BULLET_STATE::FLY;

			_burnRenderer->Play();

			_afterBurnRenderer->Play();

			break;
		}

		case BULLET_STATE::FLY:
		{
			// 도착지에 도달했는지 체크
			if ((_destination - _topTransform->GetWorldPosition()).Length() < EPSILON)
			{
				// 도착했다면 폭발 상태로 변경
				_currBulletState = BULLET_STATE::BOOM;

				// 바로 스탑하지 말고 1초 정도 후 스탑하는게 좋을 것 같은데 ..
				_burnRenderer->Stop();

				_afterBurnRenderer->Stop();

				// 상대방에게 딜을 가한다. 이것은 총알을 통한 공격
				_agent->Attack(_agent->GetTarget());

				// 폭발은 예술이다
				_boomRenderer->Play();

				_timeFromBoom = 0.f;
			}
			// 도착 안 했다면 목적지로 트랜스폼 !
			else
			{
				float delta = Muscle::CTime::GetGameDeltaTime();

				Vector3 currPos = _topTransform->GetWorldPosition();

				_topTransform->SetPosition(currPos + _topTransform->GetLook() * BULLET_SPEED * delta);
			}

			break;
		}

		case BULLET_STATE::BOOM:
		{
			// 레벨에 비례한 발사 속도
			float delta = Muscle::CTime::GetGameDeltaTime();

			Vector3 currPos = _topTransform->GetWorldPosition();

			_timeFromBoom += Muscle::CTime::GetGameDeltaTime();

			_topTransform->SetPosition(currPos + _topTransform->GetLook() * static_cast<uint32>(_currBulletLevel) * BULLET_SPEED * delta);

			if (_timeFromBoom >= _boomRenderer->GetParticleData()->_initInfo->_duration)
				_currBulletState = BULLET_STATE::END;

			break;
		}

		case BULLET_STATE::END:
		{
			_burnRenderer->Stop();

			_afterBurnRenderer->Stop();

			_boomRenderer->Stop();

			// 삭제 ! 발생 빈도가 낮으니 풀을 쓰지 않는다.
			Muscle::DeleteGameObject(GetGameObject());

			break;
		}
	}
}

// 플레이어 컨트롤러에서 조건 맞추면 Pool에서 총알 꺼내서 발사시킴 !

// 처음 추가될 때의 Update는 LOAD 상태로 시작한다. (풀에서 STATE::LOAD로 꺼내면 됨)

// LOAD 상태에서는 지정된 단계에 따라서 파티클 렌더러의 상태를 업데이트한다.

// FLY 상태에서는 그냥 날아간다 ..? 정도면 될 것 같은데.

// BOOM 상태에서는 구 모양으로 파티클을 한방에 뿌린다. 단계별 Duration ! IsLoop Of => 생성된 지역 방향으로 EmitVelocity 갈기면 될 듯 ..