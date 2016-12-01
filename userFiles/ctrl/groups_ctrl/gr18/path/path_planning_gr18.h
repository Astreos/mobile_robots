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
typedef struct PathPlanning
{
	float **map;
	int **list_goal;
	
	//RobotPosition *rob_pos_XY; // coordonnées en cases
} PathPlanning;

PathPlanning* init_path_planning();
void free_path_planning(PathPlanning *path);

void trajectory(CtrlStruct *cvs, double goal_x, double goal_y);
void create_map(CtrlStruct *cvs);
void assign_numbers(CtrlStruct *cvs);
void find_path(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
