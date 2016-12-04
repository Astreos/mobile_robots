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
			trajectory(cvs, -0.5, -1.1);
			/*for (int i=0; i<=path->nb_goals; i++)
			{
				printf("X = %d et Y= %d", path->list_goal[i][0], path->list_goal[i][1]);
				printf("goal_x = %f et goal_y = %f \n", X_to_x(path->list_goal[i][0]), Y_to_y(path->list_goal[i][1]));
			}*/
		}
		if (follow_path(cvs))
		{
			printf("OKAY");
		}
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
