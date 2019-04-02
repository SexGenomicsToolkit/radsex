#pragma once
#include <iostream>
#include <map>
#include <stdio.h>
#include <unordered_map>
#include "cli11/CLI11.hpp"
#include "depth.h"
#include "distrib.h"
#include "freq.h"
#include "loci.h"
#include "map.h"
#include "parameters.h"
#include "process.h"
#include "signif.h"
#include "subset.h"

typedef void (*run_function)(Parameters&);  // Pointer to a function to be used in the analyses function correspondance map

std::string failure_message(const CLI::App* parser, const CLI::Error& error);  // Failure message function for CLI parser

class RADSex {

    // The RADSex class parses CLI arguments and runs the function associated with the specified subcommand.

    public:

        Parameters parameters;  // Structure containing parameters values

        RADSex(int& argc, char** argv);

        CLI::App parser {""};  // Parser instance from CLI App parser

    private:

        // Analyses function correspondance
        std::unordered_map<std::string, run_function> analyses {{"depth", depth},
                                                                {"distrib", distrib},
                                                                {"freq", freq},
                                                                {"loci", loci},
                                                                {"map", map},
                                                                {"process", process},
                                                                {"signif", signif},
                                                                {"subset", subset}
                                                               };

        // Subcommand parser initiators
        void setup_depth_parser();
        void setup_distrib_parser();
        void setup_freq_parser();
        void setup_loci_parser();
        void setup_map_parser();
        void setup_process_parser();
        void setup_signif_parser();
        void setup_subset_parser();

        // Parameter initiators

        // I/O
        void add_markers_table(CLI::App* subparser);
        void add_input_dir(CLI::App* subparser);
        void add_output_file(CLI::App* subparser);
        void add_popmap(CLI::App* subparser);
        void add_genome_file(CLI::App* subparser);
        void add_subset_file(CLI::App* subparser);
        void add_output_matrix(CLI::App* subparser);
        void add_output_fasta(CLI::App* subparser);

        // Shared parameters
        void add_n_threads(CLI::App* subparser);
        void add_min_depth(CLI::App* subparser);
        void add_signif_threshold(CLI::App* subparser);
        void add_bonferroni(CLI::App* subparser);

        // "loci" specific parameters
        void add_loci_max_distance(CLI::App* subparser);
        void add_loci_freq_het(CLI::App* subparser);
        void add_loci_freq_hom(CLI::App* subparser);
        void add_loci_range_het(CLI::App* subparser);
        void add_loci_range_hom(CLI::App* subparser);

        // "subset" specific parameters
        void add_subset_min_males(CLI::App* subparser);
        void add_subset_min_females(CLI::App* subparser);
        void add_subset_max_males(CLI::App* subparser);
        void add_subset_max_females(CLI::App* subparser);
        void add_subset_min_individuals(CLI::App* subparser);
        void add_subset_max_individuals(CLI::App* subparser);

        // "map" specific parameters
        void add_map_min_quality(CLI::App* subparser);
        void add_map_min_frequency(CLI::App* subparser);

};


// Formatter for CLI
class CustomFormatter : public CLI::Formatter {

    public:

        uint column_widths[3] {0, 0, 0};  // Will be used to store the maximum width of each column : flags, type, description
        uint border_width = 4;  // Space between two columns

        // Formatter for an Option line, overrides the same function from CLI::Formatter
        virtual std::string make_option(const CLI::Option* opt, bool is_positional) const {

            std::string option = "", name = "", type = "", description = "", default_value = "", required = "REQUIRED";

            // Generate option name, if positional -> just the name, if not positional -> <short_flag, long_flag>
            is_positional ? name = opt->get_name()[0] : name = "-" + opt->get_snames()[0] + ", --" + opt->get_lnames()[0];
            type = opt->get_type_name();
            description = opt->get_description();
            default_value = opt->get_defaultval();

            // Generate the help string for this option, adding the right number of spaces after each column based on column_widths
            option = name + std::string(this->border_width + this->column_widths[0] - name.size(), ' ');
            option += type + std::string(this->border_width + this->column_widths[1] - type.size(), ' ');
            option += description + std::string(this->border_width + this->column_widths[2] - description.size(), ' ');
            if (opt->get_required()) default_value = required;
            if (default_value != "") option += "[" + default_value + "]";
            option += "\n";

            return option;
        }

};
