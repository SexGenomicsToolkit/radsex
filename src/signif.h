#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "output.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"


// Get all markers significantly associated with sex and export them in either table or fasta format
void signif(Parameters& parameters);
