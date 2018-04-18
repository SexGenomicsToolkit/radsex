#include "mapping.h"

void mapping(Parameters& parameters) {

    /* The mapping function maps all sequences from a coverage table (obtained from process, subset, or signif functions) to a reference genome and
     * generates an output file with the following format:
     *
     * SequenceID | Contig | Position | SexBias | P
     *
     * - SequenceID: the ID of the mapped sequence
     * - Contig: name of the contig where the sequence mapped
     * - Position: mapping position on this contig
     * - SexBias: (number of males with the sequence)/(total number of males) - (number of females with the sequence)/(total number of females)
     * - P: probability of association with sex given by chi-squared test with Bonferroni correction.
     *
     * Sequences are mapped using bwa by Heng Li (https://github.com/lh3/bwa). Sequences are mapped using mem_align1() they are considered correctly mapped if:
     * - Their mapping quality is > threshold given by the mapping_quality parameter
     * - They are mapped uniquely, that is, there is no other mapping position with the same mapping score for this sequence.     *
     */

    // Popmap
    std::unordered_map<std::string, bool> popmap = load_popmap(parameters);

    // Input file
    std::string par = "input_file_path";
    std::ifstream input_file;
    input_file.open(parameters.get_value_from_name<std::string>(par));

    if (not input_file) {
        exit(1);
    }

    // Genome file path
    par = "genome_file_path";
    std::string genome_file_path = parameters.get_value_from_name<std::string>(par);

    // Minimum coverage
    par = "min_cov";
    int min_cov = parameters.get_value_from_name<int>(par) - 1; // -1 allows comparison with > instead of >=

    // Minimum mapping quality
    par = "min_quality";
    int min_quality = parameters.get_value_from_name<int>(par) - 1; // -1 allows comparison with > instead of >=

    // Minimum frequency in at least one sex
    par = "min_frequency";
    float min_frequency = parameters.get_value_from_name<float>(par);

    // BWA mem parameters
    mem_opt_t *opt;
    opt = mem_opt_init(); // initialize the BWA-MEM parameters to the default values

    // First line of the input file is the header. The header is parsed to get the sex of each field in the table.
    std::vector<std::string> line;
    std::string temp = "";
    std::getline(input_file, temp);
    line = split(temp, "\t");

    // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
    std::unordered_map<uint, uint> sex_columns;

    // Detection of individuals is based on the popmap, so individuals without sex should still be in the popmap
    for (uint i=0; i<line.size(); ++i) {
        if (popmap.find(line[i]) != popmap.end()) {
            if (popmap[line[i]]) {
                sex_columns[i] = 0; // Male --> column 0
            } else {
                sex_columns[i] = 1; // Female --> column 1
            }
        } else {
            sex_columns[i] = 2; // First and second columns (id and sequence) are counted as no sex
        }
    }

    // Minimum number of males and females
    uint n_males_total = 0, n_females_total = 0;
    for (auto i: popmap) if (i.second) ++n_males_total; else ++n_females_total;
    uint min_males = uint(min_frequency * n_males_total) - 1; // -1 allows comparison with > instead of >=
    uint min_females = uint(min_frequency * n_females_total) - 1; // -1 allows comparison with > instead of >=

    // Generate BWA index if it does not exist
    std::ifstream bwa_index_temp;
    bool indexed = true;
    std::string extensions[5] = {"amb", "ann", "bwa", "pac", "sa"};

    for (auto i=0; i<5; ++i) {
        bwa_index_temp.open(genome_file_path + "." + extensions[i]);
        if (not bwa_index_temp.is_open()) indexed = false;
    }

    scaffold_lengths(genome_file_path);

    if (not indexed) {
        bwa_idx_build(genome_file_path.c_str(), genome_file_path.c_str(), 0, 10000000); // Genome file, prefix, algorithm (default 0), block size (default 10000000)
    }

    // Load BWA index
    bwaidx_t *index; // BWA index read from indexed file

    std::cout << " - Loading BWA index file ..." << std::endl;
    index = bwa_idx_load(genome_file_path.c_str(), BWA_IDX_ALL); // load the BWA index

    if (NULL == index) {
        std::cout << "Failed to load index for genome file \"" + genome_file_path + "\"." << std::endl;
        exit(1);
    }

    // Variables used to read the file
    char buffer[65536];
    std::string sequence, id;
    uint k = 0, field_n = 0, total_n_sequences = 0, retained_sequences = 0;
    uint sex_count[3] = {0, 0, 0}; // Index: 0 = male, 1 = female, 2 = no sex
    int sequence_length = 0;

    // BWA mem objects and variables
    mem_alnreg_v ar;
    mem_aln_t best;
    uint j;
    double chi_squared;
    int best_alignment[3] {0, -1, 0}; // Index, score, count
    MappedSequence seq;
    std::vector<MappedSequence> sequences;

    std::cout << " - Mapping the sequences ..." << std::endl;

    do {

        // Read a chunk of size given by the buffer
        input_file.read(buffer, sizeof(buffer));
        k = input_file.gcount();

        for (uint i=0; i<k; ++i) {

            // Read the buffer character by character
            switch(buffer[i]) {

            case '\r':
                break;
            case '\t':  // New field
                if (sex_columns[field_n] != 2 and std::stoi(temp) > min_cov) ++sex_count[sex_columns[field_n]];  // Increment the appropriate counter
                temp = "";
                ++field_n;
                break;
            case '\n':  // New line (also a new field)
                if (sex_columns[field_n] != 2 and std::stoi(temp) > min_cov) ++sex_count[sex_columns[field_n]];  // Increment the appropriate counter
                if (sex_count[0] > min_males or sex_count[1] > min_females) {
                    ar = mem_align1(opt, index->bwt, index->bns, index->pac, sequence_length, sequence.c_str()); // Map the sequence
                    for (j = 0; j < ar.n; ++j) { // Loop through alignments
                        if (ar.a[j].score > best_alignment[1]) { // Find alignment with best score
                            best_alignment[0] = j;
                            best_alignment[1] = ar.a[j].score;
                            best_alignment[2] = 0;
                        } else if (ar.a[j].score == best_alignment[1]) {
                            ++best_alignment[2];
                        }
                    }
                    best = mem_reg2aln(opt, index->bns, index->pac, sequence_length, sequence.c_str(), &ar.a[best_alignment[0]]); // Get mapping quality
                    if (best_alignment[2] < 1 and best.mapq > min_quality) { // Keep sequences with unique best alignment and with mapq > minimum quality
                        seq.sex_bias = float(sex_count[0]) / float(n_males_total) - float(sex_count[1]) / float(n_females_total); // Sex bias. There should never be 0 males or females in the entire population.
                        chi_squared = get_chi_squared(sex_count[0], sex_count[1], n_males_total, n_females_total);
                        (chi_squared == chi_squared) ? seq.p = get_chi_squared_p(chi_squared) : seq.p = 1.0; // chi square is NaN --> sequence found in all individuals --> set p to 1
                        seq.p < 0.0000000000000001 ? seq.p = 0.0000000000000001 : seq.p = seq.p;
                        seq.id = id;
                        seq.contig = index->bns->anns[best.rid].name;
                        seq.position = best.pos;
                        sequences.push_back(seq);
                        ++retained_sequences;
                    }
                    free(best.cigar); // Deallocate cigar string for best hit
                    free(ar.a); // Deallocate the hit list
                }
                ++total_n_sequences;
                if (total_n_sequences % 100000 == 0 and total_n_sequences / 100000 != 0) std::cout << "   > Processed " << total_n_sequences / 1000 << " K. sequences and retained "
                                                                                                 << retained_sequences / 1000 << " K. sequences." << std::endl;
                // Reset variables
                best_alignment[0] = 0;
                best_alignment[1] = -1;
                best_alignment[2] = 0;
                temp = "";
                sequence = "";
                id = "";
                sequence_length = 0;
                field_n = 0;
                sex_count[0] = 0;
                sex_count[1] = 0;
                break;
            default:
                temp += buffer[i];
                if (field_n == 0) id += buffer[i];
                if (field_n == 1) {
                    sequence += buffer[i];
                    ++sequence_length;
                }
                break;
            }
        }
    } while (input_file);

    par = "output_file_path";
    std::string output_file_path = parameters.get_value_from_name<std::string>(par);

    // Generate the output file
    output_mapping(output_file_path, sequences);

    input_file.close();
    free(opt);
    bwa_idx_destroy(index);
}
