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
 * @file stats.cpp
 * @brief Implements functions to compute statistical tests and p-values.
*/

#include "stats.h"

double get_chi_squared_p(double chi_squared) {

    /* p is given by 1 - P(chi_squared, df) where P is the Cumulative Distribution Function of the Chi-squared distribution.
     * P is also the regularized gamma function. P is computed using samtool's implementation of the regularized gamma function.
     * Source: https://en.wikipedia.org/wiki/Chi-squared_distribution#Cumulative_distribution_function
     * DF is always 1 in our case
     */

    return std::min(1.0, 1 - kf_gammap(0.5, chi_squared / 2));
}





double get_chi_squared(uint n_group1, uint n_group2, uint total_group1, uint total_group2) {

    /* Chi squared is computed from the number of individuals in each group.
     * Yates correction is applied and the shortcut formula for 2x2 table is used.
     * Source: https://en.wikipedia.org/wiki/Yates%27s_correction_for_continuity
     */

    uint N = total_group1 + total_group2;
    uint Ns = total_group1, Nf = total_group2;
    uint Na = n_group1 + n_group2, Nb = total_group1 + total_group2 - n_group1 - n_group2;

    int temp = static_cast<int>((n_group1 * total_group2) - (n_group2 * total_group1));
    temp = std::abs(temp);
    double temp2 = std::max(0.0, double(temp) - N/2);
    temp2 *= temp2;

    return N * temp2 / Ns / Nf / Na / Nb;
}





// Simple wrapper for get_chi_squared and get_chi_squared_p
double get_p_association(uint n_group1, uint n_group2, uint total_group1, uint total_group2) {

    double chi_squared = get_chi_squared(n_group1, n_group2, total_group1, total_group2);

    double p = 1.0;
    if (chi_squared == chi_squared) p = std::min(1.0, get_chi_squared_p(chi_squared));   // Test fails --> chi_squared is NaN --> sequence found in all individuals --> leave p to 1

    if (p < 0.0000000000000001) p = 0.0000000000000001;   // Lower bound for p: 10^-15

    return p;

}
