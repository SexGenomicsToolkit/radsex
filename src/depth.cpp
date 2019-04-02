#include "depth.h"

void depth(Parameters& parameters) {

    std::unordered_map<std::string, bool> popmap = load_popmap(parameters);

    std::ifstream input_file;
    input_file.open(parameters.markers_table_path);

    if (input_file.is_open()) {

        // First line is the header. The header is parsed to get the sex of each field in the table.
        std::vector<std::string> line, header;
        std::string temp = "";
        std::getline(input_file, temp);
        header = split(temp, "\t");

        // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
        std::unordered_map<uint, uint> sex_columns = get_column_sex(popmap, line);

        std::unordered_map<std::string, uint> individual_depths;

        // Define variables used to read the file
        char buffer[65536];
        uint k = 0, field_n = 0;
        sd_table results;

        do {

            // Read a chunk of size given by the buffer
            input_file.read(buffer, sizeof(buffer));
            k = static_cast<uint>(input_file.gcount());

            for (uint i=0; i<k; ++i) {

                // Read the buffer character by character
                switch(buffer[i]) {

                    case '\t':  // New field
                        if (sex_columns[field_n] != 2) individual_depths[header[field_n]] += std::stoul(temp);  // Increment the appropriate counter
                        temp = "";
                        ++field_n;
                        break;

                    case '\n':  // New line (also a new field)
                        if (sex_columns[field_n] != 2) individual_depths[header[field_n]] += std::stoul(temp);  // Increment the appropriate counter
                        // Reset variables
                        temp = "";
                        field_n = 0;
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

            output_file << "Individual\tSex\tMarkers\n";

            for (auto i: individual_depths) {
                output_file << i.first << "\t";
                (popmap[i.first]) ? output_file << "M" : output_file << "F";
                output_file << "\t" << i.second << '\n';
            }
        }
    }
}
