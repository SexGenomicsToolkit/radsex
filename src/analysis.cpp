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
 * @file analysis.cpp
 * @brief Implements the Analysis base class.
*/

#include "analysis.h"



Analysis::Analysis(Parameters& parameters, bool compare_groups, bool store_groups, bool store_sequence) : markers_table(&parameters) {

    this->parameters = parameters;
    this->compare_groups = compare_groups;
    this->store_groups = store_groups;
    this->store_sequence = store_sequence;

    this->t_begin = std::chrono::steady_clock::now();  // Record starting time

    (parameters.popmap_file_path != "") ? this->popmap = Popmap(this->parameters, this->compare_groups) : this->popmap = Popmap();  // Load popmap if required by the analysis
    this->markers_table.set_popmap(&this->popmap);

}





void Analysis::print_starting_log() {

    log("RADSex <" + this->parameters.command + "> started");
    if (this->compare_groups) this->print_groups();

}





void Analysis::print_groups() {

    log("Comparing groups \"" + this->parameters.group1 + "\" and \"" + this->parameters.group2 + "\"");

}





void Analysis::print_ending_log() {

    log("RADSex <" + this->parameters.command + "> ended (total runtime: " + get_runtime(t_begin) + ")");

}





void Analysis::run() {

    this->print_starting_log();
    this->extra_setup();

    // Execute parsing and processing threads
    this->parsing_thread = std::thread(&MarkersTable::start_parser, std::ref(this->markers_table), this->store_sequence, this->store_groups);  // Initialize parsing thread
    this->processing_thread = std::thread(&Analysis::processor, this);  // Initialize processing thread with the processor method of the current instance

    this->parsing_thread.join();
    this->processing_thread.join();

    this->generate_output();
    this->print_ending_log();
}





void Analysis::processor() {

    std::vector<Marker> batch;  // Store a batch of markers
    bool keep_going = true;
    const uint marker_processed_tick = static_cast<uint>(this->markers_table.header.n_markers / 100);  // Number of markers for 1% of total markers, used in logging
    uint64_t n_processed_markers = 0;

    while (keep_going) {

        this->markers_table.get_batch(batch);  // Get a batch of markers from the queue

        if (batch.size() > 0) {  // Batch not empty

            for (auto& marker: batch) {

                this->process_marker(marker);  // Do something with the marker
                log_progress_bar(++n_processed_markers, marker_processed_tick);  // Update progress bar

            }

        } else {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Batch empty: wait 10ms before asking for another batch

        }

        if (this->markers_table.parsing_ended and this->markers_table.markers_queue.size() == 0) keep_going = false;  // Member parsing_ended is updated by the table parser when parsing is done
    }

}
