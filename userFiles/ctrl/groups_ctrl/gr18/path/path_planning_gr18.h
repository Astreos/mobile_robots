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

typedef struct PositionXY
{
	int X; ///< X position
	int Y; ///< Y position
} PositionXY;

/// path-planning main structure
typedef struct PathPlanning
{
	int **map;
	int flag_trajectory;
	int **list_goal;
	int nb_goals;
	int count_actions;
	
	PositionXY *rob_pos_XY; // coordonnées en cases
	PositionXY *goal_XY;
} PathPlanning;

PathPlanning* init_path_planning();
void free_path_planning(PathPlanning *path);

void trajectory(CtrlStruct *cvs, double goal_x, double goal_y);
void create_map(CtrlStruct *cvs);
void manage_opp(CtrlStruct *cvs, int delta);
void assign_numbers(CtrlStruct *cvs);
void find_path(CtrlStruct *cvs);

int x_to_X(double x);
int y_to_Y(double y);
double X_to_x(int X);
double Y_to_y(int Y);

NAMESPACE_CLOSE();

#endif
