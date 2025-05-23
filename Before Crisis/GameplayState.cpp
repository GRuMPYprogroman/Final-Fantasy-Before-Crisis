#include "GameplayState.h"
#include "InventoryPanel.h"
#include "ExitPanel.h"
#include "SavePanel.h"
#include "ShopPanel.h"
#include "UpgradePanel.h"
#include "ContractsPanel.h"
#include "fstream"
#include "Message.h"

GameplayState::GameplayState(
	std::shared_ptr<RenderService> renderService,
	std::shared_ptr<AudioService> audioService,
	std::shared_ptr<StateService> stateService,
    std::shared_ptr<Character> player)
	: render_service_(renderService),
	audio_service_(audioService),
	state_service_(stateService),
	player_(player)
{
	registerPanels();
	setupNavButtons();
    if (!panelFactories_.empty()) {
        switchTo(panelOrder_.at(2));
    }

    audio_service_->playMusic(MusicID::GameplayTheme, 0);

    Message mes(story_flags_::Initial,render_service_,audio_service_);
}

void GameplayState::update(float& dt)
{
    audio_service_->update(dt);
    if (activePanel_) activePanel_->update(dt);
    for (auto& btn : navButtons_) btn->update();
}

void GameplayState::render()
{
    render_service_->getRenderWindow().draw(panels_area_);

    if (activePanel_) activePanel_->render();
    for (auto& button : navButtons_) {
        render_service_->getRenderWindow().draw(button->getBackground());
        render_service_->getRenderWindow().draw(button->getLabel());
    }
}

void GameplayState::handleInput(const sf::Event& evt)
{
    for (auto& btn : navButtons_) btn->handleInput(evt, render_service_->getRenderWindow());
    if (activePanel_) activePanel_->handleInput(evt);
}

void GameplayState::registerPanels() {
    panelFactories_.emplace("Contracts",
        [this]() -> std::unique_ptr<IGameplayPanel> {
            return std::make_unique<ContractsPanel>(render_service_, audio_service_, state_service_, player_, "../tables/Locations.csv");
        }
    );
    panelOrder_.push_back("Contracts");

    panelFactories_.emplace("Shop",
        [this]() -> std::unique_ptr<IGameplayPanel> {
            return std::make_unique<ShopPanel>(render_service_, audio_service_, player_, "../tables/Items.csv");
        }
    );
    panelOrder_.push_back("Shop");

    panelFactories_.emplace("Inventory",
        [this]() -> std::unique_ptr<IGameplayPanel> {
            return std::make_unique<InventoryPanel>(render_service_,audio_service_,player_);
        }
    );
    panelOrder_.push_back("Inventory");

    panelFactories_.emplace("Upgrade",
        [this]() -> std::unique_ptr<IGameplayPanel> {
            return std::make_unique<UpgradePanel>(render_service_, audio_service_, player_);
        }
    );
    panelOrder_.push_back("Upgrade");

    panelFactories_.emplace("Save",
        [this]() -> std::unique_ptr<IGameplayPanel> {
            return std::make_unique<SavePanel>(render_service_, audio_service_,player_);
        }
    );
    panelOrder_.push_back("Save");

    panelFactories_.emplace("Exit",
        [this]() -> std::unique_ptr<IGameplayPanel> {
            return std::make_unique<PauseMenuPanel>(render_service_, audio_service_, state_service_);
        }
    );
    panelOrder_.push_back("Exit");
}


void GameplayState::switchTo(const std::string& key) {
    auto it = panelFactories_.find(key);
    if (it != panelFactories_.end()) {
        activePanel_ = it->second();
    }
}

void GameplayState::setupNavButtons() {
    float winX = render_service_->getWindowSize().x;
    float winY = render_service_->getWindowSize().y;
    size_t count = panelFactories_.size();
    float buttonWidth = 180.f;
    float spacing = 20.f;

    float totalWidth = count * buttonWidth + (count - 1) * spacing;
    float startX = (winX - totalWidth) / 2.f;
    float yPos = winY * 0.75f + 70.f;

    panels_area_.setPosition({ 0.f, yPos - 20.f });
    panels_area_.setSize({ winX,winY / 4.f});
    panels_area_.setFillColor({ 40,40,40});

    font_ = std::make_shared<sf::Font>(render_service_->getDefaultFont());
    size_t idx = 0;
    for (auto& key : panelOrder_) {
        auto& factory = panelFactories_.at(key);
        float xPos = startX + idx * (buttonWidth + spacing);
        navButtons_.emplace_back(
            std::make_shared<Button>(
                key, *font_, sf::Vector2f(xPos, yPos),
                [this, key]() {
                    audio_service_->playSound(SoundID::Click);
                    switchTo(key);
                },
                audio_service_
            )
        );
        ++idx;
    }
}
