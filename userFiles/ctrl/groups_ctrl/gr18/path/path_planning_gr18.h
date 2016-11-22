/*!
* \author Group 18
* \file path_planning_gr18.h
* \brief path-planning algorithm
*/

#define CELL_X 20
#define CELL_Y 30
#define CELL_SIZE 250


#ifndef _PATH_PLANNING_GR18_H_
#define _PATH_PLANNING_GR18_H_

#include "namespace_ctrl.h"
#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// path-planning main structure
struct PathPlanning
{
	RobotPosition *rob_pos_XY; // coordonnées pour tableau
	RobotPosition *rob_goal_XY;
	int goal_id;

};

PathPlanning* init_path_planning(CtrlStruct *cvs, RobotPosition *robposition);
void free_path_planning(PathPlanning *path);

void update_path_planning(CtrlStruct *cvs);
void mapcreating();
void number_assigment(int x, int y);
void trajectory(int x, int y);
void coordonnee_tab(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
