#include "depth.h"
#include <iostream>

void depth(Parameters& parameters) {

    std::unordered_map<std::string, bool> popmap = load_popmap(parameters);

    std::ifstream input_file;
    input_file.open(parameters.markers_table_path);

    if (input_file.is_open()) {

        std::vector<std::string> line;
        std::string temp = "";

        // First line is a comment with number of markers in the table
        std::getline(input_file, temp);
        line = split(temp, " : ");
        if (line.size() == 2) uint n_markers = static_cast<uint>(std::stoi(line[1]));

        // Second line is the header. The header is parsed to get the sex of each field in the table.
        std::getline(input_file, temp);
        line = split(temp, "\t");

        // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
        std::unordered_map<uint, uint> sex_columns = get_column_sex(popmap, line);

        // Total number of reads in each individual
        std::unordered_map<std::string, uint> individual_depths;

        // Individual depths for a single marker
        std::unordered_map<std::string, ulong> marker_depths;

        // Table of depth frequencies
        std::unordered_map<std::string, std::vector<ulong>> median_depth;

        // Define variables used to read the file
        char buffer[65536];
        uint k = 0, field_n = 0;
        ulong depth = 0;
        uint n_individuals = 0;

        do {

            // Read a chunk of size given by the buffer
            input_file.read(buffer, sizeof(buffer));
            k = static_cast<uint>(input_file.gcount());

            for (uint i=0; i<k; ++i) {

                // Read the buffer character by character
                switch(buffer[i]) {

                    case '\t':  // New field
                        if (sex_columns[field_n] != 2) {
                            depth = std::min(static_cast<ulong>(500), std::stoul(temp));
                            individual_depths[line[field_n]] += depth;
                            marker_depths[line[field_n]] = depth;
                            if(depth > 0) ++n_individuals;
                        }
                        temp = "";
                        ++field_n;
                        break;

                    case '\n':  // New line (also a new field)
                        if (sex_columns[field_n] != 2) {
                            depth = std::min(static_cast<ulong>(500), std::stoul(temp));
                            individual_depths[line[field_n]] += depth;
                            marker_depths[line[field_n]] = depth;
                            if(depth > 1) ++n_individuals;
                        }

                        if (n_individuals == marker_depths.size()) {  // Only keep markers present in all individuals to estimate expected depths
                            for (auto m: marker_depths) {
                                median_depth[m.first].push_back(m.second);
                            }
                        }
                        // Reset variables
                        temp = "";
                        field_n = 0;
                        n_individuals = 0;
                        break;

                    default:
                        temp += buffer[i];
                        break;
                }
            }

        } while (input_file);

        input_file.close();

        std::ofstream output_file;
        output_file.open(parameters.output_file_path);

        if (output_file.is_open()) {

            output_file << "Individual\tSex\tMarkers\tDepth\n";

            for (auto i: median_depth) {

                output_file << i.first << "\t";
                (popmap[i.first]) ? output_file << "M" : output_file << "F";
                output_file << "\t" << individual_depths[i.first] << "\t";

                std::sort(i.second.begin(), i.second.end()); // Sort depth vector for easy median finding

                if (i.second.size() % 2 == 0) {  // Find median for an even sized vector (average of two possible median points)

                    const auto median_it1 = i.second.begin() + i.second.size() / 2 - 1;
                    const auto median_it2 = i.second.begin() + i.second.size() / 2;

                    std::nth_element(i.second.begin(), median_it1 , i.second.end());
                    const auto e1 = *median_it1;

                    std::nth_element(i.second.begin(), median_it2 , i.second.end());
                    const auto e2 = *median_it2;

                    output_file << (e1 + e2) / 2 << "\n";

                } else {  // Find median for odd sized vector

                    const auto median_it = i.second.begin() + i.second.size() / 2;
                    std::nth_element(i.second.begin(), median_it , i.second.end());
                    output_file << *median_it << "\n";
                }

            }
        }
    }
}
