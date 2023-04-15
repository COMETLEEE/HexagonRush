#include "pch.h"
#include "Stage1Scene.h"
#include "MeshRenderer.h"

#include "PausePanel_Prefab.h"
#include "HPPanel_Prefab.h"

#include "Bullet_Prefab.h"
#include "BulletController.h"

#include "SceneFader.h"

#include "PlatformWall.h"

float Stage1Scene::_playTime = 0.f;

Stage1Scene::Stage1Scene() : IScene("Stage1Scene")
{

}

Stage1Scene::~Stage1Scene()
{
	_pausePanelUI.reset();

	_hpPanelUI.reset();
}

float Stage1Scene::GetPlayTime()
{
	return _playTime;
}

// ------------------------------- Stage 1 -------------------------------
void Stage1Scene::CreateStageOneObjects()
{
	std::shared_ptr<Muscle::GameObject> doorbox = Muscle::CreateFromPrefab(TEXT("Door_box"));

	std::shared_ptr<Muscle::GameObject> realDoorbox = doorbox->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> doorboxRen = realDoorbox->GetComponent<Muscle::MeshRenderer>();

	doorboxRen->_renderingData->_objectInfo->_usingShadow = false;
	doorboxRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_box_Albedo"));
	doorboxRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_box_Normal"));
	doorboxRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_box_Metallic"));
	doorboxRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_box_Roughness"));
	doorboxRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_box_AO"));
	doorboxRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_box_Emissive"));

	doorboxRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	doorboxRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");




	std::shared_ptr<Muscle::GameObject> door = Muscle::CreateFromPrefab(TEXT("Door"));

	std::shared_ptr<Muscle::GameObject> realDoor = door->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> doorRen = realDoor->GetComponent<Muscle::MeshRenderer>();

	doorRen->_renderingData->_objectInfo->_usingShadow = false;
	doorRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Albedo"));
	doorRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Normal"));
	doorRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Metallic"));
	doorRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Roughness"));
	doorRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_AO"));

	doorRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	doorRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");







	std::shared_ptr<Muscle::GameObject> door2 = Muscle::CreateFromPrefab(TEXT("Door2"));

	std::shared_ptr<Muscle::GameObject> realDoor2 = door2->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> door2Ren = realDoor2->GetComponent<Muscle::MeshRenderer>();

	door2Ren->_renderingData->_objectInfo->_usingShadow = false;
	door2Ren->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Albedo"));
	door2Ren->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Normal"));
	door2Ren->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Metallic"));
	door2Ren->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_Roughness"));
	door2Ren->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_AO"));

	door2Ren->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	door2Ren->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");







	std::shared_ptr<Muscle::GameObject> floor = Muscle::CreateFromPrefab(TEXT("Floor"));

	std::shared_ptr<Muscle::GameObject> realfloor = floor->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realfloorRen = realfloor->GetComponent<Muscle::MeshRenderer>();

	realfloorRen->_renderingData->_objectInfo->_usingShadow = false;
	realfloorRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Floor_Albedo"));
	realfloorRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Floor_Normal"));
	realfloorRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Floor_Metallic"));
	realfloorRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Floor_Roughness"));
	realfloorRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Floor_AO"));
	realfloorRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Floor_Emissive"));

	realfloorRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realfloorRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");





	std::shared_ptr<Muscle::GameObject> frontWall = Muscle::CreateFromPrefab(TEXT("Front_wall"));

	std::shared_ptr<Muscle::GameObject> realfrontWall = frontWall->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realfrontWallRen = realfrontWall->GetComponent<Muscle::MeshRenderer>();

	realfrontWallRen->_renderingData->_objectInfo->_usingShadow = false;
	realfrontWallRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_wall_Albedo"));
	realfrontWallRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_wall_Normal"));
	realfrontWallRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_wall_Metallic"));
	realfrontWallRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_wall_Roughness"));
	realfrontWallRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_wall_AO"));
	realfrontWallRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_wall_Emissive"));

	realfrontWallRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realfrontWallRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");





	std::shared_ptr<Muscle::GameObject> doorScreen = Muscle::CreateFromPrefab(TEXT("Door_screen"));

	std::shared_ptr<Muscle::GameObject> realdoorScreen = doorScreen->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realdoorScreenRen = realdoorScreen->GetComponent<Muscle::MeshRenderer>();

	realdoorScreenRen->_renderingData->_objectInfo->_usingShadow = false;
	realdoorScreenRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_screen_Albedo"));
	realdoorScreenRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_screen_Normal"));
	realdoorScreenRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_screen_Metallic"));
	realdoorScreenRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_screen_Roughness"));
	realdoorScreenRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Door_screen_AO"));

	realdoorScreenRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	realdoorScreenRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");




	std::shared_ptr<Muscle::GameObject> frontDoorWall = Muscle::CreateFromPrefab(TEXT("Front_door_wall"));

	std::shared_ptr<Muscle::GameObject> realfrontDoorWall = frontDoorWall->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realfrontDoorWallRen = realfrontDoorWall->GetComponent<Muscle::MeshRenderer>();

	realfrontDoorWallRen->_renderingData->_objectInfo->_usingShadow = false;
	realfrontDoorWallRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_door_wall_Albedo"));
	realfrontDoorWallRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_door_wall_Normal"));
	realfrontDoorWallRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_door_wall_Metallic"));
	realfrontDoorWallRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_door_wall_Roughness"));
	realfrontDoorWallRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Front_door_wall_AO"));

	realfrontDoorWallRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	realfrontDoorWallRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");






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





	std::shared_ptr<Muscle::GameObject> pfloor = Muscle::CreateFromPrefab(TEXT("Passage_floor"));

	std::shared_ptr<Muscle::GameObject> realpFloor = pfloor->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realpFloorRen = realpFloor->GetComponent<Muscle::MeshRenderer>();

	realpFloorRen->_renderingData->_objectInfo->_usingShadow = false;
	realpFloorRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_floor_Albedo"));
	realpFloorRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_floor_Normal"));
	realpFloorRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_floor_Metallic"));
	realpFloorRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_floor_Roughness"));
	realpFloorRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_floor_AO"));
	realpFloorRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_floor_Emissive"));

	realpFloorRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realpFloorRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");






	std::shared_ptr<Muscle::GameObject> psideWall = Muscle::CreateFromPrefab(TEXT("Passage_side_wall"));

	std::shared_ptr<Muscle::GameObject> realpsideWall = psideWall->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realpsideWallRen = realpsideWall->GetComponent<Muscle::MeshRenderer>();

	realpsideWallRen->_renderingData->_objectInfo->_usingShadow = false;
	realpsideWallRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_side_wall_Albedo"));
	realpsideWallRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_side_wall_Normal"));
	realpsideWallRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_side_wall_Metallic"));
	realpsideWallRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_side_wall_Roughness"));
	realpsideWallRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_side_wall_AO"));
	realpsideWallRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Passage_side_wall_Emissive"));

	realpsideWallRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realpsideWallRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");





	std::shared_ptr<Muscle::GameObject> sideWall = Muscle::CreateFromPrefab(TEXT("Side_wall"));

	std::shared_ptr<Muscle::GameObject> realsideWall = sideWall->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realsideWallRen = realsideWall->GetComponent<Muscle::MeshRenderer>();

	realsideWallRen->_renderingData->_objectInfo->_usingShadow = false;
	realsideWallRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Side_wall_Albedo"));
	realsideWallRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Side_wall_Normal"));
	realsideWallRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Side_wall_Metallic"));
	realsideWallRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Side_wall_Roughness"));
	realsideWallRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Side_wall_AO"));
	realsideWallRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Side_wall_Emissive"));

	realsideWallRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realsideWallRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");




	std::shared_ptr<Muscle::GameObject> topWall = Muscle::CreateFromPrefab(TEXT("Top_wall"));

	std::shared_ptr<Muscle::GameObject> realtopWall = topWall->GetChildrens()[0];

	std::shared_ptr<Muscle::MeshRenderer> realtopWallRen = realtopWall->GetComponent<Muscle::MeshRenderer>();

	realtopWallRen->_renderingData->_objectInfo->_usingShadow = false;
	realtopWallRen->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Top_wall_Albedo"));
	realtopWallRen->_renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Top_wall_Normal"));
	realtopWallRen->_renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Top_wall_Metallic"));
	realtopWallRen->_renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Top_wall_Roughness"));
	realtopWallRen->_renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Top_wall_AO"));
	realtopWallRen->_renderingData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Top_wall_Emissive"));

	realtopWallRen->_renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	realtopWallRen->_renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");

	 


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
}

