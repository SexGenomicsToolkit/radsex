#pragma once
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "stats.h"
#include "utils.h"

// Generate an output file for "process"
void output_process_reads(std::string& output_file_path, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, uint min_depth);

// Generate an output file for "distrib" in matrix format
void output_sex_distribution_matrix(std::string& output_file_path, sd_table& results, uint n_males, uint n_females);

// Generate an output file for "distrib" in table format
void output_sex_distribution(std::string& output_file_path, sd_table& results, uint n_males, uint n_females);

// Generate an output file for "loci"
void output_group_loci(std::string& output_file_path, std::unordered_map<std::string, std::vector<Locus>>& results, std::vector<std::string>& header);

// Generate an output file for "map"
void output_mapping(std::string& output_file_path, std::vector<MappedSequence> sequences);
