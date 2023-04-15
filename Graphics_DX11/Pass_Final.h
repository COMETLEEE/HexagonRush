#pragma once
#include "Pass_Interface.h"
#include "Graphics_RenderingData_UI.h"
#include "Graphics_RenderingData_3D.h"

namespace GraphicsEngine
{
	struct CB_COLOR;
	struct CB_TONEMAPPING;
	class ViewPort;
	class RenderTarget;
	class SwapChain;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;
	class SpriteBatcher;
	class VertexShader;
	class PixelShader;
	class Mesh;

	class Pass_Debug;
	
	class Pass_Final : public Pass_Interface
	{
	public:
		virtual void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		virtual void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		virtual void Release() override;

	public:
		void Render(std::shared_ptr<RenderTarget> finalScreen, std::vector<std::shared_ptr<TextData>>& textDataQueue,
			std::vector<std::shared_ptr<RenderingData_UI>>& renderQueueUI, POSTPROCESS_OPTION& postProcessingOption);

	private:
		void BeginRender();

		// UI Data를 뿌린다.
		void ExecuteRenderQueueUI(std::vector<std::shared_ptr<RenderingData_UI>>& renderQueueUI);

		// Text들을 그린다. => 조금 특수하다. Back Buffer에 마지막으로 쏴줘야하는 것이다.
		void ExecuteTextDataQueue(std::vector<std::shared_ptr<TextData>>& textDataQueue);

		void EndRender();

	private:
		std::shared_ptr<CB_COLOR> _cbColor;

		std::shared_ptr<CB_TONEMAPPING> _cbToneMapping;

		std::shared_ptr<ViewPort> _screenViewPort;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psSample;

		std::shared_ptr<PixelShader> _psFinalHDRReinhard;

		std::shared_ptr<PixelShader> _psFinalHDRFilmic;

		std::shared_ptr<PixelShader> _psSampleColor;

		std::shared_ptr<Mesh> _meshScreen;

		std::shared_ptr<Mesh> _meshDynamicQuad;

		std::shared_ptr<SwapChain> _swapChain;

		std::shared_ptr<DepthStencilState> _depthStencilState;

		std::shared_ptr<BlendState> _blendState;

		std::shared_ptr<SpriteBatcher> _spriteBatcher;

		// Final Pass에 종속되는 Pass
	private:
		std::shared_ptr<Pass_Debug> _debugPass;
	};
}