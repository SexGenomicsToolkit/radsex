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
 * @file analysis.h
 * @brief Defines the Analysis base class.
*/

#pragma once
#include "markers_table.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"


/*!
 * \brief Base class for analyses subclasses.
 *
 * This class implements methods and attributes shared by all analyses:
 *
 * - Load parameters, load popmap, and initialize markers table parser in the constructor
 * - Start markers table parsing thread and marker processing thread in run()
 * - Read batches of markers from the markers queue in processor()
 *
 * Three virtual methods are defined to implement analyses-specific behaviour:
 *
 * - extra_setup(): called at the beginning of run(), implement analysis-specific variables setup (e.g. BWA index in map function)
 * - process_marker(): called for each marker obtained from the markers queue, implement what to do with a marker
 * - generate_output(): called at the end of run(), implement generating output file when needed
 *
 */

class Analysis {

    public:

        std::chrono::steady_clock::time_point t_begin;   ///< Record analysis starting time

        Parameters parameters;   ///< Store RADSex parameters
        bool compare_groups;   ///< Flag indicating whether groups should be checked when loading the popmap
        bool store_groups;   ///< Flag indicating whether group info should be computed for each marker when parsing the markers table
        bool store_sequence;   ///< Flag indicating whether marker sequences should be stored for each marker when parsing the markers table

        Popmap popmap;   ///< Popmap instance storing group for each individual
        MarkersTable markers_table;   ///< Markers table instance implementing the markers table parser and markers queue

        std::thread parsing_thread;   ///< Markers table parsing thread
        std::thread processing_thread;   ///< Markers processing thread


        /*!
         * \brief Default Analysis constructor
         */

        Analysis() {};


        /*!
         * \brief Standard Analysis constructor
         *
         * Initialize MarkersTable instance, load parameters and popmap
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Analysis(Parameters& parameters, bool compare_groups, bool store_groups, bool store_sequence);


        /*!
         * \brief Print analysis starting log
         *
         * Print starting log message with the name of the analysis and group counts for analyses that make use of groups
         */

        void print_starting_log();


        /*!
         * \brief Print group counts
         *
         * Print log message with group counts for groups compared by an analysis
         */

        void print_groups();


        /*!
         * \brief Print analysis ending log
         *
         * Print ending log message with total runtime
         */

        void print_ending_log();


        /*!
         * \brief Process batches of markers
         *
         * Get a batch of markers from the markers queue and call process_marker() on each marker. \n
         * Wait if queue is empty and update progress bar with processed markers.
         */

        void processor();


        /*!
         * \brief Run the analysis
         *
         * Start the parsing and processing threads and print logs at start and end. Call extra_setup() and the start and generate_output() at the end.
         */

        void run();


        /*!
         * \brief Extra setup steps
         *
         * Function to be overloaded to implement extra setup steps required by an analysis, called at the beginning of run().
         */

        virtual void extra_setup() {};


        /*!
         * \brief Process a single marker
         *
         * Function to be overloaded to implement processing of a single marker in an analysis, called on each marker by processor().
         */

        virtual void process_marker(Marker&) {};


        /*!
         * \brief Generate output
         *
         * Function to be overloaded to implement output file generation when required by an analysis, called at the end of run().
         */

        virtual void generate_output() {};
};

