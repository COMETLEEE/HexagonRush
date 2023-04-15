#include "pch.h"
#include "ElectronicField.h"

#include "Player.h"

#define ELECTRONIC_TIME 5.f

ElectronicField::ElectronicField(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _timer(0.f)
{

}

ElectronicField::~ElectronicField()
{

}

void ElectronicField::Start()
{
	std::vector<std::shared_ptr<Muscle::GameObject>> objects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : objects)
	{
		if (object->GetName() == "Player")
			_currentPlayer = object->GetComponent<Player>();
	}
}

void ElectronicField::Update()
{
	_timer += Muscle::CTime::GetGameDeltaTime();

	if (_timer > ELECTRONIC_TIME)
	{
		Muscle::DeleteGameObject(GetGameObject());
	}
}

void ElectronicField::OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	// 플레이어와 부딪혔습니다.
	if (_Other->GetGameObject()->GetName() == "Player")
	{
		// 일렉트로닉 모드
		_currentPlayer->SetStun();

		Muscle::DeleteGameObject(GetGameObject());
	}
}