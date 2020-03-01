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
 * @file stats.h
 * @brief Defines functions to compute statistical tests and p-values.
*/

#pragma once
#include "kfun/kfun.h"
#include "utils.h"


/*!
 * \brief Compute Chi-squared
 *
 * \param n_group1     Number of individuals in the first group
 * \param n_group2     Number of individuals in the second group
 * \param total_group1 Total number of individuals in the first group
 * \param total_group2 Total number of individuals in the second group
 * \return
 */

double get_chi_squared(uint n_group1, uint n_group2, uint total_group1, uint total_group2);



/*!
 * \brief Get P-value for a Chi-squared test
 *
 * \param chi_squared Chi-squared value obtained with get_chi_squared()
 *
 * \return The p-value associated with chi_squared
 */

double get_chi_squared_p(double chi_squared);


/*!
 * \brief Compute the p-value of association with group with a Chi-squared test
 *
 * \param n_group1     Number of individuals in the first group
 * \param n_group2     Number of individuals in the second group
 * \param total_group1 Total number of individuals in the first group
 * \param total_group2 Total number of individuals in the second group
 * \return
 */

double get_p_association(uint n_group1, uint n_group2, uint total_group1, uint total_group2);



template<typename T>

/*!
 * \brief Find median value for a vector
 *
 * \param container Vector of values
 *
 * \return Median value in the vector
 */

T find_median(std::vector<T>& container) {

    T median;

    const auto start = container.begin();
    const auto end = container.end() - 1;
    const auto size = container.size();

    if (size % 2 == 0) {  // Find median for an even sized container (average of two possible median points)

        const auto median_it1 = start + size / 2 - 1;
        const auto median_it2 = start + size / 2;

        std::nth_element(start, median_it1 , end);
        const auto e1 = *median_it1;

        std::nth_element(start, median_it2 , end);
        const auto e2 = *median_it2;

        median= (e1 + e2) / 2;

    } else {  // Find median for odd sized vector

        const auto median_it = start + size / 2;
        std::nth_element(start, median_it , end);
        median = *median_it;
    }

    return median;
}
