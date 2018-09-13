#pragma once
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <thread>
#include "utils.h"
#include "parameters.h"
#include "sequence_file.h"
#include "output.h"
#include "kseq/kseq.h"

// Main function implementing the analysis
void process_reads(Parameters& parameters);

// Look for an unprocessed file and analyze it
void file_processor(std::vector<InputFile>& input_files, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex, std::mutex& files_mutex);

// Process a file
inline void process_file(InputFile& input_file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex);
