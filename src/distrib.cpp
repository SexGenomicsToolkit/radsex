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

#include "distrib.h"

void distrib(Parameters& parameters) {

    // Collect info
    const std::chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();
    const Popmap popmap = load_popmap(parameters, false);
    const Header header = get_header(parameters.markers_table_path);

    log("RADSex distrib started");
    log("Comparing groups \"" + parameters.group1 + "\" and \"" + parameters.group2 + "\"");

    // Initialize structure to store results
    DistribResults results;

    // Initialize parsing parameters
    MarkersQueue markers_queue;
    std::mutex queue_mutex;
    bool parsing_ended = false;

    // Execute parsing and processing threads
    std::thread parsing_thread(table_parser, std::ref(parameters), std::ref(popmap), std::ref(markers_queue), std::ref(parsing_ended), true, false);
    std::thread processing_thread(processor, std::ref(markers_queue), std::ref(parameters), std::ref(queue_mutex), std::ref(results), std::ref(parsing_ended), BATCH_SIZE);

    parsing_thread.join();
    processing_thread.join();

    double chi_squared = 0;

    // Compute p-values of association with group
    for (uint g = 0; g <= popmap.group_counts.at(parameters.group1); ++g) {

        for (uint h = 0; h <= popmap.group_counts.at(parameters.group2); ++h) {

            if (g + h != 0) {  // Exclude markers absent from all individuals

                chi_squared = get_chi_squared(g, h, popmap.group_counts.at(parameters.group1), popmap.group_counts.at(parameters.group2));
                results.distribution[g][h].second = std::min(1.0, get_chi_squared_p(chi_squared));  // p-value corrected with Bonferroni, with max value of 1

            }
        }
    }

    if (not parameters.disable_correction) parameters.signif_threshold /= results.n_markers; // Bonferroni correction: divide threshold by total number of tests (i.e. number of retained markers)

    // Generate the output file
    std::ofstream output_file(parameters.output_file_path);
    output_file << parameters.group1 << "\t" << parameters.group2 << "\t" << "Markers" << "\t" << "P" << "\t" << "Signif" << "\t" << "Bias" << "\n";

    for (uint g = 0; g <= popmap.group_counts.at(parameters.group1); ++g) {

        for (uint h = 0; h <= popmap.group_counts.at(parameters.group2); ++h) {

            if (g + h != 0) { // Exclude markers absent from all individuals

                output_file << g << "\t" << h << "\t" << results.distribution[g][h].first << "\t" << results.distribution[g][h].second << "\t"
                            << (static_cast<float>(results.distribution[g][h].second) < parameters.signif_threshold ? "True" : "False") << "\t"
                            << static_cast<float>(g) / static_cast<float>(popmap.group_counts.at(parameters.group1)) - (static_cast<float>(h) / static_cast<float>(popmap.group_counts.at(parameters.group2))) << "\n";

            }
        }
    }

    log("RADSex distrib ended (total runtime: " + get_runtime(t_begin) + ")");
}


void processor(MarkersTable& markers_queue, Parameters& parameters, DistribResults& results, bool& parsing_ended) {

    std::vector<Marker> batch;  // Store a batch of markers
    bool keep_going = true;

    const uint marker_processed_tick = static_cast<uint>(markers_queue.n_markers / 100);  // Number of markers for 1% of total markers, used in logging
    uint64_t n_processed_markers = 0;

    while (keep_going) {

        // Get a batch of markers from the queue
        batch = get_batch(markers_queue);

        if (batch.size() > 0) {  // Batch not empty

            for (auto marker: batch) {

                if (marker.n_individuals > 0) {

                    ++results.distribution[marker.groups[parameters.group1]][marker.groups[parameters.group2]].first;
                    ++results.n_markers;

                }

                log_progress_bar(n_processed_markers, marker_processed_tick);

            }

        } else {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Batch empty: wait 10ms before asking for another batch

        }

        if (parsing_ended and markers_queue.markers.size() == 0) keep_going = false;
    }
}
