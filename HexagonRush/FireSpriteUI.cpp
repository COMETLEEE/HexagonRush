#include "pch.h"
#include "FireSpriteUI.h"

FireSpriteUI::FireSpriteUI(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject)
{

}

FireSpriteUI::~FireSpriteUI()
{

}

void FireSpriteUI::Start()
{
	_fireRenderer = GetComponent<Muscle::UIRenderer>();
	_fireRenderer->SetDrawNDCPosition(Vector2(-0.2f, -0.3f), Vector2(0.2f, -0.93f));
	_fireRenderer->SetSortOrder(6);
	_fireRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Fire000")));
	_fireRenderer->GetUIData()->_drawInfo->_color = Vector4(1.0f, 1.0f, 1.0f, 0.3f);
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire000")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire001")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire002")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire003")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire004")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire005")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire006")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire007")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire008")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire009")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire010")));
	_resourceID.push_back(ResourceManager::Get()->GetTextureID(TEXT("Fire011")));
}

void FireSpriteUI::Update()
{
	_timer += Muscle::CTime::GetGameDeltaTime();

	if (_timer < 0.07f) return;
	_timer = 0;

	if (_resourceID.size() <= _Animindex)
		_Animindex = 0;

	_fireRenderer->SetTextureID(_resourceID[_Animindex++]);
}
