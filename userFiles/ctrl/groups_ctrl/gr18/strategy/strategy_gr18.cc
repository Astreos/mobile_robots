#include "strategy_gr18.h"
#include "init_pos_gr18.h"
#include "path_planning_gr18.h"
#include "speed_regulation_gr18.h"
#include "path_regulation_gr18.h"
#include "opp_pos_gr18.h"
#include "odometry_gr18.h"
#include "useful_gr18.h"
#include "kalman_gr18.h"
#include <math.h>

#define DIST_MAX 3778.0
#define DIST_MIN 10.0
#define TPS_MAX 90.0

#define AREA 250

#define FINAL_POS_X -700.0
#define FINAL_POS_Y -1150.0

#define COEFF_PTS 35.0
#define COEFF_DIST_TPS 5.0
#define COEFF_BASE_TPS 5.0

#define COORD_X 0
#define COORD_Y 1
#define VALUE   2
#define WEIGHT  3

#define CHECK1 0
#define CHECK2 1
#define OPP 2
#define US 3

#define GOAL0 0
#define GOAL1 1
#define GOAL2 2
#define GOAL3 3
#define GOAL4 4
#define GOAL5 5
#define GOAL6 6
#define GOAL7 7

NAMESPACE_INIT(ctrlGr18);

/*! \brief intitialize the strategy structure
 * 
 * \return strategy structure initialized
 */
Strategy* init_strategy()
{
	// variables initialization
	Strategy *strat;
	
	int i, j;
	
	// memory allocation
	strat = (Strategy*) malloc(sizeof(Strategy));
	if (strat == NULL) {exit(0);}
	
	// ----- strategy initialization start ----- //
	
	// main state
	strat->main_state = FIRST_TARGET;
	
	// sub state
	strat->sub_state = TRAJECTORY;
	
	// goals_tab initialization (x,y,value,weight)
	strat->goals_tab = (double**) malloc(8*sizeof(double*));
	if (strat->goals_tab == NULL) {exit(0);}
	
	for (i = 0; i < 8; i++)
	{
		strat->goals_tab[i] = (double*) malloc(4*sizeof(double));
		if (strat->goals_tab[i] == NULL) {exit(0);}
		
		strat->goals_tab[i][WEIGHT] = 100.0;		
	}
	
	// GOAL 0 (700,600)
	strat->goals_tab[GOAL0][COORD_X] = 700.0;
	strat->goals_tab[GOAL0][COORD_Y] = 600.0;
	strat->goals_tab[GOAL0][VALUE] = 1.0;
	// GOAL 1 (250,1250)
	strat->goals_tab[GOAL1][COORD_X] = 250.0;
	strat->goals_tab[GOAL1][COORD_Y] = 1250.0;
	strat->goals_tab[GOAL1][VALUE] = 4.0;
	// GOAL 2 (-400,600)
	strat->goals_tab[GOAL2][COORD_X] = -400.0;
	strat->goals_tab[GOAL2][COORD_Y] = 600.0;
	strat->goals_tab[GOAL2][VALUE] = 1.0;
	// GOAL 3 (-800,0)
	strat->goals_tab[GOAL3][COORD_X] = -800.0;
	strat->goals_tab[GOAL3][COORD_Y] = 0.0;
	strat->goals_tab[GOAL3][VALUE] = 5.0;
	// GOAL 4 (100,0)
	strat->goals_tab[GOAL4][COORD_X] = 100.0;
	strat->goals_tab[GOAL4][COORD_Y] = 0.0;
	strat->goals_tab[GOAL4][VALUE] = 3.0;
	// GOAL 5 (-400,600)
	strat->goals_tab[GOAL5][COORD_X] = -400.0;
	strat->goals_tab[GOAL5][COORD_Y] = -600.0;
	strat->goals_tab[GOAL5][VALUE] = 1.0;
	// GOAL 6 (700,-600)
	strat->goals_tab[GOAL6][COORD_X] = 700.0;
	strat->goals_tab[GOAL6][COORD_Y] = -600.0;
	strat->goals_tab[GOAL6][VALUE] = 1.0;
	// GOAL 7 (250,-1250)
	strat->goals_tab[GOAL7][COORD_X] = 250.0;
	strat->goals_tab[GOAL7][COORD_Y] = -1250.0;
	strat->goals_tab[GOAL7][VALUE] = 1.5;
	
	// targets_status
	strat->targets_status = (int**) malloc(8*sizeof(int*));
	if (strat->targets_status == NULL) {exit(0);}
	
	for (i = 0; i < 8; i++)
	{
		strat->targets_status[i] = (int*) malloc(4*sizeof(int));
		if (strat->targets_status[i] == NULL) {exit(0);}
		
		for (j = 0; j < 4; j++)
		{
			strat->targets_status[i][j] = false;
		}
	}
	
	// target not detected counter
	strat->target_not_detected = 0;
	
	//flags
	strat->current_goal = GOAL0;
	strat->goal_determination = false;
	strat->flag_finish = false;
	
	// opppnent management
	strat->opp_pos = (OpponentsPosition*) malloc(sizeof(OpponentsPosition*));
	if (strat->opp_pos == NULL) {exit(0);}
	
	strat->opponent_point = false;
	
	// last_t
	strat->last_t = 0;
	strat->last_t2 = 0;
	
	// ----- strategy initialization end ----- //
	
	return strat;
}

