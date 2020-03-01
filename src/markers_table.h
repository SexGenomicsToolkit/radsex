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
 * @file markers_table.h
 * @brief Defines the MarkersTable class implementing parsing of markers depth table into a Marker queue.
*/

#pragma once
#include <mutex>
#include <queue>
#include "marker.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"

#define BATCH_SIZE 100  ///< Size of a batch of markers in MarkersTable::get_batch()
#define TMP_QUEUE_SIZE 1000  ///< Size of the temporary markers queue in MarkersTable::start_parser()
#define MAX_QUEUE_SIZE 10000  ///< Maximum size of the markers queue */


/*!
 * \brief Header class
 *
 * Store information about a markers table from its comment and header lines.
 */

class Header {

    public:

        uint64_t n_markers = 0;  ///< Total number of markers in the table obtained from the first comment line
        uint16_t n_individuals = 0;  ///< Total number of individuals in the markers table (number of columns - 2)
        std::vector<std::string> header;  ///< Vector storing all the fields in the markers table's header


        /*!
         * \brief Default header constructor
         */

        Header() {};


        /*!
         * \brief Standard Header constructor
         *
         * Read a marker table's comment and header lines to retrieve values of n_markers, n_individuals, and header.
         *
         * \param markers_table_path Path to the markers table
         */

        Header(std::string markers_table_path);

};



/*!
 * \brief MarkersTable class
 *
 * This class stores information about a markers table file and implements two methods:
 *
 * - start_parser(): parse the markers table and store each marker in a queue of Marker objects
 * - get_batch(): get a batch of markers from the Marker queue to be processed in an analysis
 *
 * These methods are implemented to be run on two concurrent threads (one thread each).
 */

class MarkersTable {

    public:

        Header header;   ///< Header instance storing information about the markers table
        std::queue<Marker> markers_queue;   ///< Queue of Marker objects to be processed in an analysis
        uint64_t n_markers_processed = 0;   ///< Total number of markers loaded from the markers table
        std::mutex mutex;   ///< Mutex to protect the markers queue when pushing / reading batches of markers
        bool parsing_ended = false;   ///< Flag indicating whether parsing of the markers table ended
        std::vector<std::string> groups;   ///< Vector storing the group (obtained from the popmap) for each individual based on column indices

        /*!
         * \brief Default MarkersTable constructor
         */

        MarkersTable() {};


        /*!
         * \brief Standard MarkersTable constructor
         *
         * Initialize parameters and read the markers table header
         *
         * \param parameters Pointer to a Parameter object storing all RADSex parameter values
         */

        MarkersTable(Parameters* parameters);


        /*!
         * \brief Read markers from a markers depth table and store them in a queue.
         *
         * Read a markers depth table generated with process().
         * Each marker is stored in a Marker instance which is pushed to a queue.
         * The function waits if the queue gets bigger than \link MAX_QUEUE_SIZE \endlink.
         * Batches of markers are obtained from the queue with get_batch().
         *
         * \param store_sequence If true, marker sequences and IDs are be stored in Marker objects
         * \param compute_groups  If true, group counts are computed for each marker
         */

        void start_parser(bool store_sequence = true, bool compute_groups = true);


        /*!
         * \brief Get a batch of markers from the queue
         *
         * Read a batch of markers of size \link BATCH_SIZE \endlink and store it in the Markers vector batch.
         *
         * \param batch Vector of Marker objects storing the current markers batch
         */

        void get_batch(std::vector<Marker>& batch);


        /*!
         * \brief Set the popmap attribute value
         *
         * Usually, the popmap is not yet loaded when the MarkersTable instance is created. This method is used to set
         * the pointer to the Popmap object after loading the popmap.
         *
         * \param popmap Pointer to a Popmap object
         */

        void set_popmap(Popmap* popmap);

    private:

        Parameters* parameters;   ///< Pointer to a Parameters instance storing all RADSex parameters
        Popmap* popmap;  ///< Pointer to a popmap instance storing group for each individual

        /*!
         * \brief Get group for each column in the markers table
         *
         * Create a vector of groups to easily access a column's groups by index.
         */

        void get_group_columns();

};









