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

/** @file */

#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "table_parser.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"

// Get all markers significantly associated with sex and export them in either table or fasta format
void signif(Parameters& parameters);

void processor(MarkersQueue& markers_queue, Popmap& popmap, Parameters& parameters, std::mutex& queue_mutex, std::vector<Marker>& candidate_markers, uint& n_markers, bool& parsing_ended, ulong batch_size);
