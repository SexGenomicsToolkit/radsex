#include "depth.h"
#include <iostream>

void depth(Parameters& parameters) {

    Popmap popmap = load_popmap(parameters);
    Header header;

    Depths depths(popmap.n_individuals);
    std::vector<uint32_t> n_markers;

    bool parsing_ended = false;
    MarkersQueue markers_queue;
    std::mutex queue_mutex;

    std::thread parsing_thread(table_parser, std::ref(parameters), std::ref(popmap), std::ref(markers_queue), std::ref(queue_mutex), std::ref(parsing_ended), std::ref(header), true);
    std::thread processing_thread(processor, std::ref(markers_queue), std::ref(parameters), std::ref(queue_mutex), std::ref(depths), std::ref(parsing_ended), 100, popmap.n_individuals, std::ref(n_markers));

    parsing_thread.join();
    processing_thread.join();

    std::vector<uint16_t> min_depths(popmap.n_individuals, static_cast<uint16_t>(65535));
    std::vector<uint16_t> max_depths(popmap.n_individuals, 0);
    std::vector<uint16_t> median_depths(popmap.n_individuals, 0);
    std::vector<uint16_t> average_depths(popmap.n_individuals, 0);

    std::ofstream output_file;
    output_file.open(parameters.output_file_path);

    if (not output_file.is_open()) {
        std::cerr << "**Error: could not open output file <" << parameters.output_file_path << ">" << std::endl;
        exit(1);
    }

    output_file << "Individual\tGroup\tMarkers\tRetained\tMin_depth\tMax_depth\tMedian_depth\tAverage_depth\n";

    for (uint i=0; i<depths[0].size(); ++i) {

        uint64_t total_depth = 0;

        std::sort(depths[i].begin(), depths[i].end()); // Sort depth vector for easy median finding

        const auto start = depths[i].begin();
        const auto end = depths[i].end();
        const auto size = depths[i].size();

        min_depths[i] = *start;
        max_depths[i] = *end;
        average_depths[i] = static_cast<uint16_t>(static_cast<uint16_t>(std::accumulate(start, end, 0)) / size);

        if (depths[i].size() % 2 == 0) {  // Find median for an even sized vector (average of two possible median points)

            const auto median_it1 = start + size / 2 - 1;
            const auto median_it2 = start + size / 2;

            std::nth_element(start, median_it1 , end);
            const auto e1 = *median_it1;

            std::nth_element(start, median_it2 , end);
            const auto e2 = *median_it2;

            median_depths[i] = (e1 + e2) / 2;

        } else {  // Find median for odd sized vector

            const auto median_it = start + size / 2;
            std::nth_element(start, median_it , end);
            median_depths[i] = *median_it;
        }

        for (auto marker: depths[i]) {

            if (marker < min_depths[i]) min_depths[i] = marker;
            if (marker < max_depths[i]) max_depths[i] = marker;
            total_depth += marker;

        }

        output_file << header[i+2] << "\t" << popmap.groups[header[i+2]] << "\t" << depths.size() << "\t" << min_depths[i] << "\t" <<
                       max_depths[i] << "\t" << median_depths[i] << "\t" << total_depth / depths.size() << "\n";

    }

}


void processor(MarkersQueue& markers_queue, Parameters& parameters, std::mutex& queue_mutex, Depths& depths, std::vector<uint32_t> n_markers, bool& parsing_ended, ulong batch_size, uint n_individuals) {

    std::vector<Marker> batch;
    bool keep_going = true;

    while (keep_going) {

        // Get a batch of markers from the queue
        batch = get_batch(markers_queue, queue_mutex, batch_size);

        if (batch.size() > 0) {  // Batch not empty

            for (auto& marker: batch) {

                for (uint i=0; i<marker.individuals.size(); ++i) {
                    if (marker.n_individuals == n_individuals) depths[i].push_back(marker.individuals[i]); // Only consider markers present in all individuals
                    if (marker.individuals[i] > 0) ++n_markers[i];  // Increment total number of markers for this individual
                }

            }

        } else {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Batch empty: wait 10ms before asking for another batch

        }

        if (parsing_ended and markers_queue.size() == 0) keep_going = false;
    }
}



