#pragma once
#include <string>

struct Parameters {

    // Subcommand
    std::string command = "";

    // I/O parameters
    std::string input_dir_path = "";
    std::string output_file_path = "";
    std::string markers_table_path = "";
    std::string subset_file_path = "";
    std::string output_format = "";
    std::string popmap_file_path = "";
    std::string barcodes_file_path = "";
    std::string genome_file_path = "";
    std::string depths_file_path = "";
    bool output_matrix = false;
    bool output_fasta= false;

    // Group names
    std::string group1 = "";
    std::string group2 = "";

    // Shared parameters
    uint n_threads = 1;
    uint min_depth = 1;
    float signif_threshold = static_cast<float>(0.05);
    bool disable_correction = false;

    // "subset" specific parameters
    uint subset_min_group1 = 0;
    uint subset_min_group2 = 0;
    uint subset_max_group1 = 0;
    uint subset_max_group2 = 0;
    uint subset_min_individuals = 0;
    uint subset_max_individuals = 0;

    // Flags indicating whether max values should be set to max number of individuals for "subset"
    bool set_max_group1 = true;
    bool set_max_group2 = true;
    bool set_max_individuals = true;

    // "map" specific parameters
    uint map_min_quality = 20;
    float map_min_frequency = static_cast<float>(0.1);

};
