#include "init_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief set the initial robot position
 * 
 * \param[in] robot_id robot ID
 * \param[out] rob_pos robot position structure
 *
 * Adapt these initial positions, depending on the game map.
 */
void set_init_position(int robot_id, RobotPosition *rob_pos)
{
	switch (robot_id)
	{
		case ROBOT_B: // blue robot
			rob_pos->x = 0.67;
			rob_pos->y = 1.15;
			rob_pos->theta = -90.0;
			break;

		case ROBOT_R: // red robot
			rob_pos->x = 0.0;
			rob_pos->y = 0.0;
			rob_pos->theta = 0.0;
			break;

		case ROBOT_Y: // yellow robot
			rob_pos->x = 0.0;
			rob_pos->y = 0.0;
			rob_pos->theta = 0.0;
			break;

		case ROBOT_W: //  white robot
			rob_pos->x = 0.0;
			rob_pos->y = 0.0;
			rob_pos->theta = 0.0;
			break;
	
		default:
			printf("Error: unknown robot ID: %d !\n", robot_id);
			exit(EXIT_FAILURE);
	}		
}

NAMESPACE_CLOSE();
