#pragma once
#include <memory>
#include "IGameState.h"

class IGameManager
{
public:
	virtual ~IGameManager() = default;
	virtual void Run() = 0;
	virtual void ChangeState(std::unique_ptr<IGameState> newState);
};

