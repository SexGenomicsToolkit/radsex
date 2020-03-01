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
 * @file freq.h
 * @brief Defines the Freq class implementing the "freq" analysis.
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
#include "utils.h"


/*!
 * \brief Implements the "freq" analysis
 *
 * Compute the distribution of markers in all individuals. \n
 * After all markers are processed, generate a tabulated output file with columns: \n
 * Frequency | Count
 *
 */

class Freq: public Analysis {

    public:

        /*!
         * \brief Default Freq constructor
         */

        Freq() {};


        /*!
         * \brief Standard Freq constructor
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Freq(Parameters& parameters, bool compare_groups = false, bool store_groups = false, bool store_sequence = false);


        /*!
         * \brief Process a single marker
         *
         * Compute the number of individuals in which the marker is present with depth higher than parameters.min_depth.
         *
         * \param marker Current marker info stored in a Marker instance.
         */

        void process_marker(Marker& marker);


        /*!
         * \brief Generate the output file
         *
         * Generate a tabulated output file with columns: \n
         * Frequency | Count
         */

        void generate_output();

    private:

            std::vector<uint32_t> results;  ///< Vector storing the number of markers (integers) present in each number of individuals (indices)
};
