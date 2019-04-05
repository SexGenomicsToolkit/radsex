#pragma once
#include <fstream>
#include <mutex>
#include <numeric>
#include <string>
#include <vector>
#include <thread>
#include <unordered_map>
#include "edlib/edlib.h"
#include "output.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"

// Main function implementing the analysis
void loci(Parameters& parameters);
