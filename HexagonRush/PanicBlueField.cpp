#include "pch.h"
#include "PanicBlueField.h"

#define PANIC_TIME 5.f

PanicBlueField::PanicBlueField(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _timer(0.f)
{

}

PanicBlueField::~PanicBlueField()
{

}

void PanicBlueField::Start()
{
	std::vector<std::shared_ptr<Muscle::GameObject>> objects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : objects)
	{
		if (object->GetName() == "Player")
			_currentPlayer = object->GetComponent<Player>();
	}
}

void PanicBlueField::Update()
{
	_timer += Muscle::CTime::GetGameDeltaTime();

	if (_timer > PANIC_TIME)
	{
		Muscle::DeleteGameObject(GetGameObject());
	}
}

void PanicBlueField::OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	// 플레이어와 부딪혔습니다.
	if (_Other->GetGameObject()->GetName() == "Player")
	{
		// 패닉 모드
		_currentPlayer->SetPanic();

		Muscle::DeleteGameObject(GetGameObject());
	}
}