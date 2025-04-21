#include "GameManager.h"


GameManager::GameManager()
	: //dialogueManager(std::make_unique<DialogueManager>()),
    character(std::make_unique<Character>()),
    //combatManager(std::make_unique<CombatManager>()),
    missionManager(std::make_unique<MissionManager>()),
    isRunning(true)
{
    renderer = std::make_shared<Renderer>(1200, 900);
}

void GameManager::ChangeState(std::unique_ptr<IGameState> newState)
{
	currentState = std::move(newState);
}

void GameManager::Run()
{
    sf::Clock clock;
    ChangeState(std::make_unique<MenuState>(sf::Font("../fonts/Kenney Pixel Square.ttf"),renderer));
    while (isRunning) {
        std::optional<sf::Event> event;
        renderer->Clear();
        while (event = renderer->getRenderWindow().pollEvent()) {
            if (event->is<sf::Event::Closed>()) isRunning = false;

            /**if (event->is<sf::Event::KeyPressed> && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape) {
                ChangeState(std::make_unique<MenuState>(*this, sf::Font{ "../fonts/Kenney Pixel Square.ttf" }));
            }**/

            currentState->handleInput(event.value());
        }
        float deltaTime = clock.restart().asSeconds();
        currentState->update(deltaTime);
        currentState->render();
        renderer->Display();
    }
}