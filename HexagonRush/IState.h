#pragma once

template<class T>
class IState abstract
{
protected:
	std::weak_ptr<T> _character;

public:
	virtual void OnEnter(std::shared_ptr<T>) abstract;

	virtual void OnStay() abstract;

	virtual void OnExit() abstract;
};