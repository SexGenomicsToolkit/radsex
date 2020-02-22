#include "depth.h"
#include <iostream>

void depth(Parameters& parameters) {

    Popmap popmap = load_popmap(parameters);
    Header header;

    Depths depths(popmap.n_individuals);
    std::vector<uint32_t> n_markers(popmap.n_individuals, 0);

    bool parsing_ended = false;
    MarkersQueue markers_queue;
    std::mutex queue_mutex;

    std::thread parsing_thread(table_parser, std::ref(parameters), std::ref(popmap), std::ref(markers_queue), std::ref(queue_mutex), std::ref(parsing_ended), std::ref(header), true);
    std::thread processing_thread(processor, std::ref(markers_queue), std::ref(parameters), std::ref(queue_mutex), std::ref(depths), std::ref(n_markers), std::ref(parsing_ended), 100, popmap.n_individuals);

    parsing_thread.join();
    processing_thread.join();

    std::ofstream output_file;
    output_file.open(parameters.output_file_path);

    if (not output_file.is_open()) {
        std::cerr << "**Error: could not open output file <" << parameters.output_file_path << ">" << std::endl;
        exit(1);
    }

    output_file << "Individual\tGroup\tMarkers\tRetained\tMin_depth\tMax_depth\tMedian_depth\tAverage_depth\n";

    for (uint i=0; i<depths.size(); ++i) {

        std::sort(depths[i].begin(), depths[i].end()); // Sort depth vector to easily compute all metrics (necessary for median anyway)

        const auto start = depths[i].begin();
        const auto end = depths[i].end() - 1;
        const auto size = depths[i].size();

        const uint16_t min_depth = *start;
        const uint16_t max_depth = *end;
        const uint64_t total_depth = std::accumulate(depths[i].begin(), depths[i].end(), 0);

        uint16_t median_depth = 0;
        if (depths[i].size() % 2 == 0) {  // Find median for an even sized vector (average of two possible median points)

            const auto median_it1 = start + size / 2 - 1;
            const auto median_it2 = start + size / 2;

            std::nth_element(start, median_it1 , end);
            const auto e1 = *median_it1;

            std::nth_element(start, median_it2 , end);
            const auto e2 = *median_it2;

            median_depth = (e1 + e2) / 2;

        } else {  // Find median for odd sized vector

            const auto median_it = start + size / 2;
            std::nth_element(start, median_it , end);
            median_depth = *median_it;
        }

        output_file << header[i+2] << "\t" << popmap.groups[header[i+2]] << "\t" << n_markers[i] << "\t" << size << "\t" << min_depth << "\t" <<
                       max_depth << "\t" << median_depth << "\t" << total_depth / size << "\n";

    }

}


void processor(MarkersQueue& markers_queue, Parameters& parameters, std::mutex& queue_mutex, Depths& depths, std::vector<uint32_t>& n_markers, bool& parsing_ended, ulong batch_size, uint n_individuals) {

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
