#include "popmap.h"



Popmap load_popmap(Parameters& parameters) {

    /* Load a popmap file.
     * Input: a tabulated file with columns Individual Group
     * Data is stored in a map {Name: Group}
     */

    std::ifstream popmap_file;
    popmap_file.open(parameters.popmap_file_path);

    Popmap popmap;

    if (popmap_file) {

        std::string line;
        std::vector<std::string> fields;

        while(std::getline(popmap_file, line)) {

            if (line.back() == '\r') line.pop_back();
            fields = split(line, "\t");

            if (fields.size() == 2) { // Only 2 columns as it should be

                popmap.groups[fields[0]] = fields[1];
                ++popmap.counts[fields[1]];

            }
        }

    } else {

        std::cout << "**Error: cannot open popmap file \"" << parameters.popmap_file_path << "\"";
        exit(0);
    }

    if (popmap.groups.size() < 2) {

        std::cerr << "**Error: found only " << popmap.groups.size() << " groups in the popmap, at least two are required" << std::endl;

    } else if (popmap.groups.size() > 2 and (parameters.group1 == "" or parameters.group2 == "")) {

        std::cerr << "**Error: found " << popmap.groups.size() << " groups in the popmap but groups to compare were not defined (use --groups group1,group2)" << std::endl;

    }

    std::cerr << "Successfully loaded popmap (";
    uint n = 1;
    for (auto group: popmap.counts) {
        std::cerr << group.first << ": " << group.second;
        if (n < popmap.counts.size()) std::cerr << ", ";
        ++n;
    }
    std::cerr << ")" << std::endl;

    return popmap;
}

