#pragma once
#include <mutex>
#include <queue>
#include "parameters.h"
#include "popmap.h"
#include "utils.h"


struct Marker {

    std::string id = "";  // Marker ID
    std::string sequence = "";  // Marker sequence
    std::vector<uint16_t> individuals;  // Marker depth in each individual
    uint n_individuals = 0;  // Total number of individuals in which the marker is present
    std::unordered_map<std::string, uint> groups;  // Number of individuals in which the marker is present for each group

    void reset(bool sex_stats_only) {
        if (not sex_stats_only) this->id = "";
        if (not sex_stats_only) this->sequence = "";
        for (auto& group: this->groups) group.second = 0;
        this->n_individuals = 0;
        for (auto& individual: this->individuals) individual = 0;
    }

};


typedef std::vector<std::string> Header;
typedef std::queue<Marker> MarkersQueue;

void table_parser(Parameters& parameters, Popmap& popmap, MarkersQueue& markers_queue, std::mutex& queue_mutex, bool& parsing_ended, Header& header, bool sex_stats_only = false);

std::vector<Marker> get_batch(MarkersQueue& blocks_queue, std::mutex& queue_mutex, ulong batch_size=1000);

// Create a sex <-> column index correspondance map
std::vector<std::string> get_column_group(std::unordered_map<std::string, std::string>& popmap, const std::vector<std::string>& header);