/*! \brief release the strategy structure (memory released)
 * 
 * \param[out] strat strategy structure to release
 */
void free_strategy(Strategy *strat)
{
	// variables initialization
	int i;
	
	// ----- strategy memory release start ----- //
	
	// goals_tab initialization
	for (i = 0; i < 8; i++)
	{
		free(strat->goals_tab[i]);
	}
	free(strat->goals_tab);
	
	// target_status
	for (i = 0; i < 8; i++)
	{
		free(strat->targets_status[i]);
	}
	free(strat->targets_status);
	
	// opppnent
	free(strat->opp_pos);
	
	free(strat);
	
	// ----- strategy memory release end ----- //
	
	return;
}

/*! \brief startegy during the game
 * 
 * \param[in,out] cvs controller main structure
 */
void main_strategy(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	
	int i;
	
	// variables initialization
	strat  = cvs->strat;
	
	// If all weights are 0, the game is over
	strat->flag_finish = (strat->goals_tab[GOAL0][WEIGHT] == 0.0) && (strat->goals_tab[GOAL1][WEIGHT] == 0.0) && (strat->goals_tab[GOAL2][WEIGHT] == 0.0) && (strat->goals_tab[GOAL3][WEIGHT] == 0.0) && (strat->goals_tab[GOAL4][WEIGHT] == 0.0) && (strat->goals_tab[GOAL5][WEIGHT] == 0.0) && (strat->goals_tab[GOAL6][WEIGHT] == 0.0) && (strat->goals_tab[GOAL7][WEIGHT] == 0.0);
	
	// While all the targets have not been checked yet
	if (!strat->flag_finish)
	{
		manage_us(cvs); // Check on which target we are
		manage_opp_target(cvs); // Check on which target the opponents are
		update_goal(cvs); // Update our new target goal
	}
	/*
	for (i = GOAL0; i <= GOAL7; i++)
	{
		printf("(value, weight, CHECK1, CHECK2, OPP, US) [%d] = (%f, %f, %d, %d, %d, %d)\n", i, strat->goals_tab[i][VALUE], strat->goals_tab[i][WEIGHT], strat->targets_status[i][CHECK1], strat->targets_status[i][CHECK2], strat->targets_status[i][OPP], strat->targets_status[i][US]);
	}
	printf("\n");
	*/
	switch (strat->main_state)
	{
		// Enter this state if we hold not any target
		case FIRST_TARGET:
			manage_first_target(cvs);
			break;
		
		// Enter this state if we hold 1 target
		case SECOND_TARGET:
			manage_second_target(cvs);
			break;
		
		// Go to the base to release the targets
		case WIN_POINTS:
			win_points(cvs);
			break;
		
		// Calibrate the odometry thanks to the Kalman filter
		case CALIBRATE:
			calibrate(cvs);
			break;
		
		// The game is over, let's do what we want !
		case FUNNY_ACTION:
			speed_regulation(cvs, -7, 7);
			break;
			
		default:
			printf("Error: unknown strategy main state: %d !\n", strat->main_state);
			exit(EXIT_FAILURE);
	}
	
	return;
}

