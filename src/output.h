#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>


// Create output file for the process reads analysis
void output_process_reads(std::string& output_file_path, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results);

// Create output file for the sex_distribution analysis
void output_sex_distribution(std::string& output_file_path, std::unordered_map<uint, std::unordered_map<uint, uint>>& results);
