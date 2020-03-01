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
 * @file signif.h
 * @brief Defines the Signif class implementing the "signif" analysis.
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
 * \brief SignifResults struct
 *
 * Store results for the "signif" analysis.
 */

struct SignifResults {

    std::vector<Marker> candidate_markers;   ///< Vector of markers significantly associated with groups before Bonferroni correction
    uint64_t n_markers = 0;   ///< Number of markers present in at least one individual with depth higher than min_depth (for Bonferroni correction)

};



/*!
 * \brief Implements the "signif" analysis
 *
 * Find all markers significantly associated with groups. \n
 * After all markers are processed, significant markers are output either as a markers depth table (ID | Sequence | Depth in individual 1 | .... | Depth in individual N)
 * or in a FASTA file with headers formatted as "<id>_<group1:count>_<group2:count>_p:<p-value of association with group>_mindepth:<minimum depth to consider a marker present in the analysis"
 *
 */

class Signif: public Analysis {

    public:

        /*!
         * \brief Default Signif constructor
         */

        Signif() {};


        /*!
         * \brief Standard Signif constructor
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Signif(Parameters& parameters, bool compare_groups = true, bool store_groups = true, bool store_sequence = true) : Analysis(parameters, compare_groups, store_groups, store_sequence) {};


        /*!
         * \brief Process a single marker
         *
         * Store markers significantly associated with groups in results.candidate_markers.
         *
         * \param marker Current marker info stored in a Marker instance.
         */

        void process_marker(Marker& marker);


        /*!
         * \brief Generate the output file
         *
         * Apply Bonferroni correction (if not disabled) to assess significance of association with groups. \n
         * Export markers significantly associated with group to a file, either: \n
         * - a markers depth table, i.e. tabulated file for columns "ID | Sequence | Depth in individual 1 | .... | Depth in individual N"
         * - a FASTA file with headers formatted as "<id>_<group1:count>_<group2:count>_p:<p-value of association with group>_mindepth:<minimum depth to consider a marker present in the analysis"
         */

        void generate_output();

    private:

        SignifResults results;  ///< SignifResults instance to store markers significantly associated with groups before Bonferroni correction
};
