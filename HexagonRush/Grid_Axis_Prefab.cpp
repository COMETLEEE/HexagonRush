#include "pch.h"
#include "Grid_Axis_Prefab.h"

Grid_Axis_Prefab::Grid_Axis_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	std::shared_ptr<Muscle::GameObject> axis = Muscle::CreateGameObject();
	std::shared_ptr<Muscle::MeshRenderer> meshreder = axis->AddComponent<Muscle::MeshRenderer>();
	meshreder->_renderingData->_objectInfo->_meshID = Muscle::ResourceManager::Get()->GetMeshID(TEXT("Axis"));			// Axis
	meshreder->_renderingData->_objectInfo->_usingLighting = false;
	meshreder->_renderingData->_shaderInfo->_vsName = TEXT("VS_ColorOnly");
	meshreder->_renderingData->_shaderInfo->_psName = TEXT("PS_ColorOnly");

	std::shared_ptr<Muscle::GameObject> grid = Muscle::CreateGameObject();
	std::shared_ptr<Muscle::MeshRenderer> gridmesh = grid->AddComponent<Muscle::MeshRenderer>();
	gridmesh->_renderingData->_objectInfo->_meshID = Muscle::ResourceManager::Get()->GetMeshID(TEXT("Grid"));			// Grid
	gridmesh->_renderingData->_objectInfo->_usingLighting = false;
	gridmesh->_renderingData->_shaderInfo->_vsName = TEXT("VS_ColorOnly");
	gridmesh->_renderingData->_shaderInfo->_psName = TEXT("PS_ColorOnly");

	GetGameObject()->SetChild(axis);
	GetGameObject()->SetChild(grid);
}