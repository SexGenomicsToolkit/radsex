#include "radsex.h"

RADSex::RADSex(int& argc, char** argv) {

    std::shared_ptr<CustomFormatter> formatter(new CustomFormatter);

    // Main parser options
    this->parser.formatter(formatter);
    this->parser.require_subcommand();  // Check that there is a subcommand
    this->parser.failure_message(failure_message);  // Formatting for error message

    // Initialize subcommand parsers
    this->setup_depth_parser();
    this->setup_distrib_parser();
    this->setup_freq_parser();
    this->setup_loci_parser();
    this->setup_map_parser();
    this->setup_process_parser();
    this->setup_signif_parser();
    this->setup_subset_parser();

    // The parser throws an exception upon failure and implements an exit() method which output an error message and returns the exit code.
    try {

        this->parser.parse(argc, argv);

    } catch (const CLI::ParseError &e) {

        if (this->parser.get_subcommands().size() > 0) {

            std::string tmp = "";
            for (auto opt: this->parser.get_subcommands()[0]->get_options()) {
                opt->get_positional() ? tmp = opt->get_name()[0] : tmp = "-" + opt->get_snames()[0] + ", --" + opt->get_lnames()[0];
                if (tmp.size() > formatter->column_widths[0]) formatter->column_widths[0] = static_cast<uint>(tmp.size());
                tmp = opt->get_type_name();
                if (tmp.size() > formatter->column_widths[1]) formatter->column_widths[1] = static_cast<uint>(tmp.size());
                tmp = opt->get_description();
                if (tmp.size() > formatter->column_widths[2]) formatter->column_widths[2] = static_cast<uint>(tmp.size());
            }

        } else {
            formatter->column_widths[0] = 10;
            formatter->column_widths[1] = 0;
            formatter->column_widths[2] = 50;
        }

        exit(this->parser.exit(e));

    }

    // parser.get_subcommands() returns a vector of subcommands entered by the user, in this case there can only be one.
    CLI::App* subcommand = this->parser.get_subcommands()[0];

    // Set max_<N> for the 'subset' command if max was not specified by the user
    if (subcommand->get_name() == "subset") {
        if (subcommand->count("--max-males")) parameters.set_max_males = false;
        if (subcommand->count("--max-females")) parameters.set_max_females = false;
        if (subcommand->count("--max-individuals")) parameters.set_max_individuals = false;
    }

    // Run the function associated with the selected analysis.
    // Functions are associated to subcommand names in the unordered map "analyses".
    this->analyses[subcommand->get_name()](this->parameters);
}

// Parser initiators

void RADSex::setup_depth_parser() {
    CLI::App* subparser = this->parser.add_subcommand("depth", "Compute number of retained reads for each individual");
    this->add_markers_table(subparser);
    this->add_popmap(subparser);
    this->add_output_file(subparser);
}


void RADSex::setup_distrib_parser() {
    CLI::App* subparser = this->parser.add_subcommand("distrib", "Compute the distribution of markers between males and females");
    this->add_markers_table(subparser);
    this->add_popmap(subparser);
    this->add_output_file(subparser);
    this->add_min_depth(subparser);
    this->add_signif_threshold(subparser);
    this->add_disable_correction(subparser);
    this->add_output_matrix(subparser);
}


void RADSex::setup_freq_parser() {
    CLI::App* subparser = this->parser.add_subcommand("freq", "Compute marker frequencies in the population (regardless of sex)");
    this->add_markers_table(subparser);
    this->add_output_file(subparser);
    this->add_min_depth(subparser);
}


void RADSex::setup_loci_parser() {
    CLI::App* subparser = this->parser.add_subcommand("loci", "Recover alternative alleles from a set of markers based on distance and allele frequencies");
    this->add_subset_file(subparser);
    this->add_markers_table(subparser);
    this->add_output_file(subparser);
    this->add_popmap(subparser);
    this->add_n_threads(subparser);
    this->add_min_depth(subparser);
    this->add_loci_freq_het(subparser);
    this->add_loci_freq_hom(subparser);
    this->add_loci_range_het(subparser);
    this->add_loci_range_hom(subparser);
}


void RADSex::setup_map_parser() {
    CLI::App* subparser = this->parser.add_subcommand("map", "Align markers to a reference genome and compute a probability of association with sex");
    this->add_subset_file(subparser);
    this->add_output_file(subparser);
    this->add_popmap(subparser);
    this->add_genome_file(subparser);
    this->add_min_depth(subparser);
    this->add_map_min_quality(subparser);
    this->add_map_min_frequency(subparser);
    this->add_signif_threshold(subparser);
    this->add_disable_correction(subparser);
}


void RADSex::setup_process_parser() {
    CLI::App* subparser = this->parser.add_subcommand("process", "Compute a table of marker depths from a set of demultiplexed reads files");
    this->add_input_dir(subparser);
    this->add_output_file(subparser);
    this->add_n_threads(subparser);
    this->add_min_depth(subparser);
}


void RADSex::setup_signif_parser() {
    CLI::App* subparser = this->parser.add_subcommand("signif", "Extract markers significantly associated with sex from a marker depths table");
    this->add_markers_table(subparser);
    this->add_output_file(subparser);
    this->add_popmap(subparser);
    this->add_min_depth(subparser);
    this->add_output_fasta(subparser);
    this->add_signif_threshold(subparser);
    this->add_disable_correction(subparser);
}


