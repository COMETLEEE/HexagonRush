#include "pch.h"
#include "CameraMovementManager.h"
#include "SceneFader.h"


std::shared_ptr<CameraMovementManager> CameraMovementManager::_instance = nullptr;

CameraMovementManager::CameraMovementManager(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject)
{
	_isMove = false;
	_timer = 0;

}

CameraMovementManager::~CameraMovementManager()
{
}

void CameraMovementManager::Stage1Start()
{
	_camera = Muscle::IGameEngine::Get()->GetMainCamera();

	std::vector<std::shared_ptr<Muscle::GameObject>> sceneObjects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : sceneObjects)
	{
		if (object->GetName() == "Player")
		{
			_player = object;
		}
		if (object->GetName() == "Boss")
		{
			_boss = object;
		}
	}

	Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(true);

	GetComponent<SceneFader>()->FadeIn(2.0f);

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeOut(2.0f, nullptr); }, 3, _instance->GetGameObject());

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeIn(2.0f); }, 5, _instance->GetGameObject());

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeOut(2.0f, nullptr); }, 7, _instance->GetGameObject());

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeIn(3.0f); }, 10, _instance->GetGameObject());

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(false); }, 10, _instance->GetGameObject());

	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(-2.5f, 22.0f, 3.8f), Vector3(-2.5f, 22.0f, -3.8f), 5, 0, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::LookType::Look,
		false, Vector3(-2.5f, 22.0f, 0.0f) + Vector3(-10.0f, 0, 0));

	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(-11.6f, 9.0f, 3.8f), Vector3(-11.6f, 9.0f, -3.8f), 5, 5, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::LookType::Look,
		false, Vector3(11.6f, 12.48f, 0.0f) + Vector3(10.0f, 0, 0));

	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(108, 12, 0), Vector3(45, 43, -10), 5, 10, MoveInfo::Bezier, Vector3(-10, -10, 0), Vector3(0, -40, 0), MoveInfo::LookType::Look,
		false, _player->GetTransform()->GetWorldPosition());

	ObjectMover::Get()->MoveTo(_camera->GetTransform(), Vector3(42, 36, 25), 5, 15, MoveInfo::Bezier, Vector3(-140, 10, -90), Vector3(-130, -50, 80), MoveInfo::LookType::Look,
		false, _player->GetTransform()->GetWorldPosition());

	_timer += 20.0f;
	_isMove = true;
}

