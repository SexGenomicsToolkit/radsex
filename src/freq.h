#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "depth_table.h"
#include "output.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"

// Compute a table of marker frequencies in the population, regardless of sex
void freq(Parameters& parameters);

void processor(MarkersQueue& markers_queue, std::mutex& queue_mutex, std::vector<uint32_t>& frequencies, bool& parsing_ended, ulong batch_size);