void manage_first_target(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;
	CtrlOut *outputs;
	Strategy *strat;
	PosRegulation *pos_reg;
	PathPlanning *path;
	
	double goal_x, goal_y;
	int team_id;
	
	// variables initialization
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	strat  = cvs->strat;
	pos_reg = cvs->pos_reg;
	path = cvs->path;
		
	goal_x = strat->goals_tab[strat->current_goal][COORD_X]/1000;
	goal_y = strat->goals_tab[strat->current_goal][COORD_Y]/1000;
	team_id = cvs->team_id;
	
	switch (strat->sub_state)
	{
		// Let's compute a trajectory
		case TRAJECTORY:
			speed_regulation(cvs, 0, 0);
			if (!path->flag_trajectory) // If no trajectory is computed, compute one
			{
				trajectory(cvs, goal_x, goal_y*team(team_id));
			}
			else // Else, let's move
			{
				pos_reg->path_state = FOLLOW_CHECKPOINTS;
				strat->sub_state = FOLLOW_PATH;
			}
			break;
		
		// Let's move
		case FOLLOW_PATH:
			if (pos_reg->flag_asserv_done) // If we have arrived to the goal
			{
				speed_regulation(cvs, 0, 0);
				pos_reg->flag_asserv_done = false; // Clear the flag when the asserv is done
				strat->sub_state = CHECK_TARGET; // Let's check if one target can be hold
			}
			else // Else, move to the goal
			{
				follow_path(cvs, goal_x, goal_y*team(team_id));
			}
			break;
			
		// Let's check if one target can be hold
		case CHECK_TARGET:
			speed_regulation(cvs, 0, 0);
			outputs->flag_release = 0; // To be sure we can hold targets
			if (strat->targets_status[strat->current_goal][CHECK1]) // If we have already checked this one, or if we think the opponent has taken it
			{
				strat->targets_status[strat->current_goal][CHECK2] = true; // Then we won't check this target anymore
			}
			else
			{
				strat->targets_status[strat->current_goal][CHECK1] = true; // Else, remember we have already checked this target once
			}
			
			if (inputs->target_detected || (inputs->nb_targets == 1)) // If a target is detected, or if we already hold one
			{
				strat->sub_state = GET_TARGET; // Let's hold the target !
			}
			else if (strat->flag_finish) // Else if this was the last target to hold
			{
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS; // Let's go to win points
			}
			else // Else if no target is detected
			{
				strat->target_not_detected++; // We ensure we are not missing all the targets by incrementing this value
				strat->goal_determination = false; // Tell the update_goal it can determinate a new goal
				
				if (strat->target_not_detected >= 2 && strat->targets_status[strat->current_goal][CHECK1]) // If we have missed a target more than 2 times
				{
					strat->target_not_detected = 0; // Reset the counter
					strat->last_t = inputs->t; // Begin the calibration
					strat->main_state = CALIBRATE; // Let's calibrate
				}
				else // Else, let's try to find another target
				{
					strat->sub_state = TRAJECTORY;
				}
			}
			break;
		
		// Let's hold the target
		case GET_TARGET:
			speed_regulation(cvs, 0, 0);
			if (inputs->nb_targets == 1) // When the first target is hold
			{
				strat->targets_status[strat->current_goal][CHECK2] = true; // This target is won, we won't check it anymore
				strat->goal_determination = false; // Tell the update_goal it can determinate a new goal
				
				strat->sub_state = TRAJECTORY; // Let's find a new trajectory
				strat->main_state = SECOND_TARGET; // To hold a second target
			}
			else if (inputs->nb_targets == 2) // Safety, in the case of we already hold one target
			{
				strat->targets_status[strat->current_goal][CHECK2] = true; // This target is won, we won't check it anymore
				strat->goal_determination = false; // Tell the update_goal it can determinate a new goal
				
				strat->sub_state = TRAJECTORY; // Let's find a new trajectory
				strat->main_state = WIN_POINTS; // To go to the base to release the targets
			}
			break;
	}
	
	return;
}

