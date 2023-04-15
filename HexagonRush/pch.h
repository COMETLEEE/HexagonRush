#pragma once
/// <summary>
/// 미리 컴파일된 헤더(PCH)
/// 
/// 추가 방법
/// 1. pch.h, pch.cpp 추가
/// 2. 프로젝트 - 속성 - C/C++ - 미리 컴파일된 헤더 - 미리 컴파일된 헤더 "사용" 선택
/// 3. pch.cpp - 속성 - C/C++ - 미리 컴파일된 헤더 - 미리 컴파일된 헤더 "만들기" 선택
/// 4. pch.cpp에 #include "pch.h" 추가
/// 5. pch.h에 필요한 헤더들을 기술
/// 
/// </summary>

#include <windows.h>
#include <memory>
#include <map>
#include <string>
#include <queue>
#include <unordered_map>
// 수학 관련, 이 이상의 수학 헤더를 어차피 내가 만들 수 없음.
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

constexpr float _TileOffset = 1.8f; //타일간에 간격?


using tchar = TCHAR;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

#include "..\SoundLib\InstanceGetter.h"		// 둘 다 구현은 SoundLib.Lib (fmod.dll 종속성 있음)
#include "..\SoundLib\ISoundManager.h"		// 둘 다 구현은 SoundLib.Lib (fmod.dll 종속성 있음)
#include "MuscleEngine.h"
#include "SceneManager.h"
#include "ObjectManager.h"

#include "KeyBoard.h"
#include "XPad.h"
#include "CTime.h"

#include "ComponentsHead.h"

// Game Project에서 추가하는 것
#include "IPrefab.h"
#include "StateColor.h"

#include "IState.h"
#include "NumberRenderer.h"
#include "Character.h"
#include "FSM.h"
#include "Boss.h"
#include "Second_Boss.h"
#include "Player.h"
#include "BossAttackBox.h"
#include "Controller.h"
#include "Platform.h"
#include "DebugObjectManager.h"
#include "ObjectMover.h"
#include "ResourceLoadBar.h"
#include "LaserController.h"
#include "FireController.h"
#include "TileManager.h"
#include "ExplosionController.h"
#include "CameraMovementManager.h"
#include "Crosshair.h"
#include "Inventory.h"
#include "ItemUIPanel.h"
#include "Rush_BulletController.h"
#include "DangerUI.h"
#include "FireSpriteUI.h"
#include "DrillSpiner.h"
#include "Drill_Sign.h"
#include "DrillParticle.h"

#include "Grid_Axis_Prefab.h"
#include "Drill_Sign_Prefab.h"
#include "Platform_Prefab.h"
#include "Player_Prefab.h"
#include "Example_Anim_Prefab.h"
#include "SquareTiles_Prefab.h"
#include "TestTile_Prefab.h"
#include "Tutorial_Boss_Prefab.h"
#include "LoadBar_Prefab.h"
#include "Missile_Prefab.h"
#include "Crosshiar_Prefab.h"
#include "ItemUIPanel_Prefab.h"
#include "Rush_Bullet_Prefab.h"
#include "DangerUI_Prefab.h"
#include "Second_Boss_Prefab.h"
#include "Drill_Missille_Prefab.h"
#include "EndingScene.h"

#include "Player_State.h"
#include "Tu_Boss_State.h"
#include "Second_Boss_State.h"

#include "ResourceManager.h"
#include "GameProcess.h"

#include "..\Game_Engine\ResourceManager.h"
#include "..\Game_Engine\GraphicsManager.h"