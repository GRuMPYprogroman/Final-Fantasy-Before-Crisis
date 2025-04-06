#pragma once
#include <string>

class UIManager
{
public:
	void DisplayText(const std::string& text);
	int GetChoice(int min, int max);
};

