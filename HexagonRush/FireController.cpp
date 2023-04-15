#include "pch.h"
#include "FireController.h"

FireController::FireController(std::shared_ptr<Muscle::GameObject> gameObj) : IComponents(gameObj)
{
	// »¡°­~~
	_fire1 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();

	auto initInfo1 = _fire1->GetParticleData()->_initInfo;
	auto shapeInfo1 = _fire1->GetParticleData()->_shapeInfo;
	// Init Info
	initInfo1->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	initInfo1->_firstRun = true;
	initInfo1->_isLoop = true;
	initInfo1->_duration = 3.0f;
	initInfo1->_maxParticleCount = 5000;

	// Shape Info
	shapeInfo1->_emitVelocity = Vector3::Up * 10 * _size;
	shapeInfo1->_radius = 3.0f * _size;
	shapeInfo1->_emitTime = 0.005f;
	shapeInfo1->_period = 0.2f;
	shapeInfo1->_particleAverageSize = Vector2(4.0f, 4.0f) * _size;
	shapeInfo1->_deviation = Vector2(1, 1);
	shapeInfo1->_useXYSameDeviation = true;
	shapeInfo1->_acceleration = Vector3::Zero;
	shapeInfo1->_lifeSpan = 1.0f;
	// shapeInfo1->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Particle"));
	shapeInfo1->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Flame"));
	shapeInfo1->_color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	// ³ë¶û~~
	_fire2 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();

	auto initInfo2 = _fire2->GetParticleData()->_initInfo;
	auto shapeInfo2 = _fire2->GetParticleData()->_shapeInfo;

	// Init Info
	initInfo2->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	initInfo2->_firstRun = true;
	initInfo2->_isLoop = true;
	initInfo2->_duration = 3.0f;
	initInfo2->_maxParticleCount = 5000;

	// Shape Info
	shapeInfo2->_emitVelocity = Vector3::Up * 10 * _size;
	shapeInfo2->_radius = 2.0f * _size;
	shapeInfo2->_emitTime = 0.005f;
	shapeInfo2->_period = 0.2f;
	shapeInfo2->_particleAverageSize = Vector2(3.0f, 3.0f) * _size;
	shapeInfo2->_deviation = Vector2(1, 1);
	shapeInfo2->_useXYSameDeviation = true;
	shapeInfo2->_acceleration = Vector3::Zero;
	shapeInfo2->_lifeSpan = 0.5f;
	shapeInfo2->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Flame2"));
	shapeInfo2->_color = Vector4(1.0f, 1.0f, 0, 1.0f);

	// Èò»ö~~
	_fire3 = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();

	auto initInfo3 = _fire3->GetParticleData()->_initInfo;
	auto shapeInfo3 = _fire3->GetParticleData()->_shapeInfo;

	// Init Info
	initInfo3->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	initInfo3->_firstRun = true;
	initInfo3->_isLoop = true;
	initInfo3->_duration = 3.0f;
	initInfo3->_maxParticleCount = 5000;

	// Shape Info
	shapeInfo3->_emitVelocity = Vector3::Up * 10;
	shapeInfo3->_radius = 1.0f * _size;
	shapeInfo3->_emitTime = 0.005f;
	shapeInfo3->_period = 0.2f;
	shapeInfo3->_particleAverageSize = Vector2(2.5f, 2.5f) * _size;
	shapeInfo3->_deviation = Vector2(1, 1);
	shapeInfo3->_useXYSameDeviation = true;
	shapeInfo3->_acceleration = Vector3::Zero;
	shapeInfo3->_lifeSpan = 0.1f;
	shapeInfo3->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("Flame"));
	shapeInfo3->_color = Vector4(1.0f, 1.0, 1.0f, 1.0f);

	_fire1->Play();
	_fire2->Play();
	_fire3->Play();
}

FireController::~FireController()
{
}

void FireController::SetSize(float size)
{
	_size = size;
	// »¡°­~~
	auto shapeInfo1 = _fire1->GetParticleData()->_shapeInfo;
	// Shape Info
	shapeInfo1->_emitVelocity = Vector3::Up * 10 * _size;
	shapeInfo1->_radius = 3.0f * _size;
	shapeInfo1->_particleAverageSize = Vector2(4.0f, 4.0f) * _size;

	// ³ë¶û~~
	auto shapeInfo2 = _fire2->GetParticleData()->_shapeInfo;
	// Shape Info
	shapeInfo2->_emitVelocity = Vector3::Up * 10 * _size;
	shapeInfo2->_radius = 2.0f * _size;
	shapeInfo2->_particleAverageSize = Vector2(3.0f, 3.0f) * _size;

	// Èò»ö~~
	auto shapeInfo3 = _fire3->GetParticleData()->_shapeInfo;
	// Shape Info
	shapeInfo3->_emitVelocity = Vector3::Up * 10;
	shapeInfo3->_radius = 1.0f * _size;
	shapeInfo3->_particleAverageSize = Vector2(2.5f, 2.5f) * _size;
}

void FireController::Start()
{


}

void FireController::Update()
{
	_time += Muscle::CTime::GetGameDeltaTime();

	auto emitVelocity = GetGameObject()->GetTransform()->GetWorldUp() * 10 * _size;

	_fire1->GetParticleData()->_shapeInfo->_emitVelocity = emitVelocity;
	_fire2->GetParticleData()->_shapeInfo->_emitVelocity = emitVelocity;
	_fire3->GetParticleData()->_shapeInfo->_emitVelocity = emitVelocity;

	if (_time > 2.0f)
	{
		if (_fire1->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
			_fire1->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(1.5f) * Muscle::CTime::GetGameDeltaTime();
		if (_fire2->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
			_fire2->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(1.5f) * Muscle::CTime::GetGameDeltaTime();
		if (_fire3->GetParticleData()->_shapeInfo->_particleAverageSize.x > 0)
			_fire3->GetParticleData()->_shapeInfo->_particleAverageSize -= Vector2(1.5f) * Muscle::CTime::GetGameDeltaTime();
	}
	if (_time > _endTime)
		Muscle::DeleteGameObject(GetGameObject());
}
