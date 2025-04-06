#pragma once
#include "IGameState.h"

class GameplayState : public IGameState
{
public:
	void Enter() override;
	void Update() override;
	void Exit() override;
};
