#include "GameManager.h"


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
    states->pushState(std::make_unique<MenuState>(sf::Font("../fonts/Brushstroke Horror.otf"), renderer,audio,states));
    while (isRunning && !states->empty()) {
        std::optional<sf::Event> event;
        renderer->Clear();
        auto state = states->topState();
        while (event = renderer->getRenderWindow().pollEvent()) {
            if (event->is<sf::Event::Closed>()) isRunning = false;

            /**if (event->is<sf::Event::KeyPressed> && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape) {
                ChangeState(std::make_unique<MenuState>(*this, sf::Font{ "../fonts/Kenney Pixel Square.ttf" }));
            }**/

            if (!states->empty())
				state->handleInput(event.value());
        }
        float deltaTime = clock.restart().asSeconds();
        if (!states->empty())
        {
            state->update(deltaTime);
            for (const auto& state : states->getStates())
                state->render();
        }
        renderer->Display();
    }
}