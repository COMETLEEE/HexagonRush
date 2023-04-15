#include "pch.h"
#include "Pass_Final.h"

#include "Pass_Debug.h"

#include "CB_Structure.h"

#include "Renderer.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ViewPort.h"
#include "Mesh.h"
#include "Font.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace GraphicsEngine
{
	void Pass_Final::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);

		_cbColor = std::make_shared<CB_COLOR>();

		_cbToneMapping = std::make_shared<CB_TONEMAPPING>();

		_cbToneMapping->_exposure = 1.5f;

		_screenViewPort = std::make_shared<ViewPort>();

		_screenViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psSample = ShaderManager::Get()->GetPS(TEXT("PS_Sample"));

		_psSampleColor = ShaderManager::Get()->GetPS(TEXT("PS_Sample_Color"));

		_psFinalHDRReinhard = ShaderManager::Get()->GetPS(TEXT("PS_ToneMapping_Reinhard"));

		_psFinalHDRFilmic = ShaderManager::Get()->GetPS(TEXT("PS_ToneMapping_Filmic"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);

		_meshDynamicQuad = ResourceManager::Get()->GetMesh(DYNAMIC_QUAD_MESH);

		_swapChain = Renderer::Get()->GetSwapChain();

		_depthStencilState = Renderer::Get()->GetDepthStencilState();

		_blendState = Renderer::Get()->GetBlendState();

		_spriteBatcher = Renderer::Get()->GetSpriteBatcher();

		// Final Pass, 백 버퍼에 그대로 쏘는 녀석들을 담당하는 패스들은 여기에 종속됨.
		_debugPass = std::make_shared<Pass_Debug>();

		_debugPass->Initialize(screenWidth, screenHeight);
	}

	void Pass_Final::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		_screenViewPort->OnResize(screenWidth, screenHeight);

		_swapChain->OnResize(_device, _deviceContext, screenWidth, screenHeight);

		// Pass OnResize
		_debugPass->OnResize(screenWidth, screenHeight);
	}

	void Pass_Final::Release()
	{
		_screenViewPort->Release();

		_vsQuad.reset();

		_psSample.reset();

		_meshScreen.reset();

		_meshDynamicQuad.reset();

		_swapChain.reset();

		_depthStencilState.reset();

		_blendState.reset();

		_spriteBatcher.reset();

		_cbToneMapping.reset();

		_cbColor.reset();

		// Pass Release
		_debugPass->Release();

		__super::Release();
	}

	void Pass_Final::Render(std::shared_ptr<RenderTarget> finalScreen, std::vector<std::shared_ptr<TextData>>& textDataQueue, 
		std::vector<std::shared_ptr<RenderingData_UI>>& renderQueueUI, POSTPROCESS_OPTION& postProcessingOption)
	{
		BeginRender();

		_vsQuad->Update();

		/*_psSample->UpdateShaderResource(finalScreen->GetSRV().Get(), SRV_REGISTER::t0);

		_psSample->Update();*/

		// Tone Mapping
		_psFinalHDRReinhard->UpdateConstantBuffer(_cbToneMapping.get(), CBV_REGISTER::b1);

		_psFinalHDRReinhard->UpdateShaderResource(finalScreen->GetSRV().Get(), SRV_REGISTER::t0);

		_psFinalHDRReinhard->Update();

		/*_psFinalHDRFilmic->UpdateConstantBuffer(_cbToneMapping.get(), CBV_REGISTER::b1);

		_psFinalHDRFilmic->UpdateShaderResource(finalScreen->GetSRV().Get(), SRV_REGISTER::t0);

		_psFinalHDRFilmic->Update();*/
		
		_meshScreen->Update();

		// UI 데이터를 뿌림. 뿌리기 전 소트 오더에 대한 정렬 후 그림을 그려준다.
		ExecuteRenderQueueUI(renderQueueUI);

		// Text Data
		ExecuteTextDataQueue(textDataQueue);

		// Debug Data
		if ((static_cast<uint32>(postProcessingOption) & static_cast<uint32>(POSTPROCESS_OPTION::ON_DEBUG_PANEL))
			== static_cast<uint32>(POSTPROCESS_OPTION::ON_DEBUG_PANEL))
			_debugPass->Render();

		EndRender();
	}

	void Pass_Final::BeginRender()
	{
		_screenViewPort->SetViewPort(_deviceContext);

		_swapChain->BeginRenderToBackBuffer(_deviceContext);
	}

	void Pass_Final::ExecuteRenderQueueUI(std::vector<std::shared_ptr<RenderingData_UI>>& renderQueueUI)
	{
		// 렌더 큐 정렬용 => 소트 오더가 작으면 제일 늦게 그린다 => 가장 앞에 보인다
		static auto SortRenderQueueUI = [](const std::shared_ptr<RenderingData_UI>& first, const std::shared_ptr<RenderingData_UI>& second)
		{
			return first->_drawInfo->_sortOrder > second->_drawInfo->_sortOrder;
		};

		// 뿌리기 전 큐에 대한 정렬을 실시한다.
		// 2D 쌩 이미지를 렌더링하는 것이라서 정해진 그릴 순서가 중요하다.
		std::sort(renderQueueUI.begin(), renderQueueUI.end(), SortRenderQueueUI);

		for (auto& ui : renderQueueUI)
		{
			std::shared_ptr<DrawInfo> drawInfo = ui->_drawInfo;

			std::shared_ptr<NDCInfo> posInfo = ui->_ndcInfo;

			// 알파 블렌드를 사용하는 UI라면 블렌드 스테이트를 변경해준다.
			if (ui->_drawInfo->_useAlphaBlend)
				_blendState->SetUIBlendState(_deviceContext);

			Vector2 leftTop = posInfo->_leftTop;

			Vector2 rightBottom = posInfo->_rightBottom;

			// 해당 NDC 좌표에 맞게 Dynamic Quad를 업데이트
			static std::vector<StaticMeshVertex> vertices(6);

			// 좌 하단
			vertices[0]._position = Vector3(leftTop.x, rightBottom.y, 0.f);
			vertices[0]._uv = Vector2(0.f, 1.f);

			// 좌 상단
			vertices[1]._position = Vector3(leftTop.x, leftTop.y, 0.f);
			vertices[1]._uv = Vector2(0.f, 0.f);

			// 우 상단
			vertices[2]._position = Vector3(rightBottom.x, leftTop.y, 0.f);
			vertices[2]._uv = Vector2(1.f, 0.f);

			// 좌 하단
			vertices[3]._position = Vector3(leftTop.x, rightBottom.y, 0.f);
			vertices[3]._uv = Vector2(0.f, 1.f);

			// 우 상단
			vertices[4]._position = Vector3(rightBottom.x, leftTop.y, 0.f);
			vertices[4]._uv = Vector2(1.f, 0.f);

			// 우 하단
			vertices[5]._position = Vector3(rightBottom.x, rightBottom.y, 0.f);
			vertices[5]._uv = Vector2(1.f, 1.f);

			_meshDynamicQuad->ResetVertexBuffer(vertices);

			_vsQuad->Update();

			_cbColor->_color = drawInfo->_color;
			
			// 쉐이더 리소스 업데이트
			_psSampleColor->UpdateShaderResource(ResourceManager::Get()->GetTexture(drawInfo->_textureID)->GetSRV(), 
				SRV_REGISTER::t0);

			// 컨스턴트 버퍼 업데이트
			_psSampleColor->UpdateConstantBuffer(_cbColor.get(), CBV_REGISTER::b1);

			_psSampleColor->Update();

			_meshDynamicQuad->Update();

			// 다 그렸으므로 블렌드 스테이트를 꺼준다. 끄고 키는거에서 오버헤드가 좀 있는 편이니까
			// 블렌드를 사용했을 경우에만 다시 꺼주는걸로 한다.
			if (ui->_drawInfo->_useAlphaBlend)
				_blendState->SetBlendStateOff(_deviceContext);
		}
	}

	void Pass_Final::ExecuteTextDataQueue(std::vector<std::shared_ptr<TextData>>& textDataQueue)
	{
		_spriteBatcher->Begin();

		for (auto& textData : textDataQueue)
		{
			std::shared_ptr<Font> font = ResourceManager::Get()->GetFont(textData->_fontID);

			font->DrawTextColor(_spriteBatcher->GetSpriteBatch(), textData->_startCoord, textData->_color, textData->_outText);
		}

		_spriteBatcher->End();

		// SpriteBatch랑 SpriteFont가 사용이 되면 Blent State가 난장판이 됨. 따라서 끝나면 Default로 변경해준다.
		_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	}

	void Pass_Final::EndRender()
	{
		_depthStencilState->SetDepthEnableState(_deviceContext);

		_swapChain->Present();
	}
}