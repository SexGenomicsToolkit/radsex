#include "popmap_file.h"



std::unordered_map<std::string, bool> load_popmap(Parameters& parameters) {

    /* Load a popmap file.
     * Input: a tabulated file with columns Name Sex
     * Data is stored in a map of structure Name (string) Male (bool)
     */

    std::ifstream file;
    std::string par = "popmap_file_path";
    file.open(parameters.get_value_from_name<std::string>(par));

    std::unordered_map<std::string, bool> results;

    if (file) {

        std::string line;
        std::vector<std::string> fields;

        while(std::getline(file, line)) {

            if (line.back() == '\r') line.pop_back();  // Deal with windows line return

            fields = split(line, "\t");
            if (fields.size() == 2) { // Only 2 columns as it should be
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

        std::cout << "**Error: cannot open popmap file \"" << parameters.get_value_from_name<std::string>(par) << "\"";
        exit(0);
    }

    return results;
}

