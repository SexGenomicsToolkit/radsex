#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "depth_table.h"
#include "stats.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"

// Compute the distribution of markers between males and females
void distrib(Parameters& parameters);

void processor(MarkersQueue& markers_queue, Parameters& parameters, std::mutex& queue_mutex, sd_table& results, uint64_t& n_markers, bool& parsing_ended, ulong batch_size);
