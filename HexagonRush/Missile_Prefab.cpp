#include "pch.h"
#include "Missile_Prefab.h"

Missile_Prefab::Missile_Prefab() : IPrefab(Muscle::CreateFromPrefab(TEXT("Missile")))
{
	static int count = 0; // 발사횟수 체크 좌우를 반복하기 위함!
	GetSoundManager()->Play("Missile", IPlayMode::Effect);

	GetGameObject()->GetTransform()->SetPosition(1000.0f, 1000.0f, 1000.0f);
	GetGameObject()->SetTag("Enemy");

	auto fire = Muscle::CreateGameObject();
	fire->AddComponent<FireController>()->SetSize(0.5f);
	fire->SetParent(GetGameObject());
	fire->GetTransform()->UpAt(Vector3(0, -1, 0));
	fire->GetTransform()->SetPosition(0, -5, 0);
	
	Vector3 from;
	Vector3 To;

	if (count++ % 2 > 0)
		from = Vector3(-32.0f, 30.0f, -3.0f);
	else
		from = Vector3(-32.0f, 30.0f, 3.0f);

	To = Vector3(Muscle::CTime::GetFloatRand<float>(-18, 28), 4.0f, Muscle::CTime::GetFloatRand<float>(-25, 25));

	std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(To);

	ObjectMover::Get()->MoveFromTo(GetGameObject()->GetTransform(),
		from,
		To,
		3.0f,
		0,
		MoveInfo::Bezier,
		Vector3(0.0f, 40.0f, 0.0f),
		Vector3(0.0f, 70.0f, 0.0f),
		MoveInfo::Up,
		true
	);

	auto func = [](std::shared_ptr<Muscle::GameObject> _gameobj)
	{
		auto object = Muscle::CreateGameObject();
		object->AddComponent<ExplosionController>();
		auto col = object->AddComponent<Muscle::SphereCollider>();
		col->SetRadius(4.0f);
		col->SetLocalPostion(Vector3(0, -3.5f, 0));
		object->AddComponent<BossAttackBox>()->SetLifeTime(1.0f);
		object->GetTransform()->SetPosition(_gameobj->GetTransform()->GetPosition() + Vector3(0, 3.0f, 0));
		object->SetTag("Enemy");
		Muscle::DeleteGameObject(_gameobj);
		GetSoundManager()->Play("MissileBoom", IPlayMode::Effect);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f);
	};

	Muscle::CTime::Invoke(func, 3.0f, GetGameObject());
}