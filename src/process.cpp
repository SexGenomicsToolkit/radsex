#include "process.h"

KSEQ_INIT(gzFile, gzread) // Kseq init macro

// List of currently supported file types with extensions
std::vector<std::string> extensions {".fq.gz", ".fq", ".fastq.gz", ".fastq", ".fasta.gz", ".fasta", ".fa.gz", ".fa", ".fna.gz", ".fna"};

std::vector<InputFile> get_input_files(const std::string& input_dir_path) {

    /* Gets all relevant files (from supported formats) from the input directory.
     * Files are stored as InputFile object which contains all relevant information about the file.
     * The function returns a vector of InputFiles.
     */

    DIR* dir = opendir(input_dir_path.c_str());

    if(!dir) {
        std::cout << std::endl << "** Error: could not read the directory \"" << input_dir_path << "\"" << std::endl;
        exit(0);
    }

    struct dirent* dir_content;
    std::vector<InputFile> files;
    std::string current_file, extension, individual_name;
    std::vector<std::string> split_name;
    InputFile temp;

    while ((dir_content = readdir(dir))) {

        current_file = dir_content->d_name;
        split_name = split(current_file, ".");
        size_t s = split_name.size();
        extension = "";
        individual_name = split_name[0];

        // Get file name and extension (even when there is "." in the file name)
        if (s > 1) {
            if (split_name[s - 1] == "gz" and s > 2) {
                extension = "." + split_name[s - 2] + "." + split_name[s - 1];
                for (uint i=1; i<split_name.size() - 2; ++i) individual_name += "." + split_name[i];
            } else {
                extension = "." + split_name[s - 1];
                for (uint i=1; i<split_name.size() - 1; ++i) individual_name += "." + split_name[i];
            }
        }

        if(std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
            temp.individual_name = individual_name;
            temp.path = input_dir_path + current_file;
            temp.extension = extension;
            temp.processed = false;
            files.push_back(temp);
        }
    }

    if (files.size() == 0) {

        std::cout << " ** Error: no valid input file found in input directory \"" << input_dir_path <<"\"." << std::endl;
        exit(1);
    }

    return files;
}



void process(Parameters& parameters) {

    /* The process_reads function does the following:
     * - retrieve all sequence files from the input directory
     * - create file processors according to the value of n_threads (allows to process files in parallel)
     * - once all files are processed, output the results
     */

    std::string par = "input_dir_path";
    std::vector<InputFile> input_files = get_input_files(parameters.get_value_from_name<std::string>(par));
    std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>> results;

    par = "min_cov";
    uint min_cov = parameters.get_value_from_name<int>(par) - 1;

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
    output_process_reads(output_file_path, individuals, results, min_cov);

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
        if (line_n % 1000 == 0) std::cout << line_n / 1000 << std::endl;
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
