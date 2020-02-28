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
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "markers_table.h"
#include "stats.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"


/*!
 * \brief DistribResults struct
 *
 * Store results for the distrib function
 */

struct DistribResults {

    /// Map storing the distribution of markers: {n_individuals_group1 -> {n_individuals_group2 -> {markers_count, p-value}}}
    std::unordered_map<uint, std::unordered_map<uint, std::pair<uint64_t, double>>> distribution;
    uint64_t n_markers;  ///< Number of markers present in at least one individual with depth higher than min_depth (for Bonferroni correction)
};

/*!
 * \brief Main function called by the distrib command
 *
 * Compute the distribution of markers between two groups. \n
 * This function creates a parsing thread which reads a markers table file and stores markers into a queue,
 * and a processing thread which reads batches of markers from the queue and compute metrics. \n
 * After all markers are processed, the function generates an tabulated output file with columns: \n
 * Group1 | Group2 | Markers | P | Signif Bias
 *
 * \param parameters  Parameters object storing the value of all RADSex parameters
 */

void distrib(Parameters& parameters);


/*!
 * \brief Process markers from a MarkersQueue
 *
 * Read batches of markers from a markers queue, and compute the number of individuals from each group (defined in
 * the popmap) in which each marker is present with depth higher than parameters.min_depth
 *
 * \param markers_queue  MarkersQueue instance
 * \param parameters     Parameters object storing the value of all RADSex parameters
 * \param results        DepthResults instance storing individual marker depths and individual marker counts
 * \param parsing_ended  A flag indicating that the parsing ended, updated by the table_parser() function
 */

void processor(MarkersTable& markers_queue, Parameters& parameters, DistribResults& results, bool& parsing_ended);
