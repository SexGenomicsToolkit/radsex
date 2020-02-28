#include "marker.h"


Marker::Marker(uint16_t n_individuals) {

    this->individual_depths = std::vector<uint16_t>(n_individuals, 0);

}



void Marker::reset(bool sex_stats_only) {

    if (not sex_stats_only) {

        this->id = "";
        this->sequence = "";

    }

    for (auto& group: this->group_counts) group.second = 0;
    for (auto& individual: this->individual_depths) individual = 0;
    this->n_individuals = 0;

}



void Marker::output_as_table(std::ofstream& output_file) const {

    output_file << this->id << "\t" << this->sequence;
    for (auto& individual: this->individual_depths) output_file << "\t" << individual;
    output_file << "\n";

}



void Marker::output_as_fasta(std::ofstream& output_file, const uint min_depth) const {

    output_file << ">" << this->id;
    for (auto group: this->group_counts) output_file << "_" << group.first << ":" << group.second;
    output_file << "_p:" << this->p << "_mindepth:" << min_depth << "\n" << this->sequence << "\n";

}
