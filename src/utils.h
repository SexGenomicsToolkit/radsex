#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <time.h>
#include <unordered_map>
#include <vector>
#define DTTMFMT "%Y-%m-%d %H:%M:%S"
#define DTTMSZ 21

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


// Output current date and time in format specified in utils.h
inline char* print_time (char *buff) {

    time_t t = time (nullptr);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}



// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
inline std::vector<std::string> split(std::string str, const std::string delimiter){

    std::vector<std::string> output;
    size_t pos;

    while ((pos = str.find(delimiter)) != std::string::npos){

        output.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }

    output.push_back(str.substr(0, pos));

    return output;
}


// Faster string to int conversion
inline int fast_stoi(const char* str) {

    int val = 0;
    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}


// Log output formatting
template<typename T>
inline void log(T line) {

    char logtime[DTTMSZ];
    std::cerr << "[" << print_time(logtime) << "]" << "  ";
    std::cerr << std::boolalpha << line << std::endl;

}



inline std::vector<std::string> get_column_sex(std::unordered_map<std::string, std::string>& popmap, const std::vector<std::string>& header) {

    // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
    std::vector<std::string> sex_columns;

    for (uint i=0; i<header.size(); ++i) {

        if (popmap.find(header[i]) != popmap.end()) {

            sex_columns.push_back(popmap[header[i]]);

        } else {

            sex_columns.push_back(""); // First and second columns (id and sequence) are not counted

        }
    }

    return sex_columns;
}


inline const std::vector<std::string> get_header(const std::string& input_file_path) {

    std::string line = "#";
    std::vector<std::string> header;

    std::ifstream input_file;
    input_file.open(input_file_path);

    if (not input_file.is_open()) {
        std::cerr << "**Error: could not open input file <" << input_file_path << ">" << std::endl;
        exit(1);
    }

    while (line[0] == '#') {
        std::getline(input_file, line);
    }

    input_file.close();

    header = split(line, "\t");
    return header;
}
