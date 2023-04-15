#include "pch.h"
#include "IGameEngine.h"


namespace Muscle
{
	std::shared_ptr<IGameEngine> IGameEngine::_gameEngine = nullptr;

	std::shared_ptr<IGameEngine> IGameEngine::Get()
	{
		if (_gameEngine == nullptr)
			_gameEngine = Muscle::MuscleEngine::GetInstance();

		return _gameEngine;
	}
}