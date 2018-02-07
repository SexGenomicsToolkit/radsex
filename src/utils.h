#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <unordered_map>

#define DTTMFMT "%Y-%m-%d %H:%M:%S"
#define DTTMSZ 21


// Output current date and time in format specified with DMTTMFMT and DTTMSZ
char* print_time (char *buff);

// Split a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter);

void rev_comp(const std::string& sequence, std::string& revcomp_sequence, char nuc);
