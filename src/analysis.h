#pragma once
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "markers_table.h"
#include "utils.h"


class Analysis {

    public:

        std::chrono::steady_clock::time_point t_begin;

        Parameters parameters;
        bool compare_groups;
        bool store_groups;
        bool store_sequence;

        Popmap popmap;
        MarkersTable markers_table;

        std::thread parsing_thread;
        std::thread processing_thread;

        Analysis();
        Analysis(Parameters& parameters, bool check_groups, bool store_groups, bool store_sequence);
        void print_starting_log();
        void print_groups();
        void print_ending_log();
        void processor();
        void run();

        virtual void extra_setup() {};
        virtual void process_marker(Marker& marker) {};
        virtual void generate_output() {};
};

