#include "pch.h"
#include "Platform_Prefab.h"

Platform_Prefab::Platform_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("Tile")))
{
	std::shared_ptr<Muscle::SphereCollider> col = GetGameObject()->AddComponent<Muscle::SphereCollider>();
	col->SetLocalPostion(Vector3(0, 1.0f, 0));
	col->SetRadius(1.0f);
	GetGameObject()->AddComponent<Platform>();
	GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	GetGameObject()->GetChildrens()[0]->GetTransform()->SetPosition(0, 0, 0);
}