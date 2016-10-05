/*! 
 * \author Group 18
 * \file calibration_gr18.h
 * \brief calibration of the robot
 */

#ifndef _CALIBRATION_GR18_H_
#define _CALIBRATION_GR18_H_ 
 
#include "namespace_ctrl.h"
#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// robot calibration
typedef struct RobotCalibration
{
	double t_flag; ///< time for flag

	int flag; ///< flag for calibration

} RobotCalibration;

void calibration(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
