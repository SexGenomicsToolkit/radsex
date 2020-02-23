#include "subset.h"

void subset(Parameters& parameters) {

    Popmap popmap = load_popmap(parameters);
    Header header;

    bool parsing_ended = false;
    MarkersQueue markers_queue;
    std::mutex queue_mutex;

    std::ofstream output_file = open_output(parameters.output_file_path);

    std::thread parsing_thread(table_parser, std::ref(parameters), std::ref(popmap), std::ref(markers_queue), std::ref(queue_mutex), std::ref(header), std::ref(parsing_ended), true, false);
    std::thread processing_thread(processor, std::ref(markers_queue), std::ref(popmap), std::ref(parameters), std::ref(queue_mutex), std::ref(output_file), std::ref(parsing_ended), 100);

    parsing_thread.join();
    processing_thread.join();

    output_file.close();
}


void processor(MarkersQueue& markers_queue, Popmap& popmap, Parameters& parameters, std::mutex& queue_mutex, std::ofstream& output_file, bool& parsing_ended, ulong batch_size) {

    // Give 100ms headstart to table parser thread (to get number of individuals from header)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::vector<Marker> batch;
    bool keep_going = true;

    double chi_squared = 0;

    uint marker_processed_tick = static_cast<uint>(markers_queue.n_markers / 100);
    uint64_t n_processed_markers = 0;

    while (keep_going) {

        // Get a batch of markers from the queue
        batch = get_batch(markers_queue, queue_mutex, batch_size);

        if (batch.size() > 0) {  // Batch not empty

            for (auto& marker: batch) {

                if (marker.groups[parameters.group1] >= parameters.subset_min_group1 and marker.groups[parameters.group1] <= parameters.subset_max_group1 and
                    marker.groups[parameters.group2] >= parameters.subset_min_group2 and marker.groups[parameters.group2] <= parameters.subset_max_group2 and
                    marker.n_individuals >= parameters.subset_min_individuals and marker.n_individuals <= parameters.subset_max_individuals) {

                    chi_squared = get_chi_squared(marker.groups[parameters.group1], marker.groups[parameters.group2], popmap.counts[parameters.group1], popmap.counts[parameters.group2]);
                    marker.p = get_chi_squared_p(chi_squared);

                    parameters.output_fasta ? marker.output_fasta(output_file, parameters.min_depth) : marker.output_table(output_file);

                }

                log_progress(n_processed_markers, marker_processed_tick);

            }

        } else {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Batch empty: wait 10ms before asking for another batch

        }

        if (parsing_ended and markers_queue.markers.size() == 0) keep_going = false;
    }
}
