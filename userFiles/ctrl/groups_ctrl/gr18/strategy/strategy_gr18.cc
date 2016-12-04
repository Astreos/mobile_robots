#include "strategy_gr18.h"
#include "init_pos_gr18.h"
#include "path_planning_gr18.h"
#include "speed_regulation_gr18.h"
#include "path_regulation_gr18.h"
#include "opp_pos_gr18.h"
#include "odometry_gr18.h"
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
    PathPlanning *path;

    // variables initialization
    strat  = cvs->strat;
    inputs = cvs->inputs;
    path = cvs->path;

    switch (strat->main_state)
    {
    case GAME_STATE_A:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.77, 0);
		}
		if (follow_path(cvs, -0.77, 0))
		{
			path->flag_trajectory = 0;
			strat->main_state = GAME_STATE_B;
		}
        break;

    case GAME_STATE_B:
		if (turn(cvs, M_PI/4.0, 0))
		{
			strat->main_state = GAME_STATE_C;
		}
        break;

    case GAME_STATE_C:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.40, 0.60);
		}
		if (follow_path(cvs, -0.40, 0.60))
		{
			path->flag_trajectory = 0;
			strat->main_state = GAME_STATE_D;
		}
        break;

    case GAME_STATE_D:
		if (turn(cvs, 0, 0))
		{
			strat->main_state = GAME_STATE_E;
		}
        break;

    case GAME_STATE_E:
		if (path->flag_trajectory != 1)
		{
			trajectory(cvs, -0.70, 1.30);
		}
		if (follow_path(cvs, -0.70, 1.30))
		{
			speed_regulation(cvs, 0, 0);
		}
        break;

    default:
        printf("Error: unknown strategy main state: %d !\n", strat->main_state);
        exit(EXIT_FAILURE);
    }
}

NAMESPACE_CLOSE();
