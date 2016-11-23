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

	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;

	switch (strat->main_state)
	{
            case GAME_STATE_A:
            /*
            somme_pre = 4000;
			xy_to_XY(cvs);
			if (trajectory(cvs, 0, 0))
			{
				strat->main_state = GAME_STATE_B;
			}
			* */
			speed_regulation(cvs, 0.0, 0.0);
			break;

		case GAME_STATE_B:
			speed_regulation(cvs, 0.0, 0.0);
			break;

		case GAME_STATE_C:
			speed_regulation(cvs, 0.0, 0.0);
			break;

		case GAME_STATE_D:
			speed_regulation(cvs, 0.0, 0.0);
			break;

		case GAME_STATE_E:
			speed_regulation(cvs, 0.0, 0.0);
			break;

		default:
			printf("Error: unknown strategy main state: %d !\n", strat->main_state);
			exit(EXIT_FAILURE);
	}
}

NAMESPACE_CLOSE();
