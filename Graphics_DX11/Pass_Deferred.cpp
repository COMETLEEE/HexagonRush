#include "pch.h"
#include "Pass_Deferred.h"

#include "Renderer.h"
#include "ViewPort.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "RasterizerState.h"

namespace GraphicsEngine
{
	void Pass_Deferred::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);
		
		_gBuffers = std::vector<std::shared_ptr<RenderTarget>>(DEFERRED_RENDERTARGET_COUNT);

		for (int i = 0; i < DEFERRED_RENDERTARGET_COUNT - 1; i++)
		{
			_gBuffers[i] = std::make_shared<RenderTarget>();

			_gBuffers[i]->Initialize(_device, screenWidth, screenHeight);

			_gBufferViews[i] = _gBuffers[i]->GetRTV();
		}

		// 마지막 G Buffer는 ObjectID
		_gBuffers[DEFERRED_RENDERTARGET_COUNT - 1] = std::make_shared<RenderTarget>();

		_gBuffers[DEFERRED_RENDERTARGET_COUNT - 1]->Initialize(_device, screenWidth, screenHeight, false, DXGI_FORMAT_R32_UINT);

		_gBufferViews[DEFERRED_RENDERTARGET_COUNT - 1] = _gBuffers[DEFERRED_RENDERTARGET_COUNT - 1]->GetRTV();

		_cbPerObject = std::make_shared<CB_PEROBJECT>();

		_screenSizeViewPort = std::make_shared<ViewPort>();

		_screenSizeViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);

		_deferredDepth = std::make_shared<DepthStencilBuffer>();

		_deferredDepth->Initialize(_device, screenWidth, screenHeight);

		_rasterizerState = Renderer::Get()->GetRasterizerState();

		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));
		
		_psDeferred = ShaderManager::Get()->GetPS(TEXT("PS_Deferred"));

		_meshScreen = ResourceManager::Get()->GetMesh(STATIC_QUAD_MESH);
	}

	void Pass_Deferred::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);

		for (int i = 0; i < DEFERRED_RENDERTARGET_COUNT; i++)
		{
			_gBuffers[i]->OnResize(_device, screenWidth, screenHeight);

			_gBufferViews[i].ReleaseAndGetAddressOf();

			_gBufferViews[i] = _gBuffers[i]->GetRTV();
		}

		_screenSizeViewPort->OnResize(screenWidth, screenHeight);

		_deferredDepth->OnResize(_device, screenWidth, screenHeight);
	}

	void Pass_Deferred::Release()
	{
		for (int i = 0; i < DEFERRED_RENDERTARGET_COUNT; i++)
		{
			_gBuffers[i]->Release();

			_gBufferViews[i].ReleaseAndGetAddressOf();
		}

		_screenSizeViewPort->Release();
		
		_deferredDepth->Release();

		_cbPerObject.reset();

		__super::Release();
	}

	void Pass_Deferred::Render(std::vector<std::shared_ptr<RenderingData_3D>>& renderQueue)
	{
		BeginRender();

		for (auto& renderingData : renderQueue)
		{
			if (renderingData->_shaderInfo->_rasterizerState == ShaderInfo::RASTERIZER_STATE::SOLID)
				_rasterizerState->SetSolidRenderState(_deviceContext);
			else if (renderingData->_shaderInfo->_rasterizerState == ShaderInfo::RASTERIZER_STATE::WIREFRAME)
				_rasterizerState->SetWireFrameRenderState(_deviceContext);

			switch (renderingData->_dataType)
			{
				case RENDERINGDATA_TYPE::STATIC_MESH_OBJECT:
				{
					ExecuteForStaticMeshObject(renderingData);

					break;
				}

				case RENDERINGDATA_TYPE::SKINNED_MESH_OBJECT:
				{
					ExecuteForSkinnedMeshObject(renderingData);

					break;
				}

				case RENDERINGDATA_TYPE::SKY_BOX:
				{
					// SkyBox는 None Cull을 사용한다.
					_rasterizerState->SetSolidRenderStateNoneCull(_deviceContext);

					// TEST TEST 긴급
					Renderer::Get()->_currCubeMapID = renderingData->_materialInfo->_diffuseMapID;

					ExecuteForStaticMeshObject(renderingData);

					_rasterizerState->SetSolidRenderState(_deviceContext);

					break;
				}
			}
		}

		EndRender();
	}

	void Pass_Deferred::BeginRender()
	{
		for (int i = 0; i < DEFERRED_RENDERTARGET_COUNT; i++)
			_gBuffers[i]->ClearRenderTargetView(_deviceContext);

		_deferredDepth->ClearDepthStencilView(_deviceContext);

		// G-Buffer들이 혹시 쉐이더 리소스로 박혀있을 수 있으니 Unbind
		ID3D11ShaderResourceView* nullSRV[DEFERRED_RENDERTARGET_COUNT] = { nullptr };

		_deviceContext->PSSetShaderResources(0, DEFERRED_RENDERTARGET_COUNT, nullSRV);

		_deviceContext->OMSetRenderTargets(DEFERRED_RENDERTARGET_COUNT, _gBufferViews[0].GetAddressOf(), _deferredDepth->GetDSV().Get());
	
		_screenSizeViewPort->SetViewPort(_deviceContext);
	}

	void Pass_Deferred::ExecuteForStaticMeshObject(std::shared_ptr<RenderingData_3D> renderingData)
	{
		std::shared_ptr<ObjectInfo> objectInfo = renderingData->_objectInfo;
		std::shared_ptr<GeometryInfo> geoInfo = renderingData->_geoInfo;
		std::shared_ptr<ShaderInfo> shaderInfo = renderingData->_shaderInfo;
		std::shared_ptr<MaterialInfo> materialInfo = renderingData->_materialInfo;

		CB_PEROBJECT* cbPerObject = _cbPerObject.get();

		// 지오메트리 정보 복사
		memcpy(cbPerObject, geoInfo.get(), sizeof(GeometryInfo));

		// 물체 머터리얼 정보 복사 (뒤의 diffuseMapID, NormalMapID는 제외한다)
		cbPerObject->_metallic = renderingData->_materialInfo->_metallic;

		cbPerObject->_roughtness = renderingData->_materialInfo->_roughness;

		cbPerObject->_emissive = renderingData->_materialInfo->_emissive;

		// Legacy Material
		// memcpy(reinterpret_cast<char*>(cbPerObject) + sizeof(GeometryInfo), materialInfo.get(), sizeof(MaterialInfo) - 2 * sizeof(uint64));

		// 오브젝트 아이디 복사
		cbPerObject->_objectID = objectInfo->_objectID;

		// 라이팅 적용 여부 복사
		if (!objectInfo->_usingLighting)
			cbPerObject->_objectID |= LIGHT_CULL_MASK;

		std::shared_ptr<VertexShader> vs = ShaderManager::Get()->GetVS(shaderInfo->_vsName);
		std::shared_ptr<PixelShader> ps = ShaderManager::Get()->GetPS(shaderInfo->_psName);

		vs->UpdateConstantBuffer(reinterpret_cast<CB_PEROBJECT*>(cbPerObject), CBV_REGISTER::b1);

		if (materialInfo->_diffuseMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_diffuseMapID)->GetSRV(), SRV_REGISTER::t0);

		if (materialInfo->_normalMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_normalMapID)->GetSRV(), SRV_REGISTER::t1);

		if (materialInfo->_metallicMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_metallicMapID)->GetSRV(), SRV_REGISTER::t2);

		if (materialInfo->_roughnessMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_roughnessMapID)->GetSRV(), SRV_REGISTER::t3);

		if (materialInfo->_ambientOcclusionMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_ambientOcclusionMapID)->GetSRV(), SRV_REGISTER::t4);

		if (materialInfo->_emissiveMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_emissiveMapID)->GetSRV(), SRV_REGISTER::t5);

		vs->Update();
		ps->Update();

		ResourceManager::Get()->GetMesh(objectInfo->_meshID)->Update();
	}

	void Pass_Deferred::ExecuteForSkinnedMeshObject(std::shared_ptr<RenderingData_3D> renderingData)
	{
		std::shared_ptr<ObjectInfo> objectInfo = renderingData->_objectInfo;
		std::shared_ptr<GeometryInfo> geoInfo = renderingData->_geoInfo;
		std::shared_ptr<ShaderInfo> shaderInfo = renderingData->_shaderInfo;
		std::shared_ptr<MaterialInfo> materialInfo = renderingData->_materialInfo;
		std::shared_ptr<AnimationInfo> animInfo = renderingData->_animInfo;

		CB_PEROBJECT* cbPerObject = _cbPerObject.get();

		// 지오메트리 정보 복사
		memcpy(cbPerObject, geoInfo.get(), sizeof(GeometryInfo));

		// 물체 머터리얼 정보 복사 (뒤의 diffuseMapID, NormalMapID는 제외한다)
		cbPerObject->_metallic = renderingData->_materialInfo->_metallic;

		cbPerObject->_roughtness = renderingData->_materialInfo->_roughness;

		cbPerObject->_emissive = renderingData->_materialInfo->_emissive;

		// Legacy Material
		// memcpy(reinterpret_cast<char*>(cbPerObject) + sizeof(GeometryInfo), materialInfo.get(), sizeof(MaterialInfo) - 2 * sizeof(uint64));

		// 애니메이션 정보 복사
		memcpy(reinterpret_cast<char*>(cbPerObject->_boneMatrix), animInfo.get(), sizeof(AnimationInfo));

		// 오브젝트 아이디 복사
		cbPerObject->_objectID = objectInfo->_objectID;

		// 라이팅 적용 여부 복사
		if (!objectInfo->_usingLighting)
			cbPerObject->_objectID |= LIGHT_CULL_MASK;

		std::shared_ptr<VertexShader> vs = ShaderManager::Get()->GetVS(shaderInfo->_vsName);
		std::shared_ptr<PixelShader> ps = ShaderManager::Get()->GetPS(shaderInfo->_psName);

		vs->UpdateConstantBuffer(reinterpret_cast<CB_PEROBJECT*>(cbPerObject), CBV_REGISTER::b1);

		if (materialInfo->_diffuseMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_diffuseMapID)->GetSRV(), SRV_REGISTER::t0);

		if (materialInfo->_normalMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_normalMapID)->GetSRV(), SRV_REGISTER::t1);

		if (materialInfo->_metallicMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_metallicMapID)->GetSRV(), SRV_REGISTER::t2);

		if (materialInfo->_roughnessMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_roughnessMapID)->GetSRV(), SRV_REGISTER::t3);

		if (materialInfo->_ambientOcclusionMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_ambientOcclusionMapID)->GetSRV(), SRV_REGISTER::t4);

		if (materialInfo->_emissiveMapID != ULLONG_MAX)
			ps->UpdateShaderResource(ResourceManager::Get()->GetTexture(materialInfo->_emissiveMapID)->GetSRV(), SRV_REGISTER::t5);

		vs->Update();
		ps->Update();

		ResourceManager::Get()->GetMesh(objectInfo->_meshID)->Update();
	}

	void Pass_Deferred::EndRender()
	{
		// 아마 선을 그리는 렌더링은 여기서 끝날 것입니다.
		_rasterizerState->SetSolidRenderState(_deviceContext);
	}
}