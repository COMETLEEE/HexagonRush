#include "pch.h"
#include "ResourceManager.h"
#include <thread>
std::shared_ptr<ResourceManager> ResourceManager::_instance = nullptr;

void ResourceManager::Initialize()
{
	_graphicsResourceManager = Muscle::ResourceManager::Get();

	// SoundLoad 사운드는 리소스를 내리고 올리고 하지 않을것이다.. 한번만 로드
	LoadSound();

}

void ResourceManager::Finalize()
{
	_instance.reset();
}

void ResourceManager::Update()
{
}

void ResourceManager::DataQueuePopToThead()
{
	while (!_aseLoadDataQueue.empty())
	{
		_graphicsResourceManager->LoadASE(std::get<0>(_aseLoadDataQueue.front()), std::get<1>(_aseLoadDataQueue.front()), std::get<2>(_aseLoadDataQueue.front()), std::get<3>(_aseLoadDataQueue.front()));
		_aseLoadDataQueue.pop();
	}

	while (!_textureLoadDataQueue.empty())
	{
		_graphicsResourceManager->LoadTexture(std::get<0>(_textureLoadDataQueue.front()), std::get<1>(_textureLoadDataQueue.front()));
		_textureLoadDataQueue.pop();
	}

	while (!_environmentPanoramaMapLoadDataQueue.empty())
	{
		_graphicsResourceManager->LoadEnvironmentPanoramaMap(std::get<0>(_environmentPanoramaMapLoadDataQueue.front()), std::get<1>(_environmentPanoramaMapLoadDataQueue.front()));
		_environmentPanoramaMapLoadDataQueue.pop();
	}
	// 매 프레임마다 관리를...
	while (!_soundLoadDataQueue.empty())
	{
		GetSoundManager()->LoadSoundFile(std::get<0>(_soundLoadDataQueue.front()), std::get<1>(_soundLoadDataQueue.front()), std::get<2>(_soundLoadDataQueue.front()));
		_soundLoadDataQueue.pop();
	}
	_isComplete = true;
}

void ResourceManager::LoadAllASE()
{
	LoadTileASE();
	LoadStage1MapASE();
	LoadStage2MapASE();
	LoadTu_BossASE();

	//플레이어 더미
	LoadASE(TEXT("WorkMan"), TEXT("Resource\\ASE\\WorkMan.ase"), true, false);

	// 진짜 플레이어
	LoadASE(TEXT("Player"), TEXT("Resource\\Player_Ch\\hexagon_player_ch_0824_animation.ase"), true, false);

	Boss2_BossASE();
}
void ResourceManager::ResetTexture()
{
	Muscle::MuscleEngine::Get()->GetResourceManager()->ResetTexture();
}

void ResourceManager::LoadStage1()
{
	// 빨리 로딩해서 사용해야하는 리소스들.
	LoadLoadingScene();

	// 큐브맵 => 노트북 이슈로 인해서 잠깐 IBL 꺼둔다 ..
	_graphicsResourceManager->LoadEnvironmentCubeMap(TEXT("Environment"), TEXT("Resource\\Texture\\Stage1_2048.dds"));

	// 인게임 UI
	LoadInGameUI();

	// 스테이지 1의 맵관련 텍스쳐 및 ase
	LoadStage1MapTexture();

	// 보스 1의 텍스쳐 ase
	LoadTu_BossTexture();

	// 플레이어의 텍스쳐
	LoadPlayerTexture();

	// 타일의 Texture, ase
	LoadTileTexture();

	// 게임의 모든 UI
	LoadUITexture();

	_resourceAllCount = _aseLoadDataQueue.size() + _textureLoadDataQueue.size() + _environmentPanoramaMapLoadDataQueue.size() + _soundLoadDataQueue.size();
}

void ResourceManager::LoadStage2()
{
	LoadLoadingScene();

	// 임시..!
	_graphicsResourceManager->LoadEnvironmentCubeMap(TEXT("Environment2"), TEXT("Resource\\Texture\\Stage2_2048.dds"));
	// _graphicsResourceManager->LoadEnvironmentCubeMap(TEXT("Environment"), TEXT("Resource\\Texture\\Stage1.dds"));

	Boss2_BossTexture();

	// 타일의 Texture
	LoadTileTexture();

	// 스테이지 2의 맵관련 텍스쳐 
	LoadStage2MapTexture();

	// 플레이어의 텍스쳐
	LoadPlayerTexture();

	// 게임의 모든 UI
	LoadUITexture();

	LoadInGameUI();

	_resourceAllCount = _aseLoadDataQueue.size() + _textureLoadDataQueue.size() + _environmentPanoramaMapLoadDataQueue.size() + _soundLoadDataQueue.size();
}

void ResourceManager::LoadUIScene()
{
	// 빨리 로딩해서 사용해야하는 리소스들.
	LoadLoadingScene();

	// 게임의 모든 UI
	LoadUITexture();

	_resourceAllCount = _aseLoadDataQueue.size() + _textureLoadDataQueue.size() + _environmentPanoramaMapLoadDataQueue.size() + _soundLoadDataQueue.size();
}

