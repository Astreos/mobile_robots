/*!
* \author Group 18
* \file path_planning_gr18.h
* \brief path-planning algorithm
*/

#ifndef _PATH_PLANNING_GR18_H_
#define _PATH_PLANNING_GR18_H_

#include "namespace_ctrl.h"
#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// path-planning main structure
struct PathPlanning
{
	RobotPosition *rob_pos_XY; // coordonnées en cases
};

PathPlanning* init_path_planning(CtrlStruct *cvs);
void free_path_planning(PathPlanning *path);

void create_map(void);
void trajectory(CtrlStruct *cvs, double goal_x, double goal_y);

NAMESPACE_CLOSE();

#endif
