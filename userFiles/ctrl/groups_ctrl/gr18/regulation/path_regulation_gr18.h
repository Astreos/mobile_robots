/*! 
 * \author Group 18
 * \file path_regulation_gr18.h
 * \brief regulation to follow a given path
 */

#ifndef _PATH_REGULATION_GR18_H_
#define _PATH_REGULATION_GR18_H_

#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// position regulation
typedef struct PosRegulation
{
	double int_error_r; ///< integral term of the error for the right wheel
	double int_error_l; ///< integral term of the error for the left wheel

	double last_t; ///< last time the speed regulation was updated

} PosRegulation;

int follow_path(CtrlStruct *cvs);
int turn(CtrlStruct *cvs, double theta_ref, int sens);
int run_x(CtrlStruct *cvs, double x_ref);
int run_y(CtrlStruct *cvs, double y_ref);
int run(CtrlStruct *cvs, double x_ref, double y_ref, double theta_ref);

NAMESPACE_CLOSE();

#endif
