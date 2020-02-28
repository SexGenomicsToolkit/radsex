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

/** @file */

#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"

class Popmap {

    public:

        uint16_t n_individuals = 0;  ///< Total number of individuals

        Popmap() {};
        Popmap(Parameters& parameters, bool check_groups = false);
        std::string get_group(const std::string& individual) const;
        uint get_count(const std::string& group) const;
        std::string print_groups(const bool counts = false) const;

    private:

        std::string file;
        std::unordered_map<std::string, std::string> individual_groups;  ///< Group for each individual
        std::unordered_map<std::string, uint> group_counts;  ///< Number of individuals in each group

};
