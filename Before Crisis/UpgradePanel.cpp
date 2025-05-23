#include "UpgradePanel.h"


UpgradePanel::UpgradePanel(std::shared_ptr<RenderService> render_service,
    std::shared_ptr<AudioService> audio_service, std::shared_ptr<Character> player)
: render_service_(render_service), audio_service_(audio_service), player_(player)
{
	font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());

	player_info_ = std::make_unique<sf::Text>(*font_, "", 36);
	player_info_->setPosition({ 30.f,30.f });
	updatePlayerInfo();

	sf::Vector2f pos = sf::Vector2f{ render_service_->getWindowSize() };
	float centerX = pos.x / 2.f - 100.f;
	float y = pos.y * 3.f / 8.f - 100.f;
	float offset = 100.0f;

	auto hp_button = std::make_shared<Button>("+5 HP", *font_, sf::Vector2f(centerX,y),
			[this]() {
				if (player_->getAbilityPoints() > 0)
				{
					player_->setAbilityPoints(-1);
					player_->setHp(5);
					audio_service_->playSound(SoundID::Upgrade);
					updatePlayerInfo();
				}
			}, audio_service_);
	auto mp_button = std::make_shared<Button>("+5 MP", *font_, sf::Vector2f(centerX, y + offset),
		[this]() {
			if (player_->getAbilityPoints() > 0)
			{
				player_->setAbilityPoints(-1);
				player_->setMp(5);
				audio_service_->playSound(SoundID::Upgrade);
				updatePlayerInfo();
			}
		}, audio_service_);
	auto strength_button = std::make_shared<Button>("+5 STR", *font_, sf::Vector2f(centerX, y + 2*offset),
		[this]() {
			if (player_->getAbilityPoints() > 0)
			{
				player_->setAbilityPoints(-1);
				player_->setStrength(5);
				audio_service_->playSound(SoundID::Upgrade);
				updatePlayerInfo();
			}
		}, audio_service_);
	ability_buttons_.push_back(hp_button);
	ability_buttons_.push_back(mp_button);
	ability_buttons_.push_back(strength_button);
}

void UpgradePanel::update(float deltaTime)
{
	for (auto& btn : ability_buttons_) {
		btn->update();
	}
}

void UpgradePanel::render()
{
	render_service_->getRenderWindow().draw(*player_info_);

	for (auto& btn : ability_buttons_) {
		render_service_->getRenderWindow().draw(btn->getBackground());
		render_service_->getRenderWindow().draw(btn->getLabel());
	}
}

void UpgradePanel::handleInput(const sf::Event& evt)
{
	for (auto& btn : ability_buttons_) {
		btn->handleInput(evt,render_service_->getRenderWindow());
	}
}

void UpgradePanel::updatePlayerInfo()
{
	player_info_->setString("Level: " + std::to_string(player_->GetLevel()) +
		"\nHP: " + std::to_string(player_->GetStats().hp) +
		"\nMP: " + std::to_string(player_->GetStats().mp) +
		"\nStrength: " + std::to_string(player_->GetStats().strength) +
		"\nAbility Points: " + std::to_string(player_->getAbilityPoints()) +
		"\nExp: " + std::to_string(player_->getExp()) + 
		"\nNeed exp till lvl up: " + std::to_string(player_->GetLevel() * 100 - player_->getExp()));
}
