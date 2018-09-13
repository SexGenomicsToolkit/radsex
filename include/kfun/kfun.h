#pragma once
#include <math.h>


double kf_lgamma(double z);

double kf_erfc(double x);

#define KF_GAMMA_EPS 1e-14
#define KF_TINY 1e-290

double kf_gammap(double s, double z);

double kf_gammaq(double s, double z);

double kf_betai(double a, double b, double x);
