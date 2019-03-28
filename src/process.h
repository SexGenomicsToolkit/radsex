#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <thread>
#include <zlib.h>
#include "kseq/kseq.h"
#include "output.h"
#include "parameters.h"
#include "utils.h"


struct InputFile {

    /* The InputFile structure stores information about an input file:
     * - Path to the file
     * - The file's extension
     * - Name of the individual (from the file name)
     * - A boolean indicating whether the file was already processed
     */

    std::string individual_name;
    std::string path;
    std::string extension;
    bool processed;
};

// Read a directory
std::vector<InputFile> get_input_files(const std::string& input_dir_path);

// Main function implementing the analysis
void process(Parameters& parameters);

// Look for an unprocessed file and analyze it
void file_processor(std::vector<InputFile>& input_files, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex, std::mutex& files_mutex);

// Process a file
inline void process_file(InputFile& input_file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex);
