#pragma once
#include <mutex>
#include <queue>
#include "parameters.h"
#include "popmap.h"
#include "utils.h"


struct Marker {

    std::string id = "";
    std::string sequence = "";
    std::vector<uint16_t> individuals;
    std::unordered_map<std::string, uint> groups;

};


typedef std::queue<Marker> MarkersQueue;

void table_parser(Parameters& parameters, Popmap& popmap, MarkersQueue& markers_queue, std::mutex& queue_mutex, bool& parsing_ended);

std::vector<Marker> get_batch(MarkersQueue& blocks_queue, std::mutex& queue_mutex, ulong batch_size=1000);

// Create a sex <-> column index correspondance map
std::vector<std::string> get_column_group(std::unordered_map<std::string, std::string>& popmap, const std::vector<std::string>& header);
