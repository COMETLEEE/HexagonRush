#include "pch.h"
#include "LaserController.h"

LaserController::LaserController(std::shared_ptr<Muscle::GameObject> gameObj) : IComponents(gameObj)
{

	_charging0 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo0 = _charging0->GetParticleData()->_initInfo;
	auto shapeInfo0 = _charging0->GetParticleData()->_shapeInfo;
	initInfo0->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo0->_duration = 5.0f; // 지속시간
	initInfo0->_firstRun = true; //시작
	initInfo0->_isLoop = true; //반복
	initInfo0->_maxParticleCount = 500; //갯수

	shapeInfo0->_emitVelocity = Vector3::Zero;
	shapeInfo0->_radius = 5.0f;
	shapeInfo0->_emitTime = 0.005f; // 다음 입자의 방출 시간.
	shapeInfo0->_period = 0.2f;
	shapeInfo0->_particleAverageSize = Vector2(0.2f, 0.2f);
	shapeInfo0->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
	shapeInfo0->_lifeSpan = 0.2f;
	shapeInfo0->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("LaserParticle"));

	_charging1 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo1 = _charging1->GetParticleData()->_initInfo;
	auto shapeInfo1 = _charging1->GetParticleData()->_shapeInfo;
	initInfo1->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo1->_duration = 5.0f; // 지속시간
	initInfo1->_firstRun = true; //시작
	initInfo1->_isLoop = true; //반복
	initInfo1->_maxParticleCount = 500; //갯수

	shapeInfo1->_emitVelocity = Vector3::Zero;
	shapeInfo1->_radius = 8.0f;
	shapeInfo1->_emitTime = 0.005f; // 다음 입자의 방출 시간.
	shapeInfo1->_period = 0.2f;
	shapeInfo1->_particleAverageSize = Vector2(0.2f, 0.2f);
	shapeInfo1->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
	shapeInfo1->_lifeSpan = 0.2f;
	shapeInfo1->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("LaserParticle"));

	_charging2 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo2 = _charging2->GetParticleData()->_initInfo;
	auto shapeInfo2 = _charging2->GetParticleData()->_shapeInfo;
	initInfo2->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo2->_duration = 8.0f; // 지속시간
	initInfo2->_firstRun = true; //시작
	initInfo2->_isLoop = true; //반복
	initInfo2->_maxParticleCount = 500; //갯수

	shapeInfo2->_emitVelocity = Vector3::Zero;
	shapeInfo2->_radius = 8.0f;
	shapeInfo2->_emitTime = 0.005f; // 다음 입자의 방출 시간.
	shapeInfo2->_period = 0.2f;
	shapeInfo2->_particleAverageSize = Vector2(0.2f, 0.2f);
	shapeInfo2->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
	shapeInfo2->_lifeSpan = 1.0f;
	shapeInfo2->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("LaserParticle"));

	_charging3 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo3 = _charging3->GetParticleData()->_initInfo;
	auto shapeInfo3 = _charging3->GetParticleData()->_shapeInfo;
	// Init Info
	initInfo3->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo3->_firstRun = true;
	initInfo3->_isLoop = true;
	initInfo3->_duration = 10.0f;
	initInfo3->_maxParticleCount = 500;

	// Shape Info
	shapeInfo3->_emitVelocity = Vector3(0, 0.1, 0);	// 어차피 구 모양의 쉐이더는 Speed Zero이면 제자리에 있는다
	shapeInfo3->_radius = 0.1f;
	shapeInfo3->_emitTime = 0.005f;
	shapeInfo3->_period = 0.2f;
	shapeInfo3->_particleAverageSize = Vector2(0.1f, 0.1f);
	shapeInfo3->_acceleration = Vector3::Zero;
	shapeInfo3->_lifeSpan = 1.0f;
	shapeInfo3->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("LaserParticle"));

	_laser = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo4 = _laser->GetParticleData()->_initInfo;
	auto shapeInfo4 = _laser->GetParticleData()->_shapeInfo;
	// Init Info
	initInfo4->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	initInfo4->_firstRun = true;
	initInfo4->_isLoop = true;
	initInfo4->_duration = 5.0f;
	initInfo4->_maxParticleCount = 500;

	// Shape Info
	shapeInfo4->_emitVelocity = Vector3::Up * 15;	// 어차피 구 모양의 쉐이더는 Speed Zero이면 제자리에 있는다
	shapeInfo4->_radius = 0.2f;
	shapeInfo4->_emitTime = 0.005f;
	shapeInfo4->_period = 0.2f;
	shapeInfo4->_particleAverageSize = Vector2(3.0f, 3.0f);
	shapeInfo4->_acceleration = Vector3::Zero;
	shapeInfo4->_lifeSpan = 3.0f;
	shapeInfo4->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("LaserParticle"));


	_laser->Stop();
	_charging0->Play();
	_charging1->Stop();
	_charging2->Stop();
	_charging3->Stop();


}

