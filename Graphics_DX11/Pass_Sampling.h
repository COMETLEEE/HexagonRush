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

	constexpr uint32 SAMPLING_OPTION_COUNT = 4;

	enum class SAMPLING_OPTION
	{
		Half = 0b0001,
		Quarter = 0b0010,
		Octar = 0b0100,
		Sixteen = 0b1000,
		ALL = Half | Quarter | Octar | Sixteen,
	};

	// 얘는 좀 특수한데 .. 한 번에 Down Sample을 여러 개 넘겨주고 싶을 수 있음. (2, 4, 8, 16 등 ..)
	// => 샘플링 비율에 맞춰서 다 가지고 있자. 어차피 그래픽 작업은 순차적이다 ..!
	class Pass_Sampling : public Pass_Interface
	{
	public:

		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget> target, SAMPLING_OPTION option = SAMPLING_OPTION::ALL);
		
	private:
		std::vector<std::function<void(Pass_Sampling&, std::shared_ptr<RenderTarget>)>> _workList;

		void DrawHalf(std::shared_ptr<RenderTarget> target);

		void DrawQuarter(std::shared_ptr<RenderTarget> target);

		void DrawOctar(std::shared_ptr<RenderTarget> target);

		void DrawSixteen(std::shared_ptr<RenderTarget> target);

	private:
		std::shared_ptr<ViewPort> _halfViewPort;

		std::shared_ptr<RenderTarget> _sampleHalf;

		std::shared_ptr<ViewPort> _quarterViewPort;

		std::shared_ptr<RenderTarget> _sampleQuarter;

		std::shared_ptr<ViewPort> _octarViewPort;

		std::shared_ptr<RenderTarget> _sampleOctar;

		std::shared_ptr<ViewPort> _sixteenViewPort;

		std::shared_ptr<RenderTarget> _sampleSixteen;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psSample;

		std::shared_ptr<Mesh> _meshScreen;

		friend class Renderer;

		friend class Pass_Bloom;
	};
}