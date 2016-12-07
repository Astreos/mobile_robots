/*! 
 * \author Group 18
 * \file speed_regulation.h
 * \brief speed regulation
 */

#ifndef _SPEED_REGULATION_GR18_H_
#define _SPEED_REGULATION_GR18_H_ 

#include "namespace_ctrl.h"
#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// speed regulation
typedef struct SpeedRegulation
{
	double int_error_r; ///< integral term of the error for the right wheel
	double int_error_l; ///< integral term of the error for the left wheel

	double last_t; ///< last time the speed regulation was updated

} SpeedRegulation;

void speed_regulation(CtrlStruct *cvs, double r_sp_ref, double l_sp_ref);

NAMESPACE_CLOSE();

#endif
