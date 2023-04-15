#include "pch.h"
#include "Player_Prefab.h"
#include "Bullet_Prefab.h"
#include "BulletController.h"

#include "SceneFader.h"
#include "Inventory.h"
#include "AuraController.h"
#include "BuffController.h"

#include "Rush_Bullet_Prefab.h"

void Player_NormalAttack(std::shared_ptr<Muscle::GameObject> gameObject)
{
	std::shared_ptr<Controller> control = gameObject->GetComponent<Controller>();

	std::shared_ptr<Player> player = gameObject->GetComponent<Player>();

	if (player == nullptr)
		return;

	// 어택 함수 실행 ! (몬스터에게 딜을 가한다.) => 이거 Bullet 맞았을 때 표현하기 위해서 옮김
	// player->Attack(player->GetTarget());

	ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.2f);

	std::shared_ptr<Bullet_Prefab> testBullet = std::make_shared<Bullet_Prefab>();

	testBullet->GetGameObject()->GetComponent<Muscle::Transform>()->SetPosition(gameObject->GetTransform()->GetWorldPosition() + Vector3(0, 3, 0));

	// 대충 스테이지 보스까지 갔다가 .. 보스에서 살짝 위에 있는 코어를 맞출 수 있도록 총알의 도착 위치를 하드 코딩으로 지정 !
	Vector3 dest = gameObject->GetTransform()->GetWorldPosition() + (player->GetTarget()->GetGameObject()->GetTransform()->GetWorldPosition() - gameObject->GetTransform()->GetWorldPosition())
		+ player->GetTarget()->GetGameObject()->GetTransform()->GetUp() * 8;

	// 지정하기 전, 색상과 몇 개로 터트리는지 ! 확인해야함. => 타일 매니저에서 셋 할 때 플레이어에게 알려주자.
	const TileManager::ATTACK_PLATFORM_INFO& attackInfo = TileManager::Get()->GetCurrentAttackInfo();

	// 총알 발사할 때는 이렇게 지정을 해주어야 한다.
	// 
	// 플레이에게 지가 현재 쏜 총알을 알려주자.
	uint32 info = 0;

	if (attackInfo._color == StateColor::Red)
		info += 0;
	else if (attackInfo._color == StateColor::Green)
		info += 1;
	else if (attackInfo._color == StateColor::Blue)
		info += 2;

	// 단계에 비례해서 정보를 넣어준다.
	info += (attackInfo._count - 3) * 3;

	// 공격 정보를 보내줍니다 !
	player->PostShootingBulletInfo(static_cast<Player::SHOOTING_BULLET_INFO>(info));

	// 주체 (누가 쐈나요)
	testBullet->GetGameObject()->GetComponent<BulletController>()->SettingAgent(player);

	// 색상
	testBullet->GetGameObject()->GetComponent<BulletController>()->SetBulletColor(static_cast<BulletController::BULLET_COLOR>(attackInfo._color));

	// 레벨 (갯수가 3개일 때가 1레벨 ..!)
	testBullet->GetGameObject()->GetComponent<BulletController>()->SetBulletLevel(static_cast<BulletController::BULLET_LEVEL>(attackInfo._count - 2));

	// 목적지
	testBullet->GetGameObject()->GetComponent<BulletController>()->SetDestination(dest);
}