void manage_second_target(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;
	CtrlOut *outputs;
	Strategy *strat;
	PosRegulation *pos_reg;
	PathPlanning *path;
	
	double goal_x, goal_y;
	int team_id;
	
	// variables initialization
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	strat  = cvs->strat;
	pos_reg = cvs->pos_reg;
	path = cvs->path;
	
	goal_x = strat->goals_tab[strat->current_goal][COORD_X]/1000;
	goal_y = strat->goals_tab[strat->current_goal][COORD_Y]/1000;
	team_id = cvs->team_id;
	
	switch (strat->sub_state)
	{
		// Let's compute a trajectory
		case TRAJECTORY:
			speed_regulation(cvs, 0, 0);
			if (!path->flag_trajectory)
			{
				trajectory(cvs, goal_x, goal_y*team(team_id));
			}
			else
			{
				pos_reg->path_state = FOLLOW_CHECKPOINTS;
				strat->sub_state = FOLLOW_PATH;
			}
			break;
		
		// Let's move
		case FOLLOW_PATH:
			if (pos_reg->flag_asserv_done)
			{
				speed_regulation(cvs, 0, 0);
				pos_reg->flag_asserv_done = false;
				strat->sub_state = CHECK_TARGET;
			}
			else
			{
				follow_path(cvs, goal_x, goal_y*team(team_id));
			}
			break;
			
		// Let's check if one target can be hold
		case CHECK_TARGET:
			speed_regulation(cvs, 0, 0);
			outputs->flag_release = 0; // To be sure we can hold targets
			if (strat->targets_status[strat->current_goal][CHECK1])
			{
				strat->targets_status[strat->current_goal][CHECK2] = true;
			}
			else
			{
				strat->targets_status[strat->current_goal][CHECK1] = true;
			}
			
			if (inputs->target_detected || (inputs->nb_targets == 2))
			{				
				strat->sub_state = GET_TARGET;
			}
			else if (strat->flag_finish)
			{
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS;
			}
			else
			{
				strat->target_not_detected++;
				strat->goal_determination = false;
				
				if (strat->target_not_detected >= 2 && strat->targets_status[strat->current_goal][CHECK1])
				{
					strat->target_not_detected = 0;
					strat->last_t = inputs->t;
					strat->main_state = CALIBRATE;
				}
				else
				{
					strat->sub_state = TRAJECTORY;
				}
			}
			break;
			
		// Let's hold the target
		case GET_TARGET:
			speed_regulation(cvs, 0, 0);
			if (inputs->nb_targets == 2)  // When the second target is hold
			{
				strat->targets_status[strat->current_goal][CHECK2] = true;
				strat->goal_determination = false;
				
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS;
			}
			break;
	}
	
	return;
}