void CameraMovementManager::Stage2Start()
{
	_camera = Muscle::IGameEngine::Get()->GetMainCamera();
	_camera->GetTransform()->SetPosition(-21.4f, 7.0f, 10.5f);
	std::vector<std::shared_ptr<Muscle::GameObject>> sceneObjects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : sceneObjects)
	{
		if (object->GetName() == "Player")
		{
			_player = object;
		}
		if (object->GetName() == "Boss")
		{
			_boss = object;
		}
	}
	Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(true);

	///// ------------------------------------------------------------------ 보스 등장 ------------------------------------------
	GetComponent<SceneFader>()->FadeIn(2.0f);
	ObjectMover::Get()->MoveFromTo(_boss->GetTransform(), Vector3(-30.5f, 400.0f, -30.8f), Vector3(-27, 4.0f, 0), 4, 0, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::LookType::Look,
		false);
	_camera->LookAt(Vector3(-30.5f, 400.0f, -30.8f), Vector3(-21.4f, 7.0f, 10.5f));
	ObjectMover::Get()->MoveBy(_camera->GetTransform(), Vector3(0, 0, 0), 4.0f, 0, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::Look, false, Vector3::Zero, _boss->GetTransform());
	ObjectMover::Get()->Shake(_camera->GetTransform(), 1.5f, 0.2f, 4.0f);
	// 보스 등장...//
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("StartLanding", IPlayMode::Effect); }, 2.5f, nullptr);
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetTransform()->SetPosition(gameObject->GetComponent<Second_Boss>()->GetInitialPosition()); }, 4.1f, _boss);
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Big_Impact", IPlayMode::Effect); }, 4.0f, nullptr);

	ObjectMover::Get()->LinearLook(_camera->GetTransform(), 2.0f, nullptr, _boss->GetTransform()->GetWorldPosition() + Vector3(0, 20, 0), 1, 4.5f);

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeOut(2.0f, nullptr); }, 4.5f, _instance->GetGameObject()); // 페이드 아웃. //6.5f

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeIn(2.0f); }, 6.5f, _instance->GetGameObject()); // 6.5f

	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(-14.9f, 22.48f, -1.19f) - Vector3(-0.309415, -0.656778, -6.87678) * 2, Vector3(-14.9f, 22.48f, -1.19f), 3.0f, 6.5f, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::LookType::Look,
		false, Vector3(-25, 2.0f, -15.0f)); // 드릴 감상

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeIn(2.0f); }, 9.5f, _instance->GetGameObject()); // 6.5f

	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(-51.7086f, 6.210144f, -0.248639f), Vector3(-10.0f, 16.0f, 0), 5, 9.5f, MoveInfo::Bezier, Vector3(-10, 20, 0), Vector3(10, 20, 0), MoveInfo::LookType::Look,
		false, Vector3(-30.0f, 18.0f, 0));
	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(-10.0f, 16.0f, 0), Vector3(-13.0f, 16.0f, 0), 0.15f, 14.5f, MoveInfo::Linear, Vector3(-10, 20, 0), Vector3(10, 20, 0), MoveInfo::LookType::Look); // 보스 얼굴 줌인.
	ObjectMover::Get()->Shake(_camera->GetTransform(), 0.2f, 0.1f, 14.65f);
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Big_Impact", IPlayMode::Effect); }, 14.65f, nullptr); // 페이드 아웃. //6.5f
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeOut(2.0f, nullptr); }, 15.0f, _instance->GetGameObject()); // 페이드 아웃. //6.5f
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeIn(1.0f); }, 17.0f, _instance->GetGameObject()); // 페이드 인
	///// ------------------------------------------------------------------ 보스 등장 ------------------------------------------

	/// ------------------------------------------------------ 플레이어 등장 ----------------------------------------
	float walkOffset = 17.5f;
	ObjectMover::Get()->MoveFromTo(_player->GetTransform(), Vector3(32.4f, 4.2f, 0.0f), Vector3(22.4f, 4.2f, 0.0f), 0.5f, 17.0f, MoveInfo::Linear); // 걸어온다.
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<Muscle::Animater>()->SetAnimSpeed(2.0f, 100.0f); gameObject->GetComponent<Muscle::Animater>()->Play("Ch_Run"); }, 17.0, _player);


	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<Muscle::Animater>()->SetAnimSpeed(2.0f, 100.0f); gameObject->GetComponent<Muscle::Animater>()->Play("Ch_evasion");  }, walkOffset, _player); // 다이빙
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {Muscle::CTime::SetGameSpeed(0.5f, 0.9f);  }, walkOffset + 0.1f, nullptr); // 다이빙 슬로우 모션

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<Muscle::Animater>()->Play("Ch_Attack_Rush"); gameObject->GetTransform()->SetPosition(0, 4.2f, 0); }, 0.91f + walkOffset, _player); // 발사
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Rush", IPlayMode::Effect); ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.2f); }, 0.96f + walkOffset, nullptr); // 발사
	ObjectMover::Get()->MoveFromTo(_player->GetTransform(), Vector3(22.4f, 4.2f, 0.0f), Vector3(0.0f, 4.2f, 0.0f), 0.9f, walkOffset,
		MoveInfo::Bezier, Vector3(1.0f, 8.0f, 0.0f), Vector3(0.0f, -2.0f, 0.0f));

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->SetPosition(Vector3(-11.6f, 8.3f, 3.8f)); Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->LookAt(Vector3(32.4f, 4.2f, 0.0f)); }, 17.0f, nullptr); // 카메라 시작 위치 세팅

	ObjectMover::Get()->MoveFromTo(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), Vector3(10.7f, 5.7f, 3.9f), Vector3(-11.6f, 8.3f, 3.8f), 1.7f, walkOffset - 0.50f,
		MoveInfo::Bezier, Vector3(0.0f, 3.0f, 0.0f), Vector3(0.0f, -1.0f, -2.0f));

	ObjectMover::Get()->MoveBy(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetLook(), 0.1f, walkOffset  + 1.8f); // 줌인
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Impact", IPlayMode::Effect); ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.2f); }, walkOffset  + 1.8f + 0.1f, nullptr); // 화면 흔들림 + 효과음

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {
		ObjectMover::Get()->LinearLook(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 1.7f, gameObject->GetTransform(), Vector3::Zero, 2.0f, 0.0f, Vector3(-1.0f, 2.3f, 0));
		ObjectMover::Get()->LinearLook(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.11f, gameObject->GetTransform(), Vector3::Zero, 1.0f, 1.7f, Vector3(0, 2.5f, 0)); // 18.11
		}, 17.0f, _player);



	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), Vector3(-11.6f, 7.0f, 3.8f), Vector3(-11.6f, 7.0f, -3.8f), 4.0f, 19.5f, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::LookType::Look, // 플레이어 등장! 22.11f
		false, Vector3(11.6f, 12.48f, 0.0f) + Vector3(10.0f, 0, 0));

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {gameObject->GetComponent<SceneFader>()->FadeIn(2.0f); }, 23.5f, _instance->GetGameObject()); // 6.5f

	//// 페이드 인 넣자~
	ObjectMover::Get()->MoveFromTo(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), Vector3(-5.663, 6.7654, 10.800), Vector3(-5.663, 6.7654, 29.800), 3.0f, 23.5f, MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::Look, false, Vector3(-5.663, 6.7654, 10.800) + Vector3(-0.135261, 0.114147, -0.984210) * 30);
	///// ------------------------------------------------------ 플레이어 등장 ----------------------------------------






	_timer += 28.0f;
	_isMove = true;
}

