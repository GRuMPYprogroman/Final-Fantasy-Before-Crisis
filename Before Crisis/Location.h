#pragma once
#include <vector>
#include <string>

struct Location {
    int id;
    std::string title;
    std::string description;
    std::vector<int> neighbors;
    bool has_enemy = false;
};

