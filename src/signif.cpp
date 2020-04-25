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
 * @file signif.cpp
 * @brief Implements the Signif class.
*/

#include "signif.h"


void Signif::process_marker(Marker& marker) {

    if (marker.n_individuals > 0) {

        ++this->results.n_markers;  // Increment total number of markers for Bonferroni correction

        marker.p = get_p_association(marker.group_counts[parameters.group1], marker.group_counts[parameters.group2], this->popmap.get_count(this->parameters.group1), this->popmap.get_count(this->parameters.group2));

        // First pass: filter markers with non-corrected p < 0.05
        if (static_cast<float>(marker.p) < this->parameters.signif_threshold) this->results.candidate_markers.push_back(marker);

    }

}





void Signif::generate_output() {

    std::ofstream output_file = open_output(this->parameters.output_file_path);

    if (not this->parameters.output_fasta) {

        output_file << "#source:radsex-signif;min_depth:" << parameters.min_depth << ";signif_threshold:" << parameters.signif_threshold <<
                       ";bonferroni:" <<  std::boolalpha << (not parameters.disable_correction) << "\n";
        output_file << print_list(this->markers_table.header.header, "\t") << "\n";

    }

    if (not this->parameters.disable_correction) {

        // Bonferroni correction: divide threshold by total number of tests (i.e. number of retained markers)
        this->parameters.signif_threshold /= this->results.n_markers;

    } else {

        // Set number of markers to 1 so corrected p-values are the same as original p-values
        this->results.n_markers = 1;

    }

    // Second pass: filter markers with threshold corrected p < 0.05
    for (auto marker: this->results.candidate_markers) {

        if (static_cast<float>(marker.p) < this->parameters.signif_threshold) {

            marker.p_corr = std::min(1.0, marker.p * static_cast<double>(results.n_markers));

            this->parameters.output_fasta ? marker.output_as_fasta(output_file, this->parameters.min_depth) : marker.output_as_table(output_file);

        }

    }

    output_file.close();

}