void CameraMovementManager::BossDieMove()
{
	Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(true);
	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), _player->GetTransform()->GetPosition() + Vector3(5, 5, -3), _boss->GetTransform()->GetPosition() + Vector3(35, 25, 10), 4, 0,
		MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::Look, false, _boss->GetTransform()->GetPosition() + Vector3(0, 13, 0)
	);
	auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
		Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(false);
		gameObject->GetComponent<SceneFader>()->FadeOut(3.0f, []() { Muscle::IGameEngine::Get()->GetSceneManager()->LoadScene("StageClearScene"); });

	};
	Muscle::CTime::Invoke(func, 4, _instance->GetGameObject());

	_timer = 4.0f;
	_isMove = true;
}

void CameraMovementManager::BossPhaseChangeMove()
{
	Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(true);
	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), _player->GetTransform()->GetPosition() + Vector3(5, 5, -3), _boss->GetTransform()->GetPosition() + Vector3(35, 25, 10), 3, 0,
		MoveInfo::Linear, Vector3::Zero, Vector3::Zero, MoveInfo::Look, false, _boss->GetTransform()->GetPosition() + Vector3(0, 13, 0)
	);

	auto dangerUI = std::make_shared<DangerUI_Prefab>()->GetGameObject();

	auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
		Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(false);
		gameObject->GetComponent<SceneFader>()->FadeIn(2.0f);
	};
	Muscle::CTime::Invoke(func, 3.0f, _instance->GetGameObject());
	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {
		Muscle::DeleteGameObject(gameObject);
		}, 3.0f, dangerUI);
	_timer = 3.0f;
	_isMove = true;
}

void CameraMovementManager::PlayerDieMove()
{
	Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(true);
	Muscle::CTime::SetGameSpeed(0.4f, 5.0f);
	GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::Effect);
	GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::BGM);

	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), _camera->GetTransform()->GetPosition(), _player->GetTransform()->GetPosition() + Vector3(-13, 20, -5), 2, 0,
		MoveInfo::Bezier, Vector3(-20, -30, -20), Vector3(-30, -15, 20), MoveInfo::Look, false, _player->GetTransform()->GetPosition() + Vector3(0, 3, 0)
	);


	auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
		Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(false);
		GetSoundManager()->SetSoundSpeed(1.0f, IPlayMode::Effect);
		GetSoundManager()->SetSoundSpeed(1.0f, IPlayMode::BGM);
		gameObject->GetComponent<SceneFader>()->FadeOut(3.0f, []() { Muscle::IGameEngine::Get()->GetSceneManager()->LoadScene("DieScene"); });
	};

	Muscle::CTime::Invoke(func, 1.0f, _instance->GetGameObject());
	_timer = 2.0f;
	_isMove = true;
}

void CameraMovementManager::PlayerRushMove()
{
	Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(true);

	// 현재 플레이어의 더미 모델이 Look이 Up, Up이 Look Vector여서 이러한 결과가 일어난다.
	Vector3 playerLook = -_player->GetTransform()->GetLook();

	Vector3 playerUp = _player->GetTransform()->GetUp();

	Vector3 PlayerRight = _player->GetTransform()->GetRight();

	Vector3 destination = _player->GetTransform()->GetWorldPosition() - playerLook * 4.f + playerUp * 2.0f + PlayerRight * 3.f;

	// Rush Speed가 0.2니까 0.1f De => 0.5초 !
	ObjectMover::Get()->MoveFromTo(_camera->GetTransform(), _camera->GetTransform()->GetPosition(), destination, 0.1f, 0.0f,
		MoveInfo::Bezier, Vector3(5.f, 5.f, 0.f), Vector3(10.f, 10.f, 0.f),
		MoveInfo::Look);

	ObjectMover::Get()->LinearLook(_camera->GetTransform(), 1.0f, _boss->GetTransform(), Vector3::Zero, 5.0f, 0, Vector3(0.0f, 10.0f, 0.0f));

	std::function<void(std::shared_ptr<Muscle::GameObject> gameObject)> callBack = [](std::shared_ptr<Muscle::GameObject> gameObject)
	{
		Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(false);
	};

	// 0.2로 5초 => 1초
	// 러쉬 시간만큼 후 비네팅 끈다.
	Muscle::CTime::Invoke(callBack, PlayerState::RUSH_FULL_TIME * PlayerState::RUSH_GAME_SPEED, _instance->GetGameObject());
	_timer = 1.0f;
	_isMove = true;
}

