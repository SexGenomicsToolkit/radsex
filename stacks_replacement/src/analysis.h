#pragma once
#include <thread>
#include "input_dir.h"
#include "process_file.h"
#include "output.h"

void analysis(Parameters& parameters);

void file_processor(std::vector<InputFile>& input_files, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex, std::mutex& files_mutex);
