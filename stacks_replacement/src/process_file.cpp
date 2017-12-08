#include "process_file.h"


void process_file(InputFile file, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, std::mutex& results_mutex) {

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
                results_mutex.lock();
                ++results[line][file.individual_name];
                results_mutex.unlock();
            }
            break;
        }
    }

    return;
}