LaserController::~LaserController()
{
}

void LaserController::SetTarget(Vector3 targetPos)
{
	_targetPos = targetPos;
}

void LaserController::Start()
{


}

void LaserController::Update()
{
	if (GameProcess::Get()->GetIsPausePanelUp())
		return;

	if (!_isCharging)
	{
		_time += Muscle::CTime::GetGameDeltaTime() * _speed;
		if (_charging0->GetParticleData()->_shapeInfo->_radius > 0.2f)
			_charging0->GetParticleData()->_shapeInfo->_radius -= Muscle::CTime::GetGameDeltaTime() * _speed;

		if (_time > 1.5)
		{
			if (!_charging1->_isPlay)
			{
				GetSoundManager()->Play("LaserCharge", IPlayMode::Effect);
				_charging1->Play();
			}
			if (_charging1->GetParticleData()->_shapeInfo->_radius > 0.6f)
				_charging1->GetParticleData()->_shapeInfo->_radius -= Muscle::CTime::GetGameDeltaTime() * _speed;
			_charging1->GetParticleData()->_shapeInfo->_radius = 6.0f;
		}

		if (_time > 3)
		{
			if (!_charging2->_isPlay)
				_charging2->Play();
			if (_charging2->GetParticleData()->_shapeInfo->_radius > 1.0f)
			{
				_charging2->GetParticleData()->_shapeInfo->_radius -= Muscle::CTime::GetGameDeltaTime() * _speed;
			}
			else
			{
				_charging2->GetParticleData()->_shapeInfo->_radius = 6.0f;
			}
		}

		if (_time > 5)
		{
			if (!_charging3->_isPlay)
				_charging3->Play();
			if (_charging3->GetParticleData()->_shapeInfo->_particleAverageSize.x < 6.0f)
				_charging3->GetParticleData()->_shapeInfo->_particleAverageSize += Vector2(0.1f, 0.1f) * Muscle::CTime::GetGameDeltaTime() * _speed;
			else
			{
				_isCharging = true;
			}

		}
	}

	if (_isCharging)
	{

		_time1 += Muscle::CTime::GetGameDeltaTime() * _speed;
		if (_time1 > 2)
		{
			if (!_laser->_isPlay)
			{
				auto explosion = Muscle::CreateGameObject();
				auto explosionController = explosion->AddComponent<ExplosionController>();
				explosion->GetTransform()->SetPosition(_targetPos);
				//explosionController->SetSize()
				explosionController->SetSpeed(0.4f);
				explosionController->SetTexture(TEXT("LaserParticle"));
				auto directional = _targetPos - GetGameObject()->GetTransform()->GetPosition();
				directional.Normalize();
				//_explosion->Play();
				GetSoundManager()->Play("LaserShot", IPlayMode::Effect);
				_charging1->Stop();
				_charging2->Stop();
				_laser->Play();
				GetGameObject()->GetComponent<Muscle::Collider>()->SetIsEnable(true);
				_laser->GetParticleData()->_shapeInfo->_emitVelocity = directional * 25;
				ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 2, 0.05f);
			}
			if (_charging3->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
				_charging3->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(0.001f, 0.001f) * _speed;
			else
				Muscle::DeleteGameObject(GetGameObject());
		}
		if (_time1 > 3)
		{
			if (_laser->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
				_laser->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(0.001f, 0.001f) * _speed;
		}
	}
}
