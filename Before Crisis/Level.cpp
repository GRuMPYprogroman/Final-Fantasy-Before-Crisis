#include "Level.h"
#include <random>
#include <iostream>

Level::Level(int width, int height, AreaType areaType) : width(width), height(height), areaType(areaType)
{
	grid.resize(height, std::string(width, '#')); // WALL
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
			if (dis(gen) % 2 == 0) grid[y][x] = '.'; // FLOOR
		}
	}

	grid[1][1] = '@'; // PLAYER START
}

bool Level::IsWalkable(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height) return false;
	return grid[y][x] == '.' || grid[y][x] == '@';
}