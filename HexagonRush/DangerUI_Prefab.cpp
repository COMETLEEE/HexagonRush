#include "pch.h"
#include "DangerUI_Prefab.h"

DangerUI_Prefab::DangerUI_Prefab():IPrefab(Muscle::CreateGameObject())
{
	GetGameObject()->AddComponent<Muscle::UIRenderer>();
	GetGameObject()->AddComponent<DangerUI>();

}

DangerUI_Prefab::~DangerUI_Prefab()
{
}
