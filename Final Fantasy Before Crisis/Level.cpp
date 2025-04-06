#include "Level.h"
#include <random>
#include <iostream>

Level::Level(int width, int height, AreaType areaType) : width(width), height(height), areaType(areaType)
{
	map.resize(height, std::string(width, '#')); // WALL
	Generate();
}

void Level::Generate()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, width-2);

	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			if (dis(gen) % 2 == 0) map[y][x] = '.'; // FLOOR
		}
	}

	map[1][1] = '@'; // PLAYER START
}

void Level::PrintLevel() const
{
	for (const auto& row : map)
	{
		for (const auto& tile: row)
		{
			std::cout << tile;
		}
		std::cout << std::endl;
	}
}

std::vector<std::pair<int, int>> Level::GetFloorTiles() const {
	std::vector<std::pair<int, int>> floors;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (map[y][x] == '.') floors.emplace_back(x, y);
		}
	}
	return floors;
}
