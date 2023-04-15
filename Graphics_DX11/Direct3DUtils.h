#pragma once

#ifndef D3DUTIL_H
#define D3DUTIL_H

#include <d3d11_1.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <string>

// Direct SDK가 Windows SDK로 합쳐지면서 DXTrace가 사라짐.
// 그래서 책과는 다른 방법으로 HR 오류 메세지를 출력 방법을 수정한다.
#if defined(DEBUG) || defined(_DEBUG)
#ifndef HR
#define HR(x)                                            \
    {                                                        \
        HRESULT hr = (x);                                    \
        if(FAILED(hr))                                       \
        {                                                    \
            LPWSTR output;                                   \
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |       \
                FORMAT_MESSAGE_IGNORE_INSERTS      |         \
                FORMAT_MESSAGE_ALLOCATE_BUFFER,              \
                NULL,                                        \
                hr,                                          \
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   \
                (LPTSTR) &output,                            \
                0,                                           \
                NULL);                                       \
            MessageBox(NULL, output, L"Error", MB_OK);       \
        }                                                    \
    }
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

#ifndef GRAPHICS_DEBUGGER_OBJECT_NAME
#define GRAPHICS_DEBUGGER_OBJECT_NAME (1)
#endif

// COM 객체에 대한 Release 매크로
#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }

//
// 디버깅 보조 관련 기능
//

// ------------------------------
// D3D11SetDebugObjectName 함수
// ------------------------------
// Direct3D Device에 의해 생성된 개체에 대한 그래픽 디버거에서의 개체 이름을 설정
// [In]resource				D3D11 디바이스에서 생성된 개체
// [In]name					개체 이름
template<UINT TNameLength>
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// [In]resource				D3D11 디바이스에서 생성된 개체
// [In]name					개체 이름
// [In]length				문자열 길이
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ LPCSTR name, _In_ UINT length)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, length, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(length);
#endif
}

// [In]resource				D3D11 디바이스에서 생성된 개체
// [In]name					개체 이름
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// 그래픽 디버거에서 Direct3D Device에 의해 생성된 개체의 이름을 지우는 함수.
// [In]resource				D3D11 디바이스에서 생성된 개체
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
	UNREFERENCED_PARAMETER(resource);
#endif
}

// ------------------------------
// DXGISetDebugObjectName 함수
// ------------------------------
// DXGI 개체에 대한 그래픽 디버거에서의 개체 이름을 설정합니다.
// [In]object				DXGI 개체
// [In]name					개체 이름
template<UINT TNameLength>
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
#endif
}

// [In]object				DXGI 개체
// [In]name					개체 이름
// [In]length				문자열 길이
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ LPCSTR name, _In_ UINT length)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, length, name);
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(length);
#endif
}

// [In]object				DXGI 개체
// [In]name					개체 이름
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
#endif
}

// 그래픽 디버거에서 DXGI 개체의 이름을 지우는 함수.
// [In]object				DXGI 개체
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
	UNREFERENCED_PARAMETER(object);
#endif
}

#endif