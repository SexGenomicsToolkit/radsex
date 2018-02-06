#pragma once
#include <mutex>
#include <unordered_map>
#include "gzstream.h"
#include "zlib.h"
#include "input_file.h"
#include "utils.h"

void process_file(InputFile file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex);
