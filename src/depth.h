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
 * @file depth.h
 * @brief Defines the Depth class implementing the "depth" analysis.
*/

#pragma once
#include <numeric>
#include "analysis.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "markers_table.h"
#include "utils.h"


/*!
 * \brief DepthResults struct
 *
 * Store results for the "depth" analysis.
 */

struct DepthResults {

    std::vector<std::vector<uint16_t>> depths;  ///< Vector of size n_individuals storing the depth of each marker in each individual
    std::vector<uint32_t> individual_markers_count;   ///< Vector of size n_individuals storing the number of markers retained in each individual
    std::vector<uint32_t> individual_reads_count;   ///< Vector of size n_individuals storing the number of reads in each individual


    /*!
     * \brief DepthsResults default constructor
     */

    DepthResults() {};


    /*!
     * \brief DepthsResults standard constructor
     *
     * Initialize the size of #depths and #individual_markers_count to n_individuals.
     *
     * \param n_individuals  Number of individuals in the dataset
     */

    DepthResults(const uint16_t& n_individuals) {

        this->depths = std::vector<std::vector<uint16_t>>(n_individuals);
        this->individual_markers_count = std::vector<uint32_t>(n_individuals);
        this->individual_reads_count = std::vector<uint32_t>(n_individuals);

    };

};



/*!
 * \brief Implements the "depth" analysis
 *
 * Compute the minimum, maximum, median, and average marker depth for each individual. \n
 * After all markers are processed, generate a tabulated output file with columns: \n
 * Individual | Group | Markers | Retained | Min_depth | Max_depth | Median_depth | Average_depth
 *
 */

class Depth: public Analysis {

    public:

        /*!
         * \brief Default Depth constructor
         */

        Depth();


        /*!
         * \brief Standard Depth constructor
         *
         * Initialize the DepthResults instance.
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Depth(Parameters& parameters, bool compare_groups = false, bool store_groups = false, bool store_sequence = false);


        /*!
         * \brief Process a single marker
         *
         * If a marker is present in more than 75% of individuals, store individual depths in results.
         *
         * \param marker Current marker info stored in a Marker instance.
         */

        void process_marker(Marker& marker);


        /*!
         * \brief Generate the output file
         *
         * Compute minimum, maximum, median, and average depth for all markers retained in process_marker. \n
         * Generate a tabulated output file with columns: \n
         * Individual | Group | Reads | Markers | Retained | Min_depth | Max_depth | Median_depth | Average_depth
         */

        void generate_output();

    private:

        DepthResults results;  ///< DepthResults instance to store individual marker depths and total number of retained markers
};
