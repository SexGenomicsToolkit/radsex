#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"

// Load a popmap file

struct Popmap {

    std::unordered_map<std::string, std::string> groups;  // Group for each individual
    std::unordered_map<std::string, uint> counts;  // Number of individual in each group
    uint n_individuals;  // Total number of individuals

};

Popmap load_popmap(Parameters& parameters);
