#include "pch.h"
#include "EndingScene.h"
#include "SceneFader.h"
EndingScene::EndingScene() : IScene("EndingScene")
{

}

EndingScene::~EndingScene()
{

}

void EndingScene::Start()
{
	GetSoundManager()->Play("Ending", IPlayMode::BGM);
	Muscle::CTime::SetGameSpeed(1.0f, 0);

	_textRenderer.reset();

	_BackgroundRenderer.reset();

	_to_be_Continue.reset();

	_textPosition = Vector2(0.0f, -2.0f);
	std::shared_ptr<Muscle::GameObject> UiObject = Muscle::CreateGameObject();

	_textRenderer = UiObject->AddComponent<Muscle::UIRenderer>();
	_BackgroundRenderer = UiObject->AddComponent<Muscle::UIRenderer>();
	_to_be_Continue = UiObject->AddComponent<Muscle::UIRenderer>();

	_textRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Hex_Ending_text")));
	_textRenderer->SetDrawNDCPosition(Vector2(-1.0f, 1.0f) + _textPosition, Vector2(1.0f, -1.4f) + _textPosition);

	_BackgroundRenderer->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Hex_Ending_background")));
	_BackgroundRenderer->SetDrawNDCPosition(Vector2(-1.f, 1.f), Vector2(1.f, -1.f));
	_BackgroundRenderer->SetSortOrder(100);

	_to_be_Continue->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("To_Be_Continue")));
	_to_be_Continue->SetDrawNDCPosition(Vector2(-1.f, 1.f), Vector2(1.f, -1.f));
	_to_be_Continue->SetSortOrder(99);
	_to_be_Continue->GetUIData()->_drawInfo->_color.w = 2.0f;
}

void EndingScene::RapidUpdate()
{

}

void EndingScene::Update()
{
	_to_be_Continue->GetUIData()->_drawInfo->_color.w -= 0.5f * Muscle::CTime::GetGameDeltaTime();
	if (_to_be_Continue->GetUIData()->_drawInfo->_color.w > 0) return;

	if (_textPosition.y < 2.4f)
	{
		_textRenderer->SetDrawNDCPosition(Vector2(-1.0f, 1.0f) + _textPosition, Vector2(1.0f, -1.4f) + _textPosition);
		_textPosition.y += 0.1f * Muscle::CTime::GetGameDeltaTime();
	}
	else
	{
		Muscle::IGameEngine::Get()->GetSceneManager()->LoadScene("TitleScene");
	}
}
