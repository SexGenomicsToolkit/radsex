#include "process_file.h"


void process_file(InputFile file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex) {

    std::unordered_map<std::string, uint16_t> temp_results;
    igzstream input_file(file.path.c_str());
    std::string line;
    bool write_next_line = false;
    while(std::getline(input_file, line)) {
        switch (line[0]) {
        case '@':
            write_next_line = true;
            break;
        case '+':
            write_next_line = false;
            break;
        default:
            if (write_next_line) {
                ++temp_results[line];
            }
            break;
        }
    }

    results_mutex.lock();
    for (auto sequence : temp_results) {
        results[sequence.first][file.individual_name] += sequence.second;
    }
    results_mutex.unlock();

    std::cout << " - Finished processing individual : " + file.individual_name + "." << std::endl;

    return;
}
