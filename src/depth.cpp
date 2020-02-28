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

#include "depth.h"


Depth::Depth(Parameters& parameters, bool check_groups, bool store_groups, bool store_sequence) : Analysis(parameters, check_groups, store_groups, store_sequence) {

    this->results = DepthResults(this->markers_table.header.n_individuals);

}



void Depth::generate_output() {

    // Generate output file
    std::ofstream output_file = open_output(this->parameters.output_file_path);
    output_file << "Individual\tGroup\tMarkers\tRetained\tMin_depth\tMax_depth\tMedian_depth\tAverage_depth\n";

    // Compute metrics for each individual
    for (uint i = 0; i < this->results.depths.size(); ++i) {

        std::sort(this->results.depths[i].begin(), this->results.depths[i].end());  // Sort depth vector to easily compute all metrics (necessary for median anyway)

        const auto start = this->results.depths[i].begin();
        const auto end = this->results.depths[i].end();
        const auto size = this->results.depths[i].size();

        const uint16_t min_depth = *start;  // Sorted vector: minimum depth is the first element
        const uint16_t max_depth = *(end - 1);  // Sorted vector: maximum depth is the last element
        const uint64_t total_depth = static_cast<uint64_t>(std::accumulate(start, end, 0));

//        uint16_t median_depth = find_median(this->results.depths[i]);
        uint16_t median_depth = 0;

        // Output metrics
        output_file << this->markers_table.header.header[i+2] << "\t" << this->popmap.get_group(this->markers_table.header.header[i + 2]) << "\t" << this->results.individual_markers_count[i] << "\t"
                    << size << "\t" << min_depth << "\t" << max_depth << "\t" << median_depth << "\t" << total_depth / size << "\n";

    }

}



void Depth::process_marker(Marker& marker) {

    for (uint i = 0; i < marker.individual_depths.size(); ++i) {

        if (marker.n_individuals >= 0.75 * this->markers_table.header.n_individuals) this->results.depths[i].push_back(marker.individual_depths[i]); // Only consider markers present in at least 75% of individuals
        if (marker.individual_depths[i] > 0) ++this->results.individual_markers_count[i];  // Increment total number of markers for this individual

    }

}
