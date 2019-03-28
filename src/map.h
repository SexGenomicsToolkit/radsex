#pragma once
#include "utils.h"
#include "parameters.h"
#include "popmap.h"
#include "output.h"
#include "stats.h"
#include "bwa/bwamem.h"
#include "bwa/kseq.h"


void map(Parameters& parameters);

void scaffold_lengths(const std::string& genome_file_path);
