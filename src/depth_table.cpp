#include "depth_table.h"

void table_parser(Parameters& parameters, const Popmap& popmap, MarkersQueue& markers_queue, std::mutex& queue_mutex, Header& header, bool& parsing_ended, bool no_seq, bool no_group) {

    std::ifstream input_file;
    input_file.open(parameters.markers_table_path);

    if (not input_file) {

        log("Could not open marker depths table <" + parameters.markers_table_path + ">", LOG_ERROR);
        exit(1);

    }

    std::string temp = "";

    // First line (in depth table) is a comment with number of markers in the table
    std::getline(input_file, temp);

    // Check if first line is a comment line with total number of markers
    if (temp[0] == '#') {
        header = split(temp, " : ");
        if (header.size() == 2) markers_queue.n_markers = static_cast<uint>(std::stoi(header[1]));
        std::getline(input_file, temp);  // Load the second line which contains the header
    }

    // Next line (or first if no comment line) is the header
    header = split(temp, "\t");

    // Vector of group for each individual (by column index)
    std::vector<std::string> groups;
    if (not no_group) groups = get_column_group(popmap.groups, header);

    // Define variables used to read the file
    char buffer[65536];
    uint k = 0, field_n = 0, marker_n = 0;
    Marker marker;
    marker.individuals.resize(header.size() - 2);
    std::vector<Marker> tmp_queue(TMP_QUEUE_SIZE);  // Temporary block queue to avoid locking the shared blocks queue too often
    uint tmp_queue_real_size = 0;
    uint marker_queue_size = 0;

    do {

        // Read a chunk of size given by the buffer
        input_file.read(buffer, sizeof(buffer));
        k = static_cast<uint>(input_file.gcount());

        for (uint i=0; i<k; ++i) {

            // Read the buffer character by character
            switch(buffer[i]) {

                case '\t':  // New field

                    switch (field_n) {
                        case 0:
                            if (not no_seq) marker.id = temp;
                            break;
                        case 1:
                            if (not no_seq) marker.sequence = temp;
                            break;
                        default:
                            marker.individuals[field_n - 2] = static_cast<uint16_t>(fast_stoi(temp.c_str()));
                            if (marker.individuals[field_n - 2] >= parameters.min_depth) {
                                if (not no_group) ++marker.groups[groups[field_n]];
                                ++marker.n_individuals;
                            }
                            break;
                    }

                    temp = "";
                    ++field_n;
                    break;

                case '\n':  // New line (also a new field)
                    marker.individuals[field_n - 2] = static_cast<uint16_t>(fast_stoi(temp.c_str()));
                    if (field_n > 1 and marker.individuals[field_n - 2] >= parameters.min_depth) {
                        if (not no_group) ++marker.groups[groups[field_n]];
                        ++marker.n_individuals;
                    }
                    // Add marker to the queue
                    tmp_queue[marker_n % TMP_QUEUE_SIZE] = marker;  // Empty line means end of a block, we add it to the queue
                    ++tmp_queue_real_size;
                    ++marker_n;
                    if (marker_n % TMP_QUEUE_SIZE == 0) {  // Merge temporary queue with shared queue after 1000 blocks
                        do {
                            marker_queue_size = markers_queue.markers.size();
                            if (marker_queue_size > MAX_QUEUE_SIZE) std::this_thread::sleep_for(std::chrono::microseconds(10));
                        } while (marker_queue_size > MAX_QUEUE_SIZE);
                        queue_mutex.lock();
                        for (auto& tmp_marker: tmp_queue) {
                            markers_queue.markers.push(tmp_marker);
                            ++markers_queue.loaded_markers;
                            // Reset marker attributes
                            tmp_marker.reset(no_seq);
                        }
                        queue_mutex.unlock();
                        tmp_queue_real_size = 0;
                    }

                    // Reset variables
                    temp = "";
                    field_n = 0;
                    marker.reset(no_seq);
                    break;

                default:
                    temp += buffer[i];
                    break;
            }
        }

    } while (input_file);

    // Last batch: real number of markers to push is given by tmp_queue_real_size
    queue_mutex.lock();
    for (uint i=0; i<tmp_queue_real_size; ++i) {
        markers_queue.markers.push(tmp_queue[i]);
        ++markers_queue.n_markers;
    }
    queue_mutex.unlock();

    input_file.close();
    parsing_ended = true;  // Shared flag indicating that the parsing is finished

}



std::vector<Marker> get_batch(MarkersQueue& markers_queue, std::mutex& queue_mutex, ulong batch_size) {

    /* Get a batch of <batch_size> blocks from the shared queue.
     * The batch is stored as a vector of blocks. Extracted blocks are removed from the shared queue.
     */

    queue_mutex.lock();

    ulong batch_size_real = std::min(batch_size, markers_queue.markers.size());
    std::vector<Marker> batch(batch_size_real);

    if (batch_size_real > 0) {
        for (uint i=0; i<batch_size_real; ++i) {
            batch[i] = markers_queue.markers.front();
            markers_queue.markers.pop();
        }
    }

    queue_mutex.unlock();

    return batch;
}



std::vector<std::string> get_column_group(const std::unordered_map<std::string, std::string>& groups, const std::vector<std::string>& header) {

    // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
    std::vector<std::string> sex_columns;

    for (uint i=0; i<header.size(); ++i) {

        if (groups.find(header[i]) != groups.end()) {

            sex_columns.push_back(groups.at(header[i]));

        } else {

            sex_columns.push_back(""); // First and second columns (id and sequence) are not counted

        }
    }

    return sex_columns;
}
