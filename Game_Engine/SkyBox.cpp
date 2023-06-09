#include "pch.h"
#include "SkyBox.h"

namespace Muscle
{

	Muscle::SkyBox::SkyBox(std::shared_ptr<GameObject> _GameObj) :IComponents(_GameObj)
	{
	}

	void Muscle::SkyBox::Initialize(const uint64& cubeMapID, std::shared_ptr<Camera> camera)
	{
		_renderingData = std::make_shared<::RenderingData_3D>();

		_renderingData->_dataType = RENDERINGDATA_TYPE::SKY_BOX;

		// Cube랑 Sphere 에 약간 다이나믹한 차이가 있을 줄 알았는데 별로 차이가 없다.
		_renderingData->_objectInfo->_meshID = ResourceManager::Get()->GetMeshID(TEXT("WhiteCube"));

		// _renderingData->_objectInfo->_meshID = ResourceManager::Get()->GetMeshID(TEXT("GreenSphere"));

		_renderingData->_objectInfo->_objectID = m_GameObject.lock()->GetObjectID();

		_renderingData->_shaderInfo->_vsName = TEXT("VS_SkyBox");

		_renderingData->_shaderInfo->_psName = TEXT("PS_SkyBox");

		_renderingData->_materialInfo->_diffuseMapID = cubeMapID;

		_renderingData->_objectInfo->_usingLighting = false;

		_camera = camera;

	}

	void SkyBox::Start()
	{
		_transform = m_GameObject.lock()->GetTransform();
	}

	void Muscle::SkyBox::LateUpdate()
	{


	}

	void SkyBox::Render()
	{
		if (_renderingData->_materialInfo->_diffuseMapID == ULLONG_MAX)
			return;

		_renderingData->_geoInfo->_world = Matrix::CreateScale(Vector3(100.f, 100.f, 100.f)) * Matrix::CreateTranslation(_transform->GetWorldPosition());
		_renderingData->_geoInfo->_worldViewProj = Matrix::CreateTranslation(_transform->GetWorldPosition()) * _camera->View() * _camera->Proj();


		MuscleEngine::GetInstance()->GetGraphicsManager()->PostRenderingData_3D(_renderingData);
	}

}