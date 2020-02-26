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
#include <iostream>

void depth(Parameters& parameters) {

    std::chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();

    Popmap popmap = load_popmap(parameters, false);

    log("RADSex depth started");
    Header header = get_header(parameters.markers_table_path);
    uint n_individuals = static_cast<uint>(header.size()) - 2; // Number of columns - 2 (id and seq columns)

    Depths depths(n_individuals);
    std::vector<uint32_t> n_markers(n_individuals, 0);

    bool parsing_ended = false;
    MarkersQueue markers_queue;
    std::mutex queue_mutex;

    std::thread parsing_thread(table_parser, std::ref(parameters), std::ref(popmap), std::ref(markers_queue), std::ref(queue_mutex), std::ref(parsing_ended), true, true);
    std::thread processing_thread(processor, std::ref(markers_queue), std::ref(parameters), std::ref(queue_mutex), std::ref(depths), std::ref(n_markers), std::ref(parsing_ended), BATCH_SIZE, popmap.n_individuals);

    parsing_thread.join();
    processing_thread.join();

    std::ofstream output_file = open_output(parameters.output_file_path);

    output_file << "Individual\tGroup\tMarkers\tRetained\tMin_depth\tMax_depth\tMedian_depth\tAverage_depth\n";

    for (uint i=0; i<depths.size(); ++i) {

        std::sort(depths[i].begin(), depths[i].end()); // Sort depth vector to easily compute all metrics (necessary for median anyway)

        const auto start = depths[i].begin();
        const auto end = depths[i].end() - 1;
        const auto size = depths[i].size();

        const std::string group = popmap.get_group(header[i + 2]);

        const uint16_t min_depth = *start;
        const uint16_t max_depth = *end;
        const uint64_t total_depth = static_cast<uint64_t>(std::accumulate(depths[i].begin(), depths[i].end(), 0));

        uint16_t median_depth = 0;
        if (depths[i].size() % 2 == 0) {  // Find median for an even sized vector (average of two possible median points)

            const auto median_it1 = start + size / 2 - 1;
            const auto median_it2 = start + size / 2;

            std::nth_element(start, median_it1 , end);
            const auto e1 = *median_it1;

            std::nth_element(start, median_it2 , end);
            const auto e2 = *median_it2;

            median_depth = (e1 + e2) / 2;

        } else {  // Find median for odd sized vector

            const auto median_it = start + size / 2;
            std::nth_element(start, median_it , end);
            median_depth = *median_it;
        }

        output_file << header[i+2] << "\t" << group << "\t" << n_markers[i] << "\t" << size << "\t" << min_depth << "\t" <<
                       max_depth << "\t" << median_depth << "\t" << total_depth / size << "\n";

    }

    log("RADSex depth ended (total runtime: " + get_runtime(t_begin) + ")");

}


void processor(MarkersQueue& markers_queue, Parameters& parameters, std::mutex& queue_mutex, Depths& depths, std::vector<uint32_t>& n_markers, bool& parsing_ended, ulong batch_size, uint n_individuals) {

    // Give 100ms headstart to table parser thread (to get number of individuals from header)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::vector<Marker> batch;
    bool keep_going = true;

    uint marker_processed_tick = static_cast<uint>(markers_queue.n_markers / 100);
    uint64_t n_processed_markers = 0;

    while (keep_going) {

        // Get a batch of markers from the queue
        batch = get_batch(markers_queue, queue_mutex, batch_size);

        if (batch.size() > 0) {  // Batch not empty

            for (auto& marker: batch) {

                for (uint i=0; i<marker.individuals.size(); ++i) {

                    if (marker.n_individuals == n_individuals) depths[i].push_back(marker.individuals[i]); // Only consider markers present in all individuals
                    if (marker.individuals[i] > 0) ++n_markers[i];  // Increment total number of markers for this individual

                }

                log_progress_bar(n_processed_markers, marker_processed_tick);

            }

        } else {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Batch empty: wait 10ms before asking for another batch

        }

        if (parsing_ended and markers_queue.markers.size() == 0) keep_going = false;
    }
}
