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
 * @file subset.cpp
 * @brief Implements the Subset class.
*/

#include "subset.h"


void Subset::extra_setup() {

    this->output_file = open_output(parameters.output_file_path);
    if (not parameters.output_fasta) this->output_file << print_list(this->markers_table.header.header, "\t") << "\n";

}





void Subset::process_marker(Marker& marker) {

    if (marker.group_counts[parameters.group1] >= this->parameters.subset_min_group1 and marker.group_counts[parameters.group1] <= this->parameters.subset_max_group1 and
        marker.group_counts[parameters.group2] >= this->parameters.subset_min_group2 and marker.group_counts[parameters.group2] <= this->parameters.subset_max_group2 and
        marker.n_individuals >= this->parameters.subset_min_individuals and marker.n_individuals <= this->parameters.subset_max_individuals) {

        marker.p = get_p_association(marker.group_counts[parameters.group1], marker.group_counts[parameters.group2], this->popmap.get_count(parameters.group1), this->popmap.get_count(parameters.group2));

        this->parameters.output_fasta ? marker.output_as_fasta(this->output_file, this->parameters.min_depth) : marker.output_as_table(this->output_file);

    }

}

