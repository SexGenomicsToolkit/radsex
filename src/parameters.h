#pragma once
#include "parameter.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>


struct Parameters {

    /* The Parameters structure store all parameters to be used in the analyses and provides a few methods
     * to access these parameters easily.
     */

    // Initialize all possible parameters to be used in the analyses
    // Arguments: name, help message, flag, default value, type, internal type, value, required
    // Flags : -h, -f, -d, -o, -u, -t, -c, -p, -b, -a, -m, -g, --min-males, --min-females, --max-males, --max-females, --min-individuals, --output-matrix, -q,
    // Parameter constructor: Parameter(name, description, flag, default_value, type, internal_type, value, required)
    std::vector<Parameter> list {Parameter("help", "Prints this message", "-h", "0", "bool", "bool", "", false),
                                 Parameter("input_file_path", "Path to an input file", "-f", "", "string", "ifile", "", true),
                                 Parameter("input_dir_path", "Path to an input directory", "-d", "", "string", "dir", "", true),
                                 Parameter("output_file_path", "Path to an output file", "-o", "", "string", "ofile", "", true),
                                 Parameter("output_dir_path", "Path to an output directory", "-u", "", "string", "dir", "", true),
                                 Parameter("coverage_matrix_path", "Path to an coverage matrix file", "-a", "", "string", "ifile", "", true),
                                 Parameter("n_threads", "Number of threads", "-t", "1", "int", "int", "", false),
                                 Parameter("min_cov", "Minimum coverage to consider a marker", "-c", "1", "int", "int", "", false),
                                 Parameter("max_distance", "Maximum Levenstein distance between two sequences in a locus", "-m", "1", "int", "int", "", false),
                                 Parameter("popmap_file_path", "Path to a popmap file", "-p", "", "string", "ifile", "", true),
                                 Parameter("barcodes_file_path", "Path to a barcodes file", "-b", "", "string", "ifile", "", true),
                                 Parameter("genome_file_path", "Path to a genome file in fasta format", "-g", "", "string", "ifile", "", true),
                                 Parameter("min_males", "Minimum number of males in the subset", "--min-males", "0", "int", "int", "", false),
                                 Parameter("min_females", "Minimum number of females in the subset", "--min-females", "0", "int", "int", "", false),
                                 Parameter("max_males", "Maximum number of males in the subset", "--max-males", "n.males", "int", "int", "", false),
                                 Parameter("max_females", "Maximum number of females in the subset", "--max-females", "n.females", "int", "int", "", false),
                                 Parameter("min_individuals", "Minimum number of individuals in the subset (overrides sex parameters)", "--min-individuals", "0", "int", "int", "", false),
                                 Parameter("max_individuals", "Maxmimum number of individuals in the subset (overrides sex parameters)", "--max-individuals", "n.individual", "int", "int", "", false),
                                 Parameter("output_matrix", "Output the sex distribution table as a matrix", "--output-matrix", "0", "bool", "bool", "", false),
                                 Parameter("min_quality", "Minimum mapping quality to keep a mapped read", "-q", "20", "int", "int", "", false),
                                 Parameter("min_frequency", "Minimum frequency of a sequence in at least one sex.", "--min-frequency", "0.25", "float", "float", "", false)
                                 };


    // Return a parameter from its flag
    Parameter get_from_flag(std::string& flag) {
        for (auto p: this->list) {
            if (p.flag == flag) return p;
        }
        return Parameter(); // If the parameter was not found (meaning it's not a valid parameter) a dummy is returned, to be tested with "is_real()" method from the Parameter structure
    }


    // Check if a flag is in the list of parameters
    bool find_flag(std::string& flag) {
        for (auto p: this->list) {
            if (p.flag == flag) return true;
        }
        return false;
    }


    // Return a parameter from its name
    Parameter get_from_name(std::string& name) {
        for (auto p: this->list) {
            if (p.name == name) return p;
        }
        return Parameter(); // If the parameter was not found (meaning it's not a valid parameter) a dummy is returned, to be tested with "is_real()" method from the Parameter structure
    }


    // Check if a parameter name is in the list of parameters
    bool find_name(std::string& name) {
        for (auto p: this->list) {
            if (p.name == name) return true;
        }
        return false;
    }


    // Check if a parameter is required for the analysis
    bool is_required(std::string& name) {
        return this->get_from_name(name).required;
    }


    // Get the value of a parameter from its name
    template<typename T>
    T get_value_from_name(std::string& name) {
        T output;
        for (auto p: this->list) {
            if (p.name == name) {
                std::string v = p.value;
                std::stringstream ss(v);
                if (ss >> output) return output;
                // TODO: Handle fail case
            }
        }
        std::cout << " ** Error: looking for the value of parameter \"" << name <<"\" which does not exist." << std::endl;
        exit(0);
    }


    // Get the value of a parameter from its name
    template<typename T>
    T get_value_from_flag(std::string& flag) {
        for (auto p: this->list) {
            if (p.flag == flag) {
                if (p.internal_type == "bool") {
                    return p.value != "0"; // TODO: handle more bool values in the future
                } else if (p.internal_type == "int") {
                    return std::stoi(p.value);
                } else if (p.internal_type == "float") {
                    return std::stod(p.value);
                } else {
                    return p.value;
                }
            }
        }
        std::cout << " ** Error: looking for the value of parameter \"" << flag <<"\" which does not exist." << std::endl;
        exit(0);
    }


    // Set the value of a parameter from its flag
    void set_value_from_flag(std::string& flag, std::string& value) {
        for (auto it = this->list.begin(); it != this->list.end(); ++it) {
            if ((*it).flag == flag) {
                (*it).set_value(value);
                return;
            }
        }
        std::cout << "Error: trying to set the value " << value << " for parameter " << flag << " which does not exist." << std::endl;
        exit(0);
    }


    // Set the value of a parameter from its flag
    void set_value_from_name(std::string& name, std::string& value) {
        for (auto it = this->list.begin(); it != this->list.end(); ++it) {
            if ((*it).name == name) {
                (*it).set_value(value);
                return;
            }
        }
        std::cout << "Error: trying to set the value " << value << " for parameter " << name << " which does not exist." << std::endl;
        exit(0);
    }


    // Print information about parameters
    void print() {
        std::cout << std::endl;
        for (auto parameter: this->list) {
            std::cout << parameter.flag << "\t" << parameter.name << "\t" << parameter.description << "\t" << parameter.value << "\t" << parameter.default_value << "\t" <<
                         parameter.type << "\t" << parameter.internal_type << "\t" << parameter.required << "\n";
        }
        std::cout << "\n";
    }


    // Print information about specific parameters given by a list
    void print(std::vector<std::string>& parameters_to_print) {
        std::cout << std::endl;
        for (auto parameter: this->list) {
            if (std::find(parameters_to_print.begin(), parameters_to_print.end(), parameter.name) != parameters_to_print.end()) {
                std::cout << parameter.flag << "\t" << parameter.name << "\t" << parameter.description << "\t" << parameter.value << "\t" << parameter.default_value << "\t" <<
                             parameter.type << "\t" << parameter.internal_type << "\t" << parameter.required << "\n";
            }
        }
        std::cout << "\n";
    }

    // Print information about parameters given by a list
    void simple_print(std::vector<std::string>& parameters_to_print) {
        std::cout << std::endl;
        for (auto parameter: this->list) {
            if (std::find(parameters_to_print.begin(), parameters_to_print.end(), parameter.name) != parameters_to_print.end()) {
                std::cout << parameter.name << " :\t" << parameter.value << std::endl;
            }
        }
        std::cout << "\n";
    }
};
