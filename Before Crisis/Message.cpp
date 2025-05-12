#include <fstream>
#include <iostream>
#include "Message.h"
#include "StoryPoints.h"
#include "json.hpp"
#include "SaveData.h"

Message::Message(story_flags_ numb, std::shared_ptr<RenderService> render_service,
	std::shared_ptr<AudioService> audio_service)
	:
	render_service_(render_service),
	audio_service_(audio_service),
	current_line_(0)
{
	font_ = std::make_unique<sf::Font>(render_service_->getDefaultFont());
	loadData(numb);
	sf::Vector2f win = sf::Vector2f{ render_service_->getWindowSize() };

	background_.setFillColor(sf::Color(0, 0, 0, 150));
	background_.setSize({500,400});
	background_.setOrigin(background_.getGlobalBounds().getCenter());
	background_.setPosition(win / 2.f);

	messageArea_.setFillColor(sf::Color(0, 0, 0, 100));
	messageArea_.setSize({ 300,200 });

	message_top_leftX = (win.x - messageArea_.getSize().x) / 2.f;
	message_top_leftY = (win.y - messageArea_.getSize().y) / 2.f;

	messageArea_.setOrigin(messageArea_.getGlobalBounds().getCenter());
	messageArea_.setPosition(win / 2.f);

	message_ = std::make_unique<sf::Text>(*font_,"", fontSize_);
	message_->setFillColor(sf::Color::White);
	message_->setPosition({message_top_leftX,message_top_leftY});


	try
	{
		std::string saveName = "../saveData/PlayerSaveData.json";
		std::ifstream file_in(saveName);
		if (!file_in.is_open()) throw std::runtime_error("Can't open save file for updating flags");

		nlohmann::json j;
		file_in >> j;
		file_in.close();

		SaveData data = j.get<SaveData>();

		if (!data.story_flags[static_cast<int>(story_flags_::Initial)]) {
			displayMessage();
		}

		data.story_flags[static_cast<int>(numb)] = true;

		std::ofstream file_out(saveName, std::ios::out | std::ios::trunc);
		if (!file_out.is_open()) throw std::runtime_error("Can't write updated save file");

		file_out << nlohmann::json(data).dump(4);
		file_out.close();
	}
	catch (const std::exception& e)
	{
		std::cout << "Failed to update story flag: " << e.what() << std::endl;
	}

}

void Message::render(std::vector<std::string> for_render)
{
	render_service_->getRenderWindow().clear();
	float y = message_top_leftY;
	render_service_->getRenderWindow().draw(background_);
	render_service_->getRenderWindow().draw(messageArea_);
	for (auto& line : for_render)
	{
		message_->setString(line);
		message_->setPosition({ message_top_leftX, y });
		render_service_->getRenderWindow().draw(*message_);
		y += fontSize_ + 2;
	}

	y += fontSize_ + 2;

	message_->setString("Press enter to continue...");
	message_->setPosition({ message_top_leftX, y });
	render_service_->getRenderWindow().draw(*message_);
	render_service_->getRenderWindow().display();
}

std::vector<std::string> Message::wrapText(std::string_view text) {
	float maxWidth = messageArea_.getGlobalBounds().size.x - 10.f;

	std::vector<std::string> out;
	std::istringstream iss(text.data());
	std::string word, line;

	while (iss >> word) {
		std::string cant = line.empty() ? word : line + " " + word;
		message_->setString(cant);
		if (message_->getGlobalBounds().size.x > maxWidth) {
			if (!line.empty()) out.push_back(line);
			line = word;
		}
		else line = cant;
	}

	if (!line.empty()) out.push_back(line);
	return out;
}

bool Message::handleInput()
{
	while (auto event = render_service_->getRenderWindow().pollEvent())
	{
		if (const auto* keyPressed = event.value().getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
			{
				audio_service_->playSound(SoundID::Flip);
				if (current_line_ + 1 < text_lines_.size())
				{
					++current_line_;
					render(wrapText(text_lines_.at(current_line_  )));
					return false;
				}
				else return true;
			}
		}
	}
	return false;
}

void Message::displayMessage()
{
	if (!text_lines_.empty()) {
		render(wrapText(text_lines_[0]));
	}

	while(true)
	{
		if (handleInput()) break;
	}
}

void Message::loadData(story_flags_ message_number)
{
	try
	{

		std::string saveName = "../saveData/PlayerSaveData.json";
		std::ifstream file(saveName);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open save file for reading: " + saveName);
		}
		nlohmann::json j;
		file >> j;
		file.close();

		SaveData data = j.get<SaveData>();
		if (!data.story_flags[static_cast<int>(message_number)])
		{
			std::ifstream init(storyFiles.at(message_number));
			std::string line;
			while (std::getline(init, line)) { text_lines_.push_back(line); }
		}
	}
	catch (...)
	{
		std::cout << "Error occured proccessing message number: " << static_cast<int>(message_number) << std::endl;
	}
}