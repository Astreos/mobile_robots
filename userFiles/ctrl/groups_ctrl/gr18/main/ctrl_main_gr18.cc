/*!
 * \author Group 18
 * \file controller_main_gr18.cc
 * \brief Initialization, loop and finilization of the controller written in C (but compiled as C++)
 */

#include "ctrl_main_gr18.h"
#include "namespace_ctrl.h"
#include "init_pos_gr18.h"
#include "odometry_gr18.h"
#include "opp_pos_gr18.h"
#include "speed_regulation_gr18.h"
#include "calibration_gr18.h"
#include "triangulation_gr18.h"
#include "strategy_gr18.h"
#include "path_regulation_gr18.h"
#include "path_planning_gr18.h"
#include "kalman_gr18.h"
#include "useful_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/*! \brief initialize controller operations (called once)
 *
 * \param[in] cvs controller main structure
 */
void controller_init(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;

	double t;

	// variables initialization
	inputs = cvs->inputs;

	t = inputs->t;

	// robot ID
	cvs->robot_id = inputs->robot_id;

	// robot team
	switch (inputs->robot_id)
	{
	case ROBOT_B: cvs->team_id = TEAM_A; break;
	case ROBOT_R: cvs->team_id = TEAM_A; break;
	case ROBOT_Y: cvs->team_id = TEAM_B; break;
	case ROBOT_W: cvs->team_id = TEAM_B; break;

	default:
		printf("Error: unknown robot ID: %d !\n", inputs->robot_id);
		exit(EXIT_FAILURE);
	}

	// number of opponents
	cvs->nb_opp = inputs->nb_opponents;

	// robot initial position
	set_init_position(cvs->robot_id, cvs->rob_pos);
	cvs->rob_pos->last_t = t;

	// speed regulation
	cvs->sp_reg->last_t = t;
	cvs->sp_reg->int_error_r = 0;
	cvs->sp_reg->int_error_l = 0;

	// position regulation
	cvs->pos_reg->last_t = t;
	cvs->pos_reg->int_error_r = 0;
	cvs->pos_reg->int_error_l = 0;

	return;
}

/*! \brief controller loop (called every time-step)
 *
 * \param[in] cvs controller main structure
 */
void controller_loop(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;
	CtrlOut *outputs;

	double t;

	// variables initialization
	inputs  = cvs->inputs;
	outputs = cvs->outputs;

	t = inputs->t;

	// update the robot odometry
	update_odometry(cvs);

	// triangulation
	triangulation(cvs);

	// opponents position
	opponents_tower(cvs);
	check_opp_front(cvs);

	// tower control
	outputs->tower_command = 15;

	switch (cvs->main_state)
	{
	// calibration
	case CALIB_STATE:
		calibration(cvs);
		break;

	// wait before match beginning
	case WAIT_INIT_STATE:
		speed_regulation(cvs, 0.0, 0.0);

		// kalman
		cvs->kalman_pos->x = cvs->rob_pos->x;
		cvs->kalman_pos->y = cvs->rob_pos->y;
		cvs->kalman_pos->theta = cvs->rob_pos->theta;

		if (t > 0.0)
		{
			cvs->main_state = RUN_STATE;
			cvs->strat->main_state = FIRST_TARGET;
			cvs->strat->sub_state = TRAJECTORY;
			cvs->pos_reg->path_state = FOLLOW_CHECKPOINTS;
		}
		break;

	// during game
	case RUN_STATE:
		// kalman
		kalman(cvs);

		main_strategy(cvs);

		if (t > 89.0) // 1 second safety
		{
			cvs->main_state = STOP_END_STATE;
		}
		break;

	// stop at the end of the game
	case STOP_END_STATE:
		speed_regulation(cvs, 0.0, 0.0);

		outputs->flag_release = 1;
		break;

	case NB_MAIN_STATES:
		printf("Error: state NB_MAIN_STATES should not be reached !\n");
		exit(EXIT_FAILURE);
		break;

	default:
		printf("Error:unknown state : %d !\n", cvs->main_state);
		exit(EXIT_FAILURE);
	}

	return;
}

/*! \brief last controller operations (called once)
 *
 * \param[in] cvs controller main structure
 */
void controller_finish(CtrlStruct *cvs)
{
	return;
}

NAMESPACE_CLOSE();
