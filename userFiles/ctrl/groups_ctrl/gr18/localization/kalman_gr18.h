/*! 
 * \author Group 18
 * \file kalman_gr18.h
 * \brief localization sensors fusion with Kalman
 */

#ifndef _KALMAN_GR18_H_
#define _KALMAN_GR18_H_

#include "CtrlStruct_gr18.h"
#include "init_pos_gr18.h"
//
NAMESPACE_INIT(ctrlGr18);

/// Kalman main structure
typedef struct KalmanStruct
{
	double x; ///< x position [m]
	double y; ///< y position [m]
	double theta; ///< robot orientation [rad]
	
	double last_t; ///< last time position was updated
	
	double P[9];
} KalmanStruct;

void kalman(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
