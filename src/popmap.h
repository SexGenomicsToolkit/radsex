#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"

// Load a popmap file

struct Popmap {
    std::unordered_map<std::string, std::string> groups;
    std::unordered_map<std::string, uint> counts;
};

Popmap load_popmap(Parameters& parameters);
