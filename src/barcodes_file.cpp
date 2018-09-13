#include "barcodes_file.h"


std::unordered_map<std::string, std::string> load_barcodes(Parameters& parameters) {

    /* Load a barcodes file.
     * Input: a tabulated file with columns Barcode Name
     * Data is stored in a map of structure Barcode (string) Name (string)
     * TODO (maybe): check that barcodes are actually DNA
     */

    std::ifstream file;
    std::string par = "barcodes_file_path";
    file.open(parameters.get_value_from_name<std::string>(par));

    std::unordered_map<std::string, std::string> results;

    if (file) {

        std::string line;
        std::vector<std::string> fields;

        while(std::getline(file, line)) {

            fields = split(line, "\t");
            if (fields.size() == 2) { // Only 2 columns as it should be
                results[fields[0]] = fields[1];
            }
        }

    } else {

        std::cout << "**Error: cannot open barcodes file \"" << parameters.get_value_from_name<std::string>(par) << "\"";
        exit(0);
    }

    return results;
}

