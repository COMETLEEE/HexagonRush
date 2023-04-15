#include "pch.h"
#include "ItemUIPanel.h"

ItemUIPanel::ItemUIPanel(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject)
{
	auto item1 = Muscle::CreateGameObject();
	auto item2 = Muscle::CreateGameObject();
	auto item3 = Muscle::CreateGameObject();
	auto comboObj = Muscle::CreateGameObject();
	auto rgbSign = Muscle::CreateGameObject();
	_fireSprite = Muscle::CreateGameObject();

	item1->SetParent(gameObject);
	item2->SetParent(gameObject);
	item3->SetParent(gameObject);
	comboObj->SetParent(gameObject);
	rgbSign->SetParent(gameObject);
	_fireSprite->SetParent(gameObject);

	_powerItem = item1->AddComponent<Muscle::UIRenderer>();
	_speedItem = item2->AddComponent<Muscle::UIRenderer>();
	_hpItem = item3->AddComponent<Muscle::UIRenderer>();
	_comboUI = comboObj->AddComponent<Muscle::UIRenderer>();
	_rgbSign = rgbSign->AddComponent<Muscle::UIRenderer>();

	_fireSprite->AddComponent<Muscle::UIRenderer>();
	_fireSprite->AddComponent<FireSpriteUI>();
	_fireSprite->SetIsEnable(false);
	for (int i = 0; i < 3; i++)
	{
		auto numberObject = Muscle::CreateGameObject();

		numberObject->SetParent(gameObject);
		_numberRenderer[i] = numberObject->AddComponent<NumberRenderer>();
		_numberRenderer[i]->SetIsEnable(true);
	}

	for (int i = 0; i < 3; i++)
	{
		auto coolTimeObject = Muscle::CreateGameObject();

		coolTimeObject->SetParent(gameObject);
		_coolTimeRender[i] = coolTimeObject->AddComponent<NumberRenderer>();
		coolTimeObject->SetIsEnable(false);
	}
}

ItemUIPanel::~ItemUIPanel()
{
}

void ItemUIPanel::Start()
{
	_powerItem->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("PowerItem")));
	_speedItem->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("SpeedItem")));
	_hpItem->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("HpItem")));

	_speedItem->SetDrawNDCPosition(Vector2(-0.65f, 0.7f), Vector2(-0.55f, 0.6f));

	_powerItem->SetDrawNDCPosition(Vector2(-0.8f, 0.7f), Vector2(-0.7f, 0.6f));

	_hpItem->SetDrawNDCPosition(Vector2(-0.95f, 0.7f), Vector2(-0.85f, 0.6f));

	_numberRenderer[0]->SetNDCPosition(Vector2(-0.65f, 0.75f), Vector2(-0.55f, 0.7f));
	_numberRenderer[1]->SetNDCPosition(Vector2(-0.8f, 0.75f), Vector2(-0.7f, 0.7f));
	_numberRenderer[2]->SetNDCPosition(Vector2(-0.95f, 0.75f), Vector2(-0.85f, 0.7f));

	_coolTimeRender[0]->SetNDCPosition(Vector2(-0.65f, 0.7f), Vector2(-0.55f, 0.6f));		// SPEED
	_coolTimeRender[1]->SetNDCPosition(Vector2(-0.8f, 0.7f), Vector2(-0.7f, 0.6f));			// POWER
	_coolTimeRender[2]->SetNDCPosition(Vector2(-0.95f, 0.7f), Vector2(-0.85f, 0.6f));		// HP

	_comboUI->SetDrawNDCPosition(Vector2(-0.08f, -0.65f), Vector2(0.08f, -0.9f));
	_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo0")));
	_comboUI->GetUIData()->_drawInfo->_color.w = 0.8f;

	_rgbSign->SetDrawNDCPosition(Vector2(0.65f, 1.0f), Vector2(0.85f, 0.7f));
	_rgbSign->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("RGBSign")));
	for (auto& object : Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects)
	{
		if (object->GetName() == "Player")
		{
			_player = object->GetComponent<Player>();
			_playerInventory = object->GetComponent<Inventory>();
		}
	}
}

void ItemUIPanel::Update()
{
	_numberRenderer[0]->SetNumber(_playerInventory->GetSpeedItemCount());
	_numberRenderer[1]->SetNumber(_playerInventory->GetPowerItemCount());
	_numberRenderer[2]->SetNumber(_playerInventory->GetHPItemCount());

	int hpCool = static_cast<int>(std::round(_player->_hpRemaningCool));
	int speedCool = static_cast<int>(std::round(_player->_speedUpRemainingCool));
	int powerCool = static_cast<int>(std::round(_player->_powerUpRemainingCool));

	_coolTimeRender[0]->GetGameObject()->SetIsEnable(false);
	_coolTimeRender[1]->GetGameObject()->SetIsEnable(false);
	_coolTimeRender[2]->GetGameObject()->SetIsEnable(false);

	if (speedCool > 0)
	{
		_coolTimeRender[0]->GetGameObject()->SetIsEnable(true);
		_coolTimeRender[0]->SetNumber(speedCool);
	}

	if (powerCool > 0)
	{
		_coolTimeRender[1]->GetGameObject()->SetIsEnable(true);
		_coolTimeRender[1]->SetNumber(powerCool);
	}

	if (hpCool > 0)
	{
		_coolTimeRender[2]->GetGameObject()->SetIsEnable(true);
		_coolTimeRender[2]->SetNumber(hpCool);
	}

	_fireSprite->SetIsEnable(false);

	switch (_player->_comboCount)
	{
	case 0:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo0")));
		break;
	case 1:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo1")));
		break;
	case 2:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo2")));
		break;
	case 3:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo3")));
		break;
	case 4:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo4")));
		break;
	case 5:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo5")));
		break;
	case 6:
		_comboUI->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Combo6")));
		_fireSprite->SetIsEnable(true);
		break;

		// 7 ÀÌ»ף
	default:
		_fireSprite->SetIsEnable(true);
		break;
	}
}