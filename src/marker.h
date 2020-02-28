#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

class Marker {

    public:

        std::string id = "";  ///< Marker ID
        std::string sequence = "";  ///< Marker sequence
        std::vector<uint16_t> individual_depths;  ///< Vector of depth in each individual
        std::unordered_map<std::string, uint> group_counts;  ///< Map {group -> number of individuals in which the marker is present for this group}
        uint n_individuals = 0;  ///< Total number of individuals in which the marker is present
        float p = 0;  ///< P-value of association with group

        Marker() {};
        Marker(uint16_t n_individuals);
        void reset(bool sex_stats_only);
        void output_as_table(std::ofstream& output_file) const;
        void output_as_fasta(std::ofstream& output_file, const uint min_depth) const;

};
