#pragma once
#include "IGameplayPanel.h"
#include "RenderService.h"
#include "AudioService.h"
#include "StateService.h"
#include "MenuState.h"

class PauseMenuPanel : public IGameplayPanel {
private:
    std::shared_ptr<RenderService> render_service_;
    std::shared_ptr<AudioService> audio_service_;
    std::shared_ptr<StateService> state_service_;
    std::shared_ptr<sf::Font> font_;
    std::unique_ptr<sf::Text> exitMessage_;
public:
    PauseMenuPanel(std::shared_ptr<RenderService> renderService, std::shared_ptr<AudioService> audioService, std::shared_ptr<StateService> stateService)
        : render_service_(renderService), audio_service_(audioService), state_service_(stateService)
    {
        font_ = std::make_shared<sf::Font>(render_service_->getDefaultFont());
        exitMessage_ = std::make_unique<sf::Text>(*font_,"Press Enter to Exit to Menu",24);

        exitMessage_->setOrigin(exitMessage_->getLocalBounds().getCenter());
        exitMessage_->setPosition({ render_service_->getRenderWindow().getSize().x / 2.f , render_service_->getRenderWindow().getSize().y * 3.f / 8.f });
    }

    void update(float deltaTime) override {}
    void render() override {
        render_service_->getRenderWindow().draw(*exitMessage_);
    }

    void handleInput(const sf::Event& evt) override {
        if (evt.is<sf::Event::KeyPressed>() && evt.getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Enter) {
            state_service_->popState();
            state_service_->pushState(std::make_unique<MenuState>(render_service_, audio_service_, state_service_));
        }
    }
};