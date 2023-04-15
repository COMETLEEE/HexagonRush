#include "pch.h"
#include "Tutorial_Boss_Prefab.h"

#include "SceneFader.h"

namespace Tu_Boss_Attack
{

	void Boss_Attack1(std::shared_ptr<Muscle::GameObject> gameObject) // 기본공격
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> attackBox = Muscle::CreateGameObject();
		attackBox->AddComponent<Muscle::OBBCollider>()->SetSize(Vector3(8.0f, 8.0f, 16.0f));
		attackBox->AddComponent<BossAttackBox>()->SetLifeTime(0.5f);
		attackBox->SetTag("Enemy");
		attackBox->GetTransform()->SetPosition(0.0f, 0.0f, -8.0f);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.5f);
		attackBox->SetParent(boss->GetHand1());
	}

	void Boss_Attack2(std::shared_ptr<Muscle::GameObject> gameObject) //attack2_down_slid
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> attackBox = Muscle::CreateGameObject();
		attackBox->AddComponent<Muscle::OBBCollider>()->SetSize(Vector3(8.0f, 8.0f, 16.0f));
		attackBox->AddComponent<BossAttackBox>()->SetLifeTime(1.5f);;
		attackBox->SetTag("Enemy");
		attackBox->GetTransform()->SetPosition(0.0f, 0.0f, -8.0f);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.5f);
		attackBox->SetParent(boss->GetHand1());
	}
	void Boss_Attack3(std::shared_ptr<Muscle::GameObject> gameObject)//attack3_shake
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> attackBox = Muscle::CreateGameObject();
		attackBox->SetTag("Enemy");
		attackBox->AddComponent<Muscle::OBBCollider>()->SetSize(Vector3(8.0f, 8.0f, 16.0f));
		//attackBox->AddComponent<Muscle::SphereCollider>()->SetRadius(10);
		attackBox->AddComponent<BossAttackBox>()->SetLifeTime(4.0f);

		attackBox->GetTransform()->SetPosition(0.0f, 0.0f, -8.0f);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f, 0.2f);
		attackBox->SetParent(boss->GetHand1());

		std::shared_ptr<Muscle::GameObject> attackBox2 = Muscle::CreateGameObject();
		attackBox2->AddComponent<Muscle::OBBCollider>()->SetSize(Vector3(8.0f, 8.0f, 16.0f));
		//attackBox->AddComponent<Muscle::SphereCollider>()->SetRadius(10);
		attackBox2->AddComponent<BossAttackBox>()->SetLifeTime(4.0f);
		attackBox2->SetTag("Enemy");
		attackBox2->GetTransform()->SetPosition(0.0f, 0.0f, -8.0f);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f, 0.2f);
		attackBox2->SetParent(boss->GetHand2());

		GetSoundManager()->Play("Impact", IPlayMode::Effect);


		auto func = [](std::shared_ptr<Muscle::GameObject>)
		{
			TileManager::Get()->RandDropTile();
		};

		for (int i = 0; i < 20; i++)
		{
			float time = 2.5f * ((float)i / (float)20);
			Muscle::CTime::Invoke(func, time, nullptr);
		}
	}
	void Boss_Attack4(std::shared_ptr<Muscle::GameObject> gameObject)// attack4_missile
	{
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
	}

	void Boss_Attack7(std::shared_ptr<Muscle::GameObject> gameObject) // attack7_fire_release
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> fire2 = Muscle::CreateGameObject();
		fire2->SetTag("Enemy");
		fire2->AddComponent<FireController>()->SetEndTime(2.0f);
		auto collider2 = fire2->AddComponent<Muscle::CapsuleCollider>();
		fire2->AddComponent<BossAttackBox>();
		collider2->SetCapsule(2, 20);
		collider2->SetLocalPostion(Vector3(0, 10, 0));
		collider2->SetIsEnable(true);
		fire2->GetTransform()->Pitch(3.14f / 2);
		fire2->SetParent(boss->GetHand2());
	}

	void Boss_Attack8(std::shared_ptr<Muscle::GameObject> gameObject) // attack8_fire_radiate
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> fire1 = Muscle::CreateGameObject();
		fire1->SetTag("Enemy");
		fire1->AddComponent<FireController>();
		auto collider = fire1->AddComponent<Muscle::CapsuleCollider>();
		collider->SetCapsule(2, 20);
		collider->SetLocalPostion(Vector3(0, 10, 0));
		collider->SetIsEnable(true);
		fire1->GetTransform()->Pitch(3.14f / 2);
		fire1->SetParent(boss->GetHand1());
		fire1->AddComponent<BossAttackBox>();

		std::shared_ptr<Muscle::GameObject> fire2 = Muscle::CreateGameObject();
		fire2->SetTag("Enemy");
		fire2->AddComponent<FireController>();
		auto collider2 = fire2->AddComponent<Muscle::CapsuleCollider>();
		collider2->SetCapsule(2, 20);
		collider2->SetLocalPostion(Vector3(0, 10, 0));
		collider2->SetIsEnable(true);
		fire2->GetTransform()->Pitch(3.14f / 2);
		fire2->SetParent(boss->GetHand2());
		fire2->AddComponent<BossAttackBox>();
	}

	void Boss_Attack6(std::shared_ptr<Muscle::GameObject> gameObject) // attack6_laser
	{
		std::shared_ptr<Muscle::GameObject> attackBox = Muscle::CreateGameObject();
		attackBox->SetTag("Enemy");

		auto collider = attackBox->AddComponent<Muscle::SphereCollider>();

		collider->SetIsEnable(false);
		attackBox->AddComponent<BossAttackBox>()->SetLifeTime(10.0f);
		attackBox->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition() + (gameObject->GetTransform()->GetLook() * 20 + (gameObject->GetTransform()->GetUp() * 10)));
		Vector3 targetPos;
		Vector3 length;
		if (gameObject->GetComponent<Boss>()->GetTarget())
		{
			targetPos = gameObject->GetComponent<Boss>()->GetTarget()->GetGameObject()->GetTransform()->GetPosition() + Vector3(0, 1, 0);
			attackBox->GetTransform()->UpAt(targetPos);
		}
		attackBox->AddComponent<LaserController>()->SetTarget(targetPos);
		std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(Vector3(targetPos));

		length = attackBox->GetTransform()->GetPosition() - targetPos;
		float floatlength = length.Length();
		collider->SetRadius(2.5f);
		collider->SetLocalPostion(Vector3(0, floatlength, 0));

	}

	void Boss_Attack9(std::shared_ptr<Muscle::GameObject> gameObject) // 기본공격
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> attackBox = Muscle::CreateGameObject();
		attackBox->AddComponent<Muscle::OBBCollider>()->SetSize(Vector3(8.0f, 8.0f, 16.0f));
		attackBox->AddComponent<BossAttackBox>();
		attackBox->SetTag("Enemy");
		attackBox->GetTransform()->SetPosition(0.0f, 0.0f, -8.0f);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.5f);
		attackBox->SetParent(boss->GetHand2());
	}

	void Boss_Attack10(std::shared_ptr<Muscle::GameObject> gameObject) //attack2_down_slid
	{
		auto boss = gameObject->GetComponent<Boss>();
		std::shared_ptr<Muscle::GameObject> attackBox = Muscle::CreateGameObject();
		attackBox->AddComponent<Muscle::OBBCollider>()->SetSize(Vector3(8.0f, 8.0f, 16.0f));
		attackBox->AddComponent<BossAttackBox>();
		attackBox->SetTag("Enemy");
		attackBox->GetTransform()->SetPosition(0.0f, 0.0f, -8.0f);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.5f);
		attackBox->SetParent(boss->GetHand2());
	}
}


