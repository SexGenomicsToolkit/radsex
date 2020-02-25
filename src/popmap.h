#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"

// Load a popmap file

struct Popmap {

    std::unordered_map<std::string, std::string> groups;  // Group for each individual
    std::unordered_map<std::string, uint> counts;  // Number of individual in each group
    uint16_t n_individuals = 0;  // Total number of individuals

    std::string get_group(const std::string& individual) {

        std::string group = "";
        if (this->groups.find(individual) != this->groups.end()) group = this->groups[individual];
        return group;

    }

};

Popmap load_popmap(Parameters& parameters, bool compare = true);

std::string print_groups(Popmap& popmap, bool counts = false);
