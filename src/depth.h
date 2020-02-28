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
#include <numeric>
#include "analysis.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "markers_table.h"
#include "utils.h"


/*!
 * \brief DepthsResults struct
 *
 * Store results for the depth function.
 */

struct DepthResults {

    std::vector<std::vector<uint16_t>> depths;  ///< Vector of size n_individuals storing the depth of each marker in each individual
    std::vector<uint32_t> individual_markers_count;   ///< Vector of size individuals storing the number of markers retained in each individual

    /*!
     * \brief DepthsResults default constructor
     */

    DepthResults() {};


    /*!
     * \brief DepthsResults constructor
     *
     * Initialize the size of #depths and #individual_markers_count to n_individuals.
     *
     * \param n_individuals  Number of individuals in the dataset
     */

    DepthResults(const uint16_t& n_individuals) {

        this->depths = std::vector<std::vector<uint16_t>>(n_individuals);
        this->individual_markers_count = std::vector<uint32_t>(n_individuals);

    };

};



/*!
 * \brief Class Depth
 *
 * Compute the minimum, maximum, median, and average marker depth for each individual. \n
 * This function creates a parsing thread which reads a markers table file and stores markers into a queue,
 * and a processing thread which reads batches of markers from the queue and compute metrics. \n
 * After all markers are processed, the function generates an tabulated output file with columns: \n
 * Individual | Group | Markers | Retained | Min_depth | Max_depth | Median_depth | Average_depth
 *
 */

class Depth: public Analysis {

    public:

        Depth();
        Depth(Parameters& parameters, bool check_groups, bool store_groups, bool store_sequence);

        void process_marker(Marker& marker);

        /*!
         * \brief generate_output
         */

        void generate_output();

    private:

        DepthResults results;  ///< Structure to store analysis results
};
