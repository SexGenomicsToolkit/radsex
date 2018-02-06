#pragma once
#include <string>
#include <unordered_map>
#include <zlib.h>
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
