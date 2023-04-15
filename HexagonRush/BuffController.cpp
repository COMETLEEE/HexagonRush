#include "pch.h"
#include "BuffController.h"

#define BUFF_EFFECT_TIME 1.5f

#define BUFF_EFFECT_SPEED Vector3::Up * 3.f

BuffController::BuffController(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj)
{
	// 비기, 생성자에서 컴포넌트 박기 !
	_hpRenderer = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();

	_speedUpRenderer = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();

	_powerUpRenderer = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();

	// 파티클 텍스처 아이디 셋팅
	_hpRenderer->SetParticleTextureID(ResourceManager::Get()->GetTextureID(TEXT("Hp_Use")));

	// OffSet (Import한 모델과의 차이를 메꿉니다.)
	_hpRenderer->SetOffsetMatrix(Matrix::CreateRotationX(3.14f / 2));

	_speedUpRenderer->SetParticleTextureID(ResourceManager::Get()->GetTextureID(TEXT("Speed_Use")));
	_speedUpRenderer->SetOffsetMatrix(Matrix::CreateRotationX(3.14f / 2));

	_powerUpRenderer->SetParticleTextureID(ResourceManager::Get()->GetTextureID(TEXT("Power_Use")));
	_powerUpRenderer->SetOffsetMatrix(Matrix::CreateRotationX(3.14f / 2));

	// 버프 파티클 애니메이션 형상 셋팅
#pragma region HP_PARTICLE_SETTING
	std::shared_ptr<RenderingData_Particle> hpData = _hpRenderer->GetParticleData();

	// Init Info
	hpData->_initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	hpData->_initInfo->_firstRun = true;
	hpData->_initInfo->_isLoop = true;
	hpData->_initInfo->_maxParticleCount = 200;

	// Shape Info
	hpData->_shapeInfo->_period = 0.2f;
	hpData->_shapeInfo->_emitTime = 0.005f;
	hpData->_shapeInfo->_radius = 3.F;
	hpData->_shapeInfo->_emitVelocity = BUFF_EFFECT_SPEED;
	hpData->_shapeInfo->_lifeSpan = 1.0f;
	hpData->_shapeInfo->_particleAverageSize = Vector2(0.6f, 0.6f);
#pragma endregion

#pragma region SPEED_UP_PARTICLE_SETTING
	std::shared_ptr<RenderingData_Particle> speedUpData = _speedUpRenderer->GetParticleData();

	// Init Info
	speedUpData->_initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	speedUpData->_initInfo->_firstRun = true;
	speedUpData->_initInfo->_isLoop = true;
	speedUpData->_initInfo->_maxParticleCount = 200;

	// Shape Info
	speedUpData->_shapeInfo->_period = 0.2f;
	speedUpData->_shapeInfo->_emitTime = 0.005f;
	speedUpData->_shapeInfo->_radius = 3.F;
	speedUpData->_shapeInfo->_emitVelocity = BUFF_EFFECT_SPEED;
	speedUpData->_shapeInfo->_lifeSpan = 1.0f;
	speedUpData->_shapeInfo->_particleAverageSize = Vector2(0.6f, 0.6f);
#pragma endregion

#pragma region POWER_UP_PARTICLE_SETTING
	std::shared_ptr<RenderingData_Particle> powerUpData = _powerUpRenderer->GetParticleData();

	// Init Info
	powerUpData->_initInfo->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
	powerUpData->_initInfo->_firstRun = true;
	powerUpData->_initInfo->_isLoop = true;
	powerUpData->_initInfo->_maxParticleCount = 200;

	// Shape Info
	powerUpData->_shapeInfo->_period = 0.2f;
	powerUpData->_shapeInfo->_emitTime = 0.005f;
	powerUpData->_shapeInfo->_radius = 3.F;
	powerUpData->_shapeInfo->_emitVelocity = BUFF_EFFECT_SPEED;
	powerUpData->_shapeInfo->_lifeSpan = 1.0f;
	powerUpData->_shapeInfo->_particleAverageSize = Vector2(0.6f, 0.6f);
#pragma endregion
}

BuffController::~BuffController()
{

}

void BuffController::OnHPBuff()
{
	_hpRenderer->Play();

	_hpBuffEffectTime = 0.f;

	_isPlayingHP = true;
}

void BuffController::OnSpeedUpBuff()
{
	_speedUpRenderer->Play();

	_speedUpBuffEffectTime = 0.f;

	_isPlayingSpeedUp = true;
}

void BuffController::OnPowerUpBuff()
{
	_powerUpRenderer->Play();

	_powerUpBuffEffectTime = 0.f;

	_isPlayingPowerUp = true;
}

void BuffController::Update()
{
	if (_isPlayingHP)
	{
		_hpBuffEffectTime += Muscle::CTime::GetGameDeltaTime();

		if (BUFF_EFFECT_TIME < _hpBuffEffectTime)
		{
			_hpRenderer->Stop();

			_isPlayingHP = false;
		}
	}

	if (_isPlayingSpeedUp)
	{
		_speedUpBuffEffectTime += Muscle::CTime::GetGameDeltaTime();

		if (BUFF_EFFECT_TIME < _speedUpBuffEffectTime)
		{
			_speedUpRenderer->Stop();

			_isPlayingSpeedUp = false;
		}
	}

	if (_isPlayingPowerUp)
	{
		_powerUpBuffEffectTime += Muscle::CTime::GetGameDeltaTime();

		if (BUFF_EFFECT_TIME < _powerUpBuffEffectTime)
		{
			_powerUpRenderer->Stop();

			_isPlayingPowerUp = false;
		}
	}
}