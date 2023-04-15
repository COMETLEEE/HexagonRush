#include "pch.h"
#include "PostProcessor.h"

#include "Renderer.h"
#include "Pass_Deferred.h"
#include "Pass_Lighting.h"

#include "Pass_GaussianBlur.h"
#include "Pass_CameraMotionBlur.h"
#include "Pass_Bloom.h"
#include "Pass_Vignetting.h"
#include "Pass_FXAA.h"
#include "Pass_Final.h"

namespace GraphicsEngine
{
	void GraphicsEngine::PostProcessor::Initialize(std::shared_ptr<Pass_Lighting> lightingPass, std::shared_ptr<Pass_Deferred> deferredPass,
		const uint32& screenWidth, const uint32& screenHeight)
	{
		_lightingPass = lightingPass;

		_deferredPass = deferredPass;

		_gaussianBlurPass = std::make_shared<Pass_GaussianBlur>();
		_gaussianBlurPass->Initialize(screenWidth, screenHeight);

		_cameraMotionBlurPass = std::make_shared<Pass_CameraMotionBlur>();
		_cameraMotionBlurPass->Initialize(screenWidth, screenHeight);

		_bloomPass = std::make_shared<Pass_Bloom>();
		_bloomPass->Initialize(screenWidth, screenHeight);
		_bloomPass->_blurPass = _gaussianBlurPass;

		_vignettingPass = std::make_shared<Pass_Vignetting>();
		_vignettingPass->Initialize(screenWidth, screenHeight);

		_fxaaPass = std::make_shared<Pass_FXAA>();
		_fxaaPass->Initialize(screenWidth, screenHeight);

		_finalPass = std::make_shared<Pass_Final>();
		_finalPass->Initialize(screenWidth, screenHeight);
	}

	void GraphicsEngine::PostProcessor::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		_cameraMotionBlurPass->OnResize(screenWidth, screenHeight);

		_gaussianBlurPass->OnResize(screenWidth, screenHeight);

		_bloomPass->OnResize(screenWidth, screenHeight);

		_vignettingPass->OnResize(screenWidth, screenHeight);

		_fxaaPass->OnResize(screenWidth, screenHeight);

		_finalPass->OnResize(screenWidth, screenHeight);
	}

	void GraphicsEngine::PostProcessor::Render(POSTPROCESS_OPTION& postProcessingOption,
		std::vector<std::shared_ptr<::TextData>>& textDataQueue,
		std::vector<std::shared_ptr<::RenderingData_UI>>& renderQueueUI)
	{
		// Post Process
		if ((static_cast<uint32>(postProcessingOption) & static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR))
			== static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR))
		{
			_cameraMotionBlurPass->Render(_deferredPass->GetPosition(), _lightingPass->GetOutput());

			_prevPassOutput = _cameraMotionBlurPass->GetOutput();
		}

		if ((static_cast<uint32>(postProcessingOption) & static_cast<uint32>(POSTPROCESS_OPTION::ON_BLOOM))
			== static_cast<uint32>(POSTPROCESS_OPTION::ON_BLOOM))
		{
			_bloomPass->Render(_prevPassOutput);

			_prevPassOutput = _bloomPass->GetOutput();
		}

		if ((static_cast<uint32>(postProcessingOption) & static_cast<uint32>(POSTPROCESS_OPTION::ON_GAUSSIAN_BLUR))
			== static_cast<uint32>(POSTPROCESS_OPTION::ON_GAUSSIAN_BLUR))
		{
			_gaussianBlurPass->Render(_prevPassOutput);

			_prevPassOutput = _gaussianBlurPass->GetOutput();
		}

		if ((static_cast<uint32>(postProcessingOption) & static_cast<uint32>(POSTPROCESS_OPTION::ON_VIGNETTING))
			== static_cast<uint32>(POSTPROCESS_OPTION::ON_VIGNETTING))
		{
			_vignettingPass->Render(_prevPassOutput);

			_prevPassOutput = _vignettingPass->GetOutput();
		}

		if ((static_cast<uint32>(postProcessingOption) & static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA))
			== static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA))
		{
			_fxaaPass->Render(_prevPassOutput);

			_prevPassOutput = _fxaaPass->GetOutput();
		}
		// Post Process

		// Tone Mapping And Text, UI µî .. ·»´õ¸µ ÈÄ Present
		_finalPass->Render(_prevPassOutput, textDataQueue, renderQueueUI, postProcessingOption);
	}

	void GraphicsEngine::PostProcessor::Release()
	{
		_prevPassOutput.reset();

		_lightingPass.reset();

		_deferredPass.reset();

		_cameraMotionBlurPass->Release();

		_gaussianBlurPass->Release();

		_bloomPass->Release();

		_vignettingPass->Release();

		_fxaaPass->Release();

		_finalPass->Release();
	}
}