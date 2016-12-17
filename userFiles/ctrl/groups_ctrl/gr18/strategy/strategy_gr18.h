/*!
 * \author Group 18
 * \file strategy_gr18.h
 * \brief strategy during the game
 */

#ifndef _STRATEGY_GR18_H_
#define _STRATEGY_GR18_H_

#include "CtrlStruct_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/// strategy main structure
typedef struct Strategy
{
	int main_state; ///< main state of the strategy
	int sub_state; ///< sub state of the strategy

	int goal_determination;
	int goal;
	
	int nb_targets;
	int current_action;

	double** goal_tab;
	int** list_targets;

	double last_t;

} Strategy;

/// 'main_state' states (adapt with your own states)
enum {FIRST_TARGET, SECOND_TARGET, WIN_POINTS, CALIBRATE, FUNNY_ACTION};

/// 'sub_state' states (adapt with your own states)
enum {TRAJECTORY, FOLLOW_PATH, CHECK_TARGET, GET_TARGET, RELEASE_TARGET};

Strategy* init_strategy();
void free_strategy(Strategy *strat);

void main_strategy(CtrlStruct *cvs);
void manage_first_target(CtrlStruct *cvs);
void manage_second_target(CtrlStruct *cvs);

void win_points(CtrlStruct *cvs);
void calibrate(CtrlStruct *cvs);
void update_goal(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