void ResourceManager::LoadInGameUI()
{
	_isComplete = false;

	// ---------------- Fire ----------------
	LoadTexture(TEXT("Fire000"), TEXT("Resource\\FireSprite\\Fire000.png"));
	LoadTexture(TEXT("Fire001"), TEXT("Resource\\FireSprite\\Fire001.png"));
	LoadTexture(TEXT("Fire002"), TEXT("Resource\\FireSprite\\Fire002.png"));
	LoadTexture(TEXT("Fire003"), TEXT("Resource\\FireSprite\\Fire003.png"));
	LoadTexture(TEXT("Fire004"), TEXT("Resource\\FireSprite\\Fire004.png"));
	LoadTexture(TEXT("Fire005"), TEXT("Resource\\FireSprite\\Fire005.png"));
	LoadTexture(TEXT("Fire006"), TEXT("Resource\\FireSprite\\Fire006.png"));
	LoadTexture(TEXT("Fire007"), TEXT("Resource\\FireSprite\\Fire007.png"));
	LoadTexture(TEXT("Fire008"), TEXT("Resource\\FireSprite\\Fire008.png"));
	LoadTexture(TEXT("Fire009"), TEXT("Resource\\FireSprite\\Fire009.png"));
	LoadTexture(TEXT("Fire010"), TEXT("Resource\\FireSprite\\Fire010.png"));
	LoadTexture(TEXT("Fire011"), TEXT("Resource\\FireSprite\\Fire011.png"));

	// ---------------- Combo ----------------
	LoadTexture(TEXT("Combo0"), TEXT("Resource\\Combo\\Combo0.png"));
	LoadTexture(TEXT("Combo1"), TEXT("Resource\\Combo\\Combo1.png"));
	LoadTexture(TEXT("Combo2"), TEXT("Resource\\Combo\\Combo2.png"));
	LoadTexture(TEXT("Combo3"), TEXT("Resource\\Combo\\Combo3.png"));
	LoadTexture(TEXT("Combo4"), TEXT("Resource\\Combo\\Combo4.png"));
	LoadTexture(TEXT("Combo5"), TEXT("Resource\\Combo\\Combo5.png"));
	LoadTexture(TEXT("Combo6"), TEXT("Resource\\Combo\\Combo6.png"));

	// ---------------- Danger ----------------
	LoadTexture(TEXT("Danger"), TEXT("Resource\\UI\\Danger\\Danger_text.png"));
	LoadTexture(TEXT("Danger_background"), TEXT("Resource\\UI\\Danger\\Danger_background.png"));

	// ---------------- ItemUI ----------------
	LoadTexture(TEXT("HpItem"), TEXT("Resource\\UI\\Heart.png"));
	LoadTexture(TEXT("PowerItem"), TEXT("Resource\\UI\\Attack.png"));
	LoadTexture(TEXT("SpeedItem"), TEXT("Resource\\UI\\Speed.png"));

	// ------------------ RGB Sign UI ----------------------
	LoadTexture(TEXT("RGBSign"), TEXT("Resource\\UI\\RGB_Sign.png"));

	// ---------------- Item Use UI ----------------
	LoadTexture(TEXT("Hp_Use"), TEXT("Resource\\UI\\Hp_up.png"));
	LoadTexture(TEXT("Power_Use"), TEXT("Resource\\UI\\ATK_up.png"));
	LoadTexture(TEXT("Speed_Use"), TEXT("Resource\\UI\\SPD_up.png"));


	// ---------------- Numbers ----------------
	LoadTexture(TEXT("0"), TEXT("Resource\\Numbers\\0_Number.png"));
	LoadTexture(TEXT("1"), TEXT("Resource\\Numbers\\1_Number.png"));
	LoadTexture(TEXT("2"), TEXT("Resource\\Numbers\\2_Number.png"));
	LoadTexture(TEXT("3"), TEXT("Resource\\Numbers\\3_Number.png"));
	LoadTexture(TEXT("4"), TEXT("Resource\\Numbers\\4_Number.png"));
	LoadTexture(TEXT("5"), TEXT("Resource\\Numbers\\5_Number.png"));
	LoadTexture(TEXT("6"), TEXT("Resource\\Numbers\\6_Number.png"));
	LoadTexture(TEXT("7"), TEXT("Resource\\Numbers\\7_Number.png"));
	LoadTexture(TEXT("8"), TEXT("Resource\\Numbers\\8_Number.png"));
	LoadTexture(TEXT("9"), TEXT("Resource\\Numbers\\9_Number.png"));


	// ---------------- Numbers ----------------
	LoadTexture(TEXT("0_gray"), TEXT("Resource\\Numbers\\color_gray\\0_Number_gray.png"));
	LoadTexture(TEXT("1_gray"), TEXT("Resource\\Numbers\\color_gray\\1_Number_gray.png"));
	LoadTexture(TEXT("2_gray"), TEXT("Resource\\Numbers\\color_gray\\2_Number_gray.png"));
	LoadTexture(TEXT("3_gray"), TEXT("Resource\\Numbers\\color_gray\\3_Number_gray.png"));
	LoadTexture(TEXT("4_gray"), TEXT("Resource\\Numbers\\color_gray\\4_Number_gray.png"));
	LoadTexture(TEXT("5_gray"), TEXT("Resource\\Numbers\\color_gray\\5_Number_gray.png"));
	LoadTexture(TEXT("6_gray"), TEXT("Resource\\Numbers\\color_gray\\6_Number_gray.png"));
	LoadTexture(TEXT("7_gray"), TEXT("Resource\\Numbers\\color_gray\\7_Number_gray.png"));
	LoadTexture(TEXT("8_gray"), TEXT("Resource\\Numbers\\color_gray\\8_Number_gray.png"));
	LoadTexture(TEXT("9_gray"), TEXT("Resource\\Numbers\\color_gray\\9_Number_gray.png"));

	LoadTexture(TEXT("NumberBack"), TEXT("Resource\\Numbers\\critical_img.png"));

	// ---------------- Button UI ---------------------------
	LoadTexture(TEXT("ButtonDown"), TEXT("Resource\\UI\\ButtonDown.png"));
	LoadTexture(TEXT("ButtonUp"), TEXT("Resource\\UI\\ButtonUp.png"));

	// ---------------- For Particle Design ----------------
	LoadTexture(TEXT("LaserParticle"), TEXT("Resource\\Texture\\LaserParticle.png"));
	LoadTexture(TEXT("Particle"), TEXT("Resource\\Texture\\Particle.png"));
	LoadTexture(TEXT("TileParticle"), TEXT("Resource\\Texture\\TileParticle.png"));
	LoadTexture(TEXT("Flame"), TEXT("Resource\\Texture\\Flame.png"));
	LoadTexture(TEXT("Flame2"), TEXT("Resource\\Texture\\Flame2.png"));

	// ---------------- Pause UI ----------------
	LoadTexture(TEXT("Cursor"), TEXT("Resource\\UI\\Tri_cursor\\Tri_Cursor.png"));

	LoadTexture(TEXT("Pause_Panel"), TEXT("Resource\\UI\\Purse_img\\Hex_Pause_background.png"));
	LoadTexture(TEXT("Pause_Resume"), TEXT("Resource\\UI\\Purse_img\\Hex_Purse_Resume.png"));
	LoadTexture(TEXT("Pause_Restart"), TEXT("Resource\\UI\\Purse_img\\Hex_Purse_Restart.png"));
	LoadTexture(TEXT("Pause_Title"), TEXT("Resource\\UI\\Purse_img\\Hex_Purse_Title.png"));
	LoadTexture(TEXT("Pause_Exit"), TEXT("Resource\\UI\\Purse_img\\Hex_Purse_exit.png"));
}

