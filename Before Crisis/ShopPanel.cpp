#include "ShopPanel.h"
#include <fstream>
#include <sstream>
#include <iostream>

ShopPanel::ShopPanel(std::shared_ptr<RenderService> renderService,
	std::shared_ptr<AudioService> audioService,
	std::shared_ptr<Character> player,
	const std::string& csvPath)
	: render_service_(renderService),
	audio_service_(audioService),
	player_(player)
{
	font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());

	title_ = std::make_unique<sf::Text>(*font_, "Shop", 36);
	title_->setPosition({ render_service_->getWindowSize().x / 2.f, 20.f });
	title_->setFillColor(sf::Color::White);

	player_info_ = std::make_unique<sf::Text>(*font_, "", 36);
	player_info_->setPosition({ 30.f,30.f });
	updatePlayerInfo();

	loadItemsFromCSV(csvPath);
	for (auto& si : shopItems_) {
		groupedItems_[si.data.type].push_back(si);
	}

	buildButtons();
	createActionButtons();
}

void ShopPanel::loadItemsFromCSV(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Cannot open CSV: " << path << "\n";
		return;
	}

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;

		ShopItem si;
		if (!std::getline(ss, cell, ';')) continue;
		si.data.id = std::stoi(cell);
		if (!std::getline(ss, cell, ';')) continue;
		si.data.name = cell;
		if (!std::getline(ss, cell, ';')) continue;
		si.data.type = cell;
		if (!std::getline(ss, cell, ';')) continue;
		si.data.effect = std::stoi(cell);
		if (!std::getline(ss, cell, ';')) continue;
		si.data.effect_type = cell;
		if (!std::getline(ss, cell, ';')) continue;
		si.price = std::stoi(cell);
		if (!std::getline(ss, cell, ';')) continue;
		si.required_level = std::stoi(cell);

		shopItems_.push_back(si);
	}
}

void ShopPanel::buildButtons() {
	float winX = render_service_->getWindowSize().x;
	float winY = render_service_->getWindowSize().y;
	float colWidth = winX / 3.f;
	float topY = winY * 0.1f;
	float spacingY = 100.f;

	std::vector<std::pair<std::string, float>> columns = {
		{"weapon", colWidth * 0.5f},
		{"armor", colWidth * 1.5f},
		{"consumable", colWidth * 2.5f}
	};

	for (auto& [type, xPos] : columns) {
		auto hdr = std::make_unique<sf::Text>(*font_, type + "s", 28);
		hdr->setPosition({ xPos, topY });
		hdr->setFillColor(sf::Color(200, 200, 50));
		columnHeaders_.push_back(std::move(hdr));

		float y = topY + 40.f;

		for (auto& item : groupedItems_[type]) {
			std::string label = item.data.name + " - " + std::to_string(item.price) + "$\n" +
				"required level: " + std::to_string(item.required_level) +
				"\n(" + std::to_string(item.data.effect) + "% " + item.data.effect_type + ")";

			auto btn = std::make_shared<Button>(label, *font_, sf::Vector2f(xPos, y),
				[this, item]() {
					if (!selection_locked_) {
						selectedItem_ = item;
						selection_locked_ = true;
						audio_service_->playSound(SoundID::Click);
					}
				},
				audio_service_);
			btn->setBackgroundSize(btn->getLabel().getGlobalBounds().size + sf::Vector2f{ 20.f, 20.f });
			itemButtons_.push_back(btn);
			y += spacingY;
		}
	}
}

void ShopPanel::createActionButtons() {
	float centerX = render_service_->getWindowSize().x / 2.f;
	float centerY = render_service_->getWindowSize().y * 3.f / 8.f;
	button_group_pos_ = { centerX - 120.f, centerY };

	buyButton_ = std::make_shared<Button>("Buy", *font_, button_group_pos_,
		[this]() {
			if (selectedItem_ && player_->getMoney() >= selectedItem_->price &&
				player_->GetLevel() >= selectedItem_->required_level) {
				player_->AddItem(selectedItem_->data);
				player_->setMoney(-selectedItem_->price);
				updatePlayerInfo();
				audio_service_->playSound(SoundID::Buy);
			}
		}, audio_service_);

	sellButton_ = std::make_shared<Button>("Sell", *font_, button_group_pos_ + sf::Vector2f(130.f, 0.f),
		[this]() {
			if (selectedItem_)
			{
				player_->RemoveItem(selectedItem_->data.id);
				player_->setMoney(selectedItem_->price);
				updatePlayerInfo();
				audio_service_->playSound(SoundID::Buy);
			}
		}, audio_service_);

	closeButton_ = std::make_shared<Button>("Close", *font_, button_group_pos_ + sf::Vector2f(260.f, 0.f),
		[this]() {}, audio_service_);

	closeButton_->setCallback([this]() {
		selectedItem_.reset();
		selection_locked_ = false;
		audio_service_->playSound(SoundID::Click);
		closeButton_->setHovered(false);
		});

	sf::Vector2f bgSize = {
		300.f + 100.f,
		300.f + 80.f
	};
	popupBackground_ = std::make_unique<sf::RectangleShape>(bgSize);
	popupBackground_->setFillColor(sf::Color(0, 0, 150, 150));
	popupBackground_->setPosition(button_group_pos_ - sf::Vector2f(10.f, bgSize.y/2.f));
}

void ShopPanel::update(float deltaTime) {
	if (selection_locked_) {
		buyButton_->update();
		sellButton_->update();
		closeButton_->update();
		return;
	}

	for (auto& btn : itemButtons_) btn->update();
}

void ShopPanel::render() {
	auto& win = render_service_->getRenderWindow();
	win.draw(*title_);
	win.draw(*player_info_);
	for (auto& hdr : columnHeaders_) win.draw(*hdr);
	for (auto& btn : itemButtons_) {
		win.draw(btn->getBackground());
		win.draw(btn->getLabel());
	}

	if (selection_locked_) {
		win.draw(*popupBackground_);
		win.draw(buyButton_->getBackground());
		win.draw(buyButton_->getLabel());
		win.draw(sellButton_->getBackground());
		win.draw(sellButton_->getLabel());
		win.draw(closeButton_->getBackground());
		win.draw(closeButton_->getLabel());
	}
}

void ShopPanel::handleInput(const sf::Event& evt) {
	if (selection_locked_) {
		buyButton_->handleInput(evt, render_service_->getRenderWindow());
		sellButton_->handleInput(evt, render_service_->getRenderWindow());
		closeButton_->handleInput(evt, render_service_->getRenderWindow());
		return;
	}


		for (auto& btn : itemButtons_)
			btn->handleInput(evt, render_service_->getRenderWindow());


}

void ShopPanel::updatePlayerInfo() {
	player_info_->setString("Money: " + std::to_string(player_->getMoney()) + "\n" +
		"Level: " + std::to_string(player_->GetLevel()));
}