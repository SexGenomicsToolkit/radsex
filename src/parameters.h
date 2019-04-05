#pragma once
#include <string>

struct Parameters {

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

    // Shared parameters
    uint n_threads = 1;
    uint min_depth = 1;
    float signif_threshold = static_cast<float>(0.05);
    bool disable_correction = true;

    // "loci" specific parameters
    uint loci_max_distance = 1;
    float loci_freq_het = static_cast<float>(0.5);
    float loci_freq_hom = static_cast<float>(1.0);
    float loci_range_het = static_cast<float>(0.1);
    float loci_range_hom = static_cast<float>(0.1);
    float loci_min_individual_frequency = static_cast<float>(0.75);

    // "subset" specific parameters
    uint subset_min_males = 0;
    uint subset_min_females = 0;
    uint subset_max_males = 0;
    uint subset_max_females = 0;
    uint subset_min_individuals = 0;
    uint subset_max_individuals = 0;

    // Flags indicating whether max values should be set to max number of individuals for "subset"
    bool set_max_males = true;
    bool set_max_females = true;
    bool set_max_individuals = true;

    // "map" specific parameters
    uint map_min_quality = 20;
    float map_min_frequency = static_cast<float>(0.1);

};
