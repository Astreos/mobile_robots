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
	int flag; ///< flag for calibration
	
	double t_flag; ///< time for flag

} RobotCalibration;

// calibration states
enum {CALIB_START, CALIB_STATE_A, CALIB_STATE_B, CALIB_STATE_C, CALIB_STATE_D, CALIB_STATE_E, CALIB_FINISH};

void calibration(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
