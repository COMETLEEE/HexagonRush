#include "pch.h"
#include "IPrefab.h"

IPrefab::IPrefab(std::shared_ptr<Muscle::GameObject> gameObject)
{
	_gameObject = gameObject;
}

IPrefab::~IPrefab()
{
	_gameObject.reset();
}

std::shared_ptr<Muscle::GameObject> IPrefab::GetGameObject()
{
	return _gameObject;
}