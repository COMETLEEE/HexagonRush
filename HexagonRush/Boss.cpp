#include "pch.h"
#include "Boss.h"

#include "SceneFader.h"
#include "Stage1Scene.h"
//#include "TutorialBossAttackPattern.h"

#define STAGE_ONE_BOSS_HP 37128


Boss::Boss(std::shared_ptr<Muscle::GameObject> gameObj) : Character(gameObj)
{
	_checkStatus = Status::IDLE;

	_currentColor = StateColor::Red;

	_phase = Phase::Phase1;

	_hp = STAGE_ONE_BOSS_HP;

	_Maxhp = STAGE_ONE_BOSS_HP;
}

Boss::~Boss()
{
}

void Boss::SetColor(StateColor color)
{
	_currentColor = color;

	std::wstring colorIDStr;

	switch (_currentColor)
	{
	case StateColor::Red:
		colorIDStr = TEXT("core_and_missile_Albedo_Red");
		break;
	case StateColor::Blue:
		colorIDStr = TEXT("core_and_missile_Albedo_Blue");
		break;
	case StateColor::Green:
		colorIDStr = TEXT("core_and_missile_Albedo_Green");
		break;
	case StateColor::None:
		colorIDStr = TEXT("core_and_missile_Albedo_Red");
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

void Boss::SetState(Status status)
{
	if (_checkStatus == status)
		return;
	if (_checkStatus == Status::DIE)
		return;

	switch (status)
	{
	case Boss::ATTACK1:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack1>());
		break;
	case Boss::ATTACK2:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack2>());
		break;
	case Boss::ATTACK3:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack3>());
		break;
	case Boss::ATTACK4:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack4>());
		break;
	case Boss::ATTACK6:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack6>());
		break;
	case Boss::ATTACK7:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack7>());
		break;
	case Boss::ATTACK8:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack8>());
		break;
	case Boss::ATTACK9:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack9>());
		break;
	case Boss::ATTACK10:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Attack10>());
		break;
	case Boss::IDLE:
		if (status == Status::DIE) return;
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Idle>());
		break;
	case Boss::HIT:
		if (status == Status::DIE) return;

		GetAnimater()->SetAnimSpeed(0.1f, 0.1f);
		ObjectMover::Get()->Shake(GetGameObject()->GetTransform(), 0.3f, 0.4f, 0, true);
		// 색상에 대해서 
		GetSoundManager()->Play("BossHit", IPlayMode::Effect);
		Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameobject) {gameobject->GetTransform()->SetPosition(gameobject->GetComponent<Boss>()->GetInitialPosition()); }, 0.15f, GetGameObject());
		//FSM<Boss>::SetState(std::make_shared<Tu_BossState::Hit>());
		return;
	case Boss::DIE:
	{
		FSM<Boss>::SetState(std::make_shared<Tu_BossState::Die>());

		

		CameraMovementManager::Get()->BossDieMove();
		StageClearScene::SetNextSceneName("Stage2Scene");

		//std::shared_ptr<SceneFader> fader = GetGameObject()->GetComponent<SceneFader>();

		//fader->FadeOut(10.f, []() { Muscle::IGameEngine::Get()->GetSceneManager()->LoadScene("StageClearScene"); });

		break;
	}

	case Boss::NONE:
		break;
	default:
		break;
	}

	_checkStatus = status;
}

