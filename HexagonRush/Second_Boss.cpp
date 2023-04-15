#include "pch.h"
#include "Second_Boss.h"
#include "Stage2Scene.h"

#define STAGE_TWO_BOSS_HP 42840

Second_Boss::Second_Boss(std::shared_ptr<Muscle::GameObject> gameObj) : Character(gameObj)
{
	_checkStatus = Status::IDLE;
	_currentColor = StateColor::Red;
	_hp = STAGE_TWO_BOSS_HP;
	_Maxhp = STAGE_TWO_BOSS_HP;
}

Second_Boss::~Second_Boss()
{

}

void Second_Boss::SetColor(StateColor color)
{
	_currentColor = color;

	std::wstring colorIDStr;

	switch (_currentColor)
	{
	case StateColor::Red:
		colorIDStr = TEXT("Boss2_Core_Albedo_Red");
		break;
	case StateColor::Blue:
		colorIDStr = TEXT("Boss2_Core_Albedo_Blue");
		break;
	case StateColor::Green:
		colorIDStr = TEXT("Boss2_Core_Albedo_Green");
		break;
	case StateColor::None:
		colorIDStr = TEXT("Boss2_Core_Albedo_Red");
		break;
	default:
		break;
	}

	for (auto& iter : _coreRenderer)
	{
		auto& renderData = iter->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(colorIDStr);
	}
}

void Second_Boss::SetState(Status status)
{
	if (_checkStatus == status)
		return;
	if (_checkStatus == Status::DIE)
		return;

	switch (status)
	{
	case Second_Boss::ATTACK1:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack1>());
		break;
	case Second_Boss::ATTACK2:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack2>());
		break;
	case Second_Boss::ATTACK3:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack3>());
		break;
	case Second_Boss::ATTACK4:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack4>());
		break;
	case Second_Boss::ATTACK5:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack5>());
		break;
	case Second_Boss::ATTACK6:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack6>());
		break;
	case Second_Boss::ATTACK7:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack7>());
		break;
	case Second_Boss::ATTACK8:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack8>());
		break;
	case Second_Boss::ATTACK9:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Attack9>());
		break;
	case Second_Boss::IDLE:
		if (status == Status::DIE) return;
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Idle>());
		break;
	case Second_Boss::HIT:
		if (status == Status::DIE) return;

		GetAnimater()->SetAnimSpeed(0.1f, 0.1f);
		if (_checkStatus != Second_Boss::ATTACK1
			&& _checkStatus != Second_Boss::ATTACK2
			&& _checkStatus != Second_Boss::ATTACK3)
		{
			ObjectMover::Get()->Shake(GetGameObject()->GetTransform(), 0.3f, 0.4f, 0, true);
			Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameobject)
				{
					gameobject->GetTransform()->SetPosition(gameobject->GetComponent<Second_Boss>()->GetInitialPosition());
				}, 0.4f, GetGameObject());
		}
		else
			ObjectMover::Get()->Shake(GetGameObject()->GetTransform(), 0.3f, 0.4f, 0, false);
		// 색상에 대해서 
		GetSoundManager()->Play("BossHit", IPlayMode::Effect);


		return;
	case Second_Boss::DIE:
	{
		FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Die>());
		CameraMovementManager::Get()->BossDieMove();
		break;
	}

	case Second_Boss::NONE:
		break;
	default:
		break;
	}

	_checkStatus = status;
}

void Second_Boss::Attack(std::shared_ptr<Character> enermy)
{
	SetState(Second_Boss::Status(Muscle::CTime::GetRand<int>(0, 8)));
}

void Second_Boss::Attack()
{
	_time = 0;

	switch (_phase)
	{
	case Second_Boss::Phase1:
		SetState(Second_Boss::Status(Muscle::CTime::GetRand<int>(0, 2)));
		break;
	case Second_Boss::Phase2:
		SetState(Second_Boss::Status(Muscle::CTime::GetRand<int>(0, 5)));
		break;
	case Second_Boss::Phase3:
		SetState(Second_Boss::Status(Muscle::CTime::GetRand<int>(3, 8)));
		break;
	case Second_Boss::Phase4:
		SetState(Second_Boss::Status(Muscle::CTime::GetRand<int>(3, 8)));
		break;
	default:
		break;
	}
}