void ResourceManager::LoadStage1MapTexture()
{
	_isComplete = false;
	// Door
	LoadTexture(TEXT("Door_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Door_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Door_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Door_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Door_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door\\DefaultMaterial_Mixed_AO.png"));

	// Door Box
	LoadTexture(TEXT("Door_box_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_box\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Door_box_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_box\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Door_box_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_box\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Door_box_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_box\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Door_box_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_box\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Door_box_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_box\\DefaultMaterial_Emissive.png"));

	// Door Screen
	LoadTexture(TEXT("Door_screen_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_screen\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Door_screen_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_screen\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Door_screen_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_screen\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Door_screen_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_screen\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Door_screen_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Door_screen\\DefaultMaterial_Mixed_AO.png"));

	// Floor
	LoadTexture(TEXT("Floor_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Floor\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Floor_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Floor\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Floor_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Floor\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Floor_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Floor\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Floor_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Floor\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Floor_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Floor\\DefaultMaterial_Emissive.png"));

	// Front Wall
	LoadTexture(TEXT("Front_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_wall\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Front_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_wall\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Front_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_wall\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Front_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_wall\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Front_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_wall\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Front_wall_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_wall\\DefaultMaterial_Emissive.png"));


	// Front Door Wall
	LoadTexture(TEXT("Front_door_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_door_wall\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Front_door_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_door_wall\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Front_door_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_door_wall\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Front_door_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_door_wall\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Front_door_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Front_door_wall\\DefaultMaterial_Mixed_AO.png"));



	// Ground
	LoadTexture(TEXT("Ground_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Ground_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Ground_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Ground_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Ground_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Ground_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Emissive.png"));


	// Ground Wall
	LoadTexture(TEXT("Ground_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Ground_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Ground_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Ground_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Ground_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Mixed_AO.png"));


	// Passage Floor
	LoadTexture(TEXT("Passage_floor_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_floor\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Passage_floor_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_floor\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Passage_floor_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_floor\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Passage_floor_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_floor\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Passage_floor_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_floor\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Passage_floor_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_floor\\DefaultMaterial_Emissive.png"));


	// Passage Side Wall
	LoadTexture(TEXT("Passage_side_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_side_wall\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Passage_side_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_side_wall\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Passage_side_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_side_wall\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Passage_side_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_side_wall\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Passage_side_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_side_wall\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Passage_side_wall_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Passage_side_wall\\DefaultMaterial_Emissive.png"));

	// Side Wall
	LoadTexture(TEXT("Side_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Side_wall\\Material.006_Base_color.png"));
	LoadTexture(TEXT("Side_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Side_wall\\Material.006_Metallic.png"));
	LoadTexture(TEXT("Side_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Side_wall\\Material.006_Roughness.png"));
	LoadTexture(TEXT("Side_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Side_wall\\Material.006_Normal_OpenGL.png"));
	LoadTexture(TEXT("Side_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Side_wall\\Material.006_Mixed_AO.png"));
	LoadTexture(TEXT("Side_wall_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Side_wall\\Material.006_Emissive.png"));

	// Top Wall
	LoadTexture(TEXT("Top_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Top_wall\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Top_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Top_wall\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Top_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Top_wall\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Top_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Top_wall\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Top_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Top_wall\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Top_wall_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Top_wall\\DefaultMaterial_Emissive.png"));

	// Tile Ground
	LoadTexture(TEXT("Tile_ground_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Tile_ground_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Tile_ground_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Tile_ground_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Tile_ground_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Mixed_AO.png"));
}