bool CameraMovementManager::GetisMove()
{
	return _isMove;
}

std::shared_ptr<CameraMovementManager> CameraMovementManager::Get()
{
	if (!_instance)
	{
		_instance = Muscle::CreateGameObject()->AddComponent<CameraMovementManager>();
		_instance->AddComponent<Muscle::UIRenderer>();
		_instance->AddComponent<SceneFader>();
	}

	return _instance;
}

void CameraMovementManager::Update()
{
#ifdef _DEBUG
	if (Muscle::KeyBoard::Get()->KeyPress(VK_SHIFT))
		Muscle::CTime::SetGameSpeed(4, 100);
	if (Muscle::KeyBoard::Get()->KeyUp(VK_SHIFT))
		Muscle::CTime::SetGameSpeed(1, 0);

	float _speed = 10.0f;

	if (Muscle::KeyBoard::Get()->KeyPress(VK_SHIFT))
		_speed = 30.0f;

	// 카메라 TEST용 키 인풋
	if (Muscle::KeyBoard::Get()->KeyPress('W'))
		Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->Walk(1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('S'))
		Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->Walk(-1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('A'))
		Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->Strafe(-1.0 * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('D'))
		Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->Strafe(1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('Q'))
		Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->WorldUpDown(-1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('E'))
		Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->WorldUpDown(1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	auto _MouseMoveUpdate = []() {

		static float m_LastMousePosx = 0;
		static float m_LastMousePosy = 0;

		if ((Muscle::KeyBoard::Get()->KeyPress(VK_RBUTTON)))
		{
			// Make each pixel correspond to a quarter of a degree.
			float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(Muscle::KeyBoard::Get()->GetPos().x - m_LastMousePosx));
			float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(Muscle::KeyBoard::Get()->GetPos().y - m_LastMousePosy));

			Muscle::MuscleEngine::GetInstance()->GetMainCamera()->Pitch(dy);
			Muscle::MuscleEngine::GetInstance()->GetMainCamera()->RotateY(dx);
		}

		m_LastMousePosx = Muscle::KeyBoard::Get()->GetPos().x;
		m_LastMousePosy = Muscle::KeyBoard::Get()->GetPos().y;
	};
	_MouseMoveUpdate();

#endif // DEBUG

	if (_timer > 0)
	{

		_isMove = true;
		_timer -= Muscle::CTime::GetGameDeltaTime();
	}
	else
	{
		_isMove = false;
		_timer = 0;
	}
}

void CameraMovementManager::LateUpdate()
{
	if (_timer > 0)
	{
		_isMove = true;
	}
	else
	{
		_isMove = false;
	}

	if (_isMove) return;

	Vector3 FixedPos = _player->GetTransform()->GetPosition() + Vector3(42, 36, 25); // + 벡터는 오프셋값

	if (FixedPos.x > 57.0f)
		FixedPos.x = 57.0f;
	if (FixedPos.z < 15.0f)
		FixedPos.z = 15.0f;
	if (FixedPos.z > 36.0f)
		FixedPos.z = 36.0f;

	// 플레이어 따라오게 하는 코드
	Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->SetPosition(XMVectorLerp(_camera->GetTransform()->GetPosition(), FixedPos, Muscle::CTime::GetGameDeltaTime())); // 속도를 빠르게 하고싶다면 t 값을 크게 하자..
	Vector3 look = XMVectorLerp(_camera->GetTransform()->GetLook(), Vector3(-0.792783, -0.452080, -0.452080), Muscle::CTime::GetGameDeltaTime() * 2);
	look.Normalize();
	Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->SetLook(look); // 속도를 빠르게 하고싶다면 t 값을 크게 하자..
}

void CameraMovementManager::Start()
{

}

void CameraMovementManager::Finalize()
{
	_instance.reset();
}
