#include "pch.h"
#include "Stage2Scene.h"

#include "PlatformWall.h"

#include "PausePanel_Prefab.h"
#include "HPPanel_Prefab.h"

float Stage2Scene::_playTime = 0.f;

Stage2Scene::Stage2Scene() : IScene("Stage2Scene")
{
}

Stage2Scene::~Stage2Scene()
{
}

float Stage2Scene::GetPlayTime()
{
	return _playTime;
}

// Stage Two Object !!
void Stage2Scene::CreateStageTwoObjects()
{
	// Floor
	std::shared_ptr<Muscle::GameObject> floor = Muscle::CreateFromPrefab(TEXT("Surface"));

	std::vector<std::shared_ptr<Muscle::GameObject>> childs = floor->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realFloor = childs[i];

		// Z-Fighting 제거
		realFloor->GetTransform()->SetPosition(realFloor->GetTransform()->GetPosition() + Vector3::Up * 0.01f);

		std::shared_ptr<Muscle::MeshRenderer> floorRen = realFloor->GetComponent<Muscle::MeshRenderer>();

		if (floor != nullptr)
		{
			// floorRen->_renderingData->_objectInfo->_usingShadow = false;
			floorRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Surface_Albedo"));
			floorRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Surface_Normal"));
			floorRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Surface_Metallic"));
			floorRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Surface_Roughness"));
			floorRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Surface_AO"));

			floorRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
			floorRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");
		}
	}

	// Building
	std::shared_ptr<Muscle::GameObject> building = Muscle::CreateFromPrefab(TEXT("Building"));

	childs = building->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBuilding = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> buildingRen = realBuilding->GetComponent<Muscle::MeshRenderer>();

		if (buildingRen != nullptr)
		{
			// buildingRen->_renderingData->_objectInfo->_usingShadow = false;
			buildingRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Building_Albedo"));
			buildingRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Building_Normal"));
			buildingRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Building_Metallic"));
			buildingRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Building_Roughness"));
			buildingRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Building_AO"));
			buildingRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Building_Emissive"));

			buildingRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			buildingRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// 2Building
	std::shared_ptr<Muscle::GameObject> building2 = Muscle::CreateFromPrefab(TEXT("2Building"));

	childs = building2->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBuilding2 = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> buildingRen2 = realBuilding2->GetComponent<Muscle::MeshRenderer>();

		if (buildingRen2 != nullptr)
		{
			// buildingRen2->_renderingData->_objectInfo->_usingShadow = false;
			buildingRen2->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("2Building_Albedo"));
			buildingRen2->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("2Building_Normal"));
			buildingRen2->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("2Building_Metallic"));
			buildingRen2->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("2Building_Roughness"));
			buildingRen2->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("2Building_AO"));
			buildingRen2->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("2Building_Emissive"));

			buildingRen2->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			buildingRen2->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// 3Building
	std::shared_ptr<Muscle::GameObject> building3 = Muscle::CreateFromPrefab(TEXT("3Building"));

	childs = building3->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBuilding3 = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> buildingRen3 = realBuilding3->GetComponent<Muscle::MeshRenderer>();

		if (buildingRen3 != nullptr)
		{
			// buildingRen3->_renderingData->_objectInfo->_usingShadow = false;
			buildingRen3->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("3Building_Albedo"));
			buildingRen3->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("3Building_Normal"));
			buildingRen3->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("3Building_Metallic"));
			buildingRen3->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("3Building_Roughness"));
			buildingRen3->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("3Building_AO"));
			buildingRen3->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("3Building_Emissive"));

			buildingRen3->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			buildingRen3->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// 4Building
	std::shared_ptr<Muscle::GameObject> building4 = Muscle::CreateFromPrefab(TEXT("4Building"));

	childs = building4->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBuilding4 = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> buildingRen4 = realBuilding4->GetComponent<Muscle::MeshRenderer>();

		if (buildingRen4 != nullptr)
		{
			// buildingRen4->_renderingData->_objectInfo->_usingShadow = false;
			buildingRen4->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("4Building_Albedo"));
			buildingRen4->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("4Building_Normal"));
			buildingRen4->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("4Building_Metallic"));
			buildingRen4->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("4Building_Roughness"));
			buildingRen4->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("4Building_AO"));
			buildingRen4->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("4Building_Emissive"));

			buildingRen4->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			buildingRen4->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// Box
	std::shared_ptr<Muscle::GameObject> box = Muscle::CreateFromPrefab(TEXT("Box"));

	childs = box->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBox = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> boxRen = realBox->GetComponent<Muscle::MeshRenderer>();

		if (boxRen != nullptr)
		{
			// boxRen->_renderingData->_objectInfo->_usingShadow = false;
			boxRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Box_Albedo"));
			boxRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Box_Normal"));
			boxRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Box_Metallic"));
			boxRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Box_Roughness"));
			boxRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Box_AO"));
			boxRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Box_Emissive"));

			boxRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			boxRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// 2Box
	std::shared_ptr<Muscle::GameObject> box2 = Muscle::CreateFromPrefab(TEXT("2Box"));

	childs = box2->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBox2 = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> boxRen2 = realBox2->GetComponent<Muscle::MeshRenderer>();

		if (boxRen2 != nullptr)
		{
			// boxRen2->_renderingData->_objectInfo->_usingShadow = false;
			boxRen2->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("2Box_Albedo"));
			boxRen2->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("2Box_Normal"));
			boxRen2->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("2Box_Metallic"));
			boxRen2->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("2Box_Roughness"));
			boxRen2->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("2Box_AO"));
			boxRen2->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("2Box_Emissive"));

			boxRen2->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			boxRen2->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// 3Box
	std::shared_ptr<Muscle::GameObject> box3 = Muscle::CreateFromPrefab(TEXT("3Box"));

	childs = box3->GetChildrens();

	for (int i = 0; i < childs.size(); i++)
	{
		std::shared_ptr<Muscle::GameObject> realBox3 = childs[i];

		std::shared_ptr<Muscle::MeshRenderer> boxRen3 = realBox3->GetComponent<Muscle::MeshRenderer>();

		if (boxRen3 != nullptr)
		{
			// boxRen3->_renderingData->_objectInfo->_usingShadow = false;
			boxRen3->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("3Box_Albedo"));
			boxRen3->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("3Box_Normal"));
			boxRen3->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("3Box_Metallic"));
			boxRen3->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("3Box_Roughness"));
			boxRen3->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("3Box_AO"));
			boxRen3->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("3Box_Emissive"));

			boxRen3->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
			boxRen3->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
		}
	}

	// Tile Ground
	std::shared_ptr<Muscle::GameObject> tileGround = Muscle::CreateFromPrefab(TEXT("Tile_ground"));

	std::shared_ptr<Muscle::GameObject> realtileGround = tileGround->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realtileGroundRen = realtileGround->GetComponent<Muscle::MeshRenderer>();

	// realtileGroundRen->_renderingData->_objectInfo->_usingShadow = false;
	realtileGroundRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_ground_Albedo"));
	realtileGroundRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_ground_Normal"));
	realtileGroundRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_ground_Metallic"));
	realtileGroundRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_ground_Roughness"));
	realtileGroundRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Tile_ground_AO"));

	realtileGroundRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	realtileGroundRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");


	// ground
	std::shared_ptr<Muscle::GameObject> ground = Muscle::CreateFromPrefab(TEXT("Ground"));

	std::shared_ptr<Muscle::GameObject> realground = ground->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realgroundRen = realground->GetComponent<Muscle::MeshRenderer>();

	// realgroundRen->_renderingData->_objectInfo->_usingShadow = false;
	realgroundRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_Albedo"));
	realgroundRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_Normal"));
	realgroundRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_Metallic"));
	realgroundRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_Roughness"));
	realgroundRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_AO"));
	realgroundRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_Emissive"));

	realgroundRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realgroundRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");





	// ground wall
	std::shared_ptr<Muscle::GameObject> groundWall = Muscle::CreateFromPrefab(TEXT("Ground_wall"));

	std::shared_ptr<Muscle::GameObject> realgroundWall = groundWall->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realgroundWallRen = realgroundWall->GetComponent<Muscle::MeshRenderer>();

	// realgroundWallRen->_renderingData->_objectInfo->_usingShadow = false;
	realgroundWallRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_wall_Albedo"));
	realgroundWallRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_wall_Normal"));
	realgroundWallRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_wall_Metallic"));
	realgroundWallRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_wall_Roughness"));
	realgroundWallRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Ground_wall_AO"));

	realgroundWallRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	realgroundWallRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");

}

