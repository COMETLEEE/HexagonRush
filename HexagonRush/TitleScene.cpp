#include "pch.h"
#include "TitleScene.h"


#include "SceneFader.h"
#include "ButtonBase.h"

// 'Z'
#define BUTTON_CLICK_KEY 0x005A

#define TITLE_BUTTON_COUNT 2

TitleScene::TitleScene() : IScene("TitleScene")
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Start()
{
	GetSoundManager()->Play("Title", IPlayMode::BGM);

	_isAnyBottonClicked = false;

	_sceneFader.reset();

	_startButton.reset();

	_startButtonRenderer.reset();

	_exitButton.reset();

	_exitButtonRenderer.reset();

	// Scene Fader
	std::shared_ptr<Muscle::GameObject> sceneFader = Muscle::CreateGameObject();

	sceneFader->AddComponent<Muscle::UIRenderer>();

	_sceneFader = sceneFader->AddComponent<SceneFader>();

	// Tri angle Cursor
	std::shared_ptr<Muscle::GameObject> tri = Muscle::CreateGameObject();

	tri->AddComponent<Muscle::UIRenderer>()->SetSortOrder(10);
	tri->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.5f, -0.3f), Vector2(0.5f, -0.5f));
	tri->GetComponent<Muscle::UIRenderer>()->SetUseAlphaBlend(true);
	tri->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("Cursor")));

	_cursor = tri->GetComponent<Muscle::UIRenderer>();


	// Title Image => 그냥 낮은 소트 오더로 타이틀 화면 넣으면됨
	std::shared_ptr<Muscle::GameObject> titleImage = Muscle::CreateGameObject();

	titleImage->AddComponent<Muscle::UIRenderer>()->SetSortOrder(100);
	titleImage->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-1.f, 1.f), Vector2(1.f, -1.f));
	titleImage->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("Title_BackGround")));

	// Game Start => 페이드 아웃이 되고 다음 턴으로 게임 스타트 한다.
	std::shared_ptr<Muscle::GameObject> startObject = Muscle::CreateGameObject();

	startObject->AddComponent<Muscle::UIRenderer>()->SetSortOrder(5);

	startObject->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.3f, -0.3f), Vector2(0.3f, -0.5f));
	startObject->GetComponent<Muscle::UIRenderer>()->SetUseAlphaBlend(true);
	startObject->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("Title_Start")));

	// Start Button은 페이드 아웃 후 인 게임으로 들어간다.
	auto launch = [&](void) 
	{ 
		_sceneFader->FadeOut(1.f, [](void)
			{
				Muscle::MuscleEngine::GetInstance()->GetSceneManager()->LoadScene("Stage1Scene");
			});
	};

	startObject->AddComponent<ButtonBase>()->RegistCallBack(launch);

	_startButton = startObject->GetComponent<ButtonBase>();
	_startButtonRenderer = startObject->GetComponent<Muscle::UIRenderer>();



	// Exit => 페이드 아웃이 되고 다 되면 ..! exit(0)
	std::shared_ptr<Muscle::GameObject> exitObject = Muscle::CreateGameObject();

	exitObject->AddComponent<Muscle::UIRenderer>()->SetSortOrder(5);
	exitObject->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.3f, -0.6f), Vector2(0.3f, -0.8f));
	exitObject->GetComponent<Muscle::UIRenderer>()->SetUseAlphaBlend(true);
	exitObject->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("Title_Exit")));

	auto drop = [&](void)
	{
		_sceneFader->FadeOut(1.f, [](void)
			{
				// ㅃㅇ !
				exit(0);
			});
	};

	exitObject->AddComponent<ButtonBase>()->RegistCallBack(drop);

	_exitButton = exitObject->GetComponent<ButtonBase>();

	_exitButtonRenderer = exitObject->GetComponent<Muscle::UIRenderer>();

}

void TitleScene::RapidUpdate()
{

}

void TitleScene::Update()
{
	if (_isAnyBottonClicked)
		return;

	static uint32 selectedButton = 0;

	if (Muscle::KeyBoard::Get()->KeyDown(VK_UP) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::Up, Muscle::XPadCommand::Push))
		selectedButton = (selectedButton + 1) % TITLE_BUTTON_COUNT;
	else if (Muscle::KeyBoard::Get()->KeyDown(VK_DOWN) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::Down, Muscle::XPadCommand::Push))
		selectedButton = (selectedButton - 1) % TITLE_BUTTON_COUNT;

	// 0은 스타트 버튼이 선택된 상태
	if (selectedButton == 0)
	{
		_cursor->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.5f, -0.3f), Vector2(0.5f, -0.5f));

		_startButton->SetButtonState(ButtonBase::BUTTON_STATE::SELECTED);

		_exitButton->SetButtonState(ButtonBase::BUTTON_STATE::NONE);

	}
	else if (selectedButton == 1)
	{
		_cursor->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.5f, -0.6f), Vector2(0.5f, -0.8f));

		_startButton->SetButtonState(ButtonBase::BUTTON_STATE::NONE);

		_exitButton->SetButtonState(ButtonBase::BUTTON_STATE::SELECTED);
	}

	// 키보드를 통한 버튼 선택 완료 !
	if (Muscle::KeyBoard::Get()->KeyDown(BUTTON_CLICK_KEY) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::ButtonX, Muscle::XPadCommand::Push))
	{
		// 0이면 시작 버튼 선택, 아니면 나가기 버튼 선택
		selectedButton == 0 ? _startButton->SetButtonState(ButtonBase::BUTTON_STATE::ON_CLICKED) : 
			_exitButton->SetButtonState(ButtonBase::BUTTON_STATE::ON_CLICKED);

		_isAnyBottonClicked = true;
	}
}