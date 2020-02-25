#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "depth_table.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"


// Extract a subset of markers from a marker depths table based on number of individuals
void subset(Parameters& parameters);

void processor(MarkersQueue& markers_queue, Popmap& popmap, Parameters& parameters, std::mutex& queue_mutex, std::ofstream& output_file, bool& parsing_ended, ulong batch_size);
