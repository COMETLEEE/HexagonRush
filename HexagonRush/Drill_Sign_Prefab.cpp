#include "pch.h"
#include "Drill_Sign_Prefab.h"

Drill_Sign_Prefab::Drill_Sign_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("Sign")))
{
	GetGameObject()->AddComponent<Drill_Sign>();
}
