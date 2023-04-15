#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class ViewPort;

	class RenderTarget;

	class DepthStencilBuffer;

	class VertexShader;

	class PixelShader;

	class Mesh;

	class Pass_FXAA : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget> targetScreen);

		inline std::shared_ptr<RenderTarget> GetOutput() { return _fxaaOutput; }

	private:
		std::shared_ptr<ViewPort> _screenViewPort;

		std::shared_ptr<RenderTarget> _fxaaOutput;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psFXAA;

		std::shared_ptr<Mesh> _meshScreen;

		friend class Renderer;
	};
}