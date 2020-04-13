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
 * @file marker.h
 * @brief Defines the Marker class storing information about a marker.
*/

#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


/*!
 * \brief Marker class
 *
 * Store information about a marker from the markers depth table and implement methods for parsing the markers depth table.
 */

class Marker {

    public:

        std::string id = "";  ///< Marker ID (e.g. "5318835")
        std::string sequence = "";  ///< Marker sequence
        std::vector<uint16_t> individual_depths;  ///< Vector storing marker depth in each individual (in order from the table's header)
        std::unordered_map<std::string, uint> group_counts;  ///< Map storing group counts for the marker {group -> number of individuals in which the marker is present for this group}
        uint n_individuals = 0;  ///< Total number of individuals in which the marker is present
        float p = 0;  ///< P-value of association with group
        float p_corr = 0;   ///< P-value of association with group after Bonferroni correction

        /*!
         * \brief Default Marker constructor
         */

        Marker() {};

        /*!
         * \brief Standard Marker constructor
         *
         * Initalized individual_depths size to n_individuals.
         *
         * \param n_individuals Number of individuals in the markers depth table
         */

        Marker(uint16_t n_individuals);


        /*!
         * \brief Reset a marker's attributes
         *
         * \param sex_stats_only If true, the marker ID and sequence won't be reset (save compute time when it's not needed)
         */

        void reset(bool sex_stats_only);


        /*!
         * \brief Output a marker in table format
         *
         * Generate a line with format: ID | Sequence | Depth in individual 1 | ... | Depth in individual N
         *
         * \param output_file Output file stream
         */

        void output_as_table(std::ofstream& output_file) const;


        /*!
         * \brief Output a marker in FASTA format
         *
         * Generate a FASTA header line with format "<id>_<group1:count>_<group2:count>_p:<p-value of association with group>_mindepth:<minimum depth to consider a marker present in the analysis"
         * followed by the marker's sequence on a single line.
         *
         * \param output_file Output file stream
         * \param min_depth   Minimum depth to consider a marker present in the current analysis
         * \param n_markers   Number of markers retained from the dataset to compute corrected p-value
         */

        void output_as_fasta(std::ofstream& output_file, const uint min_depth) const;

};
