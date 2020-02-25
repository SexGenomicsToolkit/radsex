#include "map.h"


void map(Parameters& parameters) {

    /* The mapping function maps all sequences from a coverage table (obtained from process, subset, or signif functions) to a reference genome and
     * generates an output file with the following format:
     *
     * SequenceID | Contig | Position | SexBias | P
     *
     * - SequenceID: the ID of the mapped sequence
     * - Contig: name of the contig where the sequence mapped
     * - Position: mapping position on this contig
     * - SexBias: (number of males with the sequence)/(total number of males) - (number of females with the sequence)/(total number of females)
     * - P: probability of association with sex given by chi-squared test with Bonferroni correction.
     *
     * Markers are alignedusing bwa mem (https://github.com/lh3/bwa). Sequences are mapped using mem_align1() they are considered correctly mapped if:
     * - Their mapping quality is > threshold given by the mapping_quality parameter
     * - They are mapped uniquely, that is, there is no other mapping position with the same mapping score for this sequence.     *
     */

    std::chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();

    // Popmap
    Popmap popmap = load_popmap(parameters);

    log("RADSex map started");
    log("Comparing groups \"" + parameters.group1 + "\" and \"" + parameters.group2 + "\"");

    Header header = get_header(parameters.markers_table_path);

    std::vector<AlignedMarker> aligned_markers;

    bool parsing_ended = false;
    MarkersQueue markers_queue;
    std::mutex queue_mutex;

    std::unordered_map<std::string, uint64_t> contig_lengths = get_contig_lengths(parameters.genome_file_path);

    // Check if bwa index files exist for the genome and build index if it's missing
    build_bwa_index(parameters);

    std::thread parsing_thread(table_parser, std::ref(parameters), std::ref(popmap), std::ref(markers_queue), std::ref(queue_mutex), std::ref(parsing_ended), false, false);
    std::thread processing_thread(processor, std::ref(markers_queue), std::ref(parameters), std::ref(popmap), std::ref(queue_mutex), std::ref(aligned_markers), std::ref(parsing_ended), BATCH_SIZE);

    parsing_thread.join();
    processing_thread.join();

    std::ofstream output_file = open_output(parameters.output_file_path);

    output_file << "Contig\tPosition\tLength\tMarker_id\tBias\tP\tSignif\n";

    if (not parameters.disable_correction) parameters.signif_threshold /= markers_queue.n_markers;

    for (auto marker: aligned_markers) {

        output_file << marker.contig << "\t" << marker.position << "\t" << contig_lengths[marker.contig] << "\t" << marker.id << "\t"
                    << marker.bias << "\t" << marker.p << "\t" << (static_cast<float>(marker.p) < parameters.signif_threshold ? "True" : "False") << "\n";

    }

    output_file.close();

    log("RADSex map ended (total runtime: " + get_runtime(t_begin) + ")");

}


