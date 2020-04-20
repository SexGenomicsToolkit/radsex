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
 * @file markers_table.cpp
 * @brief Implements the MarkersTable and Header classes.
*/

#include "markers_table.h"


Header::Header(std::string markers_table_path) {

    std::string line = "#";
    std::ifstream input_file(markers_table_path);

    if (not input_file.is_open()) {

        log("Could not open input file <" + markers_table_path + "> in get_header");
        exit(1);

    }

    std::getline(input_file, line);

    if (line[0] == '#') {

        this->n_markers = std::stoi(split(line, " : ")[1]);
        std::getline(input_file, line);

    }

    this->header = split(line, "\t");
    this->n_individuals = this->header.size() - 2;

    input_file.close();
}





MarkersTable::MarkersTable(Parameters* parameters) {

    this->parameters = parameters;
    this->header = Header(this->parameters->markers_table_path);
    this->popmap = nullptr;

}





void MarkersTable::start_parser(bool store_sequence, bool compute_groups) {

    if (this->popmap == nullptr) {

        log("Popmap was not defined in MarkersTable::start_parser");
        exit(1);

    }

    std::ifstream input_file = open_input(this->parameters->markers_table_path);

    if (compute_groups) this->get_group_columns();

    // Define variables used to read the file
    char buffer[65536];
    uint k = 0, field_n = 0, marker_n = 0;
    Marker marker(this->header.n_individuals);
    std::vector<Marker> tmp_queue(TMP_QUEUE_SIZE);  // Temporary block queue to avoid locking the shared blocks queue too often
    uint tmp_queue_real_size = 0;
    uint marker_queue_size = 0;
    std::string temp = "#";

    while (temp[0] == '#') std::getline(input_file, temp);  // Read comment lines and header line

    temp = "";

    do {

        // Read a chunk of size given by the buffer
        input_file.read(buffer, sizeof(buffer));
        k = static_cast<uint>(input_file.gcount());

        for (uint i=0; i<k; ++i) {

            // Read the buffer character by character
            switch(buffer[i]) {

                case '\t':  // New field

                    switch (field_n) {

                        case 0:
                            if (store_sequence) marker.id = temp;
                            break;

                        case 1:
                            if (store_sequence) marker.sequence = temp;
                            break;

                        default:
                            marker.individual_depths[field_n - 2] = static_cast<uint16_t>(fast_stoi(temp.c_str()));
                            if (marker.individual_depths[field_n - 2] >= this->parameters->min_depth) {
                                if (compute_groups) ++marker.group_counts[this->groups[field_n]];
                                ++marker.n_individuals;
                            }
                            break;

                    }

                    temp = "";
                    ++field_n;
                    break;

                case '\n':  // New line (also a new field)

                    marker.individual_depths[field_n - 2] = static_cast<uint16_t>(fast_stoi(temp.c_str()));
                    if (marker.individual_depths[field_n - 2] >= this->parameters->min_depth) {
                        if (compute_groups) ++marker.group_counts[this->groups[field_n]];
                        ++marker.n_individuals;
                    }

                    // Add marker to the queue
                    tmp_queue[marker_n % TMP_QUEUE_SIZE] = marker;
                    ++tmp_queue_real_size;
                    ++marker_n;

                    // Merge temporary queue with global queue
                    if (marker_n % TMP_QUEUE_SIZE == 0) {

                        do {

                            marker_queue_size = static_cast<uint>(this->markers_queue.size());
                            if (marker_queue_size > MAX_QUEUE_SIZE) std::this_thread::sleep_for(std::chrono::microseconds(10));  // Wait if queue is too big

                        } while (marker_queue_size > MAX_QUEUE_SIZE);

                        this->mutex.lock();

                        for (auto& tmp_marker: tmp_queue) {

                            this->markers_queue.push(tmp_marker);
                            ++this->n_markers_processed;
                            tmp_marker.reset(store_sequence);

                        }

                        this->mutex.unlock();

                        tmp_queue_real_size = 0;

                    }

                    // Reset variables
                    temp = "";
                    field_n = 0;
                    marker.reset(store_sequence);
                    break;

                default:

                    temp += buffer[i];
                    break;

            }

        }

    } while (input_file);

    // Last batch: real number of markers to push is given by tmp_queue_real_size
    this->mutex.lock();

    for (uint i=0; i<tmp_queue_real_size; ++i) {

        this->markers_queue.push(tmp_queue[i]);
        ++this->n_markers_processed;

    }

    this->mutex.unlock();

    input_file.close();

    this->parsing_ended = true;  // Shared flag indicating that the parsing is finished

}





void MarkersTable::get_batch(std::vector<Marker>& batch) {

    this->mutex.lock();

    unsigned long batch_size_real = std::min(BATCH_SIZE, static_cast<int>(this->markers_queue.size()));
    batch.resize(batch_size_real);

    if (batch_size_real > 0) {

        for (uint i=0; i<batch_size_real; ++i) {

            batch[i] = this->markers_queue.front();
            this->markers_queue.pop();

        }

    }

    this->mutex.unlock();

}





void MarkersTable::get_group_columns() {

    this->groups = {"", ""};
    for (uint i = 2; i < this->header.header.size(); ++i) this->groups.push_back(this->popmap->get_group(this->header.header.at(i)));

}





void MarkersTable::set_popmap(Popmap *popmap) {

    this->popmap = popmap;

}
