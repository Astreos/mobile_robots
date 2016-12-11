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

    strat = (Strategy*) malloc(sizeof(Strategy));
	if (strat == NULL) {exit(0);}

    return strat;
}

/*! \brief release the strategy structure (memory released)
 *
 * \param[out] strat strategy structure to release
 */
void free_strategy(Strategy *strat)
{
    free(strat);
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
		outputs->flag_release = 0;
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.77, 0*team(team_id));
		}
		if (follow_path(cvs, -0.77, 0*team(team_id)))
		{
			if (inputs->nb_targets == 1)
			{
				strat->main_state = GAME_STATE_B;
			}
		}
        break;

    case GAME_STATE_B:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.40, -0.60*team(team_id));
		}
		if (follow_path(cvs, -0.40, -0.60*team(team_id)))
		{
			if (inputs->nb_targets == 2)
			{
				strat->main_state = GAME_STATE_C;
			}
		}
        break;

	case GAME_STATE_C:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.70, -1.15*team(team_id));
		}
		if (follow_path(cvs, -0.70, -1.15*team(team_id)))
		{
			outputs->flag_release = 1;
			strat->main_state = GAME_STATE_D;
		}
		break;
		
    case GAME_STATE_D:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, 0.10, 0*team(team_id));
		}
		outputs->flag_release = 0;
		if (follow_path(cvs, 0.10, 0*team(team_id)))
		{
			if (inputs->nb_targets == 1)
			{
				strat->main_state = GAME_STATE_E;
			}
		}
        break;

    case GAME_STATE_E:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, 0.25, -1.25*team(team_id));
		}
		if (follow_path(cvs, 0.25, -1.25*team(team_id)))
		{
			if (inputs->nb_targets == 2)
			{
				strat->main_state = GAME_STATE_F;
			}
		}
		break;
		
	case GAME_STATE_F:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.70, -1.15*team(team_id));
		}
		if (follow_path(cvs, -0.70, -1.15*team(team_id)))
		{
			outputs->flag_release = 1;
			strat->main_state = GAME_STATE_G;
		}
		break;
		
	case GAME_STATE_G:
		outputs->flag_release = 0;
		speed_regulation(cvs, 0, 0);
		break;

    default:
        printf("Error: unknown strategy main state: %d !\n", strat->main_state);
        exit(EXIT_FAILURE);
    }
}

NAMESPACE_CLOSE();
