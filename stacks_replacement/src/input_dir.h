#pragma once
#include <unordered_map>
#include "parameters.h"
#include "input_file.h"
#include "utils.h"

bool correct_extension(std::string& file_name);
std::vector<InputFile> get_input_files(Parameters& parameters);
