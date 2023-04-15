#include "pch.h"
#include "Crosshiar_Prefab.h"

Crosshiar_Prefab::Crosshiar_Prefab() : IPrefab(Muscle::CreateFromPrefab(TEXT("Crosshair"))) // 크로스헤어로 수정 해야함..!
{
	GetGameObject()->AddComponent<Crosshair>();

	for (auto iter : GetGameObject()->GetChildrens())
	{
		auto renderingData = iter->GetComponent<Muscle::MeshRenderer>()->_renderingData;

		renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Crosshair_Albedo"));
		renderingData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Crosshair_Normal"));
		renderingData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Crosshair_Metallic"));
		renderingData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Crosshair_Roughness"));
		renderingData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Crosshair_AO"));

		renderingData->_shaderInfo->_psName = TEXT("PS_ANMRAO");
		renderingData->_shaderInfo->_vsName = TEXT("VS_ANMRAO");
	}


}

Crosshiar_Prefab::~Crosshiar_Prefab()
{

}
///신성현 멍청이