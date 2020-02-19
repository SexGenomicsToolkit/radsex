#include "popmap.h"



std::unordered_map<std::string, bool> load_popmap(Parameters& parameters) {

    /* Load a popmap file.
     * Input: a tabulated file with columns Name Sex
     * Data is stored in a map of structure Name (string) Male (bool)
     */

    std::ifstream popmap;
    popmap.open(parameters.popmap_file_path);

    std::unordered_map<std::string, bool> results;

    if (popmap) {

        std::string line;
        std::vector<std::string> fields;

        while(std::getline(popmap, line)) {

            fields = split(line, "\t");
            if (fields.size() == 2) { // Only 2 columns as it should be
                if (line.back() == '\r') line.pop_back();
                if (fields[1] == "F") {
                    results[fields[0]] = false; // Female --> bool set to false
                } else if (fields[1] == "M") {
                    results[fields[0]] = true; // Male --> bool set to true
                } else if (fields[1] != "N") {
                    // N can be used when sex is unknown, other cases are errors
                    std::cout << "**Error: in popmap file, unknown sex \"" << fields[1] << "\" for individual \"" << fields[0] << "\"" << std::endl;
                    exit(0);
                }
            }
        }

    } else {

        std::cout << "**Error: cannot open popmap file \"" << parameters.popmap_file_path << "\"";
        exit(0);
    }

    return results;
}

