#pragma once
#include <algorithm>
#include <fstream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include "depth_table.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"

typedef std::vector<std::vector<uint16_t>> Depths;

// Calculate the number of reads retained in each individual
void depth(Parameters& parameters);

void processor(MarkersQueue& markers_queue, Parameters& parameters, std::mutex& queue_mutex, Depths& results, std::vector<uint32_t>& n_markers, bool& parsing_ended, ulong batch_size, uint n_individuals);
