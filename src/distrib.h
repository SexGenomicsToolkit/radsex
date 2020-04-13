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
 * @file distrib.h
 * @brief Defines the Distrib class implementing the "distrib" analysis.
*/

#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "analysis.h"
#include "markers_table.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"


/*!
 * \brief DistribResults struct
 *
 * Store results for the "distrib" analysis.
 */

struct DistribResults {

    std::unordered_map<uint, std::unordered_map<uint, std::pair<uint64_t, double>>> distribution;  ///< Map storing the distribution of markers: {n_individuals_group1 -> {n_individuals_group2 -> {markers_count, p-value}}}
    uint64_t n_markers = 0;  ///< Number of markers present in at least one individual with depth higher than min_depth (for Bonferroni correction)

};



/*!
 * \brief Implements the "distrib" analysis
 *
 * Compute the distribution of markers between two groups. \n
 * After all markers are processed, generate a tabulated output file with columns: \n
 * Group1 | Group2 | Markers | P | P corrected | Signif | Bias
 *
 */

class Distrib: public Analysis {

    public:

        /*!
         * \brief Default Distrib constructor
         */

        Distrib() {};


        /*!
         * \brief Standard Distrib constructor
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Distrib(Parameters& parameters, bool compare_groups = true, bool store_groups = true, bool store_sequence = false) : Analysis(parameters, compare_groups, store_groups, store_sequence) {};


        /*!
         * \brief Process a single marker
         *
         * Updates results for the number of individuals from each group in which the marker is present with depth higher than parameters.min_depth.
         *
         * \param marker Current marker info stored in a Marker instance.
         */

        void process_marker(Marker& marker);


        /*!
         * \brief Generate the output file
         *
         * Compute probability of association with group for each combination of number of individuals from group1 and group2. \n
         * Generate a tabulated output file with columns: \n
         * Group1 | Group2 | Markers | P | P corrected | Signif | Bias
         */

        void generate_output();

    private:

        DistribResults results;  ///< DistribResults instance to store the distribution of markers between groups and total number of retained markers
};