void RADSex::setup_subset_parser() {
    CLI::App* subparser = this->parser.add_subcommand("subset", "Extract a subset of a marker depths table");
    this->add_markers_table(subparser);
    this->add_output_file(subparser);
    this->add_popmap(subparser);
    this->add_min_depth(subparser);
    this->add_output_fasta(subparser);
    this->add_subset_min_males(subparser);
    this->add_subset_max_males(subparser);
    this->add_subset_min_females(subparser);
    this->add_subset_max_females(subparser);
    this->add_subset_min_individuals(subparser);
    this->add_subset_max_individuals(subparser);
}

// Parameter initiators

void RADSex::add_markers_table(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-t,--markers-table", this->parameters.markers_table_path, "Path to a marker depths table generated by \"process\"");
    option->required();
    option->check(CLI::ExistingFile);
}

void RADSex::add_input_dir(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-i,--input-dir", this->parameters.input_dir_path, "Path to a directory contains demultiplexed sequence files");
    option->required();
    option->check(CLI::ExistingDirectory);
}

void RADSex::add_output_file(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-o,--output-file", this->parameters.output_file_path, "Path to the output file");
    option->required();
}

void RADSex::add_popmap(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-p,--popmap", this->parameters.popmap_file_path, "Path to a population map");
    option->required();
    option->check(CLI::ExistingFile);
}

void RADSex::add_genome_file(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-g,--genome-file", this->parameters.genome_file_path, "Path to a genome file in fasta format");
    option->required();
    option->check(CLI::ExistingFile);
}

void RADSex::add_subset_file(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-s,--subset-file", this->parameters.subset_file_path, "Path to a subset of markers in either table or fasta format");
    option->required();
    option->check(CLI::ExistingFile);
}

void RADSex::add_output_matrix(CLI::App* subparser) {
    subparser->add_flag("-x,--output-matrix", this->parameters.output_matrix, "If set, the distribution will be output as a matrix instead of a table");
}

void RADSex::add_output_fasta(CLI::App* subparser) {
    subparser->add_flag("-a,--output-fasta", this->parameters.output_fasta, "If set, markers will be output in fasta format instead of table format");
}

void RADSex::add_n_threads(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-T,--threads", this->parameters.n_threads, "Number of threads to use", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_min_depth(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-d,--min-depth", this->parameters.min_depth, "Minimum depth to consider a marker present in an individual", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_signif_threshold(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-S,--signif-threshold", this->parameters.signif_threshold, "Significance threshold when assessing significance", true);
    option->check(CLI::Range(0.0, 1.0));
}

void RADSex::add_disable_correction(CLI::App* subparser) {
    subparser->add_flag("-C,--disable-correction", this->parameters.disable_correction, "If set, Bonferroni correction will NOT be used when assessing significance");
}

void RADSex::add_subset_min_males(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-m,--min-males", this->parameters.subset_min_males, "Minimum number of males to retain a marker in the subset", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_subset_min_females(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-f,--min-females", this->parameters.subset_min_females, "Minimum number of females to retain a marker in the subset", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_subset_max_males(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-M,--max-males", this->parameters.subset_max_males, "Maximum number of males to retain a marker in the subset", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_subset_max_females(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-F,--max-females", this->parameters.subset_max_females, "Maximum number of females to retain a marker in the subset", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_subset_min_individuals(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-i,--min-individuals", this->parameters.subset_min_individuals, "Minimum number of individuals to retain a marker in the subset", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_subset_max_individuals(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-I,--max-individuals", this->parameters.subset_max_individuals, "Maximum number of individuals to retain a marker in the subset", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_map_min_quality(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-q,--min-quality", this->parameters.map_min_quality, "Minimum mapping quality to retain a mapped read", true);
    option->check(CLI::Range(0, 60));
}

void RADSex::add_map_min_frequency(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-Q,--min-frequency", this->parameters.map_min_frequency, "Minimum frequency in at least one sex to retain a marker", true);
    option->check(CLI::Range(0.0, 1.0));
}

void RADSex::add_loci_max_distance(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-l,--max-distance", this->parameters.loci_max_distance, "Maximum Levenstein distance between two markers within a locus", true);
    option->check(CLI::Range(1, 9999));
}

void RADSex::add_loci_freq_het(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-E,--freq-het", this->parameters.loci_freq_het, "Expected frequency of an allele in the heterozygous sex", true);
    option->check(CLI::Range(0.0, 1.0));
}

void RADSex::add_loci_freq_hom(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-O,--freq-hom", this->parameters.loci_freq_hom, "Expected frequency of an allele in the homozygous sex", true);
    option->check(CLI::Range(0.0, 1.0));
}

void RADSex::add_loci_range_het(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-r,--range-het", this->parameters.loci_range_het, "Range for the expected frequency of an allele in the heterozygous sex", true);
    option->check(CLI::Range(0.0, 1.0));
}

void RADSex::add_loci_range_hom(CLI::App* subparser) {
    CLI::Option* option = subparser->add_option("-R,--range-hom", this->parameters.loci_range_hom, "Range for the expected frequency of an allele in the homozygous sex", true);
    option->check(CLI::Range(0.0, 1.0));
}



std::string failure_message(const CLI::App* parser, const CLI::Error& error) {

    std::string message = "";

    if (error.what() == std::string("A subcommand is required")) {
        message = "\nSubcommand error: missing or invalid subcommand\n\n" + parser->help();
    } else if (error.get_exit_code() == 106) {  // 106 corresponds to wrong argument type
        message = "\nArgument error: " + std::string(error.what()) + "\n\n" + parser->help();
    } else {
        message = "\nError: " + std::string(error.what()) + "\n\n" + parser->help();
    }

    return message;
}
