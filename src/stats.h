#pragma once
#include "kfun/kfun.h"
#include "utils.h"

// Compute Chi-square from the number of males and females with a sequence as well as total males and total females
double get_chi_squared(uint n_males, uint n_females, uint total_males, uint total_females);

// Compute the p-value for a Chi-square
double get_chi_squared_p(double chi_squared);
