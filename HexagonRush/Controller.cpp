#include "pch.h"
#include "Controller.h"

// Z버튼
#define TRIGGER_BUTTON 0x005A

// SPACE BAR
#define EVASION_BUTTON VK_SPACE

#define EVASION_COOL_TIME 3.f

Controller::Controller(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject), _evasionCoolTiem(0.f)
{

}

Controller::~Controller()
{

}

void Controller::Start()
{
	_player = GetComponent<Player>();

	_playerTransform = _player->GetComponent<Muscle::Transform>();

	_evasionCoolTiem = 0.f;
}

// 플레이어 클래스에 놔두려고 했는데 조작을 모두 컨트롤러에 모으고 싶어서 여기에 배치함!
void Controller::OnStayCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	// 조작을 할 수 없는 상황들
	if ((playerStatus == Player::Status::ATTACK) || (playerStatus == Player::Status::DIE) || (playerStatus == Player::Status::HIT)
		|| (playerStatus == Player::Status::EVASION) || (playerStatus == Player::Status::RUSH))
		return;

	if (CameraMovementManager::Get()->GetisMove())
		return;

	// 충돌 중이면서 만약 트리거 버튼을 누르고 있다면 ..!
	if (Muscle::KeyBoard::Get()->KeyPress(TRIGGER_BUTTON) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::ButtonX, Muscle::XPadCommand::KeepPress))
	{
		std::shared_ptr<Platform> temp = _Other->GetComponent<Platform>();

		if (temp)
			TileManager::Get()->SelectTile(temp);
	}
}

void Controller::AttackUpdate()
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	// 공격을 할 수 없는 상황들 !
	if ((playerStatus == Player::Status::ATTACK) || (playerStatus == Player::Status::DIE) || (playerStatus == Player::Status::HIT)
		|| playerStatus == Player::Status::EVASION)
		return;

	// Z 버튼을 누르고 있다가 땐다면 ..?
	if (Muscle::KeyBoard::Get()->KeyUp(TRIGGER_BUTTON) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::ButtonX, Muscle::XPadCommand::Release))
	{
		// 어택 시도 ! => 조건이 맞지 않으면 실패할 수 있음 (내부에서 실패 판정)
		_player->Attack();
	}
}

void Controller::MoveUpdate()
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	// 무브를 할 수 없는 상황들 !
	if ((playerStatus == Player::Status::ATTACK) || (playerStatus == Player::Status::DIE) || (playerStatus == Player::Status::HIT)
		|| playerStatus == Player::Status::EVASION || playerStatus == Player::Status::RUSH)
		return;

	if (CameraMovementManager::Get()->GetisMove()) return; // 카메라가 무브중이면 쓸 수 없다..!

	Vector3 look = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetXMLook();
	look.y = 0;
	look.Normalize();

	Vector3 right = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetRight();
	right.y = 0;
	right.Normalize();

#pragma region 8 Way Move
	Vector3 dir;

#pragma region USE_XPAD
	if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left))
	{
		dir = (-right) + (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right))
	{
		dir = (right)+(-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down))
	{
		dir = (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right))
	{
		dir = (right)+(look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left))
	{
		dir = (-right) + (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up))
	{
		dir = (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right))
	{
		dir = (right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if(Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left))
	{
		dir = -(right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
#pragma endregion

#pragma region USE_KEYBOARD
	if (Muscle::KeyBoard::Get()->KeyPress(VK_DOWN) && Muscle::KeyBoard::Get()->KeyPress(VK_LEFT))
	{
		dir = (-right) + (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_DOWN) && Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT))
	{
		dir = (right)+(-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_DOWN))
	{
		dir = (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_UP) && Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT))
	{
		dir = (right)+(look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);

		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_UP) && Muscle::KeyBoard::Get()->KeyPress(VK_LEFT))
	{
		dir = (-right) + (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);

		GetGameObject()->GetTransform()->RotateY(3.14f);


		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_UP))
	{
		dir = (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT))
	{
		dir = (right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);


		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_LEFT))
	{
		dir = -(right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);


		_player->Move(dir);
	}
