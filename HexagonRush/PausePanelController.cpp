#include "pch.h"
#include "PausePanelController.h"

#include "ButtonBase.h"

#include "Player.h"

#define BUTTON_STEP 0.15f

// 'Z'
#define BUTTON_CLICK_KEY 0x005A

PausePanelController::PausePanelController(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _currPanelState(PAUSE_PANEL_STATE::RESUME_SELECTED)
{
	_isPopUp = false;

	GameProcess::Get()->SetIsPausePanelUp(false);
}

PausePanelController::~PausePanelController()
{

}

void PausePanelController::AllComponentControl(bool value)
{
	for (auto& renderer : _renderers)
		renderer->SetIsEnable(value);

	for (auto& button : _buttons)
		button->SetIsEnable(value);
}

void PausePanelController::RendererComponentControl(bool value)
{
	for (auto& renderer : _renderers)
		renderer->SetIsEnable(value);
}

void PausePanelController::Start()
{
	// Player의 상태를 확인하기 위한 Player Component 캐싱
	auto&& objects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : objects)
	{
		if (object->GetName() == "Player")
		{
			_player = object->GetComponent<Player>();

			break;
		}
	}

	auto&& childrens = GetGameObject()->GetChildrens();

	// 각각 목적에 맞는 오브젝트들을 캐싱 및 인지 후 
	for (auto& children : childrens)
	{
		auto&& tag = children->GetTag();

		if (tag == "Resume")
		{	
			_resume = children->GetComponent<ButtonBase>();

			_resumeRenderer = children->GetComponent<Muscle::UIRenderer>();

			_buttons.push_back(_resume);

			_renderers.push_back(_resumeRenderer);
		}
		else if (tag == "Restart")
		{
			_restart = children->GetComponent<ButtonBase>();

			_restartRenderer = children->GetComponent<Muscle::UIRenderer>();

			_buttons.push_back(_restart);

			_renderers.push_back(_restartRenderer);
		}
		else if (tag == "Title")
		{
			_title = children->GetComponent<ButtonBase>();

			_titleRenderer = children->GetComponent<Muscle::UIRenderer>();

			_buttons.push_back(_title);

			_renderers.push_back(_titleRenderer);
		}
		else if (tag == "Exit")
		{
			_exit = children->GetComponent<ButtonBase>();

			_exitRenderer = children->GetComponent<Muscle::UIRenderer>();

			_buttons.push_back(_exit);

			_renderers.push_back(_exitRenderer);
		}
		else if (tag == "Panel")
		{
			_panelRenderer = children->GetComponent<Muscle::UIRenderer>();

			_renderers.push_back(_panelRenderer);
		}
		else if (tag == "Cursor")
		{
			_cursorRenderer = children->GetComponent<Muscle::UIRenderer>();

			_renderers.push_back(_cursorRenderer);
		}
	}

	// 각각의 오브젝트 렌더링이 잘 이루어 질 수 있도록 NDC 위치를 지정해준다.
	// _panelRenderer->SetDrawNDCPosition(Vector2(-0.3f, 0.7f), Vector2(0.3f, -0.7f));
	_panelRenderer->SetDrawNDCPosition(Vector2(-1.f, 1.f), Vector2(1.f, -1.f));
	_panelRenderer->GetUIData()->_drawInfo->_color = Vector4(1.f, 1.f, 1.f, 1.0f);
	_panelRenderer->GetUIData()->_drawInfo->_useAlphaBlend = true;

	_cursorRenderer->SetDrawNDCPosition(Vector2(-0.4f, 0.5f - BUTTON_STEP), Vector2(0.4f, 0.5f - 2 * BUTTON_STEP));

	_resumeRenderer->SetDrawNDCPosition(Vector2(-0.2f, 0.5f - BUTTON_STEP), Vector2(0.2f, 0.5f - 2 * BUTTON_STEP));

	_restartRenderer->SetDrawNDCPosition(Vector2(-0.2f, 0.5f - 3 * BUTTON_STEP), Vector2(0.2f, 0.5f - 4 * BUTTON_STEP));

	_titleRenderer->SetDrawNDCPosition(Vector2(-0.2f, 0.5f - 5 * BUTTON_STEP), Vector2(0.2f, 0.5f - 6 * BUTTON_STEP));

	_exitRenderer->SetDrawNDCPosition(Vector2(-0.2f, 0.5f - 7 * BUTTON_STEP), Vector2(0.2f, 0.5f - 8 * BUTTON_STEP));

	// 시작은 일단 끄고 시작하자.
	AllComponentControl(false);
}

