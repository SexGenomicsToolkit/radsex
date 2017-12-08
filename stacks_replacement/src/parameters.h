#pragma once
#include <string>
#include <fstream>
#include "dirent.h"

struct Parameters{

    uint16_t n_threads;
    std::string input_dir_path;
    DIR* input_dir;
    std::string output_file_path;
    std::ofstream output_file;
};
