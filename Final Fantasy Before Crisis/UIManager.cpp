#include "UIManager.h"
#include <iostream>

void UIManager::DisplayText(const std::string& text) {
	std::cout << text << "\n";
}

int UIManager::GetChoice(int min, int max)
{
	int choice;
	do
	{
		std::cin >> choice;
	} while (choice < min || choice > max);
	return choice;
}

