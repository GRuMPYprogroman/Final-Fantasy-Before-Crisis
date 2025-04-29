#pragma once
#include "IGameplayPanel.h"
#include "RenderService.h"
#include "AudioService.h"
#include "CharacterManager.h"
#include "CharacterIO.h"
#include "thread"
#include "chrono"

class SavePanel : public IGameplayPanel {
private:
    std::shared_ptr<RenderService> render_service_;
    std::shared_ptr<AudioService> audio_service_;
    std::shared_ptr<Character> player_;
    std::unique_ptr<sf::Text> saveMessage_;
    std::shared_ptr<sf::Font> font_;
	bool isSaved_ = false;
public:
    SavePanel(std::shared_ptr<RenderService> renderService, std::shared_ptr<AudioService> audioService, std::shared_ptr<Character> player)
        : render_service_(renderService), audio_service_(audioService), player_(player)
    {
        font_ = std::make_shared<sf::Font>(render_service_->getDefaultFont());
        saveMessage_ = std::make_unique<sf::Text>(*font_, "Game Saved!",30);
        saveMessage_->setOrigin(saveMessage_->getLocalBounds().getCenter());
        saveMessage_->setPosition({ render_service_->getRenderWindow().getSize().x / 2.f , render_service_->getRenderWindow().getSize().y * 3.f / 8.f });
        saveMessage_->setFillColor(sf::Color::White);

        SaveCharacterToJsonFile(player_,"../saveData/PlayerSaveData.json");
        audio_service_->playSound(SoundID::Click);
    }

    void update(float deltaTime) override {}

    void render() override {
			render_service_->getRenderWindow().draw(*saveMessage_);
    }
    void handleInput(const sf::Event& evt) override {}
};
