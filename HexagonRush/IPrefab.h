#pragma once

class IPrefab
{
public:
	IPrefab(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~IPrefab();

protected:
	std::shared_ptr<Muscle::GameObject> _gameObject;

public:
	std::shared_ptr<Muscle::GameObject> GetGameObject();
};