void ResourceManager::LoadStage1MapASE()
{
	_isComplete = false;

	LoadASE(TEXT("Tile"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Platform.ase"), false, true);
	LoadASE(TEXT("Door"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Door.ase"), false, true);
	LoadASE(TEXT("Door2"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Door2.ase"), false, true);
	LoadASE(TEXT("Door_box"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Door_box.ase"), false, true);
	LoadASE(TEXT("Door_screen"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Door_screen.ase"), false, true);
	LoadASE(TEXT("Floor"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Floor.ase"), false, true);
	LoadASE(TEXT("Front_wall"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Front_wall.ase"), false, true);
	LoadASE(TEXT("Front_door_wall"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Front_door_wall.ase"), false, true);
	LoadASE(TEXT("Ground"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Ground.ase"), false, true);
	LoadASE(TEXT("Ground_wall"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Ground_wall.ase"), false, true);
	LoadASE(TEXT("Passage_floor"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Passage_floor.ase"), false, true);
	LoadASE(TEXT("Passage_side_wall"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Passage_side_wall.ase"), false, true);
	LoadASE(TEXT("Side_wall"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Side_wall.ase"), false, true);
	LoadASE(TEXT("Top_wall"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Top_wall.ase"), false, true);
	LoadASE(TEXT("Tile_ground"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Tile_ground.ase"), false, true);
}

void ResourceManager::LoadStage2MapTexture()
{
	_isComplete = false;

	// Floor
	LoadTexture(TEXT("Surface_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\Surface\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Surface_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\Surface\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Surface_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\Surface\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Surface_Normal"), TEXT("Resource\\Stage2\\Texture_map\\Surface\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Surface_AO"), TEXT("Resource\\Stage2\\Texture_map\\Surface\\DefaultMaterial_Mixed_AO.png"));

	// Building
	LoadTexture(TEXT("Building_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\Building\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Building_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\Building\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Building_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\Building\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Building_Normal"), TEXT("Resource\\Stage2\\Texture_map\\Building\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Building_AO"), TEXT("Resource\\Stage2\\Texture_map\\Building\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Building_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\Building\\DefaultMaterial_Emissive.png"));

	// 2Building
	LoadTexture(TEXT("2Building_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\2Building\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("2Building_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\2Building\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("2Building_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\2Building\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("2Building_Normal"), TEXT("Resource\\Stage2\\Texture_map\\2Building\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("2Building_AO"), TEXT("Resource\\Stage2\\Texture_map\\2Building\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("2Building_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\2Building\\DefaultMaterial_Emissive.png"));

	// 3Building
	LoadTexture(TEXT("3Building_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\3Building\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("3Building_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\3Building\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("3Building_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\3Building\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("3Building_Normal"), TEXT("Resource\\Stage2\\Texture_map\\3Building\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("3Building_AO"), TEXT("Resource\\Stage2\\Texture_map\\3Building\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("3Building_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\3Building\\DefaultMaterial_Emissive.png"));

	// 4Building
	LoadTexture(TEXT("4Building_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\4Building\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("4Building_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\4Building\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("4Building_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\4Building\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("4Building_Normal"), TEXT("Resource\\Stage2\\Texture_map\\4Building\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("4Building_AO"), TEXT("Resource\\Stage2\\Texture_map\\4Building\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("4Building_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\4Building\\DefaultMaterial_Emissive.png"));

	// Box
	LoadTexture(TEXT("Box_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\Box\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Box_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\Box\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Box_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\Box\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Box_Normal"), TEXT("Resource\\Stage2\\Texture_map\\Box\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Box_AO"), TEXT("Resource\\Stage2\\Texture_map\\Box\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Box_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\Box\\DefaultMaterial_Emissive.png"));

	// 2Box
	LoadTexture(TEXT("2Box_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\2Box\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("2Box_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\2Box\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("2Box_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\2Box\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("2Box_Normal"), TEXT("Resource\\Stage2\\Texture_map\\2Box\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("2Box_AO"), TEXT("Resource\\Stage2\\Texture_map\\2Box\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("2Box_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\2Box\\DefaultMaterial_Emissive.png"));

	// 3Box
	LoadTexture(TEXT("3Box_Albedo"), TEXT("Resource\\Stage2\\Texture_map\\3Box\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("3Box_Metallic"), TEXT("Resource\\Stage2\\Texture_map\\3Box\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("3Box_Roughness"), TEXT("Resource\\Stage2\\Texture_map\\3Box\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("3Box_Normal"), TEXT("Resource\\Stage2\\Texture_map\\3Box\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("3Box_AO"), TEXT("Resource\\Stage2\\Texture_map\\3Box\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("3Box_Emissive"), TEXT("Resource\\Stage2\\Texture_map\\3Box\\DefaultMaterial_Emissive.png"));

	// Tile Ground
	LoadTexture(TEXT("Tile_ground_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Tile_ground_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Tile_ground_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Tile_ground_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Tile_ground_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_ground\\DefaultMaterial_Mixed_AO.png"));

	// Ground
	LoadTexture(TEXT("Ground_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Ground_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Ground_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Ground_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Ground_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Ground_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground\\DefaultMaterial_Emissive.png"));


	// Ground Wall
	LoadTexture(TEXT("Ground_wall_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Ground_wall_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Ground_wall_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Ground_wall_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Ground_wall_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Ground_wall\\DefaultMaterial_Mixed_AO.png"));

}

void ResourceManager::LoadStage2MapASE()
{
	_isComplete = false;

	LoadASE(TEXT("Surface"), TEXT("Resource\\Stage2\\Ase_obj\\Surface.ase"), true, true);
	LoadASE(TEXT("Building"), TEXT("Resource\\Stage2\\Ase_obj\\Bulding.ase"), false, true);
	LoadASE(TEXT("2Building"), TEXT("Resource\\Stage2\\Ase_obj\\2Bulding.ase"), false, true);
	LoadASE(TEXT("3Building"), TEXT("Resource\\Stage2\\Ase_obj\\3Bulding.ase"), false, true);
	LoadASE(TEXT("4Building"), TEXT("Resource\\Stage2\\Ase_obj\\4Bulding.ase"), false, true);
	LoadASE(TEXT("Box"), TEXT("Resource\\Stage2\\Ase_obj\\Box.ase"), false, true);
	LoadASE(TEXT("2Box"), TEXT("Resource\\Stage2\\Ase_obj\\2Box.ase"), false, true);
	LoadASE(TEXT("3Box"), TEXT("Resource\\Stage2\\Ase_obj\\3Box.ase"), false, true);
}

void ResourceManager::LoadTu_BossTexture()
{
	_isComplete = false;
	// black_armadfinger
	LoadTexture(TEXT("Crosshair_Albedo"), TEXT("Resource\\Crosshair\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Crosshair_Metallic"), TEXT("Resource\\Crosshair\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Crosshair_AO"), TEXT("Resource\\Crosshair\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Crosshair_Normal"), TEXT("Resource\\Crosshair\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Crosshair_Roughness"), TEXT("Resource\\Crosshair\\DefaultMaterial_Roughness.png"));

	// black_armadfinger
	LoadTexture(TEXT("black_armandfinger_Albedo"), TEXT("Resource\\Tutorial_Boss_Texturemap\\black_arm_and_finger\\black_armandfinger_Base_color_1001.png"));
	LoadTexture(TEXT("black_armandfinger_Metallic"), TEXT("Resource\\Tutorial_Boss_Texturemap\\black_arm_and_finger\\black_armandfinger_Metallic_1001.png"));
	LoadTexture(TEXT("black_armandfinger_AO"), TEXT("Resource\\Tutorial_Boss_Texturemap\\black_arm_and_finger\\black_armandfinger_Mixed_AO_1001.png"));
	LoadTexture(TEXT("black_armandfinger_Normal"), TEXT("Resource\\Tutorial_Boss_Texturemap\\black_arm_and_finger\\black_armandfinger_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("black_armandfinger_Roughness"), TEXT("Resource\\Tutorial_Boss_Texturemap\\black_arm_and_finger\\black_armandfinger_Roughness_1001.png"));

	// core_and_missile
	LoadTexture(TEXT("core_and_missile_Albedo_Red"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Base_color_1001_red.png"));
	LoadTexture(TEXT("core_and_missile_Albedo_Blue"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Base_color_1001_blue.png"));
	LoadTexture(TEXT("core_and_missile_Albedo_Green"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Base_color_1001_green.png"));
	LoadTexture(TEXT("core_and_missile_AO"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Mixed_AO_1001.png"));
	LoadTexture(TEXT("core_and_missile_Metallic"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Metallic_1001.png"));
	LoadTexture(TEXT("core_and_missile_Normal"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("core_and_missile_Roughness"), TEXT("Resource\\Tutorial_Boss_Texturemap\\core_and_missile\\core_and_missile_Roughness_1001.png"));

	//// 
	LoadTexture(TEXT("eye_Albedo"), TEXT("Resource\\Tutorial_Boss_Texturemap\\eyes\\eyes_Base_color_1001.png"));
	LoadTexture(TEXT("eye_Metallic"), TEXT("Resource\\Tutorial_Boss_Texturemap\\eyes\\eyes_Metallic_1001.png"));
	LoadTexture(TEXT("eye_AO"), TEXT("Resource\\Tutorial_Boss_Texturemap\\eyes\\eyes_Mixed_AO_1001.png"));
	LoadTexture(TEXT("eye_Normal"), TEXT("Resource\\Tutorial_Boss_Texturemap\\eyes\\eyes_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("eye_Roughness"), TEXT("Resource\\Tutorial_Boss_Texturemap\\eyes\\eyes_Roughness_1001.png"));

	/// core_cove_lightbrown
	LoadTexture(TEXT("core_cover_lightbrown_Albedo"), TEXT("Resource\\Tutorial_Boss_Texturemap\\lightbrown_core_cover\\core_cover_lightbrown_Base_color_1001.png"));
	LoadTexture(TEXT("core_cover_lightbrown_Metallic"), TEXT("Resource\\Tutorial_Boss_Texturemap\\lightbrown_core_cover\\core_cover_lightbrown_Metallic_1001.png"));
	LoadTexture(TEXT("core_cover_lightbrown_AO"), TEXT("Resource\\Tutorial_Boss_Texturemap\\lightbrown_core_cover\\core_cover_lightbrown_Mixed_AO_1001.png"));
	LoadTexture(TEXT("core_cover_lightbrown_Normal"), TEXT("Resource\\Tutorial_Boss_Texturemap\\lightbrown_core_cover\\core_cover_lightbrown_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("core_cover_lightbrown_Roughness"), TEXT("Resource\\Tutorial_Boss_Texturemap\\lightbrown_core_cover\\core_cover_lightbrown_Roughness_1001.png"));

	/// yellow_bodycolor;
	LoadTexture(TEXT("yellow_bodycolor_Albedo"), TEXT("Resource\\Tutorial_Boss_Texturemap\\yellow_bodycolor\\yellow_bodycolor_Base_color_1001.png"));
	LoadTexture(TEXT("yellow_bodycolor_Metallic"), TEXT("Resource\\Tutorial_Boss_Texturemap\\yellow_bodycolor\\yellow_bodycolor_Metallic_1001.png"));
	LoadTexture(TEXT("yellow_bodycolor_AO"), TEXT("Resource\\Tutorial_Boss_Texturemap\\yellow_bodycolor\\yellow_bodycolor_Mixed_AO_1001.png"));
	LoadTexture(TEXT("yellow_bodycolor_Normal"), TEXT("Resource\\Tutorial_Boss_Texturemap\\yellow_bodycolor\\yellow_bodycolor_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("yellow_bodycolor_Roughness"), TEXT("Resource\\Tutorial_Boss_Texturemap\\yellow_bodycolor\\yellow_bodycolor_Roughness_1001.png"));
}

void ResourceManager::LoadTu_BossASE()
{
	_isComplete = false;

	LoadASE(TEXT("Crosshair"), TEXT("Resource\\Crosshair\\Cross_hair.ase"), false, true);
	LoadASE(TEXT("Missile"), TEXT("Resource\\ASE\\Missile.ase"), true, true);
	LoadASE(TEXT("Boss_01"), TEXT("Resource\\ASE\\tu_boss_01.ase"), true, false);
}

void ResourceManager::LoadTileTexture()
{
	_isComplete = false;

	// Tile N
	LoadTexture(TEXT("Tile_N.color_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_N.color\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Tile_N.color_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_N.color\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Tile_N.color_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_N.color\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Tile_N.color_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_N.color\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Tile_N.color_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_N.color\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Tile_N.color_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_N.color\\DefaultMaterial_Emissive.png"));

	// Tile Red
	LoadTexture(TEXT("Tile_Red_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Red\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Tile_Red_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Red\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Tile_Red_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Red\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Tile_Red_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Red\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Tile_Red_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Red\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Tile_Red_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Red\\DefaultMaterial_Emissive.png"));

	// Tile Blue
	LoadTexture(TEXT("Tile_Blue_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Blue\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Tile_Blue_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Blue\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Tile_Blue_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Blue\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Tile_Blue_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Blue\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Tile_Blue_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Blue\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Tile_Blue_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Blue\\DefaultMaterial_Emissive.png"));

	// Tile Grenn
	LoadTexture(TEXT("Tile_Green_Albedo"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Green\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Tile_Green_Metallic"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Green\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Tile_Green_Roughness"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Green\\DefaultMaterial_Roughness.png"));
	LoadTexture(TEXT("Tile_Green_Normal"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Green\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Tile_Green_AO"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Green\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Tile_Green_Emissive"), TEXT("Resource\\Tutorial_Texturemap\\Texture_map\\Tile_Green\\DefaultMaterial_Emissive.png"));
}

void ResourceManager::LoadTileASE()
{
	_isComplete = false;

	LoadASE(TEXT("Map_Tile"), TEXT("Resource\\Tutorial_Map\\Ase_obj\\Tile.ase"), false, true);
}

void ResourceManager::LoadSound()
{
	_isComplete = false;
	// Sound File Load



	//배경음
	LoadSoundFile("Title", "Resource\\Sound\\1_BackGround\\Blue Sirens - Compass [Thematic].wav", 2);  // 타이틀 재생 // 완료
	LoadSoundFile("Sinopsis", "Resource\\Sound\\1_BackGround\\Adam Angeles - Your Grace [Thematic].wav", 2); // 시놉시스 재생
	LoadSoundFile("Stage1", "Resource\\Sound\\1_BackGround\\Donald XL Robertson  - IOU [Thematic].wav", 2); // 스테이지 1 재생 // 완료
	LoadSoundFile("Stage2", "Resource\\Sound\\1_BackGround\\VirgoZilla Beatz - Mars [Thematic].wav", 2); // 스테이지 2 재생 // 완료
	LoadSoundFile("Ending", "Resource\\Sound\\1_BackGround\\OITU - Drocer Nekorb [Thematic].mp3", 2); // 엔딩 재생 // 완료

	// 캐릭터 효과음
	LoadSoundFile("Move_L", "Resource\\Sound\\3_Character\\Move_L.wav"); // 캐릭터 이동 (왼발) 110 // 완료
	LoadSoundFile("Move_R", "Resource\\Sound\\3_Character\\Move_R.wav"); // 캐릭터 이동 (오른발) 50 // 완료
	LoadSoundFile("Evade", "Resource\\Sound\\3_Character\\Evade1.wav"); // 캐릭터 회피 // 완료
	LoadSoundFile("Attack", "Resource\\Sound\\3_Character\\Attack.wav"); // 캐릭터 공격 // 완료
	LoadSoundFile("Hit", "Resource\\Sound\\3_Character\\Hit.wav"); // 캐릭터 피격 // 완료
	LoadSoundFile("Rush", "Resource\\Sound\\3_Character\\attack2.wav"); // 캐릭터 러시 // 완료

	// 아이템 효과음
	LoadSoundFile("Potion_Drink", "Resource\\Sound\\3_Character\\Drink.wav"); // 아이템 사용 시 재생 // 완료
	LoadSoundFile("LIFE_UP", "Resource\\Sound\\3_Character\\LIFE_UP.wav"); // 라이프 회복 아이템 재생 // 완료
	LoadSoundFile("ATT_UP", "Resource\\Sound\\3_Character\\ATT_UP.wav"); // 공격력 업 아이템 재생 // 완료
	LoadSoundFile("SPEED_UP", "Resource\\Sound\\3_Character\\Speed_UP.wav"); // 이동 속도 업 아이템 재생 // 완료

	//콤보 효과음
	LoadSoundFile("Combo_1", "Resource\\Sound\\3_Character\\S4_Magic_Attack_C1.wav"); // 콤보 1 재생 // 완료
	LoadSoundFile("Combo_2", "Resource\\Sound\\3_Character\\S4_Magic_Attack_C2.wav"); // 콤보 2 재생 // 완료
	LoadSoundFile("Combo_3", "Resource\\Sound\\3_Character\\S4_Magic_Attack_C3.wav"); // 콤보 3 재생 // 완료
	LoadSoundFile("Combo_4", "Resource\\Sound\\3_Character\\S4_Magic_Attack_C4.wav"); // 콤보 4 재생 // 완료
	LoadSoundFile("Combo_5", "Resource\\Sound\\3_Character\\S4_Magic_Attack_C5_Heavier.wav"); // 콤보 5 재생 // 완료

	//보스1 효과음
	LoadSoundFile("Swing", "Resource\\Sound\\2_Effect\\SWing2.wav"); // 보스 attack1_down 75프레임 재생 // 완료
	LoadSoundFile("Silde", "Resource\\Sound\\2_Effect\\Silde.wav"); // 보스 attack2_down_slid 135프레임 재생 // 완료
	LoadSoundFile("Impact", "Resource\\Sound\\2_Effect\\Impact.wav"); // 보스 attack3_shake 125,155,185,215 프레임 재생 // 완료
	LoadSoundFile("Big_Impact", "Resource\\Sound\\2_Effect\\BigImpact.wav"); // 보스 attack3_shake 255 프레임 재생 // 완료
	LoadSoundFile("TileFall", "Resource\\Sound\\2_Effect\\Fall.wav"); // 보스 attack3_shack 190프레임 재생 // 완료
	LoadSoundFile("Missile", "Resource\\Sound\\2_Effect\\MissileShot.wav"); // 보스 attack4_missile 40,50,60,70,80,90,100,110 프레임 재생 // 완료
	LoadSoundFile("MissileBoom", "Resource\\Sound\\2_Effect\\MissileBoom.wav"); // 보스 미사일 폭파 재생 // 완료
	LoadSoundFile("LaserCharge", "Resource\\Sound\\2_Effect\\LaserCharge.wav"); // 보스 attack6_laser 30프레임 재생 // 완료
	LoadSoundFile("LaserShot", "Resource\\Sound\\2_Effect\\LaserShot.wav"); // 보스 attack6_laser 150프레임 재생 // 완료
	LoadSoundFile("Fire", "Resource\\Sound\\2_Effect\\Fire.wav"); // 보스 attack7_fire_release 30프레임 재생 // 완료
	LoadSoundFile("Fireradiate", "Resource\\Sound\\2_Effect\\Fireradiate.wav"); // 보스 attack8_fire_radiate 30프레임 재생 // 완료
	LoadSoundFile("BossHit", "Resource\\Sound\\2_Effect\\BossHit.wav"); // 보스 피격 시 재생 // 완료
	LoadSoundFile("Boss1Down", "Resource\\Sound\\2_Effect\\Boss1Down.wav"); // 보스 사망 시 재생 // 완료


	//보스2 효과음
	LoadSoundFile("Landing", "Resource\\Sound\\5_Boss2\\Landing.mp3"); // 보스 점프 착지 시 재생 170프레임 재생 // 완료
	LoadSoundFile("Drill_Shoot", "Resource\\Sound\\5_Boss2\\DrillShoot.wav"); // 보스 드릴 발사 시 재생 100프레임 재생 // 완료
	LoadSoundFile("DrillMove", "Resource\\Sound\\5_Boss2\\DrillMove.wav"); // 보스 드릴 발사 후 이동 시 재생
	LoadSoundFile("Drill", "Resource\\Sound\\5_Boss2\\Drill.wav"); // 보스 찌르기 20프레임 재생 // 완료
	LoadSoundFile("DrillUp", "Resource\\Sound\\5_Boss2\\DrillUp.wav"); // 보스 드릴 상승 오브젝트 생성 시 재생 // 완료

	LoadSoundFile("OillSHoot", "Resource\\Sound\\5_Boss2\\Shoot.wav"); // 보스 오일 발사 시 재생 145프레임 재생 // 완료
	LoadSoundFile("OillFall", "Resource\\Sound\\5_Boss2\\OilFall.wav"); // 보스 오일, 타일과 충돌 시 재생

	LoadSoundFile("StartLanding", "Resource\\Sound\\5_Boss2\\StartLanding.mp3"); // 보스 등장 사운드 // 완료
}

void ResourceManager::LoadPlayerTexture()
{
	LoadTexture(TEXT("Player_Albedo"), TEXT("Resource\\Player_Ch\\Ch_texture\\hexagon_player_ch_Base_color_1001.png"));
	LoadTexture(TEXT("Player_Metallic"), TEXT("Resource\\Player_Ch\\Ch_texture\\hexagon_player_ch_Metallic_1001.png"));
	LoadTexture(TEXT("Player_Roughness"), TEXT("Resource\\Player_Ch\\Ch_texture\\hexagon_player_ch_Roughness_1001.png"));
	LoadTexture(TEXT("Player_Normal"), TEXT("Resource\\Player_Ch\\Ch_texture\\hexagon_player_ch_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Player_AO"), TEXT("Resource\\Player_Ch\\Ch_texture\\hexagon_player_ch_Mixed_AO_1001.png"));
	LoadTexture(TEXT("Player_Emissive"), TEXT("Resource\\Player_Ch\\Ch_texture\\hexagon_player_ch_Emissive_1001.png"));
}

void ResourceManager::LoadLoadingScene()
{
	_isComplete = false;

	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_01"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_001.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_02"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_002.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_03"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_003.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_04"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_004.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_05"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_005.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_06"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_006.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_07"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_007.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_08"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_008.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_09"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_009.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_10"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_010.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_11"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_011.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_12"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_012.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_13"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_013.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_14"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_014.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_15"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_015.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_16"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_016.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_17"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_017.png"));
	_graphicsResourceManager->LoadTexture(TEXT("Hex_loading_18"), TEXT("Resource\\UI\\LoadingScene\\Hex_loading_018.png"));
}

void ResourceManager::Boss2_BossTexture()
{
	_isComplete = false;
	// ----------------- Cannonball_Color -------------------
	LoadTexture(TEXT("Boss2_CannonBall_Albedo_Black"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Black\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Boss2_CannonBall_Albedo_Blue"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Blue\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Boss2_CannonBall_Albedo_Red"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Red\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Boss2_CannonBall_Albedo_Yellow"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\\Cannon_Ball\\Cannonball_Yellow\\DefaultMaterial_Base_color.png"));

	// 캐논볼
	LoadTexture(TEXT("Boss2_CannonBall_Metallic"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Red\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Boss2_CannonBall_AO"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Red\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Boss2_CannonBall_Normal"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Red\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Boss2_CannonBall_Roughness"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\Cannonball_Red\\DefaultMaterial_Roughness.png"));


	// ----------------- Sign_Base -------------------
	LoadTexture(TEXT("Sign_Albedo"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\\Sign\\Sign_base_color.png"));
	// ----------------- CrossHair -------------------
	LoadTexture(TEXT("Crosshair_Albedo"), TEXT("Resource\\Crosshair\\DefaultMaterial_Base_color.png"));
	LoadTexture(TEXT("Crosshair_Metallic"), TEXT("Resource\\Crosshair\\DefaultMaterial_Metallic.png"));
	LoadTexture(TEXT("Crosshair_AO"), TEXT("Resource\\Crosshair\\DefaultMaterial_Mixed_AO.png"));
	LoadTexture(TEXT("Crosshair_Normal"), TEXT("Resource\\Crosshair\\DefaultMaterial_Normal_OpenGL.png"));
	LoadTexture(TEXT("Crosshair_Roughness"), TEXT("Resource\\Crosshair\\DefaultMaterial_Roughness.png"));

	// ---------------- Boss2 Core Color ----------------
	LoadTexture(TEXT("Boss2_Core_Albedo_Red"), TEXT("Resource\\2nd_Boss\\texture\\Only_Core_Base\\Core_Base_color_1001_Red.png"));
	LoadTexture(TEXT("Boss2_Core_Albedo_Green"), TEXT("Resource\\2nd_Boss\\texture\\Only_Core_Base\\Core_Base_color_1001_Green.png"));
	LoadTexture(TEXT("Boss2_Core_Albedo_Blue"), TEXT("Resource\\2nd_Boss\\texture\\Only_Core_Base\\Core_Base_color_1001_Blue.png"));
	LoadTexture(TEXT("Boss2_Core_Matallic"), TEXT("Resource\\2nd_Boss\\texture\\Only_Core_Base\\Core_Metallic_1001.png"));
	LoadTexture(TEXT("Boss2_Core_Normal"), TEXT("Resource\\2nd_Boss\\texture\\Only_Core_Base\\Core_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Boss2_Core_Roughness"), TEXT("Resource\\2nd_Boss\\texture\\Only_Core_Base\\Core_Roughness_1001.png"));

	// ---------------- Boss2 ----------------
	LoadTexture(TEXT("Boss2_Albedo_Red"), TEXT("Resource\\2nd_Boss\\texture\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Boss2_Metallic"), TEXT("Resource\\2nd_Boss\\texture\\DefaultMaterial_Metallic_1001.png"));
	LoadTexture(TEXT("Boss2_Roughness"), TEXT("Resource\\2nd_Boss\\texture\\DefaultMaterial_Roughness_1001.png"));
	LoadTexture(TEXT("Boss2_Normal"), TEXT("Resource\\2nd_Boss\\texture\\DefaultMaterial_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Boss2_AO"), TEXT("Resource\\2nd_Boss\\texture\\DefaultMaterial_Mixed_AO_1001.png"));
	LoadTexture(TEXT("Boss2_Emissive"), TEXT("Resource\\2nd_Boss\\texture\\DefaultMaterial_Emissive_1001.png"));

	// ---------------- Boss2 SingleObject ----------------
	LoadTexture(TEXT("Boss2_Dril_Albedo"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\texture\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Boss2_Dril_Metallic"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\texture\\DefaultMaterial_Metallic_1001.png"));
	LoadTexture(TEXT("Boss2_Dril_Roughness"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\texture\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Boss2_Dril_Normal"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\texture\\DefaultMaterial_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Boss2_Dril_AO"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\texture\\DefaultMaterial_Roughness_1001.png"));
	LoadTexture(TEXT("Boss2_Dril_Emissive"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\texture\\DefaultMaterial_Emissive_1001.png"));

	// ---------------- Boss2 Black ----------------
	LoadTexture(TEXT("Flowed_Black_Albed"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_black\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Flowed_Black_Normal"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_black\\DefaultMaterial_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Flowed_Black_Roughness"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_black\\DefaultMaterial_Roughness_1001.png"));
	
	// ---------------- Boss2 Blue ----------------
	LoadTexture(TEXT("Flowed_Blue_Albed"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_blue\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Flowed_Blue_Normal"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_blue\\DefaultMaterial_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Flowed_Blue_Metallic"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_blue\\DefaultMaterial_Metallic_1001.png"));
	LoadTexture(TEXT("Flowed_Blue_Roughness"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_blue\\DefaultMaterial_Roughness_1001.png"));

	// ---------------- Boss2 Red ----------------
	LoadTexture(TEXT("Flowed_Red_Albed"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_red\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Flowed_Red_Normal"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_red\\DefaultMaterial_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Flowed_Red_Metallic"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_red\\DefaultMaterial_Metallic_1001.png"));
	LoadTexture(TEXT("Flowed_Red_Roughness"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_red\\DefaultMaterial_Roughness_1001.png"));

	// ---------------- Boss2 Yellow ----------------
	LoadTexture(TEXT("Flowed_Yellow_Albed"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_yellow\\DefaultMaterial_Base_color_1001.png"));
	LoadTexture(TEXT("Flowed_Yellow_Normal"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_yellow\\DefaultMaterial_Normal_OpenGL_1001.png"));
	LoadTexture(TEXT("Flowed_Yellow_Metallic"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_yellow\\DefaultMaterial_Metallic_1001.png"));
	LoadTexture(TEXT("Flowed_Yellow_Roughness"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_yellow\\DefaultMaterial_Roughness_1001.png"));
	LoadTexture(TEXT("Flowed_Yellow_Emissive"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\texture_yellow\\DefaultMaterial_Emissive_1001.png"));
}

void ResourceManager::Boss2_BossASE()
{
	_isComplete = false;

	LoadASE(TEXT("Boss_02"), TEXT("Resource\\2nd_Boss\\2nd_boss_modeling.ase"), true, false);
	LoadASE(TEXT("Boss2_Dril"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Dril_Obj\\Dril.ase"), true, true);
	LoadASE(TEXT("Boss2_Flowed"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Flowed_Obj\\flowed.ase"), true, true);
	LoadASE(TEXT("Sign"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Sign\\sign.ase"), false, true);
	LoadASE(TEXT("Cannon_Ball"), TEXT("Resource\\2nd_Boss\\2nd_Boss_singleOBJ\\Cannon_Ball\\cannon_ball.ase"), false, true);
}

void ResourceManager::LoadUITexture()
{
	_isComplete = false;

	// ------------ Tri Cursor ------------
	LoadTexture(TEXT("Cursor"), TEXT("Resource\\UI\\Tri_cursor\\Tri_Cursor.png"));

	// Colone
	LoadTexture(TEXT("Colone"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_colon.png"));

	// ---------------- Title Scene ----------------
	LoadTexture(TEXT("Title_BackGround"), TEXT("Resource\\UI\\Main_img\\Hex_Main_img1.png"));
	LoadTexture(TEXT("Title_Exit"), TEXT("Resource\\UI\\Main_img\\Hex_Main_img_exit.png"));
	LoadTexture(TEXT("Title_Start"), TEXT("Resource\\UI\\Main_img\\Hex_Main_img_start.png"));

	// ---------------- Die Scene ----------------
	LoadTexture(TEXT("DieScene_BackGround"), TEXT("Resource\\UI\\DieScene\\Hex_You_Lose_background.png"));
	LoadTexture(TEXT("DieScene_Retry"), TEXT("Resource\\UI\\DieScene\\Hex_You_Lose_Restart.png"));
	LoadTexture(TEXT("DieScene_Title"), TEXT("Resource\\UI\\DieScene\\Hex_You_Lose_Title.png"));

	// ---------------- Stage Clear Scene ----------------
	LoadTexture(TEXT("StageClear_BackGround"), TEXT("Resource\\UI\\Stage_clear\\Hex_StageClear_background.png"));
	LoadTexture(TEXT("StageClear_NextStage"), TEXT("Resource\\UI\\Stage_clear\\Hex_StageClear_Nextstage.png"));

	// ---------------- HP UI ----------------
	LoadTexture(TEXT("HP_Yes"), TEXT("Resource\\UI\\Hp2.png"));
	LoadTexture(TEXT("HP_No"), TEXT("Resource\\UI\\heart_line.png"));

	// ---------------- Fade In / Out ----------------
	LoadTexture(TEXT("Fade"), TEXT("Resource\\Texture\\Fade.png"));

	// ---------------- Ending ----------------
	LoadTexture(TEXT("Hex_Ending_background"), TEXT("Resource\\UI\\Ending_text\\Hex_Ending_background.png"));
	LoadTexture(TEXT("Hex_Ending_text"), TEXT("Resource\\UI\\Ending_text\\Hex_Ending_text.png"));
	LoadTexture(TEXT("To_Be_Continue"), TEXT("Resource\\UI\\Tobe_continue\\To_be_continue_both.png"));

	// ---------------- Numbers ----------------
	LoadTexture(TEXT("CLEARTIME"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time.png"));
	LoadTexture(TEXT("_0"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_0.png"));
	LoadTexture(TEXT("_1"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_1.png"));
	LoadTexture(TEXT("_2"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_2.png"));
	LoadTexture(TEXT("_3"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_3.png"));
	LoadTexture(TEXT("_4"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_4.png"));
	LoadTexture(TEXT("_5"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_5.png"));
	LoadTexture(TEXT("_6"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_6.png"));
	LoadTexture(TEXT("_7"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_7.png"));
	LoadTexture(TEXT("_8"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_8.png"));
	LoadTexture(TEXT("_9"), TEXT("Resource\\UI\\Stage_clear\\stage_clear_time_9.png"));
}

const uint64 ResourceManager::GetTextureID(const tstring& textureName)
{
	return _graphicsResourceManager->GetTextureID(textureName);
}

const uint64 ResourceManager::GetMeshID(const tstring& meshName)
{
	return _graphicsResourceManager->GetMeshID(meshName);
}

const uint64 ResourceManager::GetFontID(const tstring& fontName)
{
	return _graphicsResourceManager->GetFontID(fontName);
}

std::shared_ptr<ResourceManager> ResourceManager::Get()
{
	if (!_instance)
		_instance = std::make_shared<ResourceManager>();

	return _instance;
}

void ResourceManager::LoadASE(const tstring& name, const tstring& path, bool isSmooth, bool isStatic)
{
	_aseLoadDataQueue.push(make_tuple(name, path, isSmooth, isStatic));
}

void ResourceManager::LoadTexture(const tstring& name, const tstring& path)
{
	_textureLoadDataQueue.push(make_tuple(name, path));
}

void ResourceManager::LoadEnvironmentPanoramaMap(const tstring& name, const tstring& path)
{
	_environmentPanoramaMapLoadDataQueue.push(make_tuple(name, path));
}

void ResourceManager::LoadSoundFile(const std::string& name, const std::string& filePath, unsigned int mode)
{
	// 0 Defualt
	// 2 loop
	_soundLoadDataQueue.push({ name ,filePath ,mode });
}

void ResourceManager::StartDataLoadToTread()
{
	std::jthread _thread(&ResourceManager::DataQueuePopToThead, this);
	_thread.detach();
}

bool ResourceManager::GetisComplete()
{
	return _isComplete;
}

int ResourceManager::GetAllResourceCount()
{
	return _resourceAllCount;
}

int ResourceManager::GetResourcesRemainingCount()
{
	return _aseLoadDataQueue.size() + _textureLoadDataQueue.size() + _environmentPanoramaMapLoadDataQueue.size() + _soundLoadDataQueue.size();
}
