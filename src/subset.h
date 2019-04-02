#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "output.h"
#include "parameters.h"
#include "popmap.h"
#include "utils.h"


// Extract a subset of markers from a marker depths table based on number of individuals
void subset(Parameters& parameters);
