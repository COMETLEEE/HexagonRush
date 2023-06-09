#include "pch.h"
#include "GraphicsManager.h"


namespace Muscle
{
	void GraphicsManager::Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// 앤진 내부에 캐싱하고 사용하자.
		_graphicsEngine = Graphics_Interface::Get();

		_graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);

		// 개체는 만들어놓고 이것으로 재활용하면서 던지자 ..
		_perframeData = std::make_shared<PerFrameData>();
	}

	void GraphicsManager::Render()
	{
		// ------------ Multi Thread 가능 영역
		// 1. Per-Frame Data (화면 갱신에 있어서 모든 작업에서 참조해야할 사항들)
		DispatchPerFrameData();

		// 2. Render Queue (3D)에 있는 데이터들을 날려 (Frustum Culling 할 수 있다.)
		DispatchRenderingData_3D();

		// 3. 2D UI Data들을 보낸다.
		DispatchRenderingData_UI();

		// 4. Particle Data들을 보낸다.
		DispatchRenderingData_Particle();

		// 5. Text Data에 있는 데이터들을 날려
		DispatchTextData();
		// ------------ Multi Thread 가능 영역
	}

	void GraphicsManager::ExecuteRender()
	{
		_graphicsEngine->ExecuteRender();
	}

	void GraphicsManager::OnResize()
	{
		RECT _rect;
		GetClientRect(MuscleEngine::GetInstance()->GetHWND(), &_rect);
		_graphicsEngine->OnResize(_rect.right, _rect.bottom);
	}

	GraphicsManager::~GraphicsManager()
	{
		_graphicsEngine->Release();
	}

	void GraphicsManager::DispatchRenderingData_Particle()
	{
		while (!_renderQueueParticle.empty())
		{
			std::shared_ptr<RenderingData_Particle> renderingData = _renderQueueParticle.front(); _renderQueueParticle.pop();

			_graphicsEngine->PostRenderingData_Particle(renderingData);
		}
	}

	void GraphicsManager::DispatchRenderingData_UI()
	{
		while (!_renderQueueUI.empty())
		{
			std::shared_ptr<RenderingData_UI> renderingData = _renderQueueUI.front(); _renderQueueUI.pop();

			_graphicsEngine->PostRenderingData_UI(renderingData);
		}
	}

	void GraphicsManager::DispatchRenderingData_3D()
	{
		while (!_renderQueue.empty())
		{
			std::shared_ptr<RenderingData_3D> renderingData = _renderQueue.front(); _renderQueue.pop();

			_graphicsEngine->PostRenderingData_3D(renderingData);
		}
	}

	void GraphicsManager::DispatchPerFrameData()
	{

		if (MuscleEngine::GetInstance()->GetMainCamera() != nullptr)
		{
			memcpy(_perframeData->_cameraInfo.get(), MuscleEngine::GetInstance()->GetMainCamera()->_cameraInfo.get(), sizeof(CameraInfo));

			//std::shared_ptr<BoundingFrustum> _viewFrustum = _mainCamera->GetViewFrustum();

			//memcpy(_perframeData->_ssaoInfo->_cameraFrustumCorner, _viewFrustum->_frustumCorner,
			//	sizeof(Vector4) * 4);
		}

		while (!_dirLightInfoQueue.empty())
		{
			std::shared_ptr<DirectionalLightInfo> dirInfo = _dirLightInfoQueue.front(); _dirLightInfoQueue.pop();

			_perframeData->_directionalLightInfos.emplace_back(dirInfo);
		}

		while (!_pointLightInfoQueue.empty())
		{
			std::shared_ptr<PointLightInfo> pointLightInfo = _pointLightInfoQueue.front(); _pointLightInfoQueue.pop();

			_perframeData->_pointLightInfos.emplace_back(pointLightInfo);
		}

		while (!_spotLightInfoQueue.empty())
		{
			std::shared_ptr<SpotLightInfo> spotLightInfo = _spotLightInfoQueue.front(); _spotLightInfoQueue.pop();

			_perframeData->_spotLightInfos.emplace_back(spotLightInfo);
		}

		// 델타 타임 보내주는 기능을 추가한다. (그래픽스 엔진 내부에서의 물리 시뮬레이션에 필요하다.)
		// 게임 델타 타임을 던져주는구먼 !!
		_perframeData->_deltaTime = CTime::GetGameDeltaTime();			// 모든 것들이 이걸로 업데이트된다.

		_graphicsEngine->PostPerFrameData(_perframeData);

		MuscleEngine::GetInstance()->GetDebugManager()->PostPerFrameData(_perframeData);
	}

	void GraphicsManager::DispatchTextData()
	{
		while (!_textDataQueue.empty())
		{
			std::shared_ptr<TextData> textData = _textDataQueue.front(); _textDataQueue.pop();

			_graphicsEngine->PostTextData(textData);
		}
	}

	void GraphicsManager::Release()
	{
		_graphicsEngine->Release();
	}

	void GraphicsManager::TextureRelease()
	{
		_graphicsEngine->ReleaseTexture();
	}

	void GraphicsManager::PostRenderingData_UI(std::shared_ptr<RenderingData_UI>& renderingData)
	{
		_renderQueueUI.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_3D(std::shared_ptr<RenderingData_3D>& renderingData)
	{
		_renderQueue.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_Particle(std::shared_ptr<RenderingData_Particle>& renderingData)
	{
		_renderQueueParticle.emplace(renderingData);
	}

	void GraphicsManager::PostDirectionalLightInfo(std::shared_ptr<DirectionalLightInfo>& dirLightInfo)
	{
		_dirLightInfoQueue.emplace(dirLightInfo);
	}

	void GraphicsManager::PostPointLightInfo(std::shared_ptr<PointLightInfo>& pointLightInfo)
	{
		_pointLightInfoQueue.emplace(pointLightInfo);
	}

	void GraphicsManager::PostSpotLightInfo(std::shared_ptr<SpotLightInfo>& spotLightInfo)
	{
		_spotLightInfoQueue.emplace(spotLightInfo);
	}

	void GraphicsManager::PostTextData(std::shared_ptr<TextData>& textData)
	{
		_textDataQueue.emplace(textData);
	}

	void GraphicsManager::SetBloom(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_BLOOM)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_BLOOM));
	}

	void GraphicsManager::SetSSAO(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_SSAO)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_SSAO));
	}

	void GraphicsManager::SetVignetting(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_VIGNETTING)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_VIGNETTING));
	}

	void GraphicsManager::SetCameraBlur(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR));
	}

	void GraphicsManager::SetFullSceneBlur(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_GAUSSIAN_BLUR)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_GAUSSIAN_BLUR));
	}

	void GraphicsManager::SetFXAA(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA));
	}

	void GraphicsManager::SetDebugPanel(bool value)
	{
		value ? _perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) | static_cast<uint32>(POSTPROCESS_OPTION::ON_DEBUG_PANEL)) :
			_perframeData->_postProcessOption = static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(_perframeData->_postProcessOption) ^ static_cast<uint32>(POSTPROCESS_OPTION::ON_DEBUG_PANEL));
	}
}