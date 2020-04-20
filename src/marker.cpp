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
 * @file marker.cpp
 * @brief Implements the Marker class.
*/

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
    output_file << "_p:" << this->p << "_pcorr:" << this->p_corr << "_mindepth:" << min_depth << "\n" << this->sequence << "\n";

}