#pragma endregion

#pragma endregion
}

void Controller::PanicMoveUpdate()
{
	const Player::Status& playerStatus = _player->GetCheckStatus();

	// 무브를 할 수 없는 상황들 !
	if ((playerStatus == Player::Status::ATTACK) || (playerStatus == Player::Status::DIE) || (playerStatus == Player::Status::HIT)
		|| playerStatus == Player::Status::EVASION || playerStatus == Player::Status::RUSH)
		return;

	if (CameraMovementManager::Get()->GetisMove()) return; // 카메라가 무브중이면 쓸 수 없다..!

	Vector3 look = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetXMLook();
	look.y = 0;
	look.Normalize();

	look = -look;

	Vector3 right = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetRight();
	right.y = 0;
	right.Normalize();

	right = -right;

#pragma region 8 Way Move
	Vector3 dir;

#pragma region USE_XPAD
	if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left))
	{
		dir = (-right) + (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right))
	{
		dir = (right)+(-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down))
	{
		dir = (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right))
	{
		dir = (right)+(look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up) && Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left))
	{
		dir = (-right) + (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up))
	{
		dir = (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right))
	{
		dir = (right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left))
	{
		dir = -(right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
#pragma endregion

#pragma region USE_KEYBOARD
	if (Muscle::KeyBoard::Get()->KeyPress(VK_DOWN) && Muscle::KeyBoard::Get()->KeyPress(VK_LEFT))
	{
		dir = (-right) + (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_DOWN) && Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT))
	{
		dir = (right)+(-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_DOWN))
	{
		dir = (-look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_UP) && Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT))
	{
		dir = (right)+(look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);

		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_UP) && Muscle::KeyBoard::Get()->KeyPress(VK_LEFT))
	{
		dir = (-right) + (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);

		GetGameObject()->GetTransform()->RotateY(3.14f);


		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_UP))
	{
		dir = (look);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);

		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT))
	{
		dir = (right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);


		_player->Move(dir);
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_LEFT))
	{
		dir = -(right);

		dir.Normalize();

		GetGameObject()->GetTransform()->SetLook(dir);
		GetGameObject()->GetTransform()->RotateY(3.14f);


		_player->Move(dir);
	}
#pragma endregion

#pragma endregion
}

void Controller::EvasionUpdate()
{
	// CheckStatus는 PrevStatus의 개념이다.
	const Player::Status& playerStatus = _player->GetCheckStatus();

	// 회피를 할 수 없는 상황들 !
	if (/*(playerStatus == Player::Status::ATTACK) ||*/ (playerStatus == Player::Status::DIE) || (playerStatus == Player::Status::HIT)
		|| playerStatus == Player::Status::EVASION || playerStatus == Player::Status::IDLE)
		return;

	if ((Muscle::KeyBoard::Get()->KeyDown(EVASION_BUTTON) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::ButtonA, Muscle::XPadCommand::Push)) && (_evasionCoolTiem >= EVASION_COOL_TIME))
	{
		// 이 키를 누른 방향으로 DeltaTime으로 회피 이동 속도만큼 이동해야한다. (구르는 동안 ?
		// 속도는 아마 애니메이션에 맞춰서 점점 감소시킬 필요가 있을 수도 ..?
		_evasionCoolTiem = 0.f;

		// 이동은 PlayerState -> OnEnter에서 실행함.
		_player->Evasion();
	}
}

void Controller::Update()
{
	if (_player->GetIsStun())
		return;

	if (GameProcess::Get()->GetIsPausePanelUp())
		return;

	_evasionCoolTiem += Muscle::CTime::GetGameDeltaTime();

	EvasionUpdate();

	if (!_player->GetIsPanic())
		MoveUpdate();
	else
		PanicMoveUpdate();

	AttackUpdate();
}