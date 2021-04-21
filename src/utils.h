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

/*!
 * @file utils.h
 * @brief Implements general utility functions used.
*/

#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <time.h>
#include <unordered_map>
#include <vector>

#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"   ///< Date and time format for logging
#define MAX_TIME_SIZE 21   ///< Max number of characters in \link TIME_FORMAT \endlink

#define LOG_ERROR "ERROR"   ///< Message prefix for log level ERROR
#define LOG_WARNING "WARNING"   ///< Message prefix for log level WARNING
#define LOG_INFO "INFO"   ///< Message prefix for log level INFO


/*!
 * \brief Print current time
 *
 * \param buff Char array to store current time
 * \return Formatted current time as a char array
 */

inline char* print_time (char* buff) {

    time_t t = time(nullptr);
    strftime (buff, MAX_TIME_SIZE, TIME_FORMAT, localtime (&t));
    return buff;

}





/*!
 * \brief Split a string into a vector of string
 *
 * \param str       The string to split
 * \param separator Separator to split the string by
 *
 * \return The splitted string as vector of strings
 */

inline std::vector<std::string> split(std::string str, const std::string& separator = "\t"){

    std::vector<std::string> output;
    size_t pos;

    while ((pos = str.find(separator)) != std::string::npos){

        output.push_back(str.substr(0, pos));
        str.erase(0, pos + separator.length());
    }

    output.push_back(str.substr(0, pos));   // Push the last element

    return output;
}





/*!
 * \brief Fast stoi conversion
 *
 * I honestly don't know how it works but it's fast.
 *
 * \param str Pointer to a char array to convert to int
 *
 * \return The converted string as an int
 */

inline int fast_stoi(const char* str) {

    int val = 0;

    while (*str) val = val * 10 + (*str++ - '0');

    return val;

}





template<typename T>

/*!
 * \brief Print logging message
 *
 * \param line      Message to print
 * \param level     Log level, either \link LOG_INFO \endlink, \link LOG_WARNING \endlink, or \link LOG_ERROR \endlink
 * \param flushline If true, std::endl is added at the end of the log line
 */

inline void log(T line, const std::string level = LOG_INFO, bool flushline = true, uint float_precision = 2) {

    char logtime[MAX_TIME_SIZE];
    std::string indent((8 - level.size()), ' ');  // Nicely align messages for all log levels

    std::cerr << "[" << print_time(logtime) << "]::" << level << indent << std::boolalpha << std::fixed << std::setprecision(float_precision) << line;
    if (flushline) std::cerr << std::endl;
}





/*!
 * \brief Open an input file and check that the file is open properly
 *
 * \param input_file_path Path to the file to open
 *
 * \return An input file stream for the input file
 */

inline std::ifstream open_input(const std::string& input_file_path) {

    std::ifstream input_file(input_file_path);

    if (not input_file.is_open()) {

        log("Could not open input file <" + input_file_path + ">", LOG_ERROR);
        exit(1);

    }

    return input_file;

}





/*!
 * \brief Open an output file and check that the file is open properly
 *
 * \param output_file_path Path to the file to open
 *
 * \return An output file stream for the output file
 */

inline std::ofstream open_output(const std::string& output_file_path) {

    std::ofstream output_file(output_file_path);

    if (not output_file.is_open()) {

        log("Could not open output file <" + output_file_path + ">", LOG_ERROR);
        exit(1);

    }

    return output_file;

}





/*!
 * \brief Print a progress log message
 *
 * \param n_processed_markers   Number of markers processed
 * \param marker_processed_tick Number of markers corresponding to 1% of total markers
 */

inline void log_progress(uint64_t& n_processed_markers, const uint32_t marker_processed_tick) {

    if (++n_processed_markers % (10 * marker_processed_tick) == 0) {  // Print a message every 10% markers progressed

        log("Processed " + std::to_string(n_processed_markers) + " markers (" + std::to_string(n_processed_markers / (marker_processed_tick)) + " %)");

    }
}





/*!
 * \brief Compute runtime until present time
 *
 * \param t_begin Time point at the start of a measurement
 *
 * \return Runtime until now as a string with format "<H>h <M>m <S>s"
 */

inline std::string get_runtime(const std::chrono::steady_clock::time_point t_begin) {

    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_begin).count();
    long minutes = seconds / 60;
    long hours = minutes / 60;
    std::string runtime = std::to_string(hours) + "h " + std::to_string(minutes%60) + "m " + std::to_string(seconds%60) + "s";

    return runtime;
}





/*!
 * \brief Update a progress bar log message
 *
 * \param n_processed_markers   Number of markers processed
 * \param marker_processed_tick Number of markers corresponding to 1% of total markers
 * \param symbol                Symbol to use to draw the progress bar (e.g. # -> [###...])
 * \param ticks                 Total number of ticks in progress bar (should be a factor of 100)
 */

inline void log_progress_bar(uint64_t& n_processed_markers, const uint32_t marker_processed_tick, char symbol = '#', uint16_t ticks = 50) {

    uint tick_size = 100 / ticks;   // Compute the number of markers in a tick

    if (n_processed_markers % (tick_size * marker_processed_tick) != 0) return;  // Not an update step, stop there

    uint16_t progress = static_cast<uint16_t>(n_processed_markers / (tick_size * marker_processed_tick));   // Compute current progress

    std::string bar = "Progress: [";

    for(uint i=0; i < ticks; ++i) i < progress ? bar += symbol : bar += '.';   // Draw progress bar

    bar += "] - " + std::to_string(progress * tick_size) + "% (" + std::to_string(n_processed_markers) + " markers)";

    progress < ticks ? bar += '\r' : bar += '\n';

    log(bar, LOG_INFO, false);

}





template<typename T>

/*!
 * \brief Print a container as a "sep"-separated string
 *
 * \param list Container to print
 * \param sep  Separator between container elements
 *
 * \return A string of all items in the container separated by sep.
 */

inline std::string print_list(const T& list, const std::string& sep = ", ") {

    uint n = 0;
    std::string output = "";

    for (auto& field: list) {

        output += field;
        if (++n < list.size()) output += sep;

    }

    return output;
}
