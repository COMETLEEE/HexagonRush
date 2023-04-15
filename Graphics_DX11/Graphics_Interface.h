#pragma once

// 이 인터페이스를 사용하려면 이것들을 다 알아야합니다.
#include <memory>
#include <windows.h>

#include "Graphics_RenderingData_UI.h"
#include "Graphics_RenderingData_3D.h"
#include "Graphics_RenderingData_Particle.h"
#include "Vertex_def.h"

#ifdef GRAPHICSDX11_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif
class ResourceManager;

class Graphics_Interface
{
	Graphics_Interface(const Graphics_Interface& other) = delete;

	Graphics_Interface& operator =(const Graphics_Interface& other) = delete;

	Graphics_Interface(Graphics_Interface&& other) = delete;

public:
	// ---------------- Interface  ----------------
	static GRAPHICS_API std::shared_ptr<Graphics_Interface> Get();

	// ---------------- Init And Release  ----------------
	virtual GRAPHICS_API void Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight) abstract;

	virtual GRAPHICS_API void Release() abstract;
	// ---------------- Init And Release  ----------------


	// ---------------- Rendering Series ----------------
	virtual GRAPHICS_API void PostRenderingData_UI(std::shared_ptr<RenderingData_UI>& renderingData) abstract;

	virtual GRAPHICS_API void PostRenderingData_3D(std::shared_ptr<RenderingData_3D>& renderingData) abstract;

	virtual GRAPHICS_API void PostRenderingData_Particle(std::shared_ptr<RenderingData_Particle>& renderingData) abstract;

	virtual GRAPHICS_API void PostPerFrameData(std::shared_ptr<PerFrameData>& perframeData) abstract;

	virtual GRAPHICS_API void PostTextData(std::shared_ptr<TextData>& textData) abstract;

	virtual GRAPHICS_API void ExecuteRender() abstract;
	// ---------------- Rendering Series ----------------


	// ---------------- Event's Handling ----------------
	virtual GRAPHICS_API void OnResize(const uint32& screenWidth, const uint32& screenHeight) abstract;
	// ---------------- Event's Handling ----------------

	// ---------------- Resource Handling ----------------
	virtual GRAPHICS_API const uint64& LoadTexture(const tstring& textureName, const tstring& path) abstract;

	virtual GRAPHICS_API const uint64& LoadMesh(const tstring& meshName, const std::vector<StaticMeshVertex>& vertexVector, 
		const std::vector<uint32>& indexVector, bool isDynamic = false, bool isLineList = false) abstract;

	virtual GRAPHICS_API const uint64& LoadMesh(const tstring& meshName, const std::vector<SkinnedMeshVertex>& vertexVector, 
		const std::vector<uint32>& indexVector, bool isDynamic = false, bool isLineList = false) abstract;

	virtual GRAPHICS_API const uint64& LoadFont(const tstring& fontName, const tstring& path) abstract;
		
	virtual GRAPHICS_API const uint64& LoadEnvironmentCubeMap(const tstring& envMapName, const tstring& path) abstract;

	virtual GRAPHICS_API const uint64& LoadEnvironmentPanoramaMap(const tstring& envMapName, const tstring& path) abstract;

	virtual GRAPHICS_API const Vector4&& MeasureDrawBounds(std::shared_ptr<::TextData> textData) abstract;
	// ---------------- Resource Handling ----------------

	// ---------------- Resource Manager Get ----------------
	virtual GRAPHICS_API void ReleaseTexture() abstract;
	// ---------------- Resource Manager Get ----------------

protected:
	Graphics_Interface() = default;

	virtual ~Graphics_Interface() = default;

private:
	static std::shared_ptr<Graphics_Interface> _graphics;
};