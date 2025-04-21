#pragma once
#include <vector>
#include <string>

enum class AreaType {Forest, Cave, Dungeon};

class Level
{
private:
	int width;
	int height;
	AreaType areaType;
	std::vector<std::string> grid;
public:
	Level(int width, int height, AreaType areaType);
	void Generate();
	std::vector<std::string>& GetGrid() { return grid; }
	AreaType GetAreaType() const { return areaType; }
	bool IsWalkable(int x, int y) const;
};

