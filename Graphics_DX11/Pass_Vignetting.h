#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class ViewPort;

	class RenderTarget;

	class Mesh;

	class VertexShader;

	class PixelShader;

	class Pass_Vignetting : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget> targetScreen);

		inline std::shared_ptr<RenderTarget>& GetOutput() { return _vignettingOutput; }
	
	public:
		std::shared_ptr<RenderTarget> _vignettingOutput;

		std::shared_ptr<ViewPort> _screenViewPort;

		std::shared_ptr<Mesh> _screenMesh;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psVignetting;
	};
}