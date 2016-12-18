/*! 
 * \author Group 18
 * \file init_pos_gr18.h
 * \brief opponents position
 */

#ifndef _OPP_POS_GR18_H_
#define _OPP_POS_GR18_H_ 
 
#include "namespace_ctrl.h"
#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// robot position
typedef struct OpponentsPosition
{
	double x[2]; ///< x position of opponents [m]
	double y[2]; ///< y position of opponents [m]

	double last_t; ///< last time the filters were updated [s]

	int nb_opp; ///< number of opponents
	
	int opp_front;
	
	int previous_nb_rising;
	int previous_nb_falling;
	int opp_switch;
	int switch_check;

} OpponentsPosition;

// function prototype
void check_opp_front(CtrlStruct *cvs);
void opponents_tower(CtrlStruct *cvs);
int single_opp_tower(CtrlStruct *cvs, double last_rise, double last_fall, double *new_x_opp, double *new_y_opp);

NAMESPACE_CLOSE();

#endif
