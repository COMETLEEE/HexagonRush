#include "pch.h"
#include "LoadBar_Prefab.h"

LoadBar_Prefab::LoadBar_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	auto uiRenderer1 = Muscle::CreateGameObject()->AddComponent<Muscle::UIRenderer>();
	uiRenderer1->SetTextureID(Muscle::MuscleEngine::GetInstance()->GetResourceManager()->GetTextureID(TEXT("LoadOutLine")));
	uiRenderer1->SetDrawNDCPosition(Vector2(-1.0f, -0.2f), Vector2(1.0f, -0.6f));
	uiRenderer1->GetGameObject()->SetParent(GetGameObject());

	auto uiRenderer = Muscle::CreateGameObject()->AddComponent<Muscle::UIRenderer>();
	uiRenderer->SetTextureID(Muscle::MuscleEngine::GetInstance()->GetResourceManager()->GetTextureID(TEXT("LoadFill")));
	uiRenderer->AddComponent<ResourceLoadBar>();
	uiRenderer->SetDrawNDCPosition(Vector2(-1.0f, -0.2f), Vector2(1.0f, -0.6f));
	uiRenderer->GetGameObject()->SetParent(GetGameObject());
}
