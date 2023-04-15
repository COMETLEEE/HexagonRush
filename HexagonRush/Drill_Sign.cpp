#include "pch.h"
#include "Drill_Sign.h"

Drill_Sign::Drill_Sign(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject)
{

}

Drill_Sign::~Drill_Sign()
{

}

void Drill_Sign::Start()
{
	_rendererBase = GetGameObject()->GetChildrens()[0]->GetComponent<Muscle::RendererBase>();

	_rendererBase->_renderingData->_materialInfo->_diffuseMapID = ResourceManager::Get()->GetTextureID(TEXT("Sign_Albedo"));
	_rendererBase->_renderingData->_shaderInfo->_psName = TEXT("PS_A");
	_rendererBase->_renderingData->_shaderInfo->_vsName = TEXT("VS_A");

	ObjectMover::Get()->MoveBy(GetGameObject()->GetTransform(), Vector3(0, 5, 0), 1.0f);
	ObjectMover::Get()->MoveBy(GetGameObject()->GetTransform(), Vector3(0, -5, 0), 1.0f, 2.0f);

	Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObject) {Muscle::DeleteGameObject(gameObject); }, 4.5f, GetGameObject());
}

void Drill_Sign::Update()
{

}
