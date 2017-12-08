#include "output.h"

void write_output(std::ofstream& output_file, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results) {

    uint id = 0;

    output_file << "id" << "\t" << "sequence";
    for (auto i: individuals) {
        output_file << "\t" << i;
    }
    output_file << "\n";

    for (auto r: results) {
        output_file << id << "\t" << r.first;
        for (auto i: individuals) {
            output_file << "\t" << r.second[i];
        }
        output_file << "\n";
        ++id;
    }
}
