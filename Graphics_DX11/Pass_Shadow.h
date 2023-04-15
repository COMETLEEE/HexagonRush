#pragma once
#include "Pass_Interface.h"
#include "Graphics_RenderingData_3D.h"

namespace GraphicsEngine
{
	class ViewPort;

	class RenderTarget;

	class DepthStencilBuffer;

	class DepthStencilBufferArray;

	class RasterizerState;

	class VertexShader;

	class PixelShader;

	class GeometryShader;

	struct CB_DIRLIGHTVIEWPROJ;

	struct CB_LIGHTVIEWPROJ;

	struct CB_PEROBJECT;

	struct PerFrameConstantBuffer;

	class Pass_Shadow : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<PerFrameConstantBuffer> perframeCB, std::vector<std::shared_ptr<RenderingData_3D>>& renderQueue);

	private:
		void BeginRender(std::shared_ptr<PerFrameConstantBuffer> perframeCB);

		void DrawShadowMap(std::vector<std::shared_ptr<RenderingData_3D>>& renderQueue);

		void DrawAtCascadeShadowMap(std::vector<std::shared_ptr<RenderingData_3D>>& renderQueue);

		void EndRender();

		void ExecuteShadowForStaticMeshObject(std::shared_ptr<RenderingData_3D> renderingData);
		
		void ExecuteShadowForSkinnedMeshObject(std::shared_ptr<RenderingData_3D> renderingData);

		void ExecuteCSForStaticMeshObject(std::shared_ptr<RenderingData_3D> renderingData);

		void ExecuteCSForSkinnedMeshObject(std::shared_ptr<RenderingData_3D> renderingData);

		void CreateCascadeFrustums(std::shared_ptr<PerFrameConstantBuffer> perframeCB);

	private:
		std::shared_ptr<CB_LIGHTVIEWPROJ> _cbLightViewProjs;

		std::shared_ptr<CB_PEROBJECT> _cbPerObject;

		std::shared_ptr<ViewPort> _screenHalfViewPort;

		std::shared_ptr<RasterizerState> _rasterizerState;

		std::shared_ptr<VertexShader> _vsShadowStatic;

		std::shared_ptr<VertexShader> _vsShadowSkinned;

		// Depth 자체가 Output (이건 스포토 라이토)
		std::shared_ptr<DepthStencilBuffer> _shadowDepth;
		
		// TEST : Cascades Shadow Mapping
		std::shared_ptr<CB_DIRLIGHTVIEWPROJ> _cbDirLightViewProj;

		std::shared_ptr<DepthStencilBufferArray> _cascadeShadowMap;

		std::shared_ptr<VertexShader> _vsShadowCascadeStatic;

		std::shared_ptr<VertexShader> _vsShadowCascadeSkinned;

		std::shared_ptr<GeometryShader> _gsShadowCascadeTriangle;

		std::shared_ptr<GeometryShader> _gsShadowCascadeLine;
		// TEST : Cascades Shadow Mapping

		friend class Renderer;
	};
}