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
 * @file depth.cpp
 * @brief Implements the Depth class.
*/

#include "depth.h"


Depth::Depth(Parameters& parameters, bool compare_groups, bool store_groups, bool store_sequence) : Analysis(parameters, compare_groups, store_groups, store_sequence) {

    this->results = DepthResults(this->markers_table.header.n_individuals);
    this->min_individuals = static_cast<uint>(this->parameters.depth_min_frequency * this->markers_table.header.n_individuals);

}





void Depth::process_marker(Marker& marker) {

    for (uint i = 0; i < marker.individual_depths.size(); ++i) {

        if (marker.n_individuals >= this->min_individuals) this->results.depths[i].push_back(marker.individual_depths[i]); // Only consider markers present in a fraction of individuals for computations

        if (marker.individual_depths[i] > 0) {
            ++this->results.individual_markers_count[i];  // Increment total number of markers for this individual
            this->results.individual_reads_count[i] += marker.individual_depths[i];  // Add marker depth to total reads count for this individual
        }

    }

}





void Depth::generate_output() {

    // Check that there are actually markers retained for the analysis
    bool fail_exit = false;
    for (uint i = 0; i < this->results.depths.size(); ++i) {
        if (this->results.depths[i].size() == 0) fail_exit = true;
    }
    if (fail_exit) {
        std::string msg = "No markers were present in at least <" +
                          std::to_string(static_cast<uint>(this->parameters.depth_min_frequency * 100)) +
                          ">% of all individuals (" +
                          std::to_string(this->min_individuals) +
                          "/" + std::to_string(this->markers_table.header.n_individuals) +
                          " individuals)";
        log(msg, LOG_ERROR);
        exit(1);
    }

    // Generate output file
    log("Writing depth results to output file <" + this->parameters.output_file_path + ">");
    std::ofstream output_file = open_output(this->parameters.output_file_path);
    output_file << "Sample\tGroup\tReads\tMarkers\tRetained\tMin_depth\tMax_depth\tMedian_depth\tAverage_depth\n";

    // Compute metrics for each individual
    for (uint i = 0; i < this->results.depths.size(); ++i) {

        std::sort(this->results.depths[i].begin(), this->results.depths[i].end());  // Sort depth vector to easily compute all metrics (necessary for median anyway)

        const auto size = this->results.depths[i].size();
        const auto start = this->results.depths[i].begin();
        const auto end = this->results.depths[i].end();

        const uint16_t min_depth = *start;  // Sorted vector: minimum depth is the first element
        const uint16_t max_depth = *(end - 1);  // Sorted vector: maximum depth is the last element
        const uint64_t total_depth = static_cast<uint64_t>(std::accumulate(start, end, 0));

        const uint16_t median_depth = find_median(this->results.depths[i]);

        // Output metrics
        output_file << this->markers_table.header.header[i+2] << "\t" << this->popmap.get_group(this->markers_table.header.header[i + 2]) << "\t"
                    << this->results.individual_reads_count[i] << "\t" << this->results.individual_markers_count[i] << "\t"
                    << size << "\t" << min_depth << "\t" << max_depth << "\t" << median_depth << "\t" << total_depth / size << "\n";

    }

}