Tutorial_Boss_Prefab::Tutorial_Boss_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("Boss_01")))
{
	//std::shared_ptr<Muscle::CapsuleCollider> col = GetGameObject()->AddComponent<Muscle::CapsuleCollider>();
	//col->SetCapsule(10.0f, 10.0f);
	GetGameObject()->SetName("Boss");

	GetGameObject()->SetTag("Enemy");

	GetGameObject()->AddComponent<Boss>();

	GetGameObject()->GetTransform()->RotateY(3.14f);

	{
		/// <summary>
		/// Attack1
		/// </summary>
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack1;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack1";
		animEventKey._targetFream = 100.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack1_down", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Swing", IPlayMode::Effect); };
		animEventKey._eventKey = false;
		animEventKey._name = "Attack1_Sound";
		animEventKey._targetFream = 75.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack1_down", animEventKey); // 이벤트 등록 완료.
	}
	{
		/// <summary>
		/// Attack2
		/// </summary>
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack2;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack2";
		animEventKey._targetFream = 100.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack2_down_slid", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Swing", IPlayMode::Effect); };
		animEventKey._name = "Attack2_Sound";
		animEventKey._targetFream = 75.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack2_down_slid", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Silde", IPlayMode::Effect); };
		animEventKey._name = "Attack2_Sound";
		animEventKey._targetFream = 135.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack2_down_slid", animEventKey); // 이벤트 등록 완료.
	}
	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack3;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack3";
		animEventKey._targetFream = 95.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {
			ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.3f, 0.2f);
			GetSoundManager()->Play("Impact", IPlayMode::Effect); };
		animEventKey._eventKey = false;
		animEventKey._name = "Attack3_Sound";
		animEventKey._targetFream = 125.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.
		animEventKey._targetFream = 155.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.
		animEventKey._targetFream = 185.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.
		animEventKey._targetFream = 215.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Big_Impact", IPlayMode::Effect);
		ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.6f, 0.4f);
		};
		animEventKey._targetFream = 255.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("TileFall", IPlayMode::Effect); };
		animEventKey._targetFream = 190.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack3_shake", animEventKey); // 이벤트 등록 완료.


	}
	for (int i = 0; i < 4; i++)
	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack4;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack4";
		animEventKey._targetFream = (1 + i) * 10.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack4_missile", animEventKey); // 이벤트 등록 완료.
	}
	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack6;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack6";
		animEventKey._targetFream = 0 * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack6_laser", animEventKey); // 이벤트 등록 완료.

		//animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("LaserCharge", IPlayMode::Effect); };
		//animEventKey._eventKey = false;
		//animEventKey._name = "Attak6_Sound";
		//animEventKey._targetFream = 30.0f * 160;
		//GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack6_laser", animEventKey); // 이벤트 등록 완료.

	}
	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack7;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack7";
		animEventKey._targetFream = 20.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack7_fire_release", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Fire", IPlayMode::Effect); };
		animEventKey._name = "Attack7_Sound";
		animEventKey._targetFream = 20.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack7_fire_release", animEventKey); // 이벤트 등록 완료.
	}
	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack8;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack8";
		animEventKey._targetFream = 20.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack8_fire_radiate", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Fireradiate", IPlayMode::Effect); };
		animEventKey._name = "Attack8_Sound";
		animEventKey._targetFream = 20.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack8_fire_radiate", animEventKey); // 이벤트 등록 완료.
	}

	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack9;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack9";
		animEventKey._targetFream = 100.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack9_down_right", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Swing", IPlayMode::Effect); };
		animEventKey._eventKey = false;
		animEventKey._name = "Attack9_Sound";
		animEventKey._targetFream = 75.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack9_down_right", animEventKey); // 이벤트 등록 완료.
	}
	{
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Tu_Boss_Attack::Boss_Attack10;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack10";
		animEventKey._targetFream = 100.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack10_down_slid_right", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Swing", IPlayMode::Effect); };
		animEventKey._name = "Attack10_Sound";
		animEventKey._targetFream = 75.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack10_down_slid_right", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) {GetSoundManager()->Play("Silde", IPlayMode::Effect); };
		animEventKey._name = "Attack10_Sound";
		animEventKey._targetFream = 135.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("attack10_down_slid_right", animEventKey); // 이벤트 등록 완료.
	}

}