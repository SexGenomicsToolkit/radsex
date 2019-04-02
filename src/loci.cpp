#include "loci.h"

void loci(Parameters& parameters) {

    std::mutex results_mutex;

    std::unordered_map<std::string, std::vector<Locus>> results;

    std::vector<std::string> header;

//    std::cout << " - Loading subset of loci :" << std::endl;
//    par = "input_file_path";
//    std::string input_file_path = parameters.get_value_from_name<std::string>(par);
//    std::vector<Locus> sequences = load_coverage_matrix(input_file_path, 1, false, header);

//    std::cout << " - Loading coverage table :" << std::endl;

//    std::vector<Locus> coverage_matrix = load_coverage_matrix(coverage_matrix_path, min_cov, true, header);

//    par = "max_distance";
//    int max_distance = parameters.get_value_from_name<int>(par) + 1; // +1 to compare with < instead of <= later

//    std::cout << std::endl << " - Starting alignments" << std::endl;

//    auto sequence = sequences.begin();

//    // Create and run all sequence processors
//    par = "n_threads";
//    std::vector<std::thread> threads;
//    for (int i=0; i<parameters.get_value_from_name<int>(par); ++i) {
//        threads.push_back(std::thread(sequence_processor, std::ref(sequence), std::ref(sequences), std::ref(coverage_matrix), std::ref(results), std::ref(results_mutex), max_distance));
//    }

//    for (auto &t : threads) t.join();

//    par = "output_file_path";
//    std::string output_file_path = parameters.get_value_from_name<std::string>(par);
//    output_group_loci(output_file_path, results, header);
}



std::vector<Locus> load_coverage_matrix(std::string& file_path, int min_cov, bool print, std::vector<std::string>& header, float freq_het, float freq_hom, float range_het, float range_hom) {

    /* Load a coverage matrix in memory
     */

    std::ifstream input_file(file_path);
    std::vector<Locus> coverage_matrix;

    if (input_file) {

        // First line is the header. The header is used when printing the output.
        std::string temp;
        std::getline(input_file, temp);
        header = split(temp,"\t");
        temp = "";

        // Define variables used to read the file
        char buffer[65536];
        uint k = 0, field_n = 0, seq_count = 0;
        Locus locus;
        bool keep_locus = false;

        do {

            // Read a chunk of size given by the buffer
            input_file.read(buffer, sizeof(buffer));
            k = static_cast<uint>(input_file.gcount());

            for (uint i=0; i<k; ++i) {

                // Read the buffer character by character
                switch(buffer[i]) {

                    case '\t':  // New field

                        switch(field_n) {
                            case 0:
                                locus.id = temp;
                                break;
                            case 1:
                                locus.sequence =temp;
                                break;
                            default:
                                locus.coverage.push_back(temp);
                                break;
                        }

                        temp = "";
                        ++field_n;
                        break;

                    case '\n':  // New line (also a new field)

                        locus.coverage.push_back(temp);
                        keep_locus = false;

                        for (auto c: locus.coverage) {
                            if (std::stoi(c) > min_cov) {
                                keep_locus = true;
                                break;
                            }
                        }



                        if (keep_locus) coverage_matrix.push_back(locus);
                        if (seq_count % 1000000 == 0 and seq_count > 0 and print) std::cout << "   - Processed " << seq_count / 1000000 <<
                                                                                               " M. lines and stored " << coverage_matrix.size() << " sequences\n";
                        // Reset variables
                        locus.id = "";
                        locus.sequence = "";
                        locus.coverage.resize(0);
                        temp = "";
                        field_n = 0;
                        ++seq_count;
                        break;

                    default:
                        temp += buffer[i];
                        break;
                }
            }
        } while (input_file);
    }
    return coverage_matrix;
}




void sequence_processor(std::vector<Locus>::iterator& sequence, std::vector<Locus>& sequences, std::vector<Locus>& coverage_matrix,
                        std::unordered_map<std::string, std::vector<Locus>>& results, std::mutex& results_mutex, int max_distance) {

    while(sequence != sequences.end()) {

        results_mutex.lock();
        Locus locus = *sequence;
        ++sequence;
        results_mutex.unlock();
        process_sequence(locus, coverage_matrix, results, results_mutex, max_distance);
    }

}



void process_sequence(Locus& locus, std::vector<Locus>& coverage_matrix, std::unordered_map<std::string, std::vector<Locus>>& results, std::mutex& results_mutex, int max_distance) {

    uint seq_len = static_cast<uint>(locus.sequence.size());
    std::unordered_map<std::string, std::vector<Locus>> temp_results;
    const char* seq = locus.sequence.c_str();

    EdlibAlignResult alignment_result;
    for (auto l: coverage_matrix) {
        alignment_result = edlibAlign(seq, seq_len, l.sequence.c_str(), seq_len, edlibDefaultAlignConfig());
        if (alignment_result.status == EDLIB_STATUS_OK and alignment_result.editDistance < max_distance) {
            temp_results[locus.id].push_back(l);
        }
        edlibFreeAlignResult(alignment_result);
    }

    results_mutex.lock();
    for (auto l: temp_results) results[l.first] = l.second;
    results_mutex.unlock();
}


void filter(std::unordered_map<std::string, std::vector<Locus>>& results) {

}
