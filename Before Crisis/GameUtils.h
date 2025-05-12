#pragma once
#include <memory>
#include <vector>
#include <string>

struct Item {
    int id;
    std::string name;
    std::string type;
    int effect;
    std::string effect_type;
    int instanceID;
};