#include "pch.h"
#include "PausePanel_Prefab.h"

#include "SceneFader.h"
#include "ButtonBase.h"
#include "PausePanelController.h"

PausePanel_Prefab::PausePanel_Prefab(std::string currSceneName) : IPrefab(Muscle::CreateGameObject())
{
	// Prefab 객체는 참조 카운트가 사라져서 삭제됨 ..!
	_currSceneName = currSceneName;

	// SceneFader (Restart, Title 등을 위하여)
	GetGameObject()->AddComponent<Muscle::UIRenderer>();

	_fader = GetGameObject()->AddComponent<SceneFader>();


	// Panel의 맨 뒤 이미지 한 장
	std::shared_ptr<Muscle::GameObject> panelBack = Muscle::CreateGameObject();

	panelBack->AddComponent<Muscle::UIRenderer>();

	panelBack->GetComponent<Muscle::UIRenderer>()->SetSortOrder(3);

	panelBack->GetComponent<Muscle::UIRenderer>()->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Pause_Panel")));
	 

	std::shared_ptr<Muscle::GameObject> cursor = Muscle::CreateGameObject();

	cursor->AddComponent<Muscle::UIRenderer>();

	cursor->GetComponent<Muscle::UIRenderer>()->SetSortOrder(2);

	cursor->GetComponent<Muscle::UIRenderer>()->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Cursor")));



	// RESUME
	std::shared_ptr<Muscle::GameObject> resumeButton = Muscle::CreateGameObject();

	resumeButton->AddComponent<Muscle::UIRenderer>();

	resumeButton->GetComponent<Muscle::UIRenderer>()->SetSortOrder(2);

	resumeButton->GetComponent<Muscle::UIRenderer>()->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Pause_Resume")));
	
	resumeButton->AddComponent<ButtonBase>();

	resumeButton->GetComponent<ButtonBase>()->RegistCallBack(nullptr);
	// RESUME




	// RESTART
	std::shared_ptr<Muscle::GameObject> restartButton = Muscle::CreateGameObject();

	restartButton->AddComponent<Muscle::UIRenderer>();

	restartButton->GetComponent<Muscle::UIRenderer>()->SetSortOrder(2);

	restartButton->GetComponent<Muscle::UIRenderer>()->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Pause_Restart")));
	
	restartButton->AddComponent<ButtonBase>();

	// 시간을 되돌리고 FadeOut 후 다시 현재 씬을 시작합니다.
	auto restart = [&](void) {  Muscle::CTime::SetGameSpeed(1.f, 0.f); 
	_fader->FadeOut(5.f, [&](void) {

			Muscle::MuscleEngine::GetInstance()->GetSceneManager()->LoadScene(_currSceneName);
		});
	};

	restartButton->GetComponent<ButtonBase>()->RegistCallBack(restart);
	// RESTART




	// TITLE
	std::shared_ptr<Muscle::GameObject> titleButton = Muscle::CreateGameObject();

	titleButton->AddComponent<Muscle::UIRenderer>();

	titleButton->GetComponent<Muscle::UIRenderer>()->SetSortOrder(2);

	titleButton->GetComponent<Muscle::UIRenderer>()->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Pause_Title")));

	titleButton->AddComponent<ButtonBase>();

	auto title = [&](void) { Muscle::CTime::SetGameSpeed(1.f, 0.f);
	_fader->FadeOut(5.f, [&](void) {
		Muscle::MuscleEngine::GetInstance()->GetSceneManager()->LoadScene("TitleScene");
		
		});
	};

	titleButton->GetComponent<ButtonBase>()->RegistCallBack(title);
	// TITLE





	// EXIT
	std::shared_ptr<Muscle::GameObject> exitButton = Muscle::CreateGameObject();

	exitButton->AddComponent<Muscle::UIRenderer>();

	exitButton->GetComponent<Muscle::UIRenderer>()->SetSortOrder(2);

	exitButton->GetComponent<Muscle::UIRenderer>()->SetTextureID(ResourceManager::Get()->GetTextureID(TEXT("Pause_Exit")));

	exitButton->AddComponent<ButtonBase>();

	auto exitt = [&](void) { _fader->FadeOut(5.f, [&](void) {
			// 프로그램 종료 !
			exit(0);
		});		
	};

	exitButton->GetComponent<ButtonBase>()->RegistCallBack(exitt);
	// EXIT





	// Panel Controller
	GetGameObject()->AddComponent<PausePanelController>();

	// => 위의 PausePanelController에서 캐싱 후 버튼들을 컨트롤함
	cursor->SetTag("Cursor");
	GetGameObject()->SetChild(cursor);

	panelBack->SetTag("Panel");
	GetGameObject()->SetChild(panelBack);

	resumeButton->SetTag("Resume");
	GetGameObject()->SetChild(resumeButton);

	restartButton->SetTag("Restart");
	GetGameObject()->SetChild(restartButton);

	titleButton->SetTag("Title");
	GetGameObject()->SetChild(titleButton);

	exitButton->SetTag("Exit");
	GetGameObject()->SetChild(exitButton);
}

PausePanel_Prefab::~PausePanel_Prefab()
{
	_fader.reset();
}