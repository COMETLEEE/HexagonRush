#include "pch.h"
#include "LandMineField.h"

LandMineField::LandMineField(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj) , _timer(0.f)
{

}

LandMineField::~LandMineField()
{
}

void LandMineField::Start()
{

}

void LandMineField::Update()
{
	_timer += Muscle::CTime::GetGameDeltaTime();

	if (_timer > LANDMINE_TIME)
	{
		auto object = Muscle::CreateGameObject();

		object->AddComponent<ExplosionController>();

		auto col = object->AddComponent<Muscle::SphereCollider>();

		col->SetRadius(4.0f);

		col->SetLocalPostion(Vector3(0, -3.5f, 0));

		object->AddComponent<BossAttackBox>()->SetLifeTime(1.0f);

		object->GetTransform()->SetPosition(GetGameObject()->GetTransform()->GetPosition() + Vector3(0, 3.0f, 0));

		object->SetTag("Enemy");

		GetSoundManager()->Play("MissileBoom", IPlayMode::Effect);

		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f);

		// 수동으로 지워줘야 한다.

		Muscle::DeleteGameObject(GetGameObject());
	}
}