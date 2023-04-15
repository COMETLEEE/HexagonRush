#include "pch.h"
#include "Rush_BulletController.h"

#define RUSH_BULLET_DAMAGE 100

#define RUSH_BULLET_TIME 1.5f

#define EPSILON 1.5f

Rush_BulletController::Rush_BulletController(std::shared_ptr<Muscle::GameObject> gameObj) : 
	IComponents(gameObj), _isCollided(false), _deleteTimer(0.f), _rushBulletState(RUSH_BULLET_STATE::STORED), _destination(Vector3::Zero),
	_bulletLifeTimer(0.f)
{

}

Rush_BulletController::~Rush_BulletController()
{

}

float Rush_BulletController::CheckDistance()
{
	return (_destination - GetGameObject()->GetTransform()->GetWorldPosition()).Length();
}

void Rush_BulletController::SettingParticleOfBurn()
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

	initInfo->_maxParticleCount = 500;

	shapeInfo->_emitVelocity = Vector3::Zero;

	shapeInfo->_radius = 0.f;

	shapeInfo->_emitTime = 0.005f;

	shapeInfo->_period = 0.2f;

	shapeInfo->_particleAverageSize = Vector2(0.6f, 0.6f);

	shapeInfo->_color = Vector4(0.8f, 0.8f, 0.8f, 1.f);

	shapeInfo->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !

	shapeInfo->_lifeSpan = 0.5f;
}

void Rush_BulletController::SettingParticleOfAfterBurn()
{
	std::shared_ptr<RenderingData_Particle> renderingData = _afterBurnRenderer->GetParticleData();

	std::shared_ptr<Particle_InitInfo> initInfo = renderingData->_initInfo;

	std::shared_ptr<Particle_ShapeInfo> shapeInfo = renderingData->_shapeInfo;

	// Init Info
	initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;

	initInfo->_firstRun = true;

	initInfo->_isLoop = true;

	initInfo->_maxParticleCount = 100;

	// Shape Info
	shapeInfo->_emitVelocity = Vector3::Zero;

	shapeInfo->_radius = 2.5f;

	shapeInfo->_emitTime = 0.005f;

	shapeInfo->_period = 0.4f;

	// 주변 도는 것은 색상 랜덤
	shapeInfo->_color = _color;

	shapeInfo->_particleAverageSize = Vector2(0.3f, 0.3f);

	shapeInfo->_acceleration = Vector3::Zero;			// 중력 한 번 해보자

	shapeInfo->_lifeSpan = 0.3f;
}

void Rush_BulletController::SettingParticleOfBoom()
{
	std::shared_ptr<RenderingData_Particle> renderingData = _boomRenderer->GetParticleData();

	std::shared_ptr<Particle_InitInfo> initInfo = renderingData->_initInfo;

	std::shared_ptr<Particle_ShapeInfo> shapeInfo = renderingData->_shapeInfo;

	// Init Info
	initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;

	initInfo->_firstRun = true;

	initInfo->_isLoop = false;

	initInfo->_duration = 2.0f;

	initInfo->_maxParticleCount = 1200;

	// Shape Info
	shapeInfo->_emitVelocity = Vector3::Up;		// 어차피 구 모양의 쉐이더는 Speed Zero이면 제자리에 있는다

	shapeInfo->_radius = 1.5f;

	shapeInfo->_emitTime = 0.005f;

	shapeInfo->_period = 0.2f;

	// 터지는건 AfterBurn이랑 Boom만 색상 이렇게 !
	shapeInfo->_color = _color;

	shapeInfo->_particleAverageSize = Vector2(0.25f, 0.25f);

	shapeInfo->_acceleration = Vector3::Down;

	shapeInfo->_lifeSpan = 1.f;
}

