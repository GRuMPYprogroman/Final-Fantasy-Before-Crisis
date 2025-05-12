#include "GameManager.h"
#include "GameplayState.h"



GameManager::GameManager()
	: states(std::make_shared<StateStack>()),
      audio(std::make_shared<AudioManager>()),
	  renderer(std::make_shared<Renderer>(1200, 900)),
	  isRunning(true)
{
    audio->init();
}

void GameManager::Run()
{
    sf::Clock clock;
    states->pushState(std::make_unique<MenuState>(renderer,audio,states));

    states->applyPendingChanges();

    while (isRunning && !states->empty()) {
        std::optional<sf::Event> event;
        renderer->Clear();

        while (event = renderer->getRenderWindow().pollEvent()) {
            if (event->is<sf::Event::Closed>()) isRunning = false;

            if (!states->empty())
				states->topState()->handleInput(event.value());
        }

        states->applyPendingChanges();

        float deltaTime = clock.restart().asSeconds();
        if (!states->empty())
        {
            states->topState()->update(deltaTime);
           
        }

        if (!states->empty()) {
            states->topState()->render();
        }

        renderer->Display();
    }
}