#pragma once
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "stats.h"
#include "utils.h"

// Generate an output file for "process"
void output_process(std::string& output_file_path, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, uint min_depth);

// Generate an output file for "distrib" in matrix format
void output_distrib_matrix(std::string& output_file_path, sd_table& results, uint n_group1, uint n_group2);

// Generate an output file for "distrib" in table format
void output_distrib(std::string& output_file_path, sd_table& results, uint n_group1, uint n_group2, std::string& group1, std::string& group2, float signif_threshold, bool disable_correction);
