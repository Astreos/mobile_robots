/*! 
 * \author Nicolas Van der Noot
 * \file simu_functions.hh
 * \brief Useful functions for the simulation
 */

#ifndef _SIMU_FUNCTIONS_HH_
#define _SIMU_FUNCTIONS_HH_

#include "Vector3D.hh"

double limit_range(double value, double min_val, double max_val);
int is_in_range(double value, double min_val, double max_val);
double limit_angle(double value);
double linear_interp(double x, double x1, double x2, double y1, double y2);
Vector3D linear_interp_vec_3D(double x, double x1, double x2, Vector3D Y1, Vector3D Y2);

#endif
