/*
* Copyright (C) 2020 Romain Feron
* This file is part of RADSex.

* RADSex is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* RADSex is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with RADSex.  If not, see <https://www.gnu.org/licenses/>.
*/

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
