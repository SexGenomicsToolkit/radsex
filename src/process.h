/*
* Copyright (C) 2020 Romain Feron
* This file is part of RADSex.

* RADSex is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* RADSex is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with RADSex.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <zlib.h>
#include "kseq/kseq.h"
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
