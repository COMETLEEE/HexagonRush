#include "pch.h"
#include "DangerUI.h"

DangerUI::DangerUI(std::shared_ptr<Muscle::GameObject> gameObject) : IComponents(gameObject)
{
	_backGroundRenderer = AddComponent<Muscle::UIRenderer>();
}

DangerUI::~DangerUI()
{
}

void DangerUI::Start()
{
	_uiRenderer = GetComponent<Muscle::UIRenderer>();

	_uiRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Danger")));

	_uiRenderer->GetUIData()->_drawInfo->_color.w = 0.5f;

	_uiRenderer->SetDrawNDCPosition(Vector2(-3.0f, 0.3f), Vector2(1.0f, -0.3f));

	_backGroundRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Danger_background")));

	_backGroundRenderer->SetDrawNDCPosition(Vector2(-1.0f, 1.f), Vector2(1.0f, -1.f));

	_backGroundRenderer->GetUIData()->_drawInfo->_color.w = 0.0f;
	_backGroundRenderer->SetSortOrder(4);

}

void DangerUI::Update()
{
	if (isToggle) // 트루일 때 값이 떨어진다.
	{
		if (_backGroundRenderer->GetUIData()->_drawInfo->_color.w > 0.0f)
			_backGroundRenderer->GetUIData()->_drawInfo->_color.w -= 1 * Muscle::CTime::GetGameDeltaTime();
		else
			isToggle = false;
	}
	else
	{
		if (_backGroundRenderer->GetUIData()->_drawInfo->_color.w <= 0.5f)
			_backGroundRenderer->GetUIData()->_drawInfo->_color.w += 1 * Muscle::CTime::GetGameDeltaTime();
		else
			isToggle = true;
	}
	float offset = Muscle::CTime::GetGameDeltaTime();

	_uiRenderer->SetDrawNDCPosition(Vector2(_uiRenderer->GetDrawNDCPosition().x + offset, 0.3f), Vector2(_uiRenderer->GetDrawNDCPosition().z + offset, -0.3f));

	if (_uiRenderer->GetDrawNDCPosition().x >= -1.0f)
		_uiRenderer->SetDrawNDCPosition(Vector2(-3.0f, 0.3f), Vector2(1.0f, -0.3f));
}