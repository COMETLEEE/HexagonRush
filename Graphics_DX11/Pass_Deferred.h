#pragma once
#include "Pass_Interface.h"
#include "Graphics_RenderingData_3D.h"

namespace GraphicsEngine
{
	class ViewPort;

	class RenderTarget;

	class DepthStencilBuffer;

	class RasterizerState;

	class VertexShader;

	class PixelShader;

	class Mesh;

	struct CB_PEROBJECT;

	struct CB_LIGHTVIEWPROJ;

	constexpr uint32 OBJECT_ID_RTV_INDEX = 4;

	constexpr uint32 DEFERRED_RENDERTARGET_COUNT = 6;

	constexpr uint32 MAX_SHADOW_MAP_COUNT = 1;

	/// <summary>
	/// 렌더링의 시작. 씬 화면의 G-Buffer들을 뽑아낸다.
	/// 여기에 나오는 Output, Depth, ID 등 재활용 할 수 있는 버퍼가 많다.
	/// </summary>
	class Pass_Deferred : public Pass_Interface
	{
	public:
		virtual void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		virtual void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		virtual void Release() override;
		
		void Render(std::vector<std::shared_ptr<RenderingData_3D>>& renderQueue);

		void BeginRender();

		void ExecuteForStaticMeshObject(std::shared_ptr<RenderingData_3D> renderingData);
		
		void ExecuteForSkinnedMeshObject(std::shared_ptr<RenderingData_3D> renderingData);

		void EndRender();

	public:
		inline std::shared_ptr<RenderTarget>& GetDepth() { return _gBuffers[0]; }			// Depth, Metallic, Roughness, AO

		inline std::shared_ptr<RenderTarget>& GetNormal() { return _gBuffers[1]; }

		inline std::shared_ptr<RenderTarget>& GetPosition() { return _gBuffers[2]; }

		inline std::shared_ptr<RenderTarget>& GetAlbedo() { return _gBuffers[3]; }

		inline std::shared_ptr<RenderTarget>& GetEmissive() { return _gBuffers[4]; }

		inline std::shared_ptr<RenderTarget>& GetObjectID() { return _gBuffers[5]; }

	private:
		std::shared_ptr<CB_PEROBJECT> _cbPerObject;

		std::shared_ptr<ViewPort> _screenSizeViewPort;

		std::vector<std::shared_ptr<RenderTarget>> _gBuffers;

		ComPtr<ID3D11RenderTargetView> _gBufferViews[DEFERRED_RENDERTARGET_COUNT];

		std::shared_ptr<DepthStencilBuffer> _deferredDepth;

		std::shared_ptr<RasterizerState> _rasterizerState;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psDeferred;

		std::shared_ptr<Mesh> _meshScreen;

		friend class Renderer;
	};
}