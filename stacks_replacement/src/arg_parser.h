#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include "parameters.h"


class ArgParser {

    public:

        // Options: flag -> [default, type, help message]
        std::map<std::string, std::vector<std::string>> const options { {"-h", {"0", "bool", "Prints this message"} },
                                                                        {"-t", {"1", "int", "Number of threads"} },
                                                                        {"-i", {"", "string", "Input directory"} },
                                                                        {"-o", {"", "string", "Output file"} }
                                                                      };

        ArgParser(int& argc, char** argv);

        void set_parameters(Parameters& parameters);

        const std::string get_value(const std::string& setting) const;

        bool contains(const std::string &option) const ;

        const std::string set_value(const std::string& field);

        void usage();

        void print_parameters();

    private:

        std::vector<std::string> fields;
};
