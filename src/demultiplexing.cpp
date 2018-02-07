#include "demultiplexing.h"

KSEQ_INIT(gzFile, gzread) // Kseq init macro

void demultiplexing(Parameters &parameters) {

    /* Demultiplexing is actually not a trivial task if done correctly.
     * It may get implemented later but for now this pipeline relies on Stacks for this step.
     */

    const std::string sbf1 = "TGCA";

    const std::unordered_map<std::string, std::string> barcodes = load_barcodes(parameters);

    std::string par = "output_dir_path";
    std::string output_dir_path = parameters.get_value_from_name<std::string>(par);

    std::ofstream log_file(output_dir_path + "demultiplexing.log");
    std::ofstream summary_file(output_dir_path + "summary.tsv");

    std::unordered_map<std::string, uint64_t> statistics;

    std::unordered_map<std::string, gzFile> output_files;

    // Output files are mapped to their barcodes.
    std::string output_file_path;
    for (auto b: barcodes) {
        output_file_path = output_dir_path + b.second + ".fq.gz";
        // Create a compressed output file, the number after wb indicates the level of compression (1 -> low, 9 -> high)
        output_files[b.first] = gzopen(output_file_path.c_str(), "wb9");
        if(!output_files[b.first]) std::cout << std::endl << "** Error: could not open output file \"" << output_file_path << "\"" << std::endl;
    }

    gzFile input_file;
    kseq_t* sequence; // Create the seq object
    int line_n; // Stores the length of a line
    std::string revcomp = "";
    char nuc = 'A';
    uint count;

    par = "input_file_path";
    input_file = gzopen(parameters.get_value_from_name<std::string>(par).c_str(), "r");
    sequence = kseq_init(input_file); // Initialize the seq object

    // Read through the file and store the results
    while ((line_n = kseq_read(sequence)) >= 0) {
        rev_comp(sequence->seq.s, revcomp, nuc);
        ++count;
    }

    kseq_destroy(sequence); // Destroy the seq object
    gzclose(input_file);

    // Close all output files
    for(auto& ofile: output_files) {
        gzclose(ofile.second);
    }
}