void win_points(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;
	CtrlOut *outputs;
	Strategy *strat;
	PosRegulation *pos_reg;
	PathPlanning *path;
	
	int team_id;
	
	// variables initialization
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	strat  = cvs->strat;
	pos_reg = cvs->pos_reg;
	path = cvs->path;
	
	team_id = cvs->team_id;
	
	switch (strat->sub_state)
	{
		// Let's compute a trajectory
		case TRAJECTORY:
			speed_regulation(cvs, 0, 0);
			if (!path->flag_trajectory)
			{
				trajectory(cvs, -0.30, -1.30*team(team_id)); // We don't compute a trajectory to the base, but just before
			}
			else
			{
				pos_reg->path_state = FOLLOW_CHECKPOINTS;
				strat->sub_state = FOLLOW_PATH;
			}
			break;
			
		// Let's move
		case FOLLOW_PATH:
			if (pos_reg->flag_asserv_done) // If we are in the base
			{
				speed_regulation(cvs, 0, 0);
				pos_reg->flag_asserv_done = false;
				strat->sub_state = RELEASE_TARGET; // We can release the targets
			}
			else // Else move to the base
			{
				follow_path(cvs, -0.70, -1.15*team(team_id));
			}
			break;
			
		// Let's release the targets
		case RELEASE_TARGET:
			speed_regulation(cvs, 0, 0);
			outputs->flag_release = 1; // Realese the targets
			
			strat->last_t = inputs->t; // Begin the calibration
			strat->main_state = CALIBRATE; // Let's calibrate
			break;
	}
	
	return;
}

void calibrate(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;
	Strategy *strat;
	RobotPosition *rob_pos;
	KalmanStruct *pos_kalman;
	
	// variables initialization
	inputs = cvs->inputs;
	strat  = cvs->strat;
	rob_pos = cvs->rob_pos;
	pos_kalman = cvs->kalman_pos;
	
	speed_regulation(cvs, 0, 0);
	
	if (inputs->t - strat->last_t > 0.65) // Wait until the Kalman filter has updated a good value
	{
		rob_pos->x = pos_kalman->x;
		rob_pos->y = pos_kalman->y;
		// rob_pos->theta = pos_kalman->theta;
		
		if (strat->flag_finish) // If the game is over
		{
			strat->sub_state = TRAJECTORY; // Let's compute a new trajectory
			strat->main_state = FUNNY_ACTION; // To do anything we want
		}
		else // Else, go to check a new first target
		{
			strat->sub_state = TRAJECTORY;
			strat->main_state = FIRST_TARGET;
		}
	}
	
	return;
}

void update_goal(CtrlStruct *cvs)
{
	// variables initialization
	CtrlIn *inputs;
	Strategy *strat;
	RobotPosition *rob_pos;
	
	double dist_pts, dist_base;
	double t;
	int i;
	
	// variables declaration
	strat = cvs->strat;
	rob_pos = cvs->rob_pos;
	inputs = cvs->inputs;
	
	if (!strat->goal_determination) // If no goal is determinated
	{
		t = inputs->t;
		
		for(i = GOAL0; i <= GOAL7; i++) // For each goal, compute its weight
		{
			dist_pts = sqrt(pow(rob_pos->x*1000.0 - strat->goals_tab[i][COORD_X], 2) + pow(rob_pos->y*1000.0 - strat->goals_tab[i][COORD_Y], 2)); // Distance between the robot and the target
			dist_base = sqrt(pow(FINAL_POS_X - strat->goals_tab[i][COORD_X], 2) + pow(FINAL_POS_Y - strat->goals_tab[i][COORD_Y], 2)); // Distance between the target and the base
			
			if (!strat->targets_status[i][CHECK1]) // If we want to check this target
			{
				strat->goals_tab[i][WEIGHT] = 20.0*(COEFF_PTS*strat->goals_tab[i][VALUE] + (TPS_MAX + t)/TPS_MAX*(DIST_MAX/dist_pts)*COEFF_DIST_TPS + (TPS_MAX + t)/TPS_MAX*(DIST_MAX/dist_base)*COEFF_BASE_TPS);
			}
			else if (strat->targets_status[i][CHECK1] && !strat->targets_status[i][CHECK2]) // If we want to check this target
			{
				strat->goals_tab[i][WEIGHT] = COEFF_PTS*strat->goals_tab[i][VALUE] + (TPS_MAX + t)/TPS_MAX*(DIST_MAX/dist_pts)*COEFF_DIST_TPS + (TPS_MAX + t)/TPS_MAX*(DIST_MAX/dist_base)*COEFF_BASE_TPS;
			}
			else // Else if this target has already been checked twice
			{
				strat->goals_tab[i][WEIGHT] = 0.0;
			}
		}
		
		strat->current_goal = GOAL0;
		
		for (i = GOAL1; i <= GOAL7; i++)
		{
			if (strat->goals_tab[i][WEIGHT] > strat->goals_tab[strat->current_goal][WEIGHT] && !strat->targets_status[i][OPP] && !strat->targets_status[i][US])
			{
				strat->current_goal = i; // But if we find a goal with a better weight, save it as the new goal
			}
		}
		
		//printf("New goal loc: (%f,%f)\n", strat->goals_tab[strat->current_goal][COORD_X], strat->goals_tab[strat->current_goal][COORD_Y]);
		
		strat->goal_determination = true; // The new goal is determinated
	}
	
	return;
}

