#include "process_reads.h"

KSEQ_INIT(gzFile, gzread) // Kseq init macro


void process_reads(Parameters& parameters) {

    /* The process_reads function does the following:
     * - retrieve all sequence files from the input directory
     * - create file processors according to the value of n_threads (allows to process files in parallel)
     * - once all files are processed, output the results
     */

    std::string par = "input_dir_path";
    std::vector<InputFile> input_files = get_input_files(parameters.get_value_from_name<std::string>(par));
    std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>> results;

    par = "n_threads";
    std::vector<std::thread> threads;
    std::mutex results_mutex, files_mutex;

    // Create and run all file processors
    for (int i=0; i<parameters.get_value_from_name<int>(par); ++i) {
        threads.push_back(std::thread(file_processor, std::ref(input_files), std::ref(results), std::ref(results_mutex), std::ref(files_mutex)));
    }

    for (auto &t : threads) t.join();

    par = "output_file_path";
    std::string output_file_path = parameters.get_value_from_name<std::string>(par);

    // Create list of individuals
    std::vector<std::string> individuals;
    for (auto i: input_files) individuals.push_back(i.individual_name);

    // Generate the output file
    output_process_reads(output_file_path, individuals, results);

}



inline void file_processor(std::vector<InputFile>& input_files, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex, std::mutex& files_mutex) {

    /* The file processor looks for files to process and process them
     * It is meant to be used in parallel to increase throughput
     * A mutex protects the list of input files and another mutex protects the results structure
     */

    bool remaining_files = true;

    while (remaining_files) {
        files_mutex.lock();
        for (std::vector<InputFile>::iterator it = input_files.begin(); it != input_files.end(); ++it) {
            if (not it->processed) {
                it->processed = true;
                remaining_files = true;
                files_mutex.unlock();
                process_file(*it, results, results_mutex);
                break;
            } else {
                remaining_files = false;
            }
        }
        files_mutex.unlock();
    }
}



inline void process_file(InputFile& input_file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex) {

    /* This function uses the Kseq generic file stream from Heng Li to read input files.
     * The coverage of each sequence is computed without allowing for any mismatches.
     * This is basically a read counter.
     */

    std::unordered_map<std::string, uint16_t> temp_results; // Temporary result storage to avoid constantly locking/unlocking the mutex
    gzFile file;
    kseq_t* sequence; // Create the seq object
    int line_n; // Stores the length of a line

    file = gzopen(input_file.path.c_str(), "r");
    sequence = kseq_init(file); // Initialize the seq object

    // Read through the file and store the results
    while ((line_n = kseq_read(sequence)) >= 0) {
        ++temp_results[sequence->seq.s];
    }

    kseq_destroy(sequence); // Destroy the seq object
    gzclose(file);

    // Transfer the results from the temp data structure to the full data structure
    results_mutex.lock();
    for (auto sequence : temp_results) {
        results[sequence.first][input_file.individual_name] += sequence.second;
    }
    std::cout << " - Finished processing individual : " + input_file.individual_name + "." << std::endl;
    results_mutex.unlock();
}
