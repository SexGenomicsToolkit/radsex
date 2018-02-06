#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"


// Load a barcodes file
std::unordered_map<std::string, std::string> load_barcodes(Parameters& parameters);
