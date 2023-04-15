#include "pch.h"
#include "DrillSpiner.h"

DrillSpiner::DrillSpiner(std::shared_ptr<Muscle::GameObject> GameObject) : IComponents(GameObject)
{

}

DrillSpiner::~DrillSpiner()
{

}

void DrillSpiner::Start()
{
	_rendererTransform = GetGameObject()->GetChildrens()[0]->GetComponent<Muscle::Transform>();
	auto renderData = GetGameObject()->GetChildrens()[0]->GetComponent<Muscle::RendererBase>()->_renderingData;

	renderData->_objectInfo->_usingShadow = false;
	renderData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Dril_Albedo"));
	renderData->_materialInfo->_normalMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Dril_Normal"));
	renderData->_materialInfo->_metallicMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Dril_Metallic"));
	renderData->_materialInfo->_roughnessMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Dril_Roughness"));
	renderData->_materialInfo->_ambientOcclusionMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Dril_AO"));
	renderData->_materialInfo->_emissiveMapID = ResourceManager::Get()->GetTextureID(TEXT("Boss2_Dril_Emissive"));

	renderData->_shaderInfo->_vsName = TEXT("VS_ANMRAOE");
	renderData->_shaderInfo->_psName = TEXT("PS_ANMRAOE");
}

void DrillSpiner::Update()
{
	_rendererTransform->Roll(40 * Muscle::CTime::GetGameDeltaTime());
}

void DrillSpiner::Finalize()
{
	_rendererTransform.reset();
}