void processor(MarkersQueue& markers_queue, Parameters& parameters, Popmap& popmap, std::mutex& queue_mutex, std::vector<AlignedMarker>& aligned_markers, bool& parsing_ended, ulong batch_size) {

    // Give 100ms headstart to table parser thread (to get number of individuals from header)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Batch reading
    std::vector<Marker> batch;
    bool keep_going = true;

    // Filtering
    uint16_t min_individuals = static_cast<uint16_t>(popmap.n_individuals * parameters.map_min_frequency);

    // Loading bwa index
    bwaidx_t* index = bwa_idx_load(parameters.genome_file_path.c_str(), BWA_IDX_ALL);

    if (index == nullptr) {
        log("Failed to load index for genome file <" + parameters.genome_file_path + ">", LOG_ERROR);
        exit(1);
    }

    // BWA mem parameters
    mem_opt_t *opt;
    opt = mem_opt_init(); // initialize the BWA-MEM parameters to the default values
    mem_alnreg_v ar;
    mem_aln_t best;
    uint j;
    double chi_squared;
    int best_alignment[3] {0, -1, 0}; // Info about best alignment: index, score, count
    AlignedMarker seq;

    uint marker_processed_tick = static_cast<uint>(markers_queue.n_markers / 100);
    uint64_t n_processed_markers = 0;

    while (keep_going) {

        // Get a batch of markers from the queue
        batch = get_batch(markers_queue, queue_mutex, batch_size);

        if (batch.size() > 0) {  // Batch not empty

            for (auto& marker: batch) {

                if (marker.n_individuals >= min_individuals) {

                    ar = mem_align1(opt, index->bwt, index->bns, index->pac, marker.sequence.size(), marker.sequence.c_str()); // Align the marker to the reference

                    for (j = 0; j < ar.n; ++j) { // Loop through alignments to find best scoring one

                        if (ar.a[j].score > best_alignment[1]) {  // Current alignment has best score so far, store its properties

                            best_alignment[0] = static_cast<int>(j);
                            best_alignment[1] = ar.a[j].score;
                            best_alignment[2] = 1;

                        } else if (ar.a[j].score == best_alignment[1]) {  // Current alignment is as good as current best alignment, increase count

                            ++best_alignment[2];

                        }

                    }

                    best = mem_reg2aln(opt, index->bns, index->pac, marker.sequence.size(), marker.sequence.c_str(), &ar.a[best_alignment[0]]); // Get mapping quality for best alignment

                    // Retain aligned markers with unique best alignment, mapq >= minimum quality (rid < 0 means not aligned)
                    if (best.rid >= 0 and best_alignment[2] == 1 and best.mapq >= parameters.map_min_quality) {

                        // Compute group bias. Assumption: no empty group (checked in map function)
                        seq.bias = float(marker.groups[parameters.group1]) / float(popmap.counts[parameters.group1]) - float(marker.groups[parameters.group2]) / float(popmap.counts[parameters.group2]);
                        chi_squared = get_chi_squared(marker.groups[parameters.group1], marker.groups[parameters.group2], popmap.counts[parameters.group1], popmap.counts[parameters.group2]);
                        (chi_squared == chi_squared) ? seq.p = get_chi_squared_p(chi_squared) : seq.p = 1.0; // chi square is NaN (fail == test) --> sequence found in all individuals --> set p to 1
                        seq.p < 0.0000000000000001 ? seq.p = 0.0000000000000001 : seq.p = seq.p;  // Lower bound for p
                        seq.id = marker.id;
                        seq.contig = index->bns->anns[best.rid].name;
                        seq.position = best.pos;
                        aligned_markers.push_back(seq);

                    }

                    free(best.cigar); // Deallocate cigar string for best hit
                    free(ar.a); // Deallocate the hit list
                }

                // Reset variables
                best_alignment[0] = 0;
                best_alignment[1] = -1;
                best_alignment[2] = 0;

                log_progress_bar(n_processed_markers, marker_processed_tick);
            }

        } else {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Batch empty: wait 10ms before asking for another batch

        }

        if (parsing_ended and markers_queue.markers.size() == 0) keep_going = false;
    }

    // Destroy bwa objects
    free(opt);
    bwa_idx_destroy(index);
}



std::unordered_map<std::string, uint64_t> get_contig_lengths(const std::string& genome_file_path) {

    std::ifstream genome_file(genome_file_path);

    if (not genome_file.is_open()) {

        log("Could not open genome file <" + genome_file_path + "> in get_contig_lengths", LOG_ERROR);
        exit(1);

    }

    std::string line = "", contig_name = "";

    std::unordered_map<std::string, uint64_t> contig_lengths;
    uint contig_length = 0;

    while(std::getline(genome_file, line)) {

        if (line[0] == '>') {

            if (contig_name != "") contig_lengths[contig_name] = contig_length;  // Store length of previous contig
            // Get contig name from fasta header
            contig_name = split(line, " ")[0];
            contig_name = contig_name.substr(1, contig_name.size());
            contig_length= 0;

        } else {

            contig_length += line.size();

        }
    }

    contig_lengths[contig_name] = contig_length;  // Last contig
    genome_file.close();

    return contig_lengths;
}



void build_bwa_index(Parameters& parameters) {

    // Generate BWA index if it does not exist
    std::ifstream bwa_index_temp;
    bool indexed = true;
    std::string extensions[5] = {"amb", "ann", "bwt", "pac", "sa"};  // BWA index file extensions

    std::unordered_map<std::string, uint64_t> contig_lengths = get_contig_lengths(parameters.genome_file_path);

    // Checking that all index files are present
    for (auto i=0; i<5; ++i) {

        bwa_index_temp.open(parameters.genome_file_path + "." + extensions[i]);
        if (bwa_index_temp.is_open()) bwa_index_temp.close(); else indexed = false;

    }

    if (not indexed) {

        log("BWA index file not found for the reference, creating bwa index file");
        bwa_idx_build(parameters.genome_file_path.c_str(), parameters.genome_file_path.c_str(), 0, 10000000); // Arguments: genome file, prefix, algorithm (default 0), block size (default 10000000)

    }

}
