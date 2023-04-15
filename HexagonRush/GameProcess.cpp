#include "pch.h"
#include "GameProcess.h"

#include "Stage1Scene.h"
#include "IntroScene.h"
#include "TitleScene.h"
#include "DieScene.h"
#include "Stage2Scene.h"

std::shared_ptr<GameProcess> GameProcess::_gameProcess = nullptr;

GameProcess::~GameProcess()
{
	//delete m_GameEngine; 스마트 포인터로 변경할 것 이다..
	m_SoundManager->Finalize(); //교수님의 구조이기 때문에 수정 안했다..

	delete m_SoundManager;

	m_GameEngine->Finalize();
	m_GameEngine.reset();

	_resourceManager->Finalize();
	_resourceManager.reset();
}

std::shared_ptr<GameProcess> GameProcess::Get()
{
	if (_gameProcess == nullptr)
		_gameProcess = std::make_shared<GameProcess>();

	return _gameProcess;
}

void GameProcess::Initialize(HWND _Hwnd, int WindowSizeX, int WindowSizeY)
{
	m_SoundManager = ::GetSoundManager();
	m_SoundManager->Initialize();

	m_GameEngine = Muscle::IGameEngine::Get();
	m_GameEngine->Initialize(_Hwnd, WindowSizeX, WindowSizeY);

	_resourceManager = ResourceManager::Get();
	_resourceManager->Initialize();

	// 씬 생성 및 게임 엔진에 등록
	std::shared_ptr<IntroScene> introScene = std::make_shared<IntroScene>();
	std::shared_ptr<Stage1Scene> testScene = std::make_shared<Stage1Scene>();
	std::shared_ptr<Stage2Scene> stage2Scene = std::make_shared<Stage2Scene>();
	std::shared_ptr<TitleScene> titleScene = std::make_shared<TitleScene>();
	std::shared_ptr<DieScene> dieScene = std::make_shared<DieScene>();
	std::shared_ptr<StageClearScene> stageClearScene = std::make_shared<StageClearScene>();
	std::shared_ptr<EndingScene> endingScene = std::make_shared<EndingScene>();

	m_GameEngine->GetSceneManager()->RegistScene(introScene);
	m_GameEngine->GetSceneManager()->RegistScene(testScene);
	m_GameEngine->GetSceneManager()->RegistScene(titleScene);
	m_GameEngine->GetSceneManager()->RegistScene(dieScene);
	m_GameEngine->GetSceneManager()->RegistScene(stageClearScene);
	m_GameEngine->GetSceneManager()->RegistScene(stage2Scene);
	m_GameEngine->GetSceneManager()->RegistScene(endingScene);

	m_GameEngine->GetSceneManager()->LoadScene("TitleScene");
	// m_GameEngine->GetSceneManager()->LoadScene("EndingScene");
	// m_GameEngine->GetSceneManager()->LoadScene("StageClearScene");
	// m_GameEngine->GetSceneManager()->LoadScene("Stage1Scene");
	// m_GameEngine->GetSceneManager()->LoadScene("Stage2Scene");
}

void GameProcess::Update()
{
	m_SoundManager->Update();

	_resourceManager->Update();

	m_GameEngine->Update();

	// 스테이지 1, 2 로드 치트키 => 
	if (Muscle::KeyBoard::Get()->KeyDown(VK_F7))
	{
		if ((m_GameEngine->GetSceneManager()->GetCurrentSceneName() != "Stage1Scene") &&
			(m_GameEngine->GetSceneManager()->GetCurrentSceneName() != "IntroScene"))
			m_GameEngine->GetSceneManager()->LoadScene("Stage1Scene");
	}
	else if (Muscle::KeyBoard::Get()->KeyDown(VK_F8))
	{
		if ((m_GameEngine->GetSceneManager()->GetCurrentSceneName() != "Stage2Scene") &&
			(m_GameEngine->GetSceneManager()->GetCurrentSceneName() != "IntroScene"))
			m_GameEngine->GetSceneManager()->LoadScene("Stage2Scene");
	}
}

void GameProcess::Render()
{
	m_GameEngine->Render();
}

int GameProcess::MessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			// 해결!
			// width height 0이 들어가면 터진다.
		}
		else
		{
			if (m_GameEngine)
				m_GameEngine->OnResize();
		}

		break;
	case WM_MOUSEMOVE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
