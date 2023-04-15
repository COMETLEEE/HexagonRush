#include "pch.h"
#include "Second_Boss_Prefab.h"

#include "CannonBall_Prefab.h"

namespace SecondBoss_Attack
{
	void Jump(std::shared_ptr<Muscle::GameObject> gameObject) /// 2_Boss_Jump_01
	{
		auto target = gameObject->GetComponent<Second_Boss>()->GetTarget();
		ObjectMover::Get()->MoveTo(gameObject->GetTransform(), target->GetGameObject()->GetTransform()->GetPosition(),
			2.1f, 0, MoveInfo::Bezier,
			Vector3(0, 20, 0), Vector3(0, 20, 0));

		auto crossHair = std::make_shared<Crosshiar_Prefab>()->GetGameObject();
		crossHair->GetComponent<Crosshair>()->SetSize(2.0f);
		crossHair->GetTransform()->SetPosition(target->GetGameObject()->GetTransform()->GetPosition());

	}
	void Boss_Attack1(std::shared_ptr<Muscle::GameObject> gameObject) /// 2_Boss_Jump_01
	{
		auto ColliderObj = Muscle::CreateGameObject();
		ColliderObj->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition());
		ColliderObj->AddComponent<Muscle::SphereCollider>()->SetRadius(10.0f);
		ColliderObj->AddComponent<BossAttackBox>()->SetLifeTime(0.5f);

		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.5f, 0.3f, 0, true);
		GetSoundManager()->Play("Landing", IPlayMode::Effect);
	}

	void InitJump(std::shared_ptr<Muscle::GameObject> gameObject) // 복귀 점프
	{
		auto target = gameObject->GetComponent<Second_Boss>()->GetInitialPosition();
		ObjectMover::Get()->MoveTo(gameObject->GetTransform(), target,
			2.1f, 0, MoveInfo::Bezier,
			Vector3(0, 20, 0), Vector3(0, 20, 0));
	}

	void Boss_Attack2(std::shared_ptr<Muscle::GameObject> gameObject) /// 2_Boss_Stomp_02 // 복귀 점프
	{
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.5f, 0.3f, 0, true);
		GetSoundManager()->Play("Landing", IPlayMode::Effect);
	}
	void Boss_Attack3(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_thrusting_03
	{
		auto ColliderObj = Muscle::CreateGameObject();

		Vector3 drillPos = gameObject->GetTransform()->GetLook() * 18;
		ColliderObj->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition() + drillPos);
		ColliderObj->AddComponent<Muscle::SphereCollider>()->SetRadius(3.0f);
		ColliderObj->AddComponent<BossAttackBox>()->SetLifeTime(0.5f);

		auto particleObject = Muscle::CreateGameObject();
		particleObject->AddComponent<DrillParticle>()->SetTimer(1.0f);
		particleObject->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition() + drillPos);

		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 1.0f, 0.2f, 0, true);
	}
	void Boss_Attack4(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_Dril_launch_04
	{
		gameObject->GetComponent<Second_Boss>()->GetAnimater()->SetAnimSpeed(0, 100); // FSM에서 돌려줄 것이다..
		auto ColliderObj = Muscle::CreateGameObject();
		ColliderObj->AddComponent<Muscle::SphereCollider>()->SetRadius(7.0f);
		ColliderObj->GetComponent<Muscle::SphereCollider>()->SetLocalPostion(Vector3(0, 4, 0));
		ColliderObj->AddComponent<BossAttackBox>()->SetLifeTime(12.0f);
		gameObject->GetComponent<Second_Boss>()->GetAnimater()->SetAnimSpeed(0, 100); // FSM에서 돌려줄 것이다..
		ColliderObj->SetParent(gameObject->GetComponent<Second_Boss>()->GetDrill());

		GetSoundManager()->Play("Drill_Shoot", IPlayMode::Effect);
		GetSoundManager()->Play("DrillMove", IPlayMode::Effect);
	}
	void Boss_Attack5(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_thrusting_DrilUp_05
	{
		auto ColliderObj = Muscle::CreateGameObject();

		Vector3 drillPos = gameObject->GetTransform()->GetLook() * 18;
		ColliderObj->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition() + drillPos);
		ColliderObj->AddComponent<Muscle::SphereCollider>()->SetRadius(3.0f);
		ColliderObj->AddComponent<BossAttackBox>()->SetLifeTime(0.5f);

		auto particleObject = Muscle::CreateGameObject();
		particleObject->AddComponent<DrillParticle>()->SetTimer(5.0f);
		particleObject->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition() + drillPos);

		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 1.0f, 0.2f, 0, true);
	}

	void Boss_Attack5_Stop(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_thrusting_DrilUp_05
	{
		auto boss = gameObject->GetComponent<Second_Boss>();
		boss->GetAnimater()->SetAnimSpeed(0.0f, 5.0f);

		auto func = [](std::shared_ptr<Muscle::GameObject> gameObject)
		{
			std::make_shared<Drill_Missille_Prefab>();
		};

		for (int i = 0; i < 5; i++)
		{
			Muscle::CTime::Invoke(func, i, nullptr);
		}

	}

	void Boss_Attack6(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_oilBlack_Bob_06
	{
		// 오브젝트를 만들어서, 랜덤 위치로 발사시킨다. 
		for (int i = 0; i < 3; i++)
		{
			auto gameObj = std::make_shared<CannonBall_Prefab>(CannonBall_Prefab::CANNON_BALL_TYPE::OIL)->GetGameObject();

			gameObj->SetTag("Enemy");

			auto renderingData = gameObj->GetChildrens()[0]->GetComponent<Muscle::RendererBase>()->_renderingData;

			renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Albedo_Black"));
			renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Normal"));
			renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Metallic"));
			renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Roughness"));
			renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_AO"));

			renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
			renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");
		}
	}

	void Boss_Attack7(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_electronicYellow_Bob_07
	{
		for (int i = 0; i < 3; i++)
		{
			// 오브젝트를 만들어서, 랜덤 위치로 발사시킨다. 
			auto gameObj = std::make_shared<CannonBall_Prefab>(CannonBall_Prefab::CANNON_BALL_TYPE::ELECTRONIC)->GetGameObject();

			gameObj->SetTag("Enemy");

			auto renderingData = gameObj->GetChildrens()[0]->GetComponent<Muscle::RendererBase>()->_renderingData;

			renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Albedo_Yellow"));
			renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Normal"));
			renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Metallic"));
			renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Roughness"));
			renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_AO"));

			renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
			renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");
		}
	}

	void Boss_Attack8(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_LandmineRed_Bob_08
	{
		for (int i = 0; i < 3; i++)
		{
			// 오브젝트를 만들어서, 랜덤 위치로 발사시킨다. 
			auto gameObj = std::make_shared<CannonBall_Prefab>(CannonBall_Prefab::CANNON_BALL_TYPE::LANDMINE)->GetGameObject();

			gameObj->SetTag("Enemy");

			auto renderingData = gameObj->GetChildrens()[0]->GetComponent<Muscle::RendererBase>()->_renderingData;

			renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Albedo_Red"));
			renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Normal"));
			renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Metallic"));
			renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Roughness"));
			renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_AO"));

			renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
			renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");
		}
	}

	void Boss_Attack9(std::shared_ptr<Muscle::GameObject> gameObject)/// 2_Boss_PanicBlue_Bob_09
	{
		for (int i = 0; i < 3; i++)
		{
			// 오브젝트를 만들어서, 랜덤 위치로 발사시킨다. 
			auto gameObj = std::make_shared<CannonBall_Prefab>(CannonBall_Prefab::CANNON_BALL_TYPE::PANICBLUE)->GetGameObject();

			gameObj->SetTag("Enemy");

			auto renderingData = gameObj->GetChildrens()[0]->GetComponent<Muscle::RendererBase>()->_renderingData;

			renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Albedo_Blue"));
			renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Normal"));
			renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Metallic"));
			renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_Roughness"));
			renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_CannonBall_AO"));

			renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
			renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");
		}
	}
}

Second_Boss_Prefab::Second_Boss_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("Boss_02")))
{
	GetGameObject()->SetName("Boss");

	GetGameObject()->SetTag("Enemy");

	GetGameObject()->AddComponent<Second_Boss>();

	std::shared_ptr<Muscle::Animater> anim = GetGameObject()->GetComponent<Muscle::Animater>();

	{ // Attack1 AttackBoxS
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack1; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack1"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 170.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_Jump_01", animEventKey); // 이벤트 등록 완료.

		// Jump
		animEventKey._eventfunc = SecondBoss_Attack::Jump; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack_Jump"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 60.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_Jump_01", animEventKey); // 이벤트 등록 완료.
	}
	{ // Attack2
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack2; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack2"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 170.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_Stomp_02", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = SecondBoss_Attack::InitJump; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "InitJump"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 60.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_Stomp_02", animEventKey); // 이벤트 등록 완료.

	}
	{ // Attack3
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack3; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack3"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 100.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_thrusting_03", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Drill", IPlayMode::Effect); }; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._name = "Attack3_Sound"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 20.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_thrusting_03", animEventKey); // 이벤트 등록 완료.
	}
	{ // Attack4
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack4; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack4"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 100.0f * 100; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_Dril_launch_04", animEventKey); // 이벤트 등록 완료.
	}
	{ // Attack5
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack5; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack5"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 80.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_thrusting_DrilUp_05", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack5_Stop; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack5_Stop"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 150.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_thrusting_DrilUp_05", animEventKey); // 이벤트 등록 완료.

	}
	{ // Attack6
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack6; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack6"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_oilBlack_Bob_06", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("OillSHoot", IPlayMode::Effect); }; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._name = "Attack6_Sound"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_oilBlack_Bob_06", animEventKey); // 이벤트 등록 완료.

	}
	{ // Attack7
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack7; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack7"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_electronicYellow_Bob_07", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("OillSHoot", IPlayMode::Effect); }; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._name = "Attack7_Sound"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_electronicYellow_Bob_07", animEventKey); // 이벤트 등록 완료.
	}
	{ // Attack8
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack8; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack8"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_LandmineRed_Bob_08", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("OillSHoot", IPlayMode::Effect); }; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._name = "Attack8_Sound"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_LandmineRed_Bob_08", animEventKey); // 이벤트 등록 완료.
	}
	{ // Attack9
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = SecondBoss_Attack::Boss_Attack9; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._eventKey = false; // 초기화 안해도 괜찮음/기본값은 false;
		animEventKey._name = "Attack9"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		anim->AddAnimEvent("2_Boss_PanicBlue_Bob_09", animEventKey); // 이벤트 등록 완료.


		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("OillSHoot", IPlayMode::Effect); }; // 함수 포인터 안넘겨도 작동은 된다. 
		animEventKey._name = "Attack9_Sound"; // 이벤트 키의 이름을 넣는 곳.
		animEventKey._targetFream = 145.0f * 160; //신성현 익스포터는 TICKSPERFRAME 160 고정이다. 목표 프레임 * 160 한 값을 넣으면 됨!
		anim->AddAnimEvent("2_Boss_PanicBlue_Bob_09", animEventKey); // 이벤트 등록 완료.
	}
}
