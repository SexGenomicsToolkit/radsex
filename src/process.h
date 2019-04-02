#pragma once
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
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

// List of extensions for currently supported file types
const std::vector<std::string> extensions {".fq.gz", ".fq", ".fastq.gz", ".fastq", ".fasta.gz", ".fasta", ".fa.gz", ".fa", ".fna.gz", ".fna"};

// Read a directory
std::vector<InputFile> get_input_files(const std::string& input_dir_path);

// Generate a table of depth for each marker in each individual from a set of demultiplexed files
void process(Parameters& parameters);

// Look for an unprocessed file and process it
void file_processor(std::vector<InputFile>& input_files, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex, std::mutex& files_mutex);

// Process a single file
inline void process_file(InputFile& input_file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex);