void Boss::ChangePhase(Phase phase)
{
	if (CameraMovementManager::Get()->GetisMove()) return;
	if (_checkStatus != Status::IDLE) return;
	if (_phase == phase) return;
	_phase = phase;

	switch (_phase)
	{
	case Boss::Phase1:
		break;
	case Boss::Phase2:
	{

		CameraMovementManager::Get()->BossPhaseChangeMove();
		auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
			gameObject->GetComponent<Boss>()->SetState(Status::ATTACK3);
		};
		// 4초인 이유는 4초뒤에 정지가 풀리기 때문이다..!
		Muscle::CTime::Invoke(func, 4, GetGameObject());
		_attackSpeed = 2.0f;
	}
	break;
	case Boss::Phase3:
	{

		for (int i = 4; i < 8; i++)
		{
			Muscle::AnimEventKey animEventKey;

			auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {

				auto gameObj = std::make_shared<Missile_Prefab>()->GetGameObject();
				gameObj->SetTag("Enemy");
				auto rederingData = gameObj->GetChildrens()[0]->GetComponent<Muscle::RendererBase>()->_renderingData;

				std::wstring colorIDStr;
				switch (gameObject->GetComponent<Boss>()->GetColor())
				{
				case StateColor::Red:
					colorIDStr = TEXT("core_and_missile_Albedo_Red");
					break;
				case StateColor::Blue:
					colorIDStr = TEXT("core_and_missile_Albedo_Blue");
					break;
				case StateColor::Green:
					colorIDStr = TEXT("core_and_missile_Albedo_Green");
					break;
				case StateColor::None:
					colorIDStr = TEXT("core_and_missile_Albedo_Red");
					break;
				default:
					break;
				}

				rederingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(colorIDStr);
				rederingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Normal"));
				rederingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Metallic"));
				rederingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Roughness"));
				rederingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_AO"));

				rederingData->_shaderInfo->_vsName = TEXT("VS_ANMR");
				rederingData->_shaderInfo->_psName = TEXT("PS_ANMR");

			};
			animEventKey._eventfunc = func;
			animEventKey._eventKey = false;
			animEventKey._name = "Attack4";
			animEventKey._targetFream = (1 + i) * 10.0f * 160;
			animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
			GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack4_missile", animEventKey); // 이벤트 등록 완료.
		}


		CameraMovementManager::Get()->BossPhaseChangeMove();
		auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
			gameObject->GetComponent<Boss>()->SetState(Status::ATTACK6);
		};
		// 4초인 이유는 4초뒤에 정지가 풀리기 때문이다..!
		Muscle::CTime::Invoke(func, 4, GetGameObject());
		_attackSpeed = 1.0f;
	}
	break;
	default:
		break;
	}
}

void Boss::Attack(std::shared_ptr<Character> enermy)
{
	if (enermy)
		enermy->Hit(_damage);
}

void Boss::Attack()
{
	// 랜덤 공격
	_time = 0;
	switch (_phase)
	{
	case Boss::Phase1:
		SetState(Boss::Status(Muscle::CTime::GetRand<int>(0, 4)));
		break;
	case Boss::Phase2:
		SetState(Boss::Status(Muscle::CTime::GetRand<int>(0, 6)));
		break;
	case Boss::Phase3:
		SetState(Boss::Status(Muscle::CTime::GetRand<int>(0, 8)));
		break;
	default:
		break;
	}
}

