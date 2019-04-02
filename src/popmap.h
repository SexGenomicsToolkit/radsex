#pragma once
#include <unordered_map>
#include "parameters.h"
#include "utils.h"

// Load a popmap file
std::unordered_map<std::string, bool> load_popmap(Parameters& parameters);
