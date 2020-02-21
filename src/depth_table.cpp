#include "depth_table.h"

void table_parser(Parameters& parameters, Popmap& popmap, MarkersQueue& markers_queue, std::mutex& queue_mutex, bool& parsing_ended, bool sex_stats_only) {

    const uint tmp_queue_size = 1000;

    std::ifstream input_file;
    input_file.open(parameters.markers_table_path);

    if (not input_file) {
        std::cerr << "**Error: could not open marker depths table <" << parameters.markers_table_path << ">" << std::endl;
        exit(1);
    }

    std::vector<std::string> header;
    std::string temp = "";

    // First line (in depth table) is a comment with number of markers in the table
    std::getline(input_file, temp);

    // Check if first line is a comment line with total number of markers
    uint n_markers = 0;
    if (temp[0] == '#') {
        header = split(temp, " : ");
        if (header.size() == 2) n_markers = static_cast<uint>(std::stoi(header[1]));
        std::getline(input_file, temp);  // Load the second line which contains the header
    }

    uint marker_processed_tick = static_cast<uint>(n_markers / 100);

    // Next line (or first if no comment line) is the header
    header = split(temp, "\t");

    // Vector of group for each individual (by column index)
    std::vector<std::string> groups = get_column_group(popmap.groups, header);

    // Define variables used to read the file
    char buffer[65536];
    uint k = 0, field_n = 0, marker_n = 0;
    Marker marker;
    marker.individuals.resize(header.size() - 2);
    std::vector<Marker> tmp_queue(tmp_queue_size);  // Temporary block queue to avoid locking the shared blocks queue too often

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
                            if (not sex_stats_only) marker.id = temp;
                            break;
                        case 1:
                            if (not sex_stats_only) marker.sequence = temp;
                            break;
                        default:
                            marker.individuals[field_n - 2] = static_cast<uint16_t>(fast_stoi(temp.c_str()));
                            if (marker.individuals[field_n - 2] >= parameters.min_depth) ++marker.groups[groups[field_n]];
                            break;
                    }

                    temp = "";
                    ++field_n;
                    break;

                case '\n':  // New line (also a new field)
                    marker.individuals[field_n - 2] = static_cast<uint16_t>(fast_stoi(temp.c_str()));
                    if (field_n > 1 and marker.individuals[field_n - 2] >= parameters.min_depth) ++marker.groups[groups[field_n]];  // Increment the appropriate counter
                    // Add marker to the queue
                    tmp_queue[marker_n % tmp_queue_size] = marker;  // Empty line means end of a block, we add it to the queue
                    ++marker_n;
                    if (marker_n % tmp_queue_size == 0 or field_n < 2) {  // Merge temporary queue with shared queue after 1000 blocks
                        queue_mutex.lock();
                        for (auto& marker: tmp_queue) {
                            markers_queue.push(marker);
                            if (not sex_stats_only) marker.id = "";
                            if (not sex_stats_only) marker.sequence = "";
                            for (auto& group: marker.groups) group.second = 0;
                            for (auto& individual: marker.individuals) individual = 0;
                        }
                        queue_mutex.unlock();
                    }
                    if (marker_n % (10 * marker_processed_tick) == 0) std::cerr << "Processed " << marker_n << " markers (" << marker_n / (marker_processed_tick) << " %)" << std::endl;
                    // Reset variables
                    temp = "";
                    field_n = 0;
                    if (not sex_stats_only) marker.sequence = "";
                    if (not sex_stats_only) marker.id = "";
                    for (auto& individual: marker.individuals) individual = 0;
                    for (auto& group: marker.groups) group.second = 0;
                    break;

                default:
                    temp += buffer[i];
                    break;
            }
        }

    } while (input_file);

    input_file.close();
    parsing_ended = true;  // Shared flag indicating that the parsing is finished

}



std::vector<Marker> get_batch(MarkersQueue& blocks_queue, std::mutex& queue_mutex, ulong batch_size) {

    /* Get a batch of <batch_size> blocks from the shared queue.
     * The batch is stored as a vector of blocks. Extracted blocks are removed from the shared queue.
     */

    queue_mutex.lock();

    ulong batch_size_real = std::min(batch_size, blocks_queue.size());
    std::vector<Marker> batch(batch_size_real);

    if (batch_size_real > 0) {
        for (uint i=0; i<batch_size_real; ++i) {
            batch[i] = blocks_queue.front();
            blocks_queue.pop();
        }
    }

    queue_mutex.unlock();

    return batch;
}



std::vector<std::string> get_column_group(std::unordered_map<std::string, std::string>& popmap, const std::vector<std::string>& header) {

    // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
    std::vector<std::string> sex_columns;

    for (uint i=0; i<header.size(); ++i) {

        if (popmap.find(header[i]) != popmap.end()) {

            sex_columns.push_back(popmap[header[i]]);

        } else {

            sex_columns.push_back(""); // First and second columns (id and sequence) are not counted

        }
    }

    return sex_columns;
}
