#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "bwa/bwamem.h"
#include "bwa/kseq.h"
#include "depth_table.h"
#include "output.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"

// Store information about a mapped sequence for the "map" subcommand
struct AlignedMarker {
    std::string id;
    std::string contig;
    int64_t position;
    float bias;
    double p;
};

// Align markers to a reference genome using BWA algorithm and output association with sex with mapping position
void map(Parameters& parameters);

//
void processor(MarkersQueue& markers_queue, Parameters& parameters, Popmap& popmap, std::mutex& queue_mutex, std::vector<AlignedMarker>& aligned_markers, bool& parsing_ended, ulong batch_size);

// Get length of each contig in a fasta file
std::unordered_map<std::string, uint64_t> get_contig_lengths(const std::string& genome_file_path);

//
void build_bwa_index(Parameters& parameters);
