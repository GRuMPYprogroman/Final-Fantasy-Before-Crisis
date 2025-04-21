#pragma once
#include <string>
#include <vector>
#include "IDialogueManager.h"

struct DialogueNode{
	std::string text;
	std::vector<std::pair<std::string, int>> choices; // text + next node ID
	int requiredRank; // Unlock condition
};

class DialogueManager : public IDialogueManager
{
private:
	std::vector <DialogueNode> nodes;
	int currentNode;
public:
	DialogueManager();
	void LoadDialogue(const std::string& filename);
	void StartDialogue(int playerRank) override;
};

