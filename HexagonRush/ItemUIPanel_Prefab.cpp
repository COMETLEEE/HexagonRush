#include "pch.h"
#include "ItemUIPanel_Prefab.h"

ItemUIPanel_Prefab::ItemUIPanel_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	GetGameObject()->AddComponent<ItemUIPanel>();

}

ItemUIPanel_Prefab::~ItemUIPanel_Prefab()
{
}
