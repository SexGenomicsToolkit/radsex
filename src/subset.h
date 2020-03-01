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
 * @file subset.h
 * @brief Defines the Subset class implementing the "subset" analysis.
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
 * \brief Implements the "subset" analysis
 *
 * Filter markers based on presence in individuals from each group \n
 * Markers are output either as a markers depth table (ID | Sequence | Depth in individual 1 | .... | Depth in individual N)
 * or in a FASTA file with headers formatted as "<id>_<group1:count>_<group2:count>_p:<p-value of association with group>_mindepth:<minimum depth to consider a marker present in the analysis"
 *
 */

class Subset: public Analysis {

    public:

        /*!
         * \brief Default Subset constructor
         */

        Subset() {};


        /*!
         * \brief Standard Subset constructor
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Subset(Parameters& parameters, bool compare_groups = true, bool store_groups = true, bool store_sequence = true) : Analysis(parameters, compare_groups, store_groups, store_sequence) {};


        /*!
         * \brief Process a single marker
         *
         * Output markers verifying conditions defined with CLI arguments --min-group1, --max-group1, --min-group2, --max-group1, --min-individuals, and --max-individuals.
         *
         * \param marker Current marker info stored in a Marker instance.
         */

        void process_marker(Marker& marker);


        /*!
         * \brief Extra setup steps
         *
         * Open output file and print header if not a FASTA file.
         */

        void extra_setup();

    private:

        std::ofstream output_file;   ///< Output file stream
};