// 플레이어 모델 ! => 본들은 메쉬 렌더러, 스킨드 메쉬 렌더러가 박혀 있다.
Player_Prefab::Player_Prefab() :IPrefab(Muscle::CreateFromPrefab(TEXT("Player")))
{
	// Texture
	std::vector<std::shared_ptr<Muscle::GameObject>> allchilds;

	GetGameObject()->GetAllChildrens(allchilds);

	for (auto& object : allchilds)
	{
		std::shared_ptr<Muscle::SkinnedMeshRenderer> renderer = object->GetComponent<Muscle::SkinnedMeshRenderer>();

		if (renderer != nullptr)
		{
			std::shared_ptr<RenderingData_3D> renderingData = renderer->_renderingData;

			renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Player_Albedo"));
			renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Player_Normal"));
			renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Player_Metallic"));
			renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Player_Roughness"));
			renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Player_AO"));
			renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Player_Emissive"));

			//renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE_Skinning_Rim");
			//renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE_Skinning_Rim");

			renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE_Skinning_Rim");
			renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE_Skinning_Rim");
		}
	}

	// 플레이어를 캐싱하기 위해서 이름을 지어준다. (외부에서 컴포넌트 찾기용)
	GetGameObject()->SetName("Player");

	// Work Man으로 들어올 때 사이즈가 상당히 크고, 좌표계가 다르다.
	// GetGameObject()->GetTransform()->SetScale(0.05f, 0.05f, 0.05f);

	GetGameObject()->GetTransform()->SetPosition(Vector3(0.f, 4.2f, 0.f));

	// 콜라이더 추가 (보스와의 상호 작용 전용)
	std::shared_ptr<Muscle::CapsuleCollider> col = GetGameObject()->AddComponent<Muscle::CapsuleCollider>();

	col->SetCapsule(1.5f, 4.0f);

	col->SetLocalPostion(Vector3(0.f, 3.0f, 0.f));

	// 콜라이더 추가 22 (벽과의 상호 작용 전용)

	std::shared_ptr<Muscle::GameObject> wallColObj = Muscle::CreateGameObject();

	wallColObj->SetName("Player_Wall");

	wallColObj->AddComponent<Muscle::CapsuleCollider>()->SetCapsule(1.5f, 4.0f);

	col->SetLocalPostion(Vector3(0.f, 3.0f, 0.f));

	GetGameObject()->SetChild(wallColObj);

	// 플레이어 컴포넌트
	std::shared_ptr<Player> player = GetGameObject()->AddComponent<Player>();

	// 컨트롤러 컴포넌트 (움직임, 회피 등)
	GetGameObject()->AddComponent<Controller>();

	// Aura Particle Renderer와 Controller
	GetGameObject()->AddComponent<Muscle::ParticleRenderer>();
	GetGameObject()->AddComponent<AuraController>();

	// 인벤토리 컴포넌트 (스테이지 시작과 주어지는 아이템 3개)
	GetGameObject()->AddComponent<Inventory>();

	// Buff Controller (아이템 사용 등에 대한 Particle을 컨트롤해줍니다.)
	std::shared_ptr<Muscle::GameObject> buffObj = Muscle::CreateGameObject();

	buffObj->SetName("BuffController");

	buffObj->AddComponent<Muscle::ParticleRenderer>();

	buffObj->AddComponent<BuffController>();

	// 이건 자식으로 들어간다.
	GetGameObject()->SetChild(buffObj);

	// 랜덤 랜덤
	static std::random_device rd;

	static std::mt19937 gen(rd());

	static std::uniform_int_distribution<int> dist(0, 3);

	// 러쉬 불렛 프리팹 풀을 채워준다.
	for (int i = 0; i < 150; i++)
	{
		std::shared_ptr<Muscle::GameObject> rushBullet = std::make_shared<Rush_Bullet_Prefab>()->GetGameObject();

		rushBullet->SetIsEnable(false);

		std::shared_ptr<Rush_BulletController> rushBulletController = rushBullet->GetComponent<Rush_BulletController>();

		// 주변 도는 원형 랜덤 생성
		int color = dist(gen);

		if (color == 0)
			rushBulletController->SetColor(Vector4(1.f, 0.f, 0.f, 1.f));
		else if (color == 1)
			rushBulletController->SetColor(Vector4(0.f, 1.f, 0.f, 1.f));
		else
			rushBulletController->SetColor(Vector4(0.f, 0.1f, 0.9f, 1.f));

		player->_rushBulletPrefabPool.push_back(rushBullet);
	}

	{
		// 노가다가 될 예정
		// 애니메이션 키 이벤트 등록 !
		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = Player_NormalAttack;
		animEventKey._eventKey = false;
		animEventKey._name = "Attack";
		animEventKey._targetFream = 32.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("Ch_Attack", animEventKey); // 이벤트 등록 완료.
	}
	{ // WalkSound

		Muscle::AnimEventKey animEventKey;
		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) { GetSoundManager()->Play("Move_R", IPlayMode::Effect); };
		animEventKey._name = "WalkRight";
		animEventKey._targetFream = 15.0f * 160;
		animEventKey._targetGameObject = GetGameObject(); // 함수 포인터의 매개변수로 사용될 변수. 보통 자기 자신을 넣을듯?
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("Ch_Run", animEventKey); // 이벤트 등록 완료.

		animEventKey._eventfunc = [](std::shared_ptr<Muscle::GameObject> gameObject) { GetSoundManager()->Play("Move_L", IPlayMode::Effect); };
		animEventKey._name = "WalkLeft";
		animEventKey._targetFream = 45.0f * 160;
		GetGameObject()->GetComponent<Muscle::Animater>()->AddAnimEvent("Ch_Run", animEventKey); // 이벤트 등록 완료.
	}
}

Player_Prefab::~Player_Prefab()
{
	_gameObject.reset();
}