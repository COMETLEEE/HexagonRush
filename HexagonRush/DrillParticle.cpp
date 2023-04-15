#include "pch.h"
#include "DrillParticle.h"
#define ParticleCount 5
DrillParticle::DrillParticle(std::shared_ptr<Muscle::GameObject> gameObject) : IComponents(gameObject)
{
	for (int i = 0; i < ParticleCount; i++)
		_particleRenderer[i] = GetGameObject()->AddComponent<Muscle::ParticleRenderer>();


}

DrillParticle::~DrillParticle()
{

}

void DrillParticle::Start()
{
	GetGameObject()->GetTransform()->Pitch(3.14f / 2);
	Vector4 color[6];
	color[0] =Vector4(1.0f, 0.4f, 0.1f, 1.0f);
	color[1] =Vector4(0.8f, 0.2f, 0.2f, 1.0f);
	color[2] =Vector4(0.6f, 0.4f, 0.1f, 1.0f);
	color[3] =Vector4(0.6f, 0.2f, 0.2f, 1.0f);
	color[4] =Vector4(0.8f, 0.4f, 0.1f, 1.0f);
	color[5] =Vector4(1.0f, 0.3f, 0.2f, 1.0f);
	for (int i = 0; i < ParticleCount; i++)
	{
		auto initInfo0 = _particleRenderer[i]->GetParticleData()->_initInfo;
		auto shapeInfo0 = _particleRenderer[i]->GetParticleData()->_shapeInfo;

		initInfo0->_particleEffectType = PARTICLE_EFFECT_TYPE::CIRCLE;
		initInfo0->_duration = 6.0f; // 지속시간
		initInfo0->_firstRun = true; //시작
		initInfo0->_isLoop = true; //반복
		initInfo0->_maxParticleCount = 500; //갯수

		shapeInfo0->_emitVelocity = Vector3::Zero;
		shapeInfo0->_radius = 1.0f;
		shapeInfo0->_emitTime = 0.005f; // 다음 입자의 방출 시간.
		shapeInfo0->_period = 0.2f;
		shapeInfo0->_particleAverageSize = Vector2(0.2f, 0.2f);
		shapeInfo0->_acceleration = Vector3(0.0f, -15.0f, 0.0f);		// 가속을 받지 않고 게임 오브젝트가 있는 쪽에서 그냥 생성된다 !
		shapeInfo0->_lifeSpan = 1.0f;
		shapeInfo0->_refTextureID = ResourceManager::Get()->GetTextureID(TEXT("LaserParticle"));
		shapeInfo0->_color = color[i];

		_particleRenderer[i]->Play();
	}
}

void DrillParticle::Update()
{
	_timer += Muscle::CTime::GetGameDeltaTime();

	for (int i = 0; i < ParticleCount; i++)
	{
		_particleRenderer[i]->GetParticleData()->_shapeInfo->_emitVelocity = Vector3(Muscle::CTime::GetFloatRand<float>(-1.0f, 1.0f), Muscle::CTime::GetFloatRand<float>(2.0f, 5.0f), Muscle::CTime::GetFloatRand<float>(-1.0f, 1.0f));
	}

	if (_timer > _endTimer)
		Muscle::DeleteGameObject(GetGameObject());

}

void DrillParticle::Finalize()
{
	for (int i = 0; i < ParticleCount; i++)
		_particleRenderer[i].reset();
}
