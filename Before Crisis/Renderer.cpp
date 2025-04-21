#include "Renderer.h"
#include <iostream>
#include <stdexcept>

Renderer::Renderer(unsigned int width, unsigned int height, const std::string& title) {
	window.create(sf::VideoMode({ width, height }), title);
	window.setFramerateLimit(60);
	if (!LoadResources()) {
		std::cout << "Renderer: some resources were not loaded!\n";
	}
}

bool Renderer::LoadResources() {
	if (!font.openFromFile("../fonts/Kenney Pixel Square.ttf")) return false;
	if (!atlas.loadFromFile("assets/tiles.png")) return false; // Texture atlas for '@', '#', etc.
	vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	return true;
}

void Renderer::Clear() {
	window.clear(sf::Color::Black);
}

void Renderer::Display() {
	window.display();
}

/**void Renderer::RenderWorld(const Level& level, const sf::Vector2f& playerPos) {
	vertices.clear();
	const auto& grid = level..GetGrid(); // Assuming Level has GetGrid() returning a 2D vector<char>
	float tileSize = 32.f; // Adjust based on your design
	for (int y = 0; y < grid.size(); ++y) {
		for (int x = 0; x < grid[y].size(); ++x) {
			sf::Vertex quad[4];
			quad[0].position = sf::Vector2f(x * tileSize, y * tileSize);
			quad[1].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
			quad[2].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);
			quad[3].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);

			// Map characters to atlas coordinates (simplified)
			sf::Vector2f texCoords = (grid[y][x] == '@') ? sf::Vector2f(0, 0) : sf::Vector2f(32, 0);
			quad[0].texCoords = texCoords;
			quad[1].texCoords = texCoords + sf::Vector2f(32, 0);
			quad[2].texCoords = texCoords + sf::Vector2f(32, 32);
			quad[3].texCoords = texCoords + sf::Vector2f(0, 32);

			for (int i = 0; i < 4; ++i) vertices.append(quad[i]);
		}
	}
	window.draw(vertices, &atlas);
}**/

void Renderer::RenderMessage(const std::string& message) {
	sf::Text text(font, message, 20);
	text.setPosition({ 10.f, 10.f });
	text.setFillColor(sf::Color::White);
	window.draw(text);
}

void Renderer::RenderButton(const Button& button) {
	window.draw(button.getBackground());
	window.draw(button.getLabel());
}

void Renderer::Update(const std::any message)
{
	if (std::any_cast<std::shared_ptr<sf::Text>>(&message)) {
		auto message_ptr = std::any_cast<std::shared_ptr<sf::Text>>(&message);
		window.draw(**message_ptr);
	}
	else if (std::any_cast<std::shared_ptr<Button>>(&message)) {
		auto button_ptr = std::any_cast<std::shared_ptr<Button>>(&message);
		RenderButton(**button_ptr);
	}
}