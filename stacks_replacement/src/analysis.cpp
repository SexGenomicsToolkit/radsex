#include "analysis.h"

void analysis(Parameters& parameters) {

    std::vector<InputFile> input_files = get_input_files(parameters);

    std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>> results;

    std::vector<std::thread> threads;
    std::mutex results_mutex, files_mutex;

    for (uint i=0; i<parameters.n_threads; ++i) {
        threads.push_back(std::thread(file_processor, std::ref(input_files), std::ref(results), std::ref(results_mutex), std::ref(files_mutex)));
    }

    for (auto &t : threads) t.join();

    std::vector<std::string> individuals;
    for (auto i: input_files) individuals.push_back(i.individual_name);

    write_output(parameters.output_file, individuals, results);
}



void file_processor(std::vector<InputFile>& input_files, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex, std::mutex& files_mutex) {

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

    return;
}
