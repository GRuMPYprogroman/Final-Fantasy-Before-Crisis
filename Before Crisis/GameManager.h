#pragma once
#include "StateStack.h"
#include "MenuState.h"
#include "Renderer.h"
#include "AudioManager.h"
#include <memory>

class GameManager
{
private:
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<StateStack> states;
	std::shared_ptr<AudioManager> audio;
	bool isRunning;
public:
	GameManager();
	void Run();
};

