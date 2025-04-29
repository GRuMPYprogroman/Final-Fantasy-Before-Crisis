#include "CharacterIO.h"
#include <fstream>
#include "json.hpp"

void SaveCharacterToJsonFile(const std::shared_ptr<Character> character, const std::string& filename) {
    SaveData data = character->ToSaveData();
    nlohmann::json j = data;

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open save file for writing: " + filename);
    }

    file << j.dump(4);
    file.close();
}

void LoadCharacterFromJsonFile(std::shared_ptr<Character> character, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open save file for reading: " + filename);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    SaveData data = j.get<SaveData>();
    character->FromSaveData(data);
}