void Stage1Scene::Start()
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

	// UI Panel. 씬 스타트 시 새로 만들어준다 ..!
	_pausePanelUI = std::make_shared<PausePanel_Prefab>("Stage1Scene");

	_hpPanelUI = std::make_shared<HPPanel_Prefab>();

	std::make_shared<ItemUIPanel_Prefab>();
	// UI Panel. 씬 스타트 시 새로 만들어준다 ..!

	std::shared_ptr<Muscle::IGameEngine> _Temp = Muscle::IGameEngine::Get();

	// ---------------- Load Data ----------------
		// 메인 카메라로 쓸꺼에용
	std::shared_ptr<Muscle::GameObject> testCam = Muscle::CreateGameObject();

	// 메인 카메라로 사용하겠다고 셋팅
	_Temp->SetMainCamera(testCam->AddComponent<Muscle::Camera>());			// 각 씬별로 Start에 메인 카메라의 세팅이 필요하구나 !

	testCam->GetComponent<Muscle::Transform>()->SetPosition(Vector3(0.f, 3.f, -10.f));
	// 환경맵 로드 후 셋팅
	std::shared_ptr<Muscle::GameObject> skyBox = Muscle::CreateGameObject();

	// Cube
	skyBox->AddComponent<Muscle::SkyBox>()->Initialize(Muscle::ResourceManager::Get()->GetTextureID(TEXT("Environment")), testCam->GetComponent<Muscle::Camera>());

	// Panoramic
	//skyBox->AddComponent<Muscle::SkyBox>()->Initialize(Muscle::ResourceManager::Get()->GetTextureID(TEXT("Environment_Pano")), testCam->GetComponent<Muscle::Camera>());

	skyBox->SetParent(testCam);

	// Not Panoramic, Cube Map SkyBox
	skyBox->GetComponent<Muscle::SkyBox>()->_renderingData->_shaderInfo->_psName = TEXT("PS_SkyBox");

	// if Panoramic SkyBox, Use This Shader
	//skyBox->GetComponent<Muscle::SkyBox>()->_renderingData->_shaderInfo->_psName = TEXT("PS_SkyBox_Equirectangular");

	std::shared_ptr<Muscle::GameObject> light = Muscle::CreateGameObject();

	Vector3 direction = Vector3(-0.5f, -0.5f, 0.f);

	direction.Normalize();

	light->AddComponent<Muscle::DirectionalLight>()->SetDirectional(direction);

	_player = std::make_shared<Player_Prefab>()->GetGameObject();

	// std::make_shared<Grid_Axis_Prefab>();

	// 보스 (골렘처럼 생김)
	std::shared_ptr<Muscle::GameObject> boss = std::make_shared<Tutorial_Boss_Prefab>()->GetGameObject();

	std::vector<std::shared_ptr<Muscle::GameObject>> childs(0);

	boss->GetAllChildrens(childs);

	// 보스 그림자 빼준다 !
	for (auto& child : childs)
	{
		std::shared_ptr<Muscle::RendererBase> ren = child->GetComponent<Muscle::RendererBase>();

		if (ren != nullptr)
		{
			ren->_renderingData->_objectInfo->_usingShadow = false;
		}
	}

	_player->GetComponent<Player>()->SetTarget(boss->GetComponent<Boss>());
	boss->GetComponent<Boss>()->SetTarget(_player->GetComponent<Player>());
	boss->GetTransform()->SetPosition(-27, 5, 0);
	boss->GetTransform()->RotateY(-3.14f / 2);
	Vector3 bossPos = boss->GetTransform()->GetPosition();
	bossPos.y = _player->GetTransform()->GetPosition().y;
	_player->GetTransform()->LookAt(bossPos);
	_player->GetTransform()->RotateY(3.14f);

	auto tileMap = std::make_shared<TestTile_Prefab>();

	GetSoundManager()->Play("Stage1", IPlayMode::BGM);

	CreateStageOneObjects();

	CameraMovementManager::Get()->Stage1Start();

	Muscle::CTime::SetGameSpeed(1.0f, 0); // 속도 복귀.
	GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::Effect);
	GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::BGM);
}

void Stage1Scene::RapidUpdate()
{
	//Muscle::MuscleEngine::GetInstance()->GetMainCamera()->GetTransform()->LookAt(_player->GetTransform()->GetPosition());
}

void Stage1Scene::Update()
{
	_playTime += Muscle::CTime::GetGameDeltaTime();

// #ifdef _DEBUG
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
// #endif // _DEBUG
}