void Second_Boss::Hit(int damage)
{
	SetState(Second_Boss::HIT);
	_hp -= damage;

	// 패드 진동
	std::function<void(std::shared_ptr<Muscle::GameObject>)> func1 = [](std::shared_ptr<Muscle::GameObject> obj) { Muscle::XPad::Get()->StartVibration(); };

	std::function<void(std::shared_ptr<Muscle::GameObject>)> func2 = [](std::shared_ptr<Muscle::GameObject> obj) { Muscle::XPad::Get()->StopVibration(); };

	// 언제 맞을까 ..?
	Muscle::CTime::Invoke(func1, 0.3f, GetGameObject());

	Muscle::CTime::Invoke(func2, 0.8f, GetGameObject());

	for (auto& iter : _numberRenderers)
	{
		if (!iter->GetGameObject()->GetIsEnable())
		{
			iter->SetNumber(damage);
			iter->GetGameObject()->SetIsEnable(true);
			auto worldPosition = GetGameObject()->GetTransform()->GetWorldPosition();
			ObjectMover::Get()->MoveBy(iter->GetGameObject()->GetTransform(),
				Vector3(Muscle::CTime::GetFloatRand<float>(-10, 10), 0, Muscle::CTime::GetFloatRand<float>(-10, 10))
				, 1, 0, MoveInfo::Bezier,
				Vector3(0, Muscle::CTime::GetFloatRand<float>(10, 20), 0),
				Vector3(0, Muscle::CTime::GetFloatRand<float>(10, 20), 0));

			iter->Alpha_Hide_Disabled(0.5f);

			auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
				gameObject->GetTransform()->SetPosition(Muscle::CTime::GetFloatRand<float>(-10, 10), Muscle::CTime::GetFloatRand<float>(-1, 1), Muscle::CTime::GetFloatRand<float>(-10, 10));
			};
			Muscle::CTime::Invoke(func, 2.0f, iter->GetGameObject());

			break;
		}
	}
}

void Second_Boss::Die()
{
	SetState(Second_Boss::DIE);

	// 플레이어 무적 만들어주고.
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(_target.lock());

	StageClearScene::SetPrevScenePlayTime(Stage2Scene::GetPlayTime());

	StageClearScene::SetNextSceneName("EndingScene");

	if (player != nullptr)
		player->SetInvincible();
	GetSoundManager()->Play("Boss1Down", IPlayMode::Effect);
}

void Second_Boss::Finalize()
{
	Character::Finalize();

	FSM<Second_Boss>::Finalize();

	for (auto& iter : _coreRenderer)
	{
		iter.reset();
	}
	_coreRenderer.clear();

	for (auto& iter : _numberRenderers)
	{
		iter.reset();
	}
	_numberRenderers.clear();

	_dril.reset();

	_rightHand.reset();
}

void Second_Boss::ChangePhase(Phase phase)
{
	if (CameraMovementManager::Get()->GetisMove()) return;
	if (_checkStatus != Status::IDLE) return;
	if (_phase == phase) return;
	_phase = phase;




	switch (_phase)
	{
	case Second_Boss::Phase1:
		_attackSpeed = 5.0f;
		break;
	case Second_Boss::Phase2:
	{
		CameraMovementManager::Get()->BossPhaseChangeMove();

		//Muscle::CTime::Invoke(func, 4, GetGameObject());
		_attackSpeed = 5.0f;
	}
	break;
	case Second_Boss::Phase3:
	{

		CameraMovementManager::Get()->BossPhaseChangeMove();
		//auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
		//	gameObject->GetComponent<Boss>()->SetState(Status::ATTACK6);
		//};
		//// 4초인 이유는 4초뒤에 정지가 풀리기 때문이다..!
		//Muscle::CTime::Invoke(func, 4, GetGameObject());
		_attackSpeed = 3.0f;
	}
	break;
	case Second_Boss::Phase4:
	{
		CameraMovementManager::Get()->BossPhaseChangeMove();

		_attackSpeed = 3.0f;
	}
	default:
		break;
	}
}

