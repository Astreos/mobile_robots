#include "strategy_gr18.h"
#include "init_pos_gr18.h"
#include "path_planning_gr18.h"
#include "speed_regulation_gr18.h"
#include "path_regulation_gr18.h"
#include "opp_pos_gr18.h"
#include "odometry_gr18.h"
#include "useful_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief intitialize the strategy structure
 *
 * \return strategy structure initialized
 */
Strategy* init_strategy()
{
    Strategy *strat;
	int i;
	
	// memory allocation
    strat = (Strategy*) malloc(sizeof(Strategy));
	if (strat == NULL) {exit(0);}
	
	// ----- strategy initialization start ----- //
	
	// list_targets_pos
	strat->list_targets_pos = (double**) malloc(8*sizeof(double*));
	if (strat->list_targets_pos == NULL) {exit(0);}
	
	for(i=0; i<8; i++)
	{
		strat->list_targets_pos[i] = (double*) malloc(2*sizeof(double));
		if (strat->list_targets_pos[i] == NULL) {exit(0);}
	}
	
	strat->list_targets_pos[0][0] = -0.8;
	strat->list_targets_pos[0][1] = 0.0;
	strat->list_targets_pos[1][0] = -0.4;
	strat->list_targets_pos[1][1] = -0.6;
	strat->list_targets_pos[2][0] = -0.4;
	strat->list_targets_pos[2][1] = 0.6;
	strat->list_targets_pos[3][0] = 0.25;
	strat->list_targets_pos[3][1] = -1.25;
	strat->list_targets_pos[4][0] = 0.1;
	strat->list_targets_pos[4][1] = 0.0;
	strat->list_targets_pos[5][0] = 0.25;
	strat->list_targets_pos[5][1] = 1.25;
	strat->list_targets_pos[6][0] = 0.7;
	strat->list_targets_pos[6][1] = -0.6;
	strat->list_targets_pos[7][0] = 0.7;
	strat->list_targets_pos[7][1] = 0.6;
	
	// list_targets
	strat->list_targets = (int*) malloc(8*sizeof(int));
	if (strat->list_targets == NULL) {exit(0);}
	
	strat->list_targets[0] = 0;
	strat->list_targets[1] = 5;
	strat->list_targets[2] = 4;
	strat->list_targets[3] = 3;
	strat->list_targets[4] = 1;
	strat->list_targets[5] = 6;
	strat->list_targets[6] = 2;
	strat->list_targets[7] = 7;
	
	// current target
	strat->current_action = 0;
	
	// ----- strategy initialization end ----- //

    return strat;
}

/*! \brief release the strategy structure (memory released)
 *
 * \param[out] strat strategy structure to release
 */
void free_strategy(Strategy *strat)
{
	// variable declaration
	int i;
	
	// ----- strategy memory release start ----- //
	
	// list_targets_pos
	for(i=0; i<8; i++)
	{
		free(strat->list_targets_pos[i]);
	}
	free(strat->list_targets_pos);
	
	// list_targets
	free(strat->list_targets);
	
    free(strat);
	
	// ----- strategy memory release end ----- //
}

/*! \brief startegy during the game
 *
 * \param[in,out] cvs controller main structure
 */
void main_strategy(CtrlStruct *cvs)
{
    // variables declaration
    Strategy *strat;
    CtrlIn *inputs;
	CtrlOut *outputs;
    PathPlanning *path;
	int team_id;

    // variables initialization
    strat  = cvs->strat;
    inputs = cvs->inputs;
	outputs = cvs->outputs;
    path = cvs->path;
	team_id = cvs->team_id;

    switch (strat->main_state)
    {
    case GAME_STATE_A:
		manage_THE_target(cvs);
        break;

    case GAME_STATE_B:
		manage_first_target(cvs);
        break;

	case GAME_STATE_C:
		manage_second_target(cvs);
		break;
		
    case GAME_STATE_D:
		win_points(cvs);
        break;

    case GAME_STATE_E:
		speed_regulation(cvs, -7, 7);
		break;

    default:
        printf("Error: unknown strategy main state: %d !\n", strat->main_state);
        exit(EXIT_FAILURE);
    }
}

void manage_THE_target(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	int team_id;
	double goal_x, goal_y;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	team_id = cvs->team_id;
	goal_x = strat->list_targets_pos[strat->list_targets[strat->current_action]][0];
	goal_y = strat->list_targets_pos[strat->list_targets[strat->current_action]][1];
	
	outputs->flag_release = 0;
	
	if (path->flag_trajectory != 1)
	{
		trajectory(cvs, goal_x, goal_y*team(team_id));
	}
	if (follow_path(cvs, goal_x, goal_y*team(team_id)))
	{
		if (inputs->nb_targets == 1)
		{
			strat->current_action += 1;
			strat->main_state = GAME_STATE_C;
		}
	}
}

void manage_first_target(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	int team_id;
	double goal_x, goal_y;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	team_id = cvs->team_id;
	goal_x = strat->list_targets_pos[strat->list_targets[strat->current_action]][0];
	goal_y = strat->list_targets_pos[strat->list_targets[strat->current_action]][1];
	
	outputs->flag_release = 0;
	
	if (path->flag_trajectory != 1)
	{
		trajectory(cvs, goal_x, goal_y*team(team_id));
	}
	if (follow_path(cvs, goal_x, goal_y*team(team_id)))
	{
		if (inputs->nb_targets == 1 && strat->current_action == 7)
		{
			strat->main_state = GAME_STATE_D;
		}
		else if (inputs->nb_targets == 1 && strat->current_action < 7)
		{
			strat->current_action += 1;
			strat->main_state = GAME_STATE_C;
		}
	}
}

void manage_second_target(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	int team_id;
	double goal_x, goal_y;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	team_id = cvs->team_id;
	goal_x = strat->list_targets_pos[strat->list_targets[strat->current_action]][0];
	goal_y = strat->list_targets_pos[strat->list_targets[strat->current_action]][1];
	
	outputs->flag_release = 0;
	
	if (path->flag_trajectory != 1)
	{
		trajectory(cvs, goal_x, goal_y*team(team_id));
	}
	if (follow_path(cvs, goal_x, goal_y*team(team_id)))
	{
		if (inputs->nb_targets == 2)
		{
			strat->main_state = GAME_STATE_D;
		}
	}
}

void win_points(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	int team_id;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	team_id = cvs->team_id;
	
	if (path->flag_trajectory != 1)
	{
		trajectory(cvs, -0.70, -1.15*team(team_id));
	}
	if (follow_path(cvs, -0.70, -1.15*team(team_id)))
	{
		outputs->flag_release = 1;
		
		if (strat->current_action == 7)
		{
			strat->main_state = GAME_STATE_E;
		}
		else
		{
			strat->current_action += 1;
			strat->main_state = GAME_STATE_B;
		}
	}
}

NAMESPACE_CLOSE();