void PausePanelController::Update()
{
	int _temp = 5;

	// 상태 전환
	if (Muscle::KeyBoard::Get()->KeyDown(VK_ESCAPE) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::Start, Muscle::XPadCommand::Push))
	{
		_isPopUp = !_isPopUp;

		if (!_isPopUp)
		{
			AllComponentControl(false);

			GetGameObject()->SetIsRender(false);

			GameProcess::Get()->SetIsPausePanelUp(false);

			// 러쉬 상태일 때는 바로 1.f 로 돌리는 것이 아니라 Player Reserve RushTime 만큼 !
			if (_player->GetCheckStatus() == Player::Status::RUSH)
			{
				Muscle::CTime::SetGameSpeed(0.2f, _player->GetRushRemainingTime());
			}
			else
				Muscle::CTime::SetGameSpeed(1.f, 0.f);

			_currPanelState = PAUSE_PANEL_STATE::RESUME_SELECTED;
		}
		else
		{
			AllComponentControl(true);

			GetGameObject()->SetIsRender(true);

			GameProcess::Get()->SetIsPausePanelUp(true);

			Muscle::CTime::SetGameSpeed(0.f, 180.f);			// 버튼 선택에는 3분의 유효 시간을 주겠습니다.
		}
	}

	// 팝업 상태가 아니면 업데이트할 필요가 없다.
	if (!_isPopUp)
		return;

	// Input으로 선택하는거 
	if (Muscle::KeyBoard::Get()->KeyDown(VK_DOWN) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::Down, Muscle::XPadCommand::Push))
	{
		_currPanelState = static_cast<PAUSE_PANEL_STATE>((static_cast<uint32>(_currPanelState) + 1) %
			static_cast<uint32>(PAUSE_PANEL_STATE::BUTTON_COUNT));
	}
	else if (Muscle::KeyBoard::Get()->KeyDown(VK_UP) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::Up, Muscle::XPadCommand::Push))
	{
		_currPanelState = static_cast<PAUSE_PANEL_STATE>((static_cast<uint32>(_currPanelState) - 1) %
			static_cast<uint32>(PAUSE_PANEL_STATE::BUTTON_COUNT));
	}
	
	// 현재 패널 스테이트로 버튼 상태들 조정
	switch (_currPanelState)
	{
		case PAUSE_PANEL_STATE::NONE:
		{
			break;
		}

		default:
		{
			// 순서대로 박음을 가정
			_buttons[static_cast<uint32>(_currPanelState)]->SetButtonState(ButtonBase::BUTTON_STATE::SELECTED);

			if (_currPanelState == PAUSE_PANEL_STATE::RESUME_SELECTED)
				_cursorRenderer->SetDrawNDCPosition(Vector2(-0.4f, 0.5f - BUTTON_STEP), Vector2(0.4f, 0.5f - 2 * BUTTON_STEP));
			else if (_currPanelState == PAUSE_PANEL_STATE::RESTART_SELECTED)
				_cursorRenderer->SetDrawNDCPosition(Vector2(-0.4f, 0.5f - 3 * BUTTON_STEP), Vector2(0.4f, 0.5f - 4 * BUTTON_STEP));
			else if (_currPanelState == PAUSE_PANEL_STATE::TITLE_SELECTED)
				_cursorRenderer->SetDrawNDCPosition(Vector2(-0.4f, 0.5f - 5 * BUTTON_STEP), Vector2(0.4f, 0.5f - 6 * BUTTON_STEP));
			else if (_currPanelState == PAUSE_PANEL_STATE::EXIT_SELECTED)
				_cursorRenderer->SetDrawNDCPosition(Vector2(-0.4f, 0.5f - 7 * BUTTON_STEP), Vector2(0.4f, 0.5f - 8 * BUTTON_STEP));

			for (int i = 0; i < static_cast<uint32>(PAUSE_PANEL_STATE::BUTTON_COUNT); i++)
			{
				if (i == static_cast<uint32>(_currPanelState))
					continue;

				_buttons[i]->SetButtonState(ButtonBase::BUTTON_STATE::NONE);
			}

			break;
		}
	}

	// 만약 키가 눌리면 현재 선택 중인 버튼 눌렸다고 상태 변경 => 팝업 풀고 시간 되돌리고 ..
	if ((Muscle::KeyBoard::Get()->KeyDown(BUTTON_CLICK_KEY) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::ButtonX, Muscle::XPadCommand::Push)) &&
		(static_cast<uint32>(_currPanelState) < static_cast<uint32>(PAUSE_PANEL_STATE::BUTTON_COUNT)))
	{
		_buttons[static_cast<uint32>(_currPanelState)]->SetButtonState(ButtonBase::BUTTON_STATE::ON_CLICKED);

		// 팝업 상태를 풀어줍시다.
		_isPopUp = false;

		// Resume일 때는 Button 포함 드랍
		if (_currPanelState == PAUSE_PANEL_STATE::RESUME_SELECTED)
		{
			AllComponentControl(false);

			_currPanelState = PAUSE_PANEL_STATE::RESUME_SELECTED;
		}
		// 아니라면 Call Back 함수가 진행될 필요가 있다. 따라서 Renderer만 끈다.
		else
		{
			RendererComponentControl(false);

			_currPanelState = PAUSE_PANEL_STATE::NONE;
		}

		GameProcess::Get()->SetIsPausePanelUp(false);

		// 러쉬 상태일 때는 바로 1.f 로 돌리는 것이 아니라 Player Reserve RushTime 만큼 !
		if (_player->GetCheckStatus() == Player::Status::RUSH)
		{
			Muscle::CTime::SetGameSpeed(0.2f, _player->GetRushRemainingTime());
		}
		else
			Muscle::CTime::SetGameSpeed(1.f, 0.f);
	}
}