#include "pch.h"
#include "Crosshair.h"

Crosshair::Crosshair(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject)
{

}

Crosshair::~Crosshair()
{

}

void Crosshair::SetSize(float size)
{
	_size = size;
}

void Crosshair::Start()
{
	GetGameObject()->GetTransform()->SetScale(4, 1, 4);
}

void Crosshair::Update()
{
	if (_complate) return;
	_timer += Muscle::CTime::GetGameDeltaTime() * _speed;

	if (GetGameObject()->GetTransform()->GetScale().x > _size)
	{
		GetGameObject()->GetTransform()->SetScale(GetGameObject()->GetTransform()->GetScale() - (Vector3(1, 0, 1) * Muscle::CTime::GetGameDeltaTime() * _speed * _scaleSpeed));
	}
	if (_spinTimer < _timer)
	{
		_complate = true;
		Muscle::CTime::Invoke([](std::shared_ptr<Muscle::GameObject> gameObj) {Muscle::DeleteGameObject(gameObj); }, 1.0f, GetGameObject());
	}
	else
	{
		GetGameObject()->GetTransform()->RotateY(_speed * Muscle::CTime::GetGameDeltaTime() * _spinSpeed);

	}
}
