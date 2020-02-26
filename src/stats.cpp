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

#include "stats.h"

double get_chi_squared_p(double chi_squared) {

    /* p is given by 1 - P(chi_squared, df) where P is the Cumulative Distribution Function of the Chi-squared distribution.
     * P is also the regularized gamma function. Here we use samtool's implementation of the regularized gamma function by Heng Li.
     * Source: https://en.wikipedia.org/wiki/Chi-squared_distribution#Cumulative_distribution_function
     * DF is always 1 in our case
     */

    return std::min(1.0, 1 - kf_gammap(0.5, chi_squared/2));
}


double get_chi_squared(uint n_males, uint n_females, uint total_males, uint total_females) {

    /* Chi squared is computed from the number of males and females with the sequence, as well as total number of males and females in the population.
     * Yates correction is applied and the shortcut formula for 2x2 table is used.
     * Source: https://en.wikipedia.org/wiki/Yates%27s_correction_for_continuity
     */

    uint N = total_males + total_females;
    uint Ns = total_males, Nf = total_females;
    uint Na = n_males + n_females, Nb = total_males + total_females - n_males - n_females;

    int temp = static_cast<int>((n_males * total_females) - (n_females * total_males));
    temp = std::abs(temp);
    double temp2 = std::max(0.0, double(temp) - N/2);
    temp2 *= temp2;

    return N * temp2 / Ns / Nf / Na / Nb;
}