void Rush_BulletController::LaunchTo(const Vector3& dest)
{
	// 타이머 되돌리기
	_bulletLifeTimer = 0.f;

	_isCollided = false;

	_rushBulletState = RUSH_BULLET_STATE::LAUNCHED;

	_destination = dest;

	_deleteTimer = 0.f;

	_burnRenderer->Play();

	_afterBurnRenderer->Play();

	// 궤적은 랜덤으로 / 원형 주기성
	static std::random_device rd;

	static std::mt19937 gen(rd());

	static std::uniform_real_distribution<float> angleDist(0.f, 3.141592f * 2);

	static std::uniform_real_distribution<float> radiusDist(3.f, 6.f);

	float angle = angleDist(gen);

	float radius = radiusDist(gen);

	// 현재 8월 23일 기준 플레이어 모델의 룩벡터가 Up임.
	Vector3 playerLook = _playerTransform->GetUp();

	Vector3 playerRight = _playerTransform->GetRight();

	Matrix bulletRot = Matrix::CreateFromAxisAngle(playerLook, angle);

	Vector3 circleOffset = Vector3::Transform(playerRight * radius, bulletRot);

	Vector3 bulletStartPoint = _playerTransform->GetWorldPosition() + playerLook * 3.f + circleOffset;

	float distantce = (_destination - bulletStartPoint).Length();

	// 이거 나중에 모델 나오면 하드코딩으로 예쁜 위치로 조정 ! => 아니면 총구 위치
	// 지금 플레이어는 Up이 Look이다 ! (8월 21일 기준)
	// 캐릭터 나오면 예쁜 각도와 위치를 찾자.
	GetGameObject()->GetTransform()->SetPosition(bulletStartPoint);

	ObjectMover::Get()->MoveFromTo(GetGameObject()->GetTransform(), GetGameObject()->GetTransform()->GetPosition(), _destination, 1.f, 0,
		MoveInfo::Bezier, playerLook * distantce * 0.3f + circleOffset * 2.f, playerLook * distantce * 0.6f + circleOffset,
		MoveInfo::Look, false, _destination);
}

void Rush_BulletController::SetColor(const Vector4& color)
{
	_color = color;
}

void Rush_BulletController::Start()
{
	std::vector<std::shared_ptr<Muscle::GameObject>> sceneObjects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& obj : sceneObjects)
	{
		if (obj->GetName() == "Boss")
		{
			_boss = obj->GetComponent<Character>();
		}
		else if (obj->GetName() == "Player")
		{
			_playerTransform = obj->GetTransform();

			_player = obj->GetComponent<Player>();
		}
	}

	std::vector<std::shared_ptr<Muscle::GameObject>> childs = GetGameObject()->GetChildrens();

	// 부품 캐싱
	for (auto& child : childs)
	{
		if (child->GetTag() == "Burn")
		{
			_burn = child;

			_burnRenderer = _burn->GetComponent<Muscle::ParticleRenderer>();
		}
		else if (child->GetTag() == "AfterBurn")
		{
			_afterBurn = child;

			_afterBurnRenderer = _afterBurn->GetComponent<Muscle::ParticleRenderer>();
		}
		else if (child->GetTag() == "Boom")
		{
			_boom = child;

			_boomRenderer = _boom->GetComponent<Muscle::ParticleRenderer>();
		}
	}

	// 파티클 이펙트 각각 세팅
	SettingParticleOfBurn();

	SettingParticleOfAfterBurn();

	SettingParticleOfBoom();
}

void Rush_BulletController::Update()
{
	// 저장 상황이면 업데이트 노노
	if (_rushBulletState == RUSH_BULLET_STATE::STORED)
		return;

	_bulletLifeTimer += Muscle::CTime::GetGameDeltaTime();

	if (!_isCollided)
	{
		if (_bulletLifeTimer > RUSH_BULLET_TIME)
			_player->StoreRushBullet(GetGameObject());

		// 보스와의 거리 측정
		float dist = CheckDistance();

		if (dist < EPSILON)
		{
			_isCollided = true;

			// 기존 것들 다 끄고 폭발 파티클로 변경
			_burnRenderer->Stop();

			_afterBurnRenderer->Stop();

			_boomRenderer->Play();

			_boss->Hit(RUSH_BULLET_DAMAGE);
		}

		return;
	}

	// 충돌을 했다면 시간을 잰다. (모든 이펙트 끝나고 여유로울 때까지)
	_deleteTimer += Muscle::CTime::GetGameDeltaTime();

	if (_deleteTimer >= 5.f)
	{
		// 오브젝트 풀로 되돌린다.
		_boomRenderer->Stop();

		_player->StoreRushBullet(GetGameObject());
	}
}