#pragma once
#include "RenderService.h"
#include "AudioService.h"
#include "StoryPoints.h"

class Message
{
private:
	std::shared_ptr<RenderService> render_service_;
	std::shared_ptr<AudioService> audio_service_;

	sf::RectangleShape background_;
	sf::RectangleShape messageArea_;
	std::unique_ptr<sf::Text> message_;
	
	std::vector<std::string> text_lines_;
	int current_line_;

	std::unique_ptr<sf::Font> font_;

	float fontSize_ = 18;
	float message_top_leftX;
	float message_top_leftY;
public:
	Message(story_flags_ numb,std::shared_ptr<RenderService> render_service,
		std::shared_ptr<AudioService> audio_service);
	void render(std::vector<std::string> for_render);
	bool handleInput();
	void displayMessage();
	void loadData(story_flags_ message_number);

	std::vector<std::string> wrapText(std::string_view text);
};