void Boss::Hit(int damage)
{
	// 맞는 모션으로 애니메이션 변경?
	SetState(Boss::HIT);

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
			Vector3(Muscle::CTime::GetFloatRand<float>(-10, 10), 0,Muscle::CTime::GetFloatRand<float>(-10, 10))
			, 1,0,MoveInfo::Bezier,
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

void Boss::Die()
{
	SetState(Boss::DIE);

	// 플레이어 무적 만들어주고.
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(_target.lock());
	
	StageClearScene::SetPrevScenePlayTime(Stage1Scene::GetPlayTime());

	StageClearScene::SetNextSceneName("Stage2Scene");

	if (player != nullptr)
		player->SetInvincible();

	GetSoundManager()->Play("Boss1Down", IPlayMode::Effect);
}

void Boss::Finalize()
{
	Character::Finalize();

	FSM<Boss>::Finalize();

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
}


std::shared_ptr<Muscle::GameObject> Boss::GetHand1()
{
	return _hand1.lock();
}

std::shared_ptr<Muscle::GameObject> Boss::GetHand2()
{
	return _hand2.lock();
}

void Boss::Start()
{
	Character::Start();

	FSM<Boss>::SetState(std::make_shared<Tu_BossState::Idle>());

	auto obj = Muscle::CreateGameObject();

	obj->SetParent(GetGameObject());

	_numberRenderers.resize(150);

	for (auto& iter : _numberRenderers)
	{
		auto obj = Muscle::CreateGameObject();
		obj->SetParent(GetGameObject());
		obj->GetTransform()->SetPosition(Muscle::CTime::GetFloatRand<float>(-10, 10), Muscle::CTime::GetFloatRand<float>(-1, 1), Muscle::CTime::GetFloatRand<float>(-10, 10));
		iter = obj->AddComponent<NumberRenderer>();
		obj->SetIsEnable(false);
	}

	//GetGameObject()->SetChild(_numberRenderer->GetGameObject());

	_hand1 = GetGameObject()->FindChildren("Finger.R");
	_hand2 = GetGameObject()->FindChildren("Finger.L");

	// 자식을 찾아서 맵을 입혀줘야한다..!
	// arm and Finger
	std::list<std::shared_ptr<Muscle::GameObject>> armAndFingerList;

	armAndFingerList.emplace_back(GetGameObject()->FindChildren("arm_down"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("arm_down.001"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("arm_up"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("arm_up.001"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("fingers_instance.001"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("fingers_instance.006"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("body"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("body_cup"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("body_in"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("eyelash"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("eyebrow"));
	armAndFingerList.emplace_back(GetGameObject()->FindChildren("head_neck"));

	for (auto iter : armAndFingerList)
	{
		auto renderData = iter->GetComponent<Muscle::SkinnedMeshRenderer>()->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("black_armandfinger_Albedo"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("black_armandfinger_Normal"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("black_armandfinger_Metallic"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("black_armandfinger_Roughness"));
		// renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("black_armandfinger_AO"));

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO_Skinning");

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning");

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning_Rim");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning_Rim");

		iter.reset();
	}

	std::list<std::shared_ptr<Muscle::GameObject>> missileAndCoreList;

	missileAndCoreList.emplace_back(GetGameObject()->FindChildren("core"));
	missileAndCoreList.emplace_back(GetGameObject()->FindChildren("rocket"));

	// 이거 Ambient Occlusion 이상하게 나와서 .. 뺍니다
	for (auto iter : missileAndCoreList)
	{
		auto renderData = iter->GetComponent<Muscle::SkinnedMeshRenderer>()->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Albedo_Red"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Normal"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Metallic"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_Roughness"));
		//renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("core_and_missile_AO"));

		/*renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO_Skinning");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO_Skinning");*/

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning");

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning_Rim");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning_Rim");

		_coreRenderer.emplace_back(iter->GetComponent<Muscle::SkinnedMeshRenderer>());
		iter.reset();
	}

	std::list<std::shared_ptr<Muscle::GameObject>> eyeList;
	eyeList.emplace_back(GetGameObject()->FindChildren("eyes"));

	// 이거 Ambient Occlusion 이상하게 나와서 .. 뺍니다
	for (auto iter : eyeList)
	{
		auto renderData = iter->GetComponent<Muscle::SkinnedMeshRenderer>()->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("eye_Albedo"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("eye_Metallic"));
		//renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("eye_AO"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("eye_Normal"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("eye_Roughness"));

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO_Skinning");
		////renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO_Skinning");
		// 
		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning");

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning_Rim");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning_Rim");

		iter.reset();
	}


	std::list<std::shared_ptr<Muscle::GameObject>> coreCoverLightBrownList;

	coreCoverLightBrownList.emplace_back(GetGameObject()->FindChildren("core_cover002"));

	// 이거 Ambient Occlusion 이상하게 나와서 .. 뺍니다
	for (auto iter : coreCoverLightBrownList)
	{
		auto renderData = iter->GetComponent<Muscle::SkinnedMeshRenderer>()->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("core_cover_lightbrown_Albedo"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("core_cover_lightbrown_Metallic"));
		// renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("core_cover_lightbrown_AO"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("core_cover_lightbrown_Normal"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("core_cover_lightbrown_Roughness"));

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning_Rim");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning_Rim");

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning");

		/*renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO_Skinning");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO_Skinning");*/
		iter.reset();
	}

	std::list<std::shared_ptr<Muscle::GameObject>> yellow_bodycolorList;

	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("chest_cover"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("head_mouth"));

	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("head_top"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("rocket_cover"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("shoulder"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("shoulder.001"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("arms.001"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("arms.002"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("arm_mid"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("arm_mid.001"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("arm_mid.002"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("arm_mid.003"));
	yellow_bodycolorList.emplace_back(GetGameObject()->FindChildren("core_cover"));

	// 이거 Ambient Occlusion 이상하게 나와서 .. 뺍니다
	for (auto iter : yellow_bodycolorList)
	{
		auto renderData = iter->GetComponent<Muscle::SkinnedMeshRenderer>()->_renderingData;

		renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Albedo"));
		renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Metallic"));
		// renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_AO"));
		renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Normal"));
		renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Roughness"));

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO_Skinning");

		//renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning");
		//renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning");

		renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Skinning_Rim");
		renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Skinning_Rim");

		iter.reset();
	}

	// 이거 Ambient Occlusion 이상하게 나와서 .. 뺍니다
	auto renderData = GetGameObject()->FindChildren("floor")->GetComponent<Muscle::RendererBase>()->_renderingData;
	renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Albedo"));
	renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Metallic"));
	// renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_AO"));
	renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Normal"));
	renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("yellow_bodycolor_Roughness"));
	/*renderData->_shaderInfo->_vsName = TEXT("VS_ANMR");
	renderData->_shaderInfo->_psName = TEXT("PS_ANMR");*/
	renderData->_shaderInfo->_vsName = TEXT("VS_ANMR_Rim");
	renderData->_shaderInfo->_psName = TEXT("PS_ANMR_Rim");
	/*renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	renderData->_shaderInfo->_psName = TEXT("PS_ANMRAO");*/

	armAndFingerList.clear();
	missileAndCoreList.clear();
	eyeList.clear();
	coreCoverLightBrownList.clear();
	yellow_bodycolorList.clear();

	_initPosition = GetGameObject()->GetTransform()->GetPosition();
}

void Boss::Update()
{
	_currentStatus->OnStay();

	if (GameProcess::Get()->GetIsPausePanelUp()) return;
	if (CameraMovementManager::Get()->GetisMove()) return; // 카메라가 무브중이면 쓸 수 없다..!
	if (_checkStatus == Boss::DIE) return;

	if (_checkStatus == Boss::IDLE)
		_time += Muscle::CTime::GetGameDeltaTime();
	_colorTimer += Muscle::CTime::GetGameDeltaTime();

	float persent = ((float)_hp / (float)STAGE_ONE_BOSS_HP) * 100;

	if (_time > _attackSpeed)
		Attack();
	if (_colorTimer > 10)
	{
		_colorTimer = 0;
		SetColor(StateColor(Muscle::CTime::GetRand<int>(0, 2)));
	}
	if (persent >= 60)
		ChangePhase(Phase::Phase1);
	else if (persent >= 30)
		ChangePhase(Phase::Phase2);
	else if (persent > 0)
		ChangePhase(Phase::Phase3);
	else
		Die();

	/// <summary>
	/// 테스트용 !
	/// </summary>
	if (Muscle::KeyBoard::Get()->KeyDown('1'))
		SetState(Boss::ATTACK1);
	if (Muscle::KeyBoard::Get()->KeyDown('2'))
		SetState(Boss::ATTACK2);
	if (Muscle::KeyBoard::Get()->KeyDown('3'))
		SetState(Boss::ATTACK3);
	if (Muscle::KeyBoard::Get()->KeyDown('4'))
		SetState(Boss::ATTACK4);
	if (Muscle::KeyBoard::Get()->KeyDown('6'))
		SetState(Boss::ATTACK6);
	if (Muscle::KeyBoard::Get()->KeyDown('7'))
		SetState(Boss::ATTACK7);
	if (Muscle::KeyBoard::Get()->KeyDown('8'))
		SetState(Boss::ATTACK8);

	if (Muscle::KeyBoard::Get()->KeyDown('9'))
		SetState(Boss::ATTACK9);
	if (Muscle::KeyBoard::Get()->KeyDown('0'))
		SetState(Boss::ATTACK10);
}