#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class RenderTarget;

	class DepthStencilBuffer;

	class VertexShader;

	class PixelShader;

	class Mesh;

	class ViewPort;

	// 근데 이 녀석같은 경우에는 블러링 되는 방향 외에는 .. 고화질로 보여야하는데 ?
	// 따라서 다운 샘플링을 하지 않겠습니다.
	class Pass_CameraMotionBlur : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

	public:
		// Position Map, Target Map (보통 최종 렌더링 결과물)
		void Render(std::shared_ptr<RenderTarget> position, std::shared_ptr<RenderTarget> target);

		inline std::shared_ptr<RenderTarget> GetOutput() { return _blurOutput; }

	private:
		void EndRender();

		std::shared_ptr<ViewPort> _screenSizeViewPort;

		std::shared_ptr<RenderTarget> _blurOutput;

		std::shared_ptr<Mesh> _meshScreen;

		std::shared_ptr<VertexShader> _vsQuad;
		
		std::shared_ptr<PixelShader> _psCameraMotionBlur;

		friend class Renderer;
	};
}