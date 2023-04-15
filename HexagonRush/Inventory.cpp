#include "pch.h"
#include "Inventory.h"

#include "BuffController.h"

#define USE_HP_ITEM 0x0041		// A Key
#define USE_POWER_ITEM 0x0053	// S Key
#define USE_SPEED_ITEM 0x0044	// D Key

Inventory::Inventory(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _hpItemCount(0), _powerUpItemCount(0), _speedUpItemCount(0)
{

}

Inventory::~Inventory()
{

}

bool Inventory::CheckCanUseHPItem()
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	return (Muscle::KeyBoard::Get()->KeyDown(USE_HP_ITEM) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::D_Left, Muscle::XPadCommand::Push)) 
		&& (playerStatus != Player::Status::DIE) && (_player->_hpRemaningCool <= 0.f) && (_player->_hp < 3) && (_hpItemCount > 0);
}

bool Inventory::CheckCanUsePowerItem()
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	return (Muscle::KeyBoard::Get()->KeyDown(USE_POWER_ITEM) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::D_Down, Muscle::XPadCommand::Push)) 
		&& (playerStatus != Player::Status::DIE) && (_player->_powerUpRemainingCool <= 0.f) && !(_player->_isPowerUpBuff) && (_powerUpItemCount > 0);;
}

bool Inventory::CheckCanUseSpeedItem()
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	return (Muscle::KeyBoard::Get()->KeyDown(USE_SPEED_ITEM) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::D_Right, Muscle::XPadCommand::Push)) 
		&& (playerStatus != Player::Status::DIE) && (_player->_speedUpRemainingCool <= 0.f) && !(_player->_isSpeedUpBuff) && (_speedUpItemCount > 0);
}

void Inventory::Start()
{
	_player = GetGameObject()->GetComponent<Player>();

	_hpItemCount = 3;

	_powerUpItemCount = 3;

	_speedUpItemCount = 3;

	std::vector<std::shared_ptr<Muscle::GameObject>> childs = GetGameObject()->GetChildrens();

	// 자식들 중 이름이 BuffController인 녀석을 찾아온다.
	for (auto& child : childs)
	{
		if (child->GetName() == "BuffController")
		{
			// 그리고 버프 컨트롤러 컴포넌트를 캐싱해서 버프 이펙트를 조종한다.
			_buffController = child->GetComponent<BuffController>();
		}
	}
}

// A - HP
// S - Power
// D - Speed
void Inventory::Update()
{
	if (CameraMovementManager::Get()->GetisMove())
		return;

	if (_player->GetIsStun())
		return;

	const Player::Status& playerStatus = _player->GetCheckStatus();

	if (CheckCanUseHPItem())
	{
		GetSoundManager()->Play("Potion_Drink", IPlayMode::Effect);
		GetSoundManager()->Play("LIFE_UP", IPlayMode::Effect);

		_hpItemCount--;

		_player->SetHPUpBuff();


		_buffController->OnHPBuff();
	}

	if (CheckCanUsePowerItem())
	{
		GetSoundManager()->Play("Potion_Drink", IPlayMode::Effect);
		GetSoundManager()->Play("ATT_UP", IPlayMode::Effect);

		_powerUpItemCount--;

		_player->SetPowerUpBuff();

		_buffController->OnPowerUpBuff();
	}

	if (CheckCanUseSpeedItem())
	{
		GetSoundManager()->Play("Potion_Drink", IPlayMode::Effect);
		GetSoundManager()->Play("SPEED_UP", IPlayMode::Effect);

		_speedUpItemCount--;

		_player->SetSpeedUpBuff();

		_buffController->OnSpeedUpBuff();
	}
}