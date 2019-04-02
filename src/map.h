#pragma once
#include "bwa/bwamem.h"
#include "bwa/kseq.h"
#include "output.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"

// Align markers to a reference genome using BWA algorithm and output association with sex with mapping position
void map(Parameters& parameters);

// Compute a table of lengths for all scaffold in a reference genome
void scaffold_lengths(const std::string& genome_file_path);
