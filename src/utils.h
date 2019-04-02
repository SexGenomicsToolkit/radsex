#pragma once
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>
#define DTTMFMT "%Y-%m-%d %H:%M:%S"
#define DTTMSZ 21

// Store information about a locus for the "loci" subcommand
struct Locus {
    std::string id;
    std::string sequence;
    std::vector<std::string> coverage;
};

// Store information about a mapped sequence for the "map" subcommand
struct MappedSequence {
    std::string id;
    std::string contig;
    int64_t position;
    float sex_bias;
    double p;
};

// Store sex distribution results
typedef std::unordered_map<uint, std::unordered_map<uint, std::pair<uint64_t, double>>> sd_table;

// Output current date and time in format specified with DMTTMFMT and DTTMSZ
char* print_time (char *buff);

// Split a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter);

// Reverse complement of a sequence
void rev_comp(const std::string& sequence, std::string& revcomp_sequence);

// Create a sex <-> column index correspondance map
std::unordered_map<uint, uint> get_column_sex(std::unordered_map<std::string, bool>& popmap, const std::vector<std::string>& header);
