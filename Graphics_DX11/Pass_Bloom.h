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

	class Pass_Sampling;

	class Pass_GaussianBlur;

	// 들어오는 인풋이 최종 화면이라는 것을 가정하에 만듬.
	// 아니라면 들어오는 스크린 사이즈에 맞게 OnResize 해줘야함.
	class Pass_Bloom : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget> targetScreen);

		inline std::shared_ptr<RenderTarget> GetOutput() { return _bloomOutput; }

	private:
		// 블룸 커브를 활용해서 밝은 부분을 찾습니다.
		void BloomSampling(std::shared_ptr<RenderTarget> targetScreen);

		void Blur16XAddUp8X();

		void Blur8XAddUp4X();

		void Blur4XAddUp2X();

		void Blur2XAddUpOrigin(std::shared_ptr<RenderTarget> targetScreen);

		void EndRender();

	private:
		std::shared_ptr<Pass_Sampling> _samplingPass;

		std::shared_ptr<Pass_GaussianBlur> _blurPass;

		std::shared_ptr<ViewPort> _screenSizeViewPort;

		std::shared_ptr<RenderTarget> _bloomSampled;

		std::shared_ptr<RenderTarget> _bloomOutput;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psBloomSampling1;

		std::shared_ptr<PixelShader> _psBloomSampling2;

		std::shared_ptr<PixelShader> _psBloomSampling3;

		std::shared_ptr<PixelShader> _psBloomAddUp;

		std::shared_ptr<Mesh> _meshScreen;

		// 합성을 위해서 8X, 4X, 2X, 1X RenderTarget Depth ViewPort 필요함
		std::shared_ptr<ViewPort> _octarViewPort;

		std::shared_ptr<RenderTarget> _octarTarget;

		std::shared_ptr<ViewPort> _quarterViewPort;

		std::shared_ptr<RenderTarget> _quarterTarget;

		std::shared_ptr<ViewPort> _halfViewPort;

		std::shared_ptr<RenderTarget> _halfTarget;

		friend class Renderer;

		friend class PostProcessor;
	};
}

// 1. Bloom Target을 받는다.
// 2. Bloom Target에서 밝아지게 해야할 부분을 찾는다. (다운 샘플 하고 해도 된다.)
// 3. 2의 결과물을 가지고 Down Sampling을 진행한다. (2X, 4X, 8X, 16X)
// 4. 3의 결과물에서 16X를 Blur 후 8X에 합성한다.
// 5. 합성된 8X를 블러한 후 4X에 합성한다.
// 6. 합성된 4X를 블러한 후 2X에 합성한다.
// 7. 마지막으로 2X를 블러하고, 원본 타겟과 합성한다.
// 8. 톤매핑으로 마무리