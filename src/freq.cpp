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
 * @file freq.cpp
 * @brief Implements the Freq class.
*/

#include "freq.h"


Freq::Freq(Parameters& parameters, bool compare_groups, bool store_groups, bool store_sequence) : Analysis(parameters, compare_groups, store_groups, store_sequence) {

    this->results = std::vector<uint32_t>(this->markers_table.header.n_individuals + 1, 0);

};





void Freq::process_marker(Marker& marker) {

    ++results[marker.n_individuals];

}





void Freq::generate_output() {

    std::ofstream output_file = open_output(parameters.output_file_path);
    output_file << "#source:radsex-freq;min_depth:" << parameters.min_depth <<  "\n";
    output_file << "Frequency" << "\t" << "Count" << "\n";

    for (uint i=1; i <= this->markers_table.header.n_individuals; ++i) output_file << i << "\t" << this->results[i] << "\n";  // Iterate over the map

    output_file.close();

}
