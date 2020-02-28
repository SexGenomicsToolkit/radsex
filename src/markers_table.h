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
#include <mutex>
#include <queue>
#include "marker.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"

#define BATCH_SIZE 100
#define TMP_QUEUE_SIZE 1000
#define MAX_QUEUE_SIZE 10000 /**< Maximum size of the markers queue */

class Header {

    public:

        uint64_t n_markers = 0;
        uint16_t n_individuals = 0;
        std::vector<std::string> header;

        Header() {};
        Header(std::string markers_table_path);

};



class MarkersTable {

    public:

        Header header;
        std::queue<Marker> markers_queue;
        uint64_t n_markers_processed = 0;
        std::mutex mutex;
        bool parsing_ended = false;
        std::vector<std::string> groups;

        MarkersTable() {};
        MarkersTable(Parameters* parameters);

        void start_parser(bool store_sequence = true, bool compute_groups = true);
        void get_batch(std::vector<Marker>& batch);

    private:

        Parameters* parameters;
        Popmap* popmap;

        void get_group_columns();

};




/*!
 * \brief Read markers from a markers depth table and store them in a queue.
 *
 * Read a markers depth table generated with process().
 * Each marker is stored in a Marker instance which is pushed to a queue.
 * The function waits if the queue gets bigger than \link MAX_QUEUE_SIZE \endlink.
 * Batches of markers are obtained from the queue with get_batch().
 *
 * \param parameters A Parameter object which contains values of all parameters
 * \param popmap A Popmap object with information on the group of each individual
 * \param markers_queue Queue of Marker objects
 * \param no_seq If true, marker sequences and ids won't be stored in Marker objects
 * \param no_group If true, group counts won't be computed
 */


/*!
 * \brief Get a batch of markers from the queue
 *
 *
 *
 * \param markers_queue
 * \param batch_size
 * \return
 */

