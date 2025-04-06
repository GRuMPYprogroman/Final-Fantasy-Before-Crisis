#pragma once
#include "IGameState.h"

class MenuState : public IGameState
{
public:
	void Enter() override;
	void Update() override;
	void Exit() override;
};


