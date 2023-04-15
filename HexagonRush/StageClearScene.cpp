#include "pch.h"
#include "StageClearScene.h"

#include "SceneFader.h"
#include "ButtonBase.h"
#include "ClearTimePanelUI.h"

// 'Z'
#define BUTTON_CLICK_KEY 0x005A

std::string StageClearScene::_nextSceneName = "Stage2Scene";

float StageClearScene::_playTimePrevScene = 0.f;

float StageClearScene::GetPrevScenePlayTime()
{
	return _playTimePrevScene;
}

StageClearScene::StageClearScene() : IScene("StageClearScene")
{
	_isAnyBottonClicked = false;
}

StageClearScene::~StageClearScene()
{

}

void StageClearScene::SetNextSceneName(std::string sceneName)
{
	_nextSceneName = sceneName;
}

void StageClearScene::SetPrevScenePlayTime(float time)
{
	_playTimePrevScene = time;
}

void StageClearScene::Start()
{
	_isAnyBottonClicked = false;

	_sceneFader.reset();

	_nextButton.reset();

	_nextButtonRenderer.reset();

	// Scene Fader
	std::shared_ptr<Muscle::GameObject> sceneFader = Muscle::CreateGameObject();

	sceneFader->AddComponent<Muscle::UIRenderer>();

	_sceneFader = sceneFader->AddComponent<SceneFader>();

	// Back Ground
	std::shared_ptr<Muscle::GameObject> back = Muscle::CreateGameObject();

	back->AddComponent<Muscle::UIRenderer>()->SetSortOrder(100);

	back->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-1.f, 1.f), Vector2(1.f, -1.f));

	back->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("StageClear_BackGround")));

	// Cursor
	std::shared_ptr<Muscle::GameObject> cursor = Muscle::CreateGameObject();

	cursor->AddComponent<Muscle::UIRenderer>()->SetSortOrder(6);

	cursor->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.4f, -0.7f), Vector2(0.4f, -0.9f));

	cursor->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("Cursor")));

	// Next Stage Button
	std::shared_ptr<Muscle::GameObject> nextStage = Muscle::CreateGameObject();

	nextStage->AddComponent<Muscle::UIRenderer>();

	nextStage->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.2f, -0.7f), Vector2(0.2f, -0.9f));

	nextStage->GetComponent<Muscle::UIRenderer>()->SetTextureID(
		ResourceManager::Get()->GetTextureID(TEXT("StageClear_NextStage")));

	nextStage->AddComponent<ButtonBase>();

	// next => 5초 페이드 아웃 후 타이틀 씬으로 고고 !
	auto nextFunc = [&](void)
	{
		_sceneFader->FadeOut(1.f, [](void)
			{
				Muscle::MuscleEngine::GetInstance()->GetSceneManager()->LoadScene(_nextSceneName);
			});
	};

	nextStage->GetComponent<ButtonBase>()->RegistCallBack(nextFunc);

	_nextButtonRenderer = nextStage->GetComponent<Muscle::UIRenderer>();

	_nextButton = nextStage->GetComponent<ButtonBase>();
	
	// Stage Clear Time
	std::shared_ptr<Muscle::GameObject> clearBanner = Muscle::CreateGameObject();

	clearBanner->AddComponent<Muscle::UIRenderer>();
	
	clearBanner->GetComponent<Muscle::UIRenderer>()->GetUIData()->_drawInfo->_textureID =
		ResourceManager::Get()->GetTextureID(TEXT("CLEARTIME"));

	clearBanner->GetComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.8f, 0.1f), Vector2(0.8f, -0.1f));

	std::shared_ptr<Muscle::GameObject> bonche = Muscle::CreateGameObject();

	bonche->AddComponent<ClearTimePanelUI>();

	std::shared_ptr<Muscle::GameObject> clearTime1 = Muscle::CreateGameObject();

	clearTime1->AddComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.7f, -0.2f), Vector2(-0.5f, -0.4f));


	std::shared_ptr<Muscle::GameObject> clearTime2 = Muscle::CreateGameObject();

	clearTime2->AddComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.4f, -0.2f), Vector2(-0.2f, -0.4f));


	std::shared_ptr<Muscle::GameObject> clearTime3 = Muscle::CreateGameObject();

	clearTime3->AddComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(-0.02f, -0.2f), Vector2(0.02f, -0.4f));


	std::shared_ptr<Muscle::GameObject> clearTime4 = Muscle::CreateGameObject();

	clearTime4->AddComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(0.2f, -0.2f), Vector2(0.4f, -0.4f));


	std::shared_ptr<Muscle::GameObject> clearTime5 = Muscle::CreateGameObject();

	clearTime5->AddComponent<Muscle::UIRenderer>()->SetDrawNDCPosition(Vector2(0.5f, -0.2f), Vector2(0.7f, -0.4f));

	bonche->SetChild(clearTime1);
	bonche->SetChild(clearTime2);
	bonche->SetChild(clearTime3);
	bonche->SetChild(clearTime4);
	bonche->SetChild(clearTime5);
}

void StageClearScene::RapidUpdate()
{
}

void StageClearScene::Update()
{
	if (_isAnyBottonClicked)
		return;

	_nextButton->SetButtonState(ButtonBase::BUTTON_STATE::SELECTED);

	// 키보드를 통한 버튼 선택 완료 !
	if (Muscle::KeyBoard::Get()->KeyPress(BUTTON_CLICK_KEY) || Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::ButtonX, Muscle::XPadCommand::Push))
	{
		// 버튼 하나임.
		_nextButton->SetButtonState(ButtonBase::BUTTON_STATE::ON_CLICKED);

		_isAnyBottonClicked = true;
	}
}