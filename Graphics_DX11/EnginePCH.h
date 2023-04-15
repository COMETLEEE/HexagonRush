#pragma once

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// 각종 include
#include <windows.h>
#include <random>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <list>
#include <map>
#include <queue>

#include <filesystem>
namespace fs = std::filesystem;

// 수학 관련, 이 이상의 수학 헤더를 어차피 내가 만들 수 없음.
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// DirectX For Debug Utils
#include "Direct3DUtils.h"

// DirectX And COM
#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

// 각종 lib
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// DirectXTex => MicroSoft 오픈 소스. 텍스쳐를 다루는 라이브러리
#include "DirectXTex.h"
#include "..\\Library\\inc\\SpriteFont.h"
#include "..\\Library\\inc\\SpriteBatch.h"

#if defined(DEBUG) || defined(_DEBUG)
#if defined (_WIN32) && !defined(_WIN64)
#pragma comment(lib, "DirectXTex_x86_Debug.lib")
#pragma comment(lib, "DirectXTK_x86_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_x64_Debug.lib")
#pragma comment(lib, "DirectXTK_x64_Debug.lib")
#endif
#else
#if defined (_WIN32) && !defined(_WIN64)
#pragma comment(lib, "DirectXTex_x86_Release")
#pragma comment(lib, "DirectXTK_x86_Release")
#else
#pragma comment(lib, "DirectXTex_x64_Release")
#pragma comment(lib, "DirectXTK_x64_Release")
#endif
#endif

// Helper Class
#include "StringHelper.h"

// 각종 Def들
#include "Graphics_DX11_typedef.h"
#include "Vertex_def.h"
#include "Resource_def.h"
#include "Shader_def.h"
