#pragma once
#include <vector>
#include <string>

enum class AreaType {Forest, Cave, Dungeon};

class Level
{
public:
	Level(int width, int height, AreaType areaType);
	void Generate();
	void PrintLevel() const;
	std::vector<std::pair<int, int>> GetFloorTiles() const;
	AreaType GetAreaType() const { return areaType; }
private:
	int width;
	int height;
	AreaType areaType;
	std::vector<std::string> map;
};

