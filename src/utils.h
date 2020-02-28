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

#define LOG_ERROR "ERROR"
#define LOG_WARNING "WARNING"
#define LOG_INFO "INFO"


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
inline void log(T line, const std::string level = LOG_INFO, bool flushline = true) {

    char logtime[DTTMSZ];
    std::string indent((8 - level.size()), ' ');

    std::cerr << "[" << print_time(logtime) << "]::" << level << indent << std::boolalpha << line;
    if (flushline) std::cerr << std::endl;
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



inline std::ifstream open_input(const std::string& input_file_path) {

    std::ifstream input_file(input_file_path);

    if (not input_file.is_open()) {

        log("Could not open input file <" + input_file_path + ">", LOG_ERROR);
        exit(1);

    }

    return input_file;
}



inline std::ofstream open_output(const std::string& output_file_path) {

    std::ofstream output_file(output_file_path);

    if (not output_file.is_open()) {

        log("Could not open output file <" + output_file_path + ">", LOG_ERROR);
        exit(1);

    }

    return output_file;
}



inline void log_progress(uint64_t& n_processed_markers, const uint32_t marker_processed_tick) {

    if (++n_processed_markers % (10 * marker_processed_tick) == 0) {

        log("Processed " + std::to_string(n_processed_markers) + " markers (" + std::to_string(n_processed_markers / (marker_processed_tick)) + " %)");

    }
}



inline std::string get_runtime(const std::chrono::steady_clock::time_point t_begin) {

    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_begin).count();
    long minutes = seconds / 60;
    long hours = minutes / 60;
    std::string runtime = std::to_string(hours) + "h " + std::to_string(minutes%60) + "m " + std::to_string(seconds%60) + "s";

    return runtime;
}



inline void log_progress_bar(uint64_t& n_processed_markers, const uint32_t marker_processed_tick, char symbol = '#', uint16_t ticks = 50) {

    uint tick_size = 100 / ticks;

    if (n_processed_markers % (tick_size * marker_processed_tick) != 0) return;  // Not a step

    uint16_t progress = static_cast<uint16_t>(n_processed_markers / (tick_size * marker_processed_tick));

    std::string bar = "Progress: [";

    for(uint i=0; i < ticks; ++i) {
        i < progress ? bar += symbol : bar += ' ';
    }

    bar += "] - " + std::to_string(progress * tick_size) + "% (" + std::to_string(n_processed_markers) + " markers)";

    progress < ticks ? bar += '\r' : bar += '\n';

    log(bar, LOG_INFO, false);

}


template<typename T>
inline std::string print_list(const T& list, const std::string& sep = ", ") {

    uint n = 0;
    std::string output = "";

    for (auto& field: list) {

        output += field;
        if (++n < list.size()) output += sep;

    }

    return output;
}
