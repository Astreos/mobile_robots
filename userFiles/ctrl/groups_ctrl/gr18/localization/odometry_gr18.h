/*! 
 * \author Group 18
 * \file odometry_gr18.h
 * \brief odometry of the robot
 */

#ifndef _ODOMETRY_GR18_H_
#define _ODOMETRY_GR18_H_ 
 
#include "namespace_ctrl.h"
#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

void update_odometry(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