void Second_Boss::Start()
{
	Character::Start();
	FSM<Second_Boss>::SetState(std::make_shared<Second_BossState::Idle>());

	_rightHand = GetGameObject()->FindChildren("Hand.R");

	_numberRenderers.resize(150);

	for (auto& iter : _numberRenderers)
	{
		auto obj = Muscle::CreateGameObject();
		obj->SetParent(GetGameObject());
		obj->GetTransform()->SetPosition(Muscle::CTime::GetFloatRand<float>(-10, 10), Muscle::CTime::GetFloatRand<float>(-1, 1), Muscle::CTime::GetFloatRand<float>(-10, 10));
		iter = obj->AddComponent<NumberRenderer>();
		obj->SetIsEnable(false);
	}

	std::list<std::shared_ptr<Muscle::SkinnedMeshRenderer>> _renderer;
	_renderer.emplace_back(GetGameObject()->FindChildren("arm_down.L")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("arm_down.R")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Body")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Foot.Left")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Foot.Right")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Hand.Left")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Leg.L")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Leg.R")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Shoulder.L")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Shoulder.R")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Upper.Leg.L")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Upper.Leg.R")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Upperarm.L")->GetComponent<Muscle::SkinnedMeshRenderer>());
	_renderer.emplace_back(GetGameObject()->FindChildren("Upperarm.R")->GetComponent<Muscle::SkinnedMeshRenderer>());

	_coreRenderer.emplace_back(GetGameObject()->FindChildren("Core_color")->GetComponent<Muscle::SkinnedMeshRenderer>());
	GetGameObject()->FindChildren("Dril")->SetIsEnable(false);


	for (auto iter : _renderer)
	{
		auto renderData = iter->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Albedo_Red"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Normal"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Metallic"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Roughness"));
		renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_AO"));
		renderData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Emissive"));

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO_Skinning_Rim");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO_Skinning_Rim");

		iter.reset();
	}
	for (auto iter : _coreRenderer)
	{
		auto renderData = iter->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Core_Albedo_Red"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Core_Normal"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Core_Matallic"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Core_Roughness"));

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning");

		iter.reset();
	}
	{
		_dril = Muscle::CreateFromPrefab(TEXT("Boss2_Dril"));
		_dril->AddComponent<DrillSpiner>();
		_dril->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
		_dril->GetTransform()->Pitch(3.14f / 2.0f);
		_dril->SetName("CopyDril");
		_dril->GetChildrens()[0]->SetName("CopyDril");
		_dril->SetIsEnable(true);

		_dril->SetParent(_rightHand);
	}

	_initPosition = GetGameObject()->GetTransform()->GetPosition();
}

void Second_Boss::Update()
{
	_currentStatus->OnStay();


	if (GameProcess::Get()->GetIsPausePanelUp()) return;
	if (CameraMovementManager::Get()->GetisMove()) return; // 카메라가 무브중이면 쓸 수 없다..!
	if (_checkStatus == Boss::DIE) return;

	if (_checkStatus == Boss::IDLE)
		_time += Muscle::CTime::GetGameDeltaTime();
	_colorTimer += Muscle::CTime::GetGameDeltaTime();

	if (_time > _attackSpeed)
		Attack();
	if (_colorTimer > 10) // 10초마다 색상 변경
	{
		_colorTimer = 0;
		SetColor(StateColor(Muscle::CTime::GetRand<int>(0, 2)));
	}
	float persent = ((float)_hp / (float)STAGE_TWO_BOSS_HP) * 100;

	if (persent >= 80)
		ChangePhase(Phase::Phase1);
	else if (persent >= 60)
		ChangePhase(Phase::Phase2);
	else if (persent >= 30)
		ChangePhase(Phase::Phase3);
	else if (persent > 0)
		ChangePhase(Phase::Phase4);
	else
		Die();


	if (Muscle::KeyBoard::Get()->KeyDown('1'))
		SetState(Second_Boss::ATTACK1);
	if (Muscle::KeyBoard::Get()->KeyDown('2'))
		SetState(Second_Boss::ATTACK2);
	if (Muscle::KeyBoard::Get()->KeyDown('3'))
		SetState(Second_Boss::ATTACK3);
	if (Muscle::KeyBoard::Get()->KeyDown('4'))
		SetState(Second_Boss::ATTACK4);
	if (Muscle::KeyBoard::Get()->KeyDown('5'))
		SetState(Second_Boss::ATTACK5);
	if (Muscle::KeyBoard::Get()->KeyDown('6'))
		SetState(Second_Boss::ATTACK6);
	if (Muscle::KeyBoard::Get()->KeyDown('7'))
		SetState(Second_Boss::ATTACK7);
	if (Muscle::KeyBoard::Get()->KeyDown('8'))
		SetState(Second_Boss::ATTACK8);
	if (Muscle::KeyBoard::Get()->KeyDown('9'))
		SetState(Second_Boss::ATTACK9);
}
