/*
* Copyright (C) 2020 Romain Feron
* This file is part of RADSex.

* RADSex is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* RADSex is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with RADSex.  If not, see <https://www.gnu.org/licenses/>.
*/

/** @file */

#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "bwa/bwamem.h"
#include "bwa/kseq.h"
#include "table_parser.h"
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
