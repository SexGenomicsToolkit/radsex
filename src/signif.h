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

// Get all markers significantly associated with sex and export them in either table or fasta format
void signif(Parameters& parameters);

void processor(MarkersQueue& markers_queue, Popmap& popmap, Parameters& parameters, std::mutex& queue_mutex, std::vector<Marker>& candidate_markers, uint& n_markers, bool& parsing_ended, ulong batch_size);
