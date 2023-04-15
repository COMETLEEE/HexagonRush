#pragma once
#include "Graphics_Interface.h"
#include "WindowInfo.h"

#include "SamplerState.h"

namespace GraphicsEngine
{
	class Mesh;

	class VertexShader;

	class PixelShader;

	class ShaderManager;

	class ResourceManager;

	class Renderer;

	/// <summary>
	/// Deferred Rendering 방식으로 화면을 그리는 그래픽스 엔진입니다.
	/// </summary>
	class Graphics_DX11 : public Graphics_Interface
	{
		Graphics_DX11(const Graphics_DX11& other) = delete;

		Graphics_DX11& operator =(const Graphics_DX11& other) = delete;

		Graphics_DX11(Graphics_DX11&& other) = delete;

	public:
		Graphics_DX11() = default;

		virtual ~Graphics_DX11() = default;

	public:
		// ---------------- Init And Release  ----------------
		virtual void Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight) override;

		virtual void Release() override;
		// ---------------- Init And Release  ----------------


		// ---------------- Rendering Series ----------------
		virtual void PostRenderingData_UI(std::shared_ptr<::RenderingData_UI>& renderingData) override;

		virtual void PostRenderingData_3D(std::shared_ptr<::RenderingData_3D>& renderingData) override;

		virtual void PostRenderingData_Particle(std::shared_ptr<::RenderingData_Particle>& renderingData) override;

		virtual void PostPerFrameData(std::shared_ptr<::PerFrameData>& perframeData) override;

		virtual void PostTextData(std::shared_ptr<::TextData>& textData) override;

		virtual void ExecuteRender() override;
		// ---------------- Rendering Series ----------------


		// ---------------- Event's Handling ----------------
		virtual void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;
		// ---------------- Event's Handling ----------------


		// ---------------- Resource Handling ----------------
		virtual const uint64& LoadTexture(const tstring& textureName, const tstring& path) override;

		virtual const uint64& LoadEnvironmentCubeMap(const tstring& envMapName, const tstring& path) override;

		virtual const uint64& LoadEnvironmentPanoramaMap(const tstring& envMapName, const tstring& path) override;

		virtual const uint64& LoadMesh(const tstring& meshName, const std::vector<StaticMeshVertex>& vertexVector, const std::vector<uint32>& indexVector, 
			bool isDynamic = false, bool isLineList = false) override;

		virtual const uint64& LoadMesh(const tstring& meshName, const std::vector<SkinnedMeshVertex>& vertexVector, const std::vector<uint32>& indexVector, 
			bool isDynamic = false, bool isLineList = false) override;
		
		virtual const uint64& LoadFont(const tstring& fontName, const tstring& path) override;

		virtual const Vector4&& MeasureDrawBounds(std::shared_ptr<::TextData> textData) override;
		// ---------------- Resource Handling ----------------

		// ---------------- Resource Release ----------------
		virtual void ReleaseTexture() override;
		// ---------------- Resource Release ----------------
	private:
		// ---------------- Managers ----------------
		std::shared_ptr<ResourceManager> _resourceManager;

		std::shared_ptr<ShaderManager> _shaderManager;

		std::shared_ptr<Renderer> _renderer;
		// ---------------- Managers ----------------

	public:
		inline static std::shared_ptr<Graphics_DX11> GetDX11() { return _graphics_DX11; }

	private:
		static std::shared_ptr<Graphics_DX11> _graphics_DX11;
	};
}