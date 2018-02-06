#include "output.h"
#include <iostream>

void output_process_reads(std::string& output_file_path, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results) {

    /* Input:
     * - Path to an output file
     * - A list of individual names
     * - A matrix of results [Individual: [Sequence, Coverage]]
     * Output:
     * - A matrix of coverage [Individual: [Sequence, Coverage]]
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    uint id = 0;

    // Prints the header
    output_file << "id" << "\t" << "sequence";
    for (auto i: individuals) {
        output_file << "\t" << i;
    }
    output_file << "\n";

    // Fill line by line
    for (auto r: results) {
        output_file << id << "\t" << r.first;
        for (auto i: individuals) {
            output_file << "\t" << r.second[i];
        }
        output_file << "\n";
        ++id;
    }
}



void output_sex_distribution(std::string& output_file_path, std::unordered_map<uint, std::unordered_map<uint, uint>>& results) {

    /* Input:
     * - Path to an output file
     * - A matrix of counts [Males: [Females: Count]]
     * Output:
     * - A matrix of counts (males in columns and females in rows)
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    uint n_males = results.size(), n_females = results[0].size(), i = 0;

    for (uint f=0; f < n_females; ++f) {
        for (uint m=0; m < n_males; ++m) {
            output_file << results[m][f];
            if (i < n_females - 1) output_file << "\t";
            ++i;
        }
        output_file << "\n";
        i=0;
    }
}
