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

/*!
 * @file popmap.cpp
 * @brief Implements the Popmap class.
*/

#include "popmap.h"


Popmap::Popmap(Parameters& parameters, bool check_groups) {

    this->file = parameters.popmap_file_path;
    std::ifstream popmap = open_input(this->file);

    std::string line;
    uint line_number = 1;
    std::vector<std::string> fields;

    // Parse popmap file
    while(std::getline(popmap, line)) {

        if (line.back() == '\r') line.pop_back();  // Deal with windows-style line endings
        fields = split(line, "\t");

        if (fields.size() == 2) { // Expected format: individual | group

            this->individual_groups[fields[0]] = fields[1];  // Store group for this individual
            ++this->group_counts[fields[1]];  // Increase count for this group
            ++this->n_individuals;  // Increase total individuals count

        } else {

            log("Could not process line " + std::to_string(line_number) + " in popmap file <" + this->file + "> (expected format: \"individual\tgroup\")", LOG_WARNING);

        }

        ++line_number;

    }

    if (check_groups) {

        if (this->group_counts.size() < 2) {  // Fewer than two groups in the popmap

            log("Found <" + std::to_string(this->group_counts.size()) + "> groups in the popmap file (" + this->print_groups() + ") but at least two are required", LOG_ERROR);
            exit(1);

        } else if (this->group_counts.size() > 2 and (parameters.group1 == "" or parameters.group2 == "")) {  // More than two groups but groups to compare were not defined

            log("Found <" + std::to_string(this->group_counts.size()) + "> groups in the popmap file (" + this->print_groups() + ") but groups to compare were not defined (use --groups group1,group2)", LOG_ERROR);
            exit(1);

        } else if (this->group_counts.size() > 2  and (this->group_counts.find(parameters.group1) == this->group_counts.end() or this->group_counts.find(parameters.group2) == this->group_counts.end())) {  // More than two groups and groups to compare were not found

            log("Groups specified with --groups (\"" + parameters.group1 + "\", \"" + parameters.group2 + "\") were not found in popmap groups (" + this->print_groups() + ")", LOG_ERROR);
            exit(1);

        } else if (this->group_counts.size() == 2) {  // Only two groups in popmap

            if (parameters.group1 == "" or parameters.group2 == "") {  // Fill groups from popmap if not specified by the user

                auto i = std::begin(this->group_counts);
                parameters.group1 = i->first;
                parameters.group2 = (++i)->first;

            }
        }

    }

    std::string popmap_success_message = "Loaded popmap (" + this->print_groups(true) + ")";
    log(popmap_success_message);
}





std::string Popmap::get_group(const std::string& individual) const {

    std::string group = "";

    if (this->individual_groups.find(individual) != this->individual_groups.end()) {

        group = this->individual_groups.at(individual);

    } else {

        log("Trying to get group for individual <" + individual + "> which was not found in popmap <" + this->file + ">", LOG_WARNING);

    }

    return group;
}





uint Popmap::get_count(const std::string& group) const {

    uint count = 0;

    if (this->group_counts.find(group) != this->group_counts.end()) {

        count = this->group_counts.at(group);

    } else {

        log("Trying to get individual count for group <" + group + "> which was not found in popmap <" + this->file + ">", LOG_WARNING);

    }

    return count;
}





std::string Popmap::print_groups(const bool counts) const {

    std::string list = "";
    uint n = 0;

    for (auto& group: this->group_counts) {

        list += "\"" + group.first + "\"";
        if (counts) list += ": " + std::to_string(group.second);
        if (++n < this->group_counts.size()) list += ", ";

    }

    return list;
}