void manage_opp_target(CtrlStruct *cvs)
{
	// variables initialization
	CtrlIn *inputs;
	Strategy *strat;
	OpponentsPosition *opp_pos;
	
	int nb_opp;
	int i;
	
	// variables declaration
	inputs = cvs->inputs;
	strat = cvs->strat;
	opp_pos = cvs->opp_pos;
	
	nb_opp = opp_pos->nb_opp;
	
	if (!strat->opponent_point)
	{
		strat->last_t2 = inputs->t;
		strat->opp_pos = opp_pos;
		
		strat->opponent_point = true;
	}
	else
	{
		if (inputs->t - strat->last_t2 > 4.0) // After 2.0 secondes, we look if one opponent has staid in the same area
		{
			if (!nb_opp) // If there are no opponents
			{
				strat->opponent_point = false;
			}
			else if (nb_opp == 1) // If there is one opponent
			{
				if(   (opp_pos->x[0] <= strat->opp_pos->x[0]+AREA/1000) // If the oppponent doesn't move
					&&(opp_pos->x[0] >= strat->opp_pos->x[0]-AREA/1000)
					&&(opp_pos->y[0] <= strat->opp_pos->y[0]+AREA/1000)
					&&(opp_pos->y[0] >= strat->opp_pos->y[0]-AREA/1000))
				{
					for (i = GOAL0; i <= GOAL7; i++) // Check if it is on one target
					{
						if ((opp_pos->x[0]*1000 <= strat->goals_tab[i][COORD_X] + AREA)
							&& (opp_pos->x[0]*1000 >= strat->goals_tab[i][COORD_X] - AREA)
							&& (opp_pos->y[0]*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
							&& (opp_pos->y[0]*1000 >= strat->goals_tab[i][COORD_Y] - AREA))
						{
							strat->targets_status[i][CHECK1] = true; // If yes, we will check this target only after we have checked all the others
						}
					}
				}
				strat->opponent_point = false; // Let's verify again if one opponent doen't move
			}
			else // If there are two opponents
			{
				if((   (opp_pos->x[0] <= strat->opp_pos->x[0]+AREA/1000)
					&&(opp_pos->x[0] >= strat->opp_pos->x[0]-AREA/1000)
					&&(opp_pos->y[0] <= strat->opp_pos->y[0]+AREA/1000)
					&&(opp_pos->y[0] >= strat->opp_pos->y[0]-AREA/1000))
					|| ((opp_pos->x[1] <= strat->opp_pos->x[1]+AREA/1000)
					&&(opp_pos->x[1] >= strat->opp_pos->x[1]-AREA/1000)
					&&(opp_pos->y[1] <= strat->opp_pos->y[1]+AREA/1000)
					&&(opp_pos->y[1] >= strat->opp_pos->y[1]-AREA/1000)))
				{					
					for (i = GOAL0; i <= GOAL7; i++)
					{
						if (((opp_pos->x[0]*1000 <= strat->goals_tab[i][COORD_X] + AREA)
							&& (opp_pos->x[0]*1000 >= strat->goals_tab[i][COORD_X] - AREA)
							&& (opp_pos->y[0]*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
							&& (opp_pos->y[0]*1000 >= strat->goals_tab[i][COORD_Y] - AREA))
							|| ((opp_pos->x[1]*1000 <= strat->goals_tab[i][COORD_X] + AREA)
							&& (opp_pos->x[1]*1000 >= strat->goals_tab[i][COORD_X] - AREA)
							&& (opp_pos->y[1]*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
							&& (opp_pos->y[1]*1000 >= strat->goals_tab[i][COORD_Y] - AREA)))
						{
							strat->targets_status[i][CHECK1] = true;
						}
					}
				}
				strat->opponent_point = false;
			}
		}
	}
	
	// The next block is used to determinate if an opponant is on the target we want to check
	if (!nb_opp)
	{
		return;
	}
	else if (nb_opp == 1)
	{
		for (i = GOAL0; i <= GOAL7; i++)
		{
			if ((opp_pos->x[0]*1000 <= strat->goals_tab[i][COORD_X] + AREA)
				&& (opp_pos->x[0]*1000 >= strat->goals_tab[i][COORD_X] - AREA)
				&& (opp_pos->y[0]*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
				&& (opp_pos->y[0]*1000 >= strat->goals_tab[i][COORD_Y] - AREA))
			{
				strat->targets_status[i][OPP] = true;
			}
			else
			{
				strat->targets_status[i][OPP] = false;
			}
		}
	}
	else
	{
		for (i = GOAL0; i <= GOAL7; i++)
		{
			if (((opp_pos->x[0]*1000 <= strat->goals_tab[i][COORD_X] + AREA)
				&& (opp_pos->x[0]*1000 >= strat->goals_tab[i][COORD_X] - AREA)
				&& (opp_pos->y[0]*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
				&& (opp_pos->y[0]*1000 >= strat->goals_tab[i][COORD_Y] - AREA))
				|| ((opp_pos->x[1]*1000 <= strat->goals_tab[i][COORD_X] + AREA)
				&& (opp_pos->x[1]*1000 >= strat->goals_tab[i][COORD_X] - AREA)
				&& (opp_pos->y[1]*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
				&& (opp_pos->y[1]*1000 >= strat->goals_tab[i][COORD_Y] - AREA)))
			{
				strat->targets_status[i][OPP] = true;
			}
			else
			{
				strat->targets_status[i][OPP] = false;
			}
		}
	}
	
	return;
}

void manage_us(CtrlStruct *cvs)
{
	// variables initialization
	Strategy *strat;
	RobotPosition *rob_pos;
	
	int i;
	
	// variables declaration
	strat = cvs->strat;
	rob_pos = cvs->rob_pos;
	
	for (i = GOAL0; i <= GOAL7; i++) // For each target, we check if we are on it
	{
		if ((rob_pos->x*1000 <= strat->goals_tab[i][COORD_X] + AREA)
			&& (rob_pos->x*1000 >= strat->goals_tab[i][COORD_X] - AREA)
			&& (rob_pos->y*1000 <= strat->goals_tab[i][COORD_Y] + AREA)
			&& (rob_pos->y*1000 >= strat->goals_tab[i][COORD_Y] - AREA))
		{
			strat->targets_status[i][US] = true;
			strat->goals_tab[i][WEIGHT] = 0.0;
		}
		else
		{
			strat->targets_status[i][US] = false;
		}
	}
	
	return;
}

NAMESPACE_CLOSE();
