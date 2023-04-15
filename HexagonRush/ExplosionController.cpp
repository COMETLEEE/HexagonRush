#include "pch.h"
#include "ExplosionController.h"

ExplosionController::ExplosionController(std::shared_ptr<Muscle::GameObject> gameObj) :IComponents(gameObj)
{
	_charging = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo = _charging->GetParticleData()->_initInfo;
	auto shapeInfo = _charging->GetParticleData()->_shapeInfo;
	initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo->_duration = 5.0f; // 지속시간
	initInfo->_firstRun = true; //시작
	initInfo->_isLoop = true; //반복
	initInfo->_maxParticleCount = 500; //갯수

	shapeInfo->_emitVelocity = Vector3::Up * 0.1f;
	shapeInfo->_radius = 2.0f;
	shapeInfo->_emitTime = 0.005f; // 다음 입자의 방출 시간.
	shapeInfo->_period = 0.2f;
	shapeInfo->_particleAverageSize = Vector2(2.0f, 2.0f);
	shapeInfo->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
	shapeInfo->_lifeSpan = 1.0f;
	shapeInfo->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Particle"));

	_charging1 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo0 = _charging1->GetParticleData()->_initInfo;
	auto shapeInfo0 = _charging1->GetParticleData()->_shapeInfo;
	initInfo0->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo0->_duration = 5.0f; // 지속시간
	initInfo0->_firstRun = true; //시작
	initInfo0->_isLoop = true; //반복
	initInfo0->_maxParticleCount = 500; //갯수

	shapeInfo0->_emitVelocity = Vector3::Up * 0.1f;
	shapeInfo0->_radius = 2.0f;
	shapeInfo0->_emitTime = 0.005f; // 다음 입자의 방출 시간.
	shapeInfo0->_period = 0.2f;
	shapeInfo0->_particleAverageSize = Vector2(4.5f, 4.5f);
	shapeInfo0->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
	shapeInfo0->_lifeSpan = 1.0f;
	shapeInfo0->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Particle"));
	shapeInfo0->_color = Vector4(1.0f, 0.1f, 0.0f, 1.0f);


	_explosion = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	auto initInfo1 = _explosion->GetParticleData()->_initInfo;
	auto shapeInfo1 = _explosion->GetParticleData()->_shapeInfo;
	initInfo1->_particleEffectType = PARTICLE_EFFECT_TYPE::SPHERE;
	initInfo1->_duration = 5.0f; // 지속시간
	initInfo1->_firstRun = true; //시작
	initInfo1->_isLoop = true; //반복
	initInfo1->_maxParticleCount = 500; //갯수
	shapeInfo1->_emitVelocity = Vector3::Up * 0.1f;
	shapeInfo1->_radius = 2.0f;
	shapeInfo1->_emitTime = 0.005f; // 다음 입자의 방출 시간.
	shapeInfo1->_period = 0.2f;
	shapeInfo1->_particleAverageSize = Vector2(4.0f, 4.0f);
	shapeInfo1->_acceleration = Vector3::Zero;		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
	shapeInfo1->_lifeSpan = 1.0f;
	shapeInfo1->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Particle"));
	shapeInfo1->_color = Vector4(1.0f, 0.5f, 0, 1.0f);


	_charging->Play();
	_charging1->Play();
	_explosion->Play();
}

ExplosionController::~ExplosionController()
{

}

void ExplosionController::SetTexture(tstring name)
{
	_charging->SetParticleTextureID(ResourceManager::Get()->GetTextureID(name));
	_charging1->SetParticleTextureID(ResourceManager::Get()->GetTextureID(name));
	_explosion->SetParticleTextureID(ResourceManager::Get()->GetTextureID(name));

}

void ExplosionController::SetSpeed(float speed)
{
	_speed = speed;
}

void ExplosionController::SetSize(float size)
{
	_charging->GetParticleData()->_shapeInfo->_particleAverageSize = Vector2(size, size);
	_charging1->GetParticleData()->_shapeInfo->_particleAverageSize = Vector2(size, size);
	_explosion->GetParticleData()->_shapeInfo->_particleAverageSize = Vector2(size, size);
}

void ExplosionController::Start()
{

}

void ExplosionController::Update()
{
	_time += Muscle::CTime::GetGameDeltaTime() * _speed;
	if (_charging->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
		_charging->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(2.5f, 2.5f) * _speed * Muscle::CTime::GetGameDeltaTime();
	if (_charging1->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
		_charging1->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(2.5f, 2.5f) * _speed * Muscle::CTime::GetGameDeltaTime();
	if (_explosion->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
		_explosion->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(2.5f, 2.5f) * _speed * Muscle::CTime::GetGameDeltaTime();


	if (_explosion->GetParticleData()->_shapeInfo->_particleAverageSize.x <= 0 && _charging1->GetParticleData()->_shapeInfo->_particleAverageSize.x <= 0 && _charging->GetParticleData()->_shapeInfo->_particleAverageSize.x <= 0)
		Muscle::DeleteGameObject(GetGameObject());
}
