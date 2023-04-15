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

	// 폐기 처분되었습니다. (사유 : 예쁘지 안흠)
	class Pass_SSAO : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;

		void Render(std::shared_ptr<RenderTarget> depth, std::shared_ptr<RenderTarget> normal);

	private:
		void BeginRender();

		void BuildRandomVectorMap();

		void EndRender();

	private:
		ComPtr<ID3D11ShaderResourceView> _randomVectorMap;
		
		std::shared_ptr<ViewPort> _screenViewPort;

		std::shared_ptr<RenderTarget> _ssaoOutput;

		std::shared_ptr<Mesh> _meshScreen;

		std::shared_ptr<VertexShader> _vsSSAO;

		std::shared_ptr<PixelShader> _psSSAO;

		friend class Renderer;
	};
}