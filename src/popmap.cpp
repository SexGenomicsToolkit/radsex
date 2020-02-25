#include "popmap.h"

Popmap load_popmap(Parameters& parameters, bool compare) {

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
                ++popmap.n_individuals;

            }
        }

    } else {

        log("Could not open popmap file <" + parameters.popmap_file_path + ">", LOG_ERROR);
        exit(1);
    }

    if (compare and popmap.counts.size() < 2) {  // Fewer than two groups in the popmap

        log("Found <" + std::to_string(popmap.counts.size()) + "> groups in the popmap file (" + print_groups(popmap) + ") but at least two are required", LOG_ERROR);
        exit(1);

    } else if (compare and popmap.counts.size() > 2 and (parameters.group1 == "" or parameters.group2 == "")) {  // More than two groups but groups to compare were not defined

        log("Found <" + std::to_string(popmap.counts.size()) + "> groups in the popmap file (" + print_groups(popmap) + ") but groups to compare were not defined (use --groups group1,group2)", LOG_ERROR);
        exit(1);

    } else if (compare and popmap.counts.size() > 2 and (popmap.counts.find(parameters.group1) == popmap.counts.end() or popmap.counts.find(parameters.group2) == popmap.counts.end())) {  // More than two groups and groups to compare were not found

        log("Groups specified with --groups (\"" + parameters.group1 + "\", \"" + parameters.group2 + "\") were not found in popmap groups (" + print_groups(popmap) + ")", LOG_ERROR);
        exit(1);

    } else if (compare and popmap.counts.size() == 2) {  // Only two groups in popmap

        if (parameters.group1 == "" and parameters.group2 == "") {  // Fill groups from popmap if not specified by the user

            auto i = std::begin(popmap.counts);
            parameters.group1 = i->first;
            parameters.group2 = (++i)->first;

        }
    }

    std::string popmap_success_message = "Loaded popmap (" + print_groups(popmap, true) + ")";

    log(popmap_success_message);

    return popmap;
}



std::string print_groups(Popmap& popmap, bool counts) {

    std::string list = "";
    uint n = 0;

    for (auto group: popmap.counts) {
        list += "\"" + group.first + "\"";
        if (counts) list += ": " + std::to_string(group.second);
        if (++n < popmap.counts.size()) list += ", ";
    }

    return list;
}
