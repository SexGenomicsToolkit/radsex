#include "scaffold_lengths.h"

void scaffold_lengths(const std::string& genome_file_path) {

    std::ifstream genome_file(genome_file_path);
    std::ofstream lengths_file(genome_file_path + ".lengths");
    std::string line, scaffold_name;
    uint scaffold_length = 0;
    bool start = true;

    while(std::getline(genome_file, line)) {
        if (line[0] == '>') {
            if (not start) {
                lengths_file << scaffold_name << "\t" << scaffold_length << "\n";
            } else {
                start = false;
            }
            scaffold_name = split(line, " ")[0];
            scaffold_name = scaffold_name.substr(1, scaffold_name.size());
            scaffold_length = 0;
        } else {
            scaffold_length += line.size();
        }
    }

    lengths_file << scaffold_name << "\t" << scaffold_length << "\n";
    genome_file.close();
    lengths_file.close();
}