void depth(Parameters& parameters) {

    Popmap popmap = load_popmap(parameters);

    std::ifstream input_file;
    input_file.open(parameters.markers_table_path);

    if (input_file.is_open()) {

        std::vector<std::string> header;
        std::string temp = "";

        // First line is a comment with number of markers in the table
        std::getline(input_file, temp);
        header = split(temp, " : ");
        if (header.size() == 2) uint n_markers = static_cast<uint>(std::stoi(header[1]));

        // Second line is the header. The header is parsed to get the sex of each field in the table.
        std::getline(input_file, temp);
        header = split(temp, "\t");

        // Vector with sex of each column
        std::vector<std::string> sex_columns = get_column_sex(popmap.groups, header);

        // Total number of reads in each individual
        std::unordered_map<std::string, uint> individual_depths;

        // Individual depths for a single marker
        std::unordered_map<std::string, ulong> marker_depths;

        // Table of depth frequencies
        std::unordered_map<std::string, std::vector<ulong>> median_depth;

        // Define variables used to read the file
        char buffer[65536];
        uint k = 0, field_n = 0;
        ulong depth = 0;
        uint n_individuals = 0;

        do {

            // Read a chunk of size given by the buffer
            input_file.read(buffer, sizeof(buffer));
            k = static_cast<uint>(input_file.gcount());

            for (uint i=0; i<k; ++i) {

                // Read the buffer character by character
                switch(buffer[i]) {

                    case '\t':  // New field

                        if (field_n > 2) {
                            depth = std::stoul(temp);
                            individual_depths[header[field_n]] += depth;  // Add marker depth to individual total depth
                            marker_depths[header[field_n]] = depth;  // Add individual depth to marker depth
                            if(depth > 0) ++n_individuals;
                        }

                        // Reset variables
                        temp = "";
                        ++field_n;
                        break;

                    case '\n':  // New line (also a new field)

                        if (field_n > 2) {
                            depth = std::stoul(temp);
                            individual_depths[header[field_n]] += depth;  // Add marker depth to individual total depth
                            marker_depths[header[field_n]] = depth;  // Add individual depth to marker depth
                            if(depth > 0) ++n_individuals;
                        }

                        if (n_individuals == marker_depths.size()) {  // Only keep markers present in all individuals to estimate expected depths
                            for (auto m: marker_depths) {
                                median_depth[m.first].push_back(m.second);
                            }
                        }

                        // Reset variables
                        temp = "";
                        field_n = 0;
                        n_individuals = 0;
                        break;

                    default:
                        temp += buffer[i];
                        break;
                }
            }

        } while (input_file);

        input_file.close();

        std::ofstream output_file;
        output_file.open(parameters.output_file_path);

        if (output_file.is_open()) {

            output_file << "Individual\tGroup\tMarkers\tDepth\n";

            for (auto i: median_depth) {

                output_file << i.first << "\t" << popmap.groups[i.first] << "\t" << individual_depths[i.first] << "\t";

                std::sort(i.second.begin(), i.second.end()); // Sort depth vector for easy median finding

                if (i.second.size() % 2 == 0) {  // Find median for an even sized vector (average of two possible median points)

                    const auto median_it1 = i.second.begin() + i.second.size() / 2 - 1;
                    const auto median_it2 = i.second.begin() + i.second.size() / 2;

                    std::nth_element(i.second.begin(), median_it1 , i.second.end());
                    const auto e1 = *median_it1;

                    std::nth_element(i.second.begin(), median_it2 , i.second.end());
                    const auto e2 = *median_it2;

                    output_file << (e1 + e2) / 2 << "\n";

                } else {  // Find median for odd sized vector

                    const auto median_it = i.second.begin() + i.second.size() / 2;
                    std::nth_element(i.second.begin(), median_it , i.second.end());
                    output_file << *median_it << "\n";
                }

            }
        }
    }
}
