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
 * @file parameters.h
 * @brief Defines the Parameters class storing information on all RADSex parameters.
*/

#pragma once
#include <string>


/*!
 * \brief Parameters
 *
 * Store the value of all RADSex parameters.
 *
 */

struct Parameters {

    // Subcommand
    std::string command = "";   ///< Name of the command being run (i.e. analysis)

    // I/O parameters
    std::string input_dir_path = "";   ///< Path to an input directory for "process"
    std::string output_file_path = "";   ///< Path to the output file for any analysis
    std::string markers_table_path = "";   ///< Path to a markers depth table for any analysis
    std::string popmap_file_path = "";   ///< Path to a popmap file specifying group for each individual
    std::string genome_file_path = "";   ///< Path to a genome in FASTA format for "map"
    bool output_fasta= false;   ///< If true, markers are outputted in FASTA format

    // Group names (when more than two groups in popmap or to override popmap order)
    std::string group1 = "";   ///< Name of the first group for analysis comparing two groups
    std::string group2 = "";   ///< Name of the secondfirst group for analysis comparing two groups

    // Common analyses parameters
    unsigned int n_threads = 1;   ///< Number of threads to use for "process"
    unsigned int min_depth = 1;   ///< Minimum depth to consider a marker present in an individual
    float signif_threshold = static_cast<float>(0.05);   ///< P-value threshold to consider a marker significantly associated with group
    bool disable_correction = false;   ///< If true, Bonferroni correction will NOT be applied when estimating significance of association with group

    // "subset" specific parameters
    unsigned int subset_min_group1 = 0;   ///< Minimum number of individuals from the first group in which a marker is present to retain a marker
    unsigned int subset_min_group2 = 0;   ///< Minimum number of individuals from the second group in which a marker is present to retain a marker
    unsigned int subset_max_group1 = 9999;   ///< Maximum number of individuals from the first group in which a marker is present to retain a marker
    unsigned int subset_max_group2 = 9999;   ///< Maximum number of individuals from the second group in which a marker is present to retain a marker
    unsigned int subset_min_individuals = 0;   ///< Minimum number of individuals from the entire dataset in which a marker is present to retain a marker
    unsigned int subset_max_individuals = 9999;   ///< Maximum number of individuals from the entire dataset in which a marker is present to retain a marker
    bool set_max_group1 = true;   ///< If true, set subset_max_group1 to the number of group1 individuals in the popmap (i.e. it was not specified by the user)
    bool set_max_group2 = true;   ///< If true, set subset_max_group2 to the number of group1 individuals in the popmap (i.e. it was not specified by the user)
    bool set_max_individuals = true;   ///< If true, set subset_max_individuals to the number of group1 individuals in the popmap (i.e. it was not specified by the user)

    // "map" specific parameters
    unsigned int map_min_quality = 20;   ///< Minimum mapping quality to retained an aligned marker
    float map_min_frequency = static_cast<float>(0.1);   ///< Minimum frequency of a marker in the population to retain the marker

};
