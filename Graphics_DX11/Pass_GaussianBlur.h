#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class Mesh;

	class RenderTarget;

	class DepthStencilBuffer;

	class VertexShader;

	class PixelShader;

	class Pass_GaussianBlur : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget>& targetScreen);

	public:
		inline std::shared_ptr<RenderTarget> GetOutput() { return _blurOutput; }

	private:
		void BeginRender(std::shared_ptr<RenderTarget>& targetScreen);

		void EndRender();

		void BeginRenderToHorizontal();

		void BeginRenderToOutput();

		std::shared_ptr<RenderTarget> _blurHorizontal;

		std::shared_ptr<RenderTarget> _blurOutput;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psBlurX;

		std::shared_ptr<PixelShader> _psBlurY;

		std::shared_ptr<Mesh> _meshScreen;

		friend class Renderer;

		friend class Pass_Bloom;
	};
}