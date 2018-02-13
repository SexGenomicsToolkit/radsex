#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "utils.h"
#include "parameters.h"
#include "popmap_file.h"
#include "output.h"
#include "edlib/edlib.h"


// Main function implementing the analysis
void group_loci(Parameters& parameters);

// Load the coverage matrix in memory
std::vector<Locus> load_coverage_matrix(std::string& file_path, int min_cov, bool print, std::vector<std::string>& header);

// Pick the next sequence from the sequences vector and process it
void sequence_processor(std::vector<Locus>::iterator& sequence, std::vector<Locus>& sequences, std::vector<Locus>& coverage_matrix,
                        std::unordered_map<std::string, std::vector<Locus>>& results, std::mutex& results_mutex, int max_distance);

// Process a sequence (look for similar sequences)
void process_sequence(Locus& locus, std::vector<Locus>& coverage_matrix, std::unordered_map<std::string, std::vector<Locus>>& results, std::mutex& results_mutex, int max_distance);
