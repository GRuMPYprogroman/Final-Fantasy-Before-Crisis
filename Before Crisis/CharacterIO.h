#pragma once
#include "CharacterManager.h"
#include "SaveData.h"

void SaveCharacterToJsonFile(const std::shared_ptr<Character> character, const std::string& filename);
void LoadCharacterFromJsonFile(std::shared_ptr<Character> character, const std::string& filename);
