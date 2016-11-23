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
	RobotPosition *rob_pos_XY; // coordonnées pour tableau
	RobotPosition *rob_goal_XY;
	int goal_id;
};

PathPlanning* init_path_planning(CtrlStruct *cvs, RobotPosition *robposition);
void free_path_planning(PathPlanning *path);

void update_path_planning(CtrlStruct *cvs);
void mapcreating();
void number_assigment(int x, int y);
int trajectory(CtrlStruct *cvs, double goal_x, double goal_y);
void xy_to_XY(CtrlStruct *cvs);
double X_to_x(int X);
double Y_to_y(int Y);

static int somme_pre = 4000; // somme min précédente ,arbitraire pour etre hyper grand a l'initialisation

NAMESPACE_CLOSE();

#endif
