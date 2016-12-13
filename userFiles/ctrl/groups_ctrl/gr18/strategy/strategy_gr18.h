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
	
	double** list_targets_pos;
	int* list_targets;
	int current_action;
	
} Strategy;

/// 'main_state' states (adapt with your own states)
enum {GAME_STATE_A, GAME_STATE_B, GAME_STATE_C, GAME_STATE_D, GAME_STATE_E};

Strategy* init_strategy();
void free_strategy(Strategy *strat);
void main_strategy(CtrlStruct *cvs);
void manage_THE_target(CtrlStruct *cvs);
void manage_first_target(CtrlStruct *cvs);
void manage_second_target(CtrlStruct *cvs);
void win_points(CtrlStruct *cvs);

NAMESPACE_CLOSE();

#endif
