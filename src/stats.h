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
#include "kfun/kfun.h"
#include "utils.h"

// Compute Chi-square from the number of males and females with a sequence as well as total males and total females
double get_chi_squared(uint n_males, uint n_females, uint total_males, uint total_females);

// Compute the p-value for a Chi-square
double get_chi_squared_p(double chi_squared);

template<typename T>
const T find_median(const std::vector<T>& container);
