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
 * @file distrib.cpp
 * @brief Implements the Distrib class.
*/

#include "distrib.h"


void Distrib::process_marker(Marker& marker) {

    if (marker.n_individuals > 0) {

        ++this->results.distribution[marker.group_counts[parameters.group1]][marker.group_counts[parameters.group2]].first;
        ++this->results.n_markers;

    }

}





void Distrib::generate_output() {

    if (not this->parameters.disable_correction) this->parameters.signif_threshold /= this->results.n_markers; // Bonferroni correction: divide threshold by total number of tests (i.e. number of retained markers)

    // Generate the output file
    std::ofstream output_file = open_output(this->parameters.output_file_path);
    output_file << this->parameters.group1 << "\t" << this->parameters.group2 << "\t" << "Markers" << "\t" << "P" << "\t" << "Signif" << "\t" << "Bias" << "\n";

    for (uint g = 0; g <= this->popmap.get_count(this->parameters.group1); ++g) {

        for (uint h = 0; h <= this->popmap.get_count(this->parameters.group2); ++h) {

            if (g + h != 0) { // Exclude markers absent from all individuals

                this->results.distribution[g][h].second = get_p_association(g, h, this->popmap.get_count(parameters.group1), this->popmap.get_count(parameters.group2));

                output_file << g << "\t" << h << "\t" << this->results.distribution[g][h].first << "\t" << this->results.distribution[g][h].second << "\t"
                            << (static_cast<float>(this->results.distribution[g][h].second) < this->parameters.signif_threshold ? "True" : "False") << "\t"
                            << static_cast<float>(g) / static_cast<float>(this->popmap.get_count(this->parameters.group1)) - (static_cast<float>(h) / static_cast<float>(this->popmap.get_count(this->parameters.group2))) << "\n";

            }

        }

    }

}