void Stage2Scene::Start()
{
	_playTime = 0.f;

	DebugObjectManager::Get();
	CameraMovementManager::Get();
	ObjectMover::Get();
	TileManager::Get();

#pragma region Walls
	std::shared_ptr<Muscle::GameObject> wall1 = Muscle::CreateGameObject();

	wall1->GetComponent<Muscle::Transform>()->SetPosition(Vector3(32.7f, 15.f, 3.47f));
	wall1->GetComponent<Muscle::Transform>()->RotateY(3.14f / 2);
	wall1->AddComponent<Muscle::OBBCollider>();
	wall1->GetComponent<Muscle::OBBCollider>()->SetSize(Vector3(50.f, 30.f, 10.f));
	wall1->AddComponent<PlatformWall>();

	std::shared_ptr<Muscle::GameObject> wall2 = Muscle::CreateGameObject();


	wall2->GetComponent<Muscle::Transform>()->SetPosition(Vector3(-29.7f, 15.f, 3.47f));
	wall2->GetComponent<Muscle::Transform>()->RotateY(3.14f / 2);
	wall2->AddComponent<Muscle::OBBCollider>();
	wall2->GetComponent<Muscle::OBBCollider>()->SetSize(Vector3(50.f, 30.f, 10.f));
	wall2->AddComponent<PlatformWall>();


	std::shared_ptr<Muscle::GameObject> wall3 = Muscle::CreateGameObject();


	wall3->GetComponent<Muscle::Transform>()->SetPosition(Vector3(19.423f, 15.f, -26.769f));
	wall3->GetComponent<Muscle::Transform>()->RotateY((3.14f / 2) + (3.14f / 3));
	wall3->AddComponent<Muscle::OBBCollider>();
	wall3->GetComponent<Muscle::OBBCollider>()->SetSize(Vector3(50.f, 30.f, 10.f));
	wall3->AddComponent<PlatformWall>();



	std::shared_ptr<Muscle::GameObject> wall4 = Muscle::CreateGameObject();

	wall4->GetComponent<Muscle::Transform>()->SetPosition(Vector3(19.423f, 15.f, 26.769f));
	wall4->GetComponent<Muscle::Transform>()->RotateY((3.14f / 2) + 2 * (3.14f / 3));
	wall4->AddComponent<Muscle::OBBCollider>();
	wall4->GetComponent<Muscle::OBBCollider>()->SetSize(Vector3(50.f, 30.f, 10.f));
	wall4->AddComponent<PlatformWall>();


	std::shared_ptr<Muscle::GameObject> wall5 = Muscle::CreateGameObject();

	wall5->GetComponent<Muscle::Transform>()->SetPosition(Vector3(-19.423f, 15.f, 26.769f));
	wall5->GetComponent<Muscle::Transform>()->RotateY((3.14f / 2) + (3.14f / 3));
	wall5->AddComponent<Muscle::OBBCollider>();
	wall5->GetComponent<Muscle::OBBCollider>()->SetSize(Vector3(50.f, 30.f, 10.f));
	wall5->AddComponent<PlatformWall>();


	std::shared_ptr<Muscle::GameObject> wall6 = Muscle::CreateGameObject();

	wall6->GetComponent<Muscle::Transform>()->SetPosition(Vector3(-19.423f, 15.f, -26.769f));
	wall6->GetComponent<Muscle::Transform>()->RotateY((3.14f / 2) - (3.14f) / 3);
	wall6->AddComponent<Muscle::OBBCollider>();
	wall6->GetComponent<Muscle::OBBCollider>()->SetSize(Vector3(50.f, 30.f, 10.f));
	wall6->AddComponent<PlatformWall>();
#pragma endregion

	std::shared_ptr<Muscle::IGameEngine> _Temp = Muscle::IGameEngine::Get();
	std::shared_ptr<Muscle::GameObject> testCam = Muscle::CreateGameObject();
	_Temp->SetMainCamera(testCam->AddComponent<Muscle::Camera>());			// 각 씬별로 Start에 메인 카메라의 세팅이 필요하구나 !
	testCam->GetComponent<Muscle::Transform>()->SetPosition(Vector3(0.f, 3.f, -10.f));
	std::shared_ptr<Muscle::GameObject> skyBox = Muscle::CreateGameObject();
	skyBox->AddComponent<Muscle::SkyBox>()->Initialize(Muscle::ResourceManager::Get()->GetTextureID(TEXT("Environment2")), testCam->GetComponent<Muscle::Camera>());
	skyBox->SetParent(testCam);
	skyBox->GetComponent<Muscle::SkyBox>()->_renderingData->_shaderInfo->_psName = TEXT("PS_SkyBox");

	std::shared_ptr<Muscle::GameObject> light = Muscle::CreateGameObject();
	Vector3 direction = Vector3(-0.7f, -0.5f, 0.f);
	direction.Normalize();
	light->AddComponent<Muscle::DirectionalLight>()->SetDirectional(direction);


	////// ------------------------------------------ Player & Boss Setting ------------------------------------------------------------------------
	//auto player = std::make_shared<Player_Prefab>()->GetGameObject();
	_player = std::make_shared<Player_Prefab>()->GetGameObject();

	std::shared_ptr<Muscle::GameObject> boss = std::make_shared<Second_Boss_Prefab>()->GetGameObject();
	std::vector<std::shared_ptr<Muscle::GameObject>> childs(0);
	boss->GetAllChildrens(childs);
	// 보스 그림자 빼준다 !
	for (auto& child : childs)
	{
		std::shared_ptr<Muscle::RendererBase> ren = child->GetComponent<Muscle::RendererBase>();
		if (ren != nullptr)
			ren->_renderingData->_objectInfo->_usingShadow = false;
	}

	_player->GetComponent<Player>()->SetTarget(boss->GetComponent<Second_Boss>());
	boss->GetComponent<Second_Boss>()->SetTarget(_player->GetComponent<Player>());
	boss->GetTransform()->SetPosition(-27, 4.0f, 0);
	boss->GetTransform()->RotateY(3.14f / 2);
	boss->GetTransform()->SetScale(0.2f, 0.2f, 0.2f);
	Vector3 bossPos = boss->GetTransform()->GetPosition();
	bossPos.y = _player->GetTransform()->GetPosition().y;
	_player->GetTransform()->LookAt(bossPos);
	_player->GetTransform()->RotateY(3.14f);

	////// ------------------------------------------ Player & Boss Setting ------------------------------------------------------------------------
	std::make_shared<TestTile_Prefab>();

	CreateStageTwoObjects();

	// UI 만들기
	_pausePanelUI = std::make_shared<PausePanel_Prefab>("Stage2Scene");
	_hpPanelUI = std::make_shared<HPPanel_Prefab>();
	std::make_shared<ItemUIPanel_Prefab>();
	// UI 만들기

	GetSoundManager()->Play("Stage2", IPlayMode::BGM);

	CameraMovementManager::Get()->Stage2Start();
}

void Stage2Scene::RapidUpdate()
{
}

void Stage2Scene::Update()
{
	_playTime += Muscle::CTime::GetGameDeltaTime();

	// TEST : 동적으로 Post Processing 키고 끄기.
	if (Muscle::KeyBoard::Get()->KeyDown('V'))
	{
		static bool test = false;

		Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetDebugPanel(test);

		test = !test;
	}

	// TEST : 동적으로 Post Processing 키고 끄기.
	if (Muscle::KeyBoard::Get()->KeyDown('B'))
	{
		static bool test2 = false;

		Muscle::MuscleEngine::GetInstance()->GetGraphicsManager()->SetVignetting(test2);
		test2 = !test2;


	}
}