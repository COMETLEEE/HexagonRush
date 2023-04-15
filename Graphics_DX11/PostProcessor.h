#pragma once
#include "Graphics_PerFrameData.h"

#include "Graphics_RenderingData_UI.h"
#include "Graphics_TextData.h"

namespace GraphicsEngine
{
	class RenderTarget;

	// Post Processing Pass
	class Pass_CameraMotionBlur;

	class Pass_GaussianBlur;

	class Pass_Bloom;

	class Pass_Vignetting;

	class Pass_FXAA;

	class Pass_Final;
	// Post Processing Pass

	// Source Pass
	class Pass_Lighting;

	class Pass_Deferred;
	// Source Pass

	/// <summary>
	/// 포스트 프로세싱을 수행하고 마지막에 스왑체인으로 플리핑까지 담당하는 녀석
	/// </summary>
	class PostProcessor
	{
	public:
		PostProcessor() = default;

		~PostProcessor() = default;

		PostProcessor(const PostProcessor& other) = delete;

		PostProcessor& operator= (const PostProcessor& other) = delete;

		PostProcessor(PostProcessor&& other) = delete;

	private:
		// Post Processing Pass
		std::shared_ptr<Pass_CameraMotionBlur> _cameraMotionBlurPass;

		std::shared_ptr<Pass_GaussianBlur> _gaussianBlurPass;

		std::shared_ptr<Pass_Bloom> _bloomPass;

		std::shared_ptr<Pass_Vignetting> _vignettingPass;

		std::shared_ptr<Pass_FXAA> _fxaaPass;

		std::shared_ptr<Pass_Final> _finalPass;
		// Post Processing Pass

		// Source Pass
		std::shared_ptr<Pass_Lighting> _lightingPass;			// Source : Scene Output

		std::shared_ptr<Pass_Deferred> _deferredPass;			// Source : G-Buffers
		// Source Pass

		std::shared_ptr<RenderTarget> _prevPassOutput;

	public:
		// 여러가지 각종 포스트 프로세싱에 관련된 패스들을 초기화하고 가지고 있는다.
		void Initialize(std::shared_ptr<Pass_Lighting> lightingPass, std::shared_ptr<Pass_Deferred> deferredPass,
			const uint32& screenWidth, const uint32& screenHeight);

		void OnResize(const uint32& screenWidth, const uint32& screenHeight);

		// 각종 포스트 프로세싱 옵션에 따라서 적용하고 스왑 체인을 플리핑한다.
		void Render(POSTPROCESS_OPTION& postProcessingOption,
			std::vector<std::shared_ptr<::TextData>>& textDataQueue, 
			std::vector<std::shared_ptr<::RenderingData_UI>>& renderQueueUI);

		void Release();

		// 여러 선택적인 기능이 들어갈 수 있다. 오브젝트 아이디 별로 블러, 블룸 등 ..

		friend class Renderer;
	};
}