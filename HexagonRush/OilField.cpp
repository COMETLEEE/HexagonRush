#include "pch.h"
#include "OilField.h"
#include "Player.h"

#define OIL_TIME 5.f

OilField::OilField(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _timer(0.f), _isSetUp(false)
{

}

OilField::~OilField()
{

}

void OilField::Start()
{
	std::vector<std::shared_ptr<Muscle::GameObject>> objects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : objects)
	{
		if (object->GetName() == "Player")
			_currentPlayer = object->GetComponent<Player>();
	}
}

void OilField::Update()
{
	_timer += Muscle::CTime::GetGameDeltaTime();

	if (_timer >= OIL_TIME)
	{
		_currentPlayer->SetSpeedNormal();

		_isSetUp = true;

		Muscle::DeleteGameObject(GetGameObject());
	}
}

void OilField::OnStayCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	if (_isSetUp)
		return;

	if (_Other->GetGameObject()->GetName() == "Player")
		_currentPlayer->SetSpeedDown();
}

void OilField::OnExitCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	if (_Other->GetGameObject()->GetName() == "Player")
	{
		_currentPlayer->SetSpeedNormal();
	}
}