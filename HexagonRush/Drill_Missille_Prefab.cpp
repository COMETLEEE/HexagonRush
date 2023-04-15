#include "pch.h"
#include "Drill_Missille_Prefab.h"

Drill_Missille_Prefab::Drill_Missille_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("Boss2_Dril")))
{

	GetSoundManager()->Play("DrillUp", IPlayMode::Effect);

	GetGameObject()->GetTransform()->SetPosition(1000.0f, 1000.0f, 1000.0f);
	GetGameObject()->SetTag("Enemy");
	GetGameObject()->GetTransform()->SetScale(0.4f, 0.4f, 0.4f);
	GetGameObject()->AddComponent<Muscle::SphereCollider>()->SetRadius(3.0f);
	GetGameObject()->AddComponent<BossAttackBox>();
	GetGameObject()->AddComponent<DrillSpiner>();

	auto fire = Muscle::CreateGameObject();
	fire->AddComponent<FireController>()->SetSize(0.5f);
	fire->SetParent(GetGameObject());
	fire->GetTransform()->UpAt(Vector3(0, -1, 0));
	fire->GetTransform()->SetPosition(0, -5, 0);

	Vector3 from = Vector3(Muscle::CTime::GetFloatRand<float>(-20, 20), -30.0f, Muscle::CTime::GetFloatRand<float>(-20, 20));;
	GetGameObject()->GetTransform()->SetPosition(from);
	from.y = 5.0f;
	std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(from);
	ObjectMover::Get()->MoveBy(GetGameObject()->GetTransform(),
		Vector3(0, 80, 0),
		2.0f,
		0,
		MoveInfo::Linear,
		Vector3::Zero,
		Vector3::Zero,
		MoveInfo::Up,
		true
	);

	auto func = [](std::shared_ptr<Muscle::GameObject> _gameobj)
	{
		Muscle::DeleteGameObject(_gameobj);
	};

	Muscle::CTime::Invoke(func, 4.0f, GetGameObject());
}
