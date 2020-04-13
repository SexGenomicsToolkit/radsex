/*
* Copyright (C) 2020 Romain Feron
* This file is part of RADSex.

* RADSex is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* RADSex is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with RADSex.  If not, see <https://www.gnu.org/licenses/>.
*/

/*!
 * @file map.cpp
 * @brief Implements the Map class.
*/

#include "map.h"


void Map::extra_setup() {

    // Filtering
    this->min_individuals = static_cast<uint16_t>(this->popmap.n_individuals * this->parameters.map_min_frequency);

    // Loading bwa index
    this->load_bwa_index();

    // BWA mem parameters
    this->bwa_mem_options = mem_opt_init(); // initialize the BWA-MEM parameters to the default values

    // Compute contig lengths from genome file
    this->load_contig_lengths();

}





void Map::process_marker(Marker& marker) {

    this->best_alignments[0] = 0;
    this->best_alignments[1] = 0;
    this->best_alignments[2] = 0;

    if (marker.n_individuals >= this->min_individuals) {

        this->alignment_results = mem_align1(this->bwa_mem_options, this->index->bwt, this->index->bns, this->index->pac, marker.sequence.size(), marker.sequence.c_str()); // Align marker to the reference

        for (uint j = 0; j < this->alignment_results.n; ++j) { // Loop through alignments to find best scoring one

            if (this->alignment_results.a[j].score > this->best_alignments[1]) {  // Current alignment has best score so far, store its properties

                this->best_alignments[0] = static_cast<int>(j);
                this->best_alignments[1] = this->alignment_results.a[j].score;
                this->best_alignments[2] = 1;

            } else if (this->alignment_results.a[j].score == this->best_alignments[1]) {  // Current alignment is as good as current best alignment, increase count

                ++this->best_alignments[2];

            }

        }

        // Get best alignment info
        this->best_alignment = mem_reg2aln(this->bwa_mem_options, this->index->bns, this->index->pac, marker.sequence.size(), marker.sequence.c_str(), &this->alignment_results.a[this->best_alignments[0]]);

        // Retain aligned markers with unique best alignment and mapq >= minimum quality (rid < 0 means not aligned)
        if (this->best_alignment.rid >= 0 and this->best_alignments[2] == 1 and this->best_alignment.mapq >= this->parameters.map_min_quality) {

            // Compute group bias. Assumption: no empty group (checked in map function)
            this->aligned_marker.bias = float(marker.group_counts.at(this->parameters.group1)) / float(this->popmap.get_count(this->parameters.group1)) - float(marker.group_counts.at(this->parameters.group2)) / float(this->popmap.get_count(this->parameters.group2));
            this->aligned_marker.p = get_p_association(marker.group_counts.at(this->parameters.group1), marker.group_counts.at(this->parameters.group2), this->popmap.get_count(this->parameters.group1), this->popmap.get_count(this->parameters.group2));
            this->aligned_marker.id = marker.id;
            this->aligned_marker.contig = this->index->bns->anns[this->best_alignment.rid].name;
            this->aligned_marker.position = this->best_alignment.pos;
            this->results.aligned_markers.push_back(this->aligned_marker);

        }

        free(this->best_alignment.cigar); // Deallocate cigar string for best hit
        free(this->alignment_results.a); // Deallocate the hit list

    }

    if (marker.n_individuals > 0) ++this->results.n_markers;

}





void Map::generate_output() {

    // Open output file and output the header
    this->output_file = open_output(this->parameters.output_file_path);
    this->output_file << "Contig\tPosition\tLength\tMarker_id\tBias\tP\tPcorr\tSignif\n";

    if (not this->parameters.disable_correction) {

        // Bonferroni correction: divide threshold by total number of tests (i.e. number of retained markers)
        this->parameters.signif_threshold /= this->results.n_markers;

    } else {

        // Set number of markers to 1 so corrected p-values are the same as original p-values
        this->results.n_markers = 1;

    }

    for (auto marker: this->results.aligned_markers) {

        output_file << marker.contig << "\t" << marker.position << "\t" << this->contig_lengths[marker.contig] << "\t" << marker.id << "\t"
                    << marker.bias << "\t" << marker.p << "\t" << std::min(1.0, marker.p * this->results.n_markers) << "\t"
                    << (static_cast<float>(marker.p) < this->parameters.signif_threshold ? "True" : "False") << "\n";

    }

    this->output_file.close();

    free(this->bwa_mem_options);
    bwa_idx_destroy(this->index);

}





void Map::load_contig_lengths() {

    std::ifstream genome_file = open_input(this->parameters.genome_file_path);

    std::string line = "", contig_name = "";
    uint contig_length = 0;

    while(std::getline(genome_file, line)) {

        if (line[0] == '>') {

            if (contig_name != "") this->contig_lengths[contig_name] = contig_length;  // Store length of previous contig

            // Get contig name from fasta header
            contig_name = split(line, " ")[0];
            contig_name = contig_name.substr(1, contig_name.size());
            contig_length = 0;

        } else {

            contig_length += line.size();

        }
    }

    this->contig_lengths[contig_name] = contig_length;  // Last contig
    genome_file.close();

}





void Map::load_bwa_index() {

    std::ifstream bwa_index_temp;
    bool indexed = true;
    std::string extensions[5] = {"amb", "ann", "bwt", "pac", "sa"};  // BWA index file extensions

    // Check whether all index files exist
    for (auto i=0; i<5; ++i) {

        bwa_index_temp.open(this->parameters.genome_file_path + "." + extensions[i]);
        if (bwa_index_temp.is_open()) bwa_index_temp.close(); else indexed = false;

    }

    // If index does not exist, build it
    if (not indexed) {

        log("BWA index file not found for the reference, creating bwa index file");
        bwa_idx_build(this->parameters.genome_file_path.c_str(), this->parameters.genome_file_path.c_str(), 0, 10000000); // Arguments: genome file, prefix, algorithm (default 0), block size (default 10000000)

    }

    // Load index
    this->index = bwa_idx_load(this->parameters.genome_file_path.c_str(), BWA_IDX_ALL);

    if (this->index == nullptr) {  // Check that index was loaded properly

        log("Failed to load index for genome file <" + this->parameters.genome_file_path + ">", LOG_ERROR);
        exit(1);

    }

}





