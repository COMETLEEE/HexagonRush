#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class ViewPort;

	class RenderTarget;

	class DepthStencilBuffer;

	class DepthStencilBufferArray;

	class VertexShader;

	class PixelShader;

	class Mesh;

	struct CB_DIRLIGHTVIEWPROJ;

	struct CB_LIGHTVIEWPROJ;

	struct CB_PERTEXTURE;

	/// <summary>
	/// G-Buffer, SSAO, Shadow 등을 종합하여 빛 연산을 처리하는 패스.
	/// </summary>
	class Pass_Lighting : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void BeginRender();

		// TO DO : 다수 개의, 동적으로 다른 Shadow Map 갯수를 어떻게 받을까
		void Render(const std::vector<std::shared_ptr<RenderTarget>>& gBuffers, std::shared_ptr<DepthStencilBuffer> shadowMap,
			std::shared_ptr<DepthStencilBufferArray> cascadeShadowMap,
			std::shared_ptr<RenderTarget> ssaoMap, CB_LIGHTVIEWPROJ* cbLightViewProj, CB_DIRLIGHTVIEWPROJ* cbDirLightViewProj,
			std::shared_ptr<RenderTarget> emissive);

		void EndRender();

		inline std::shared_ptr<RenderTarget> GetOutput() { return _lightingOutput; }

	private:
		std::shared_ptr<CB_PERTEXTURE> _cbPerTexture;

		std::shared_ptr<ViewPort> _screenSizeViewPort;

		std::shared_ptr<RenderTarget> _lightingOutput;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psLighting;

		std::shared_ptr<Mesh> _meshScreen;
	};
}