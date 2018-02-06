#pragma once
#include <map>
#include <iostream>
#include "analysis.h"
#include "sex_distribution.h"
#include "process_reads.h"
#include "subset.h"

class RadSex {

    public:
        // Command-line arguments are stored as variables to be used by analysis-specific parsers
        int argc;
        char** argv;

        // Store the type of analysis to run detected from the command-line arguments in the RadSex constructor
        Analysis analysis;

        // Store all parameter values and properties inside a list.
        Parameters parameters;

        // Define all analyses
        std::map<std::string, Analysis> analyses { {"sex_distribution", Analysis("sex_distribution",
                                                                                 "Computes a matrix of sequences distribution between sexes",
                                                                                 std::vector<std::string> {"input_file_path", "output_file_path", "min_cov", "popmap_file_path"},
                                                                                 sex_distribution)},
                                                   {"process_reads", Analysis("process_reads",
                                                                              "Computes a matrix of coverage from a set of demultiplexed reads files",
                                                                              std::vector<std::string> {"input_dir_path", "output_file_path", "n_threads", "min_cov"},
                                                                              process_reads)},
                                                   {"subset", Analysis("subset",
                                                                       "Extracts a subset of the coverage matrix",
                                                                       std::vector<std::string> {"input_file_path", "output_file_path", "min_cov", "popmap_file_path",
                                                                                                 "min_males", "min_females", "max_males", "max_females"},
                                                                       subset)}
                                                 };

        // In the constructor, the type of analysis is detected and all analysis objects are initialized
        RadSex(int& argc, char** argv);

        // Parse command-line arguments
        void parse();

        // Runs the selected analysis
        void run();

        // Print the general radsex usage
        void usage();

        // Print usage for the specified analysis
        void usage(Analysis analysis);

};
