#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class ViewPort;

	class VertexShader;

	class PixelShader;

	class Mesh;

	class RenderTarget;

	class DepthStencilBuffer;

	constexpr uint32 BLUR_MAX_COUNT = 6;

	// 이 녀석을 사용하기 위해서는 Depth, Normal Map이 필요하다.
	// 따라서, 기존 Gaussian Blur와 그냥 클래스 단에서 구분을 만들겠음.
	class Pass_EdgePreserveBlur : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget> depth, std::shared_ptr<RenderTarget> normal,
			std::shared_ptr<RenderTarget> targetScreen, uint32 blurCount = 1);

	private:
		void BeginRender(std::shared_ptr<RenderTarget> targetScreen);

		void BeginRenderToHorizontal();

		void BeginRenderToOutput(const uint32& rounds);

		void EndRender();

	private:
		std::shared_ptr<ViewPort> _blurViewPort;

		std::shared_ptr<RenderTarget> _blurHorizontal;

		std::shared_ptr<RenderTarget> _blurOutput[BLUR_MAX_COUNT];

		std::shared_ptr<Mesh> _meshScreen;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psEdgePreserveBlurX;

		std::shared_ptr<PixelShader> _psEdgePreserveBlurY;

		friend class Renderer;
	};
}