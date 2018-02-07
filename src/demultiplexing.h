#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <zlib.h>
#include "utils.h"
#include "parameters.h"
#include "barcodes_file.h"
#include "kseq/kseq.h"


// Main function implementing the analysis
void demultiplexing(Parameters& parameters);
