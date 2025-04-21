#pragma once

class IDialogueManager {
public:
    virtual ~IDialogueManager() = default;
    virtual void StartDialogue(int playerRank) = 0;
};