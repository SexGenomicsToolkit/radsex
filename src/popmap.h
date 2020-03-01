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
 * @file popmap.h
 * @brief Defines the Popmap class storing groups information.
*/

#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"


/*!
 * \brief Popmap class
 *
 * Store group information inferred from a tabulated file (popmap) and implement methods to get an individual's group and the number of individuals for a group.
 */

class Popmap {

    public:

        uint16_t n_individuals = 0;  ///< Total number of individuals in the popmap

        /*!
         * \brief Default Popmap constructor
         */

        Popmap() {};


        /*!
         * \brief Standard Popmap constructor
         *
         * Read a popmap file and store the group for each individual as well as individual counts for each group.
         *
         * \param parameters   Parameters instance storing the value of all RADSex parameters
         * \param check_groups If true, performs checks on the groups obtained from the popmap
         */

        Popmap(Parameters& parameters, bool check_groups = false);


        /*!
         * \brief Get group for an individual
         *
         * \param individual Name of the individual
         *
         * \return A string containing the group for the individual or "" if the individual was not in the popmap
         */

        std::string get_group(const std::string& individual) const;


        /*!
         * \brief Get individual count for a group
         *
         * \param group Name of the group
         *
         * \return The number of individuals for this group
         */

        uint get_count(const std::string& group) const;


        /*!
         * \brief Print all the groups in the popmap
         *
         * \param counts If true, counts are printed along groups (e.g. "Group1: 15, Group2: 20")
         *
         * \return A string with comma-separated groups (e.g. "Group1, Group2")
         */

        std::string print_groups(const bool counts = false) const;

    private:

        std::string file;   ///< Path to the popmap file
        std::unordered_map<std::string, std::string> individual_groups;  ///< Map storing group for each individual: {individual -> group}
        std::unordered_map<std::string, uint> group_counts;  ///< Map storing the number of individuals in each group: {group -> number of individuals}

};
