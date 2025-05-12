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
	if (!default_font_.openFromFile("../fonts/Brushstroke Horror.otf")) return false;
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
	sf::Text text(default_font_, message, 20);
	sf::Vector2f pos = sf::Vector2f(getWindowSize());
	float x = pos.x;
	float y = pos.y;
	x = x / 2.f;
	y = y * 3.f / 8.f;
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(pos);
	text.setFillColor(sf::Color::White);
	window.draw(text);
}