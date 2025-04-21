#include "DialogueManager.h"
#include <fstream>
#include <iostream>

/**DialogueManager::DialogueManager() : currentNode(0)
{
	nodes.push_back({ "You're a 3rd Class SOLDIER. Ready for your first mission?", {{"Yes", 1}, {"No", -1}}, 3 });
	nodes.push_back({ "Great! Head to the dungeon.", {}, 3 });
}

void DialogueManager::StartDialogue(int playerRank)
{
	while (currentNode >= 0 && currentNode < nodes.size())
	{
		if (playerRank >= nodes[currentNode].requiredRank)
		{
			ui.DisplayText(nodes[currentNode].text);
			if (nodes[currentNode].choices.empty()) break;

			for (size_t i = 0; i < nodes[currentNode].choices.size(); ++i)
			{
				ui.DisplayText(std::to_string(i + 1) + ". " + nodes[currentNode].choices[i].first);
			}

			int choice = ui.GetChoice(1, nodes[currentNode].choices.size());
			currentNode = nodes[currentNode].choices[choice - 1].second;
		}
		else
		{
			ui.DisplayText("You need a higer rank for this!");
			break;
		}
	}
}**/