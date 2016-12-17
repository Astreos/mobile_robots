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

#define AREA 150

#define FINAL_POS_X -700.0
#define FINAL_POS_Y -1250.0

#define COEFF_PTS 35.0
#define COEFF_DIST_TPS 5.0
#define COEFF_BASE_TPS 5.0

#define COORD_X 0
#define COORD_Y 1
#define VALUE   2
#define WEIGHT  3
#define TAKE    4

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
	Strategy *strat;
	int i;
	
	// memory allocation
	strat = (Strategy*) malloc(sizeof(Strategy));
	if (strat == NULL) {exit(0);}
	
	// ----- strategy initialization start ----- //
	
	// goal_tab initialization (x,y,weight,take/not_take)
	
	strat->goal_tab = (double**) malloc(8*sizeof(double*));
	if (strat->goal_tab == NULL) {exit(0);}
	
	for(i = 0; i < 8; i++)
	{
		strat->goal_tab[i] = (double*) malloc(4*sizeof(double));
		if (strat->goal_tab[i] == NULL) {exit(0);}
	}
	
	// GOAL 0 (700,600)
	strat->goal_tab[GOAL0][COORD_X] = 700;
	strat->goal_tab[GOAL0][COORD_Y] = 600;
	strat->goal_tab[GOAL0][VALUE] = 1;
	// GOAL 1 (250,1250)
	strat->goal_tab[GOAL1][COORD_X] = 250;
	strat->goal_tab[GOAL1][COORD_Y] = 1250;
	strat->goal_tab[GOAL1][VALUE] = 2 + 1.2;
	// GOAL 2 (-400,600)
	strat->goal_tab[GOAL2][COORD_X] = -400;
	strat->goal_tab[GOAL2][COORD_Y] = 600;
	strat->goal_tab[GOAL2][VALUE] = 1;
	// GOAL 3 (-800,0)
	strat->goal_tab[GOAL3][COORD_X] = -800;
	strat->goal_tab[GOAL3][COORD_Y] = 0;
	strat->goal_tab[GOAL3][VALUE] = 3 + 2;
	// GOAL 4 (100,0)
	strat->goal_tab[GOAL4][COORD_X] = 100;
	strat->goal_tab[GOAL4][COORD_Y] = 0;
	strat->goal_tab[GOAL4][VALUE] = 2 + 1;
	// GOAL 5 (-400,600)
	strat->goal_tab[GOAL5][COORD_X] = -400;
	strat->goal_tab[GOAL5][COORD_Y] = -600;
	strat->goal_tab[GOAL5][VALUE] = 1;
	// GOAL 6 (700,-600)
	strat->goal_tab[GOAL6][COORD_X] = 700;
	strat->goal_tab[GOAL6][COORD_Y] = -600;
	strat->goal_tab[GOAL6][VALUE] = 1 ;
	// GOAL 7 (250,-1250)
	strat->goal_tab[GOAL7][COORD_X] = 250;
	strat->goal_tab[GOAL7][COORD_Y] = -1250;
	strat->goal_tab[GOAL7][VALUE] = 2;
	
	for(i = GOAL0; i <= GOAL7; i++)
	{
		strat->goal_tab[i][WEIGHT] = 0;
		strat->goal_tab[i][TAKE] = false;
	}
	
	strat->goal_determination = false;
	strat->goal = GOAL0;
	
	//nb_targets
	strat->nb_targets = 8;
	
	// current target
	strat->current_action = 0;
	
	// delta_t
	strat->last_t = 0;
	
	// ----- strategy initialization end ----- //
	
	return strat;
}

/*! \brief release the strategy structure (memory released)
 * 
 * \param[out] strat strategy structure to release
 */
void free_strategy(Strategy *strat)
{
	int i;
	
	// ----- strategy memory release start ----- //
	
	for(i = 0; i < 8; i++)
	{
		free(strat->goal_tab[i]);
	}
	
	free(strat->goal_tab);
	
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
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	
	update_goal(cvs);
	
	switch (strat->main_state)
	{
		case FIRST_TARGET:
			manage_first_target(cvs);
			break;
			
		case SECOND_TARGET:
			manage_second_target(cvs);
			break;
			
		case WIN_POINTS:
			win_points(cvs);
			break;
			
		case CALIBRATE:
			calibrate(cvs);
			break;
			
		case FUNNY_ACTION:
			speed_regulation(cvs, -7, 7);
			break;
			
		default:
			printf("Error: unknown strategy main state: %d !\n", strat->main_state);
			exit(EXIT_FAILURE);
	}
}

void manage_first_target(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	
	int team_id;double goal_x, goal_y;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	team_id = cvs->team_id;
	
	goal_x = strat->goal_tab[strat->goal][COORD_X]/1000;
	goal_y = strat->goal_tab[strat->goal][COORD_Y]/1000;
	
	outputs->flag_release = 0;
	
	switch (strat->sub_state)
	{
		case TRAJECTORY:
			if (!path->flag_trajectory)
				trajectory(cvs, goal_x, goal_y*team(team_id));
			else
				strat->sub_state = FOLLOW_PATH;
			break;
			
		case FOLLOW_PATH:
			if (follow_path(cvs, goal_x, goal_y*team(team_id)))
			{
				speed_regulation(cvs, 0, 0);
				strat->sub_state = CHECK_TARGET;
			}
			break;
			
		case CHECK_TARGET:
			if (inputs->target_detected || (inputs->nb_targets == 1))
			{
				strat->sub_state = GET_TARGET;
			}
			else if (strat->current_action < 7)
			{
				strat->current_action += 1;
				strat->sub_state = TRAJECTORY;
			}
			else
			{
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS;
			}
			break;
			
		case GET_TARGET:
			if (inputs->nb_targets == 1 && strat->current_action == strat->nb_targets-1)
			{
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS;
				
				cvs->strat->goal_determination = false;
				strat->goal_tab[strat->goal][TAKE] = true;
			}
			else if (inputs->nb_targets == 1 && strat->current_action < strat->nb_targets-1)
			{
				strat->current_action += 1;
				strat->sub_state = TRAJECTORY;
				strat->main_state = SECOND_TARGET;
				
				cvs->strat->goal_determination = false;
				strat->goal_tab[strat->goal][TAKE] = true;
			}
			break;
	}
}

void manage_second_target(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	
	int team_id; double goal_x, goal_y;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	team_id = cvs->team_id;
	
	goal_x = strat->goal_tab[strat->goal][COORD_X]/1000;
	goal_y = strat->goal_tab[strat->goal][COORD_Y]/1000;
	
	outputs->flag_release = 0;
	
	switch (strat->sub_state)
	{
		case TRAJECTORY:
			if (!path->flag_trajectory)
				trajectory(cvs, goal_x, goal_y*team(team_id));
			else
				strat->sub_state = FOLLOW_PATH;
			break;
			
		case FOLLOW_PATH:
			if (follow_path(cvs, goal_x, goal_y*team(team_id)))
			{
				speed_regulation(cvs, 0, 0);
				strat->sub_state = CHECK_TARGET;
			}
			break;
			
		case CHECK_TARGET:
			if (inputs->target_detected || (inputs->nb_targets == 2))
			{
				strat->sub_state = GET_TARGET;
			}
			else if (strat->current_action < strat->nb_targets-1)
			{
				strat->current_action += 1;
				strat->sub_state = TRAJECTORY;
			}
			else
			{
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS;
			}
			break;
			
		case GET_TARGET:
			if (inputs->nb_targets == 2)
			{
				strat->sub_state = TRAJECTORY;
				strat->main_state = WIN_POINTS;
				
				cvs->strat->goal_determination = false;
				strat->goal_tab[strat->goal][TAKE] = true;
				
			}
			break;
	}
}

void win_points(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	CtrlOut *outputs;
	PathPlanning *path;
	RobotPosition *rob_pos;
	KalmanStruct *pos_kalman;
	
	int team_id;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	outputs = cvs->outputs;
	path = cvs->path;
	rob_pos = cvs->rob_pos;
	pos_kalman = cvs->kalman_pos;
	
	team_id = cvs->team_id;
	
	switch (strat->sub_state)
	{
		case TRAJECTORY:
			if (!path->flag_trajectory)
				trajectory(cvs, -0.75, -1.15*team(team_id));
			else
				strat->sub_state = FOLLOW_PATH;
			break;
			
		case FOLLOW_PATH:
			if (follow_path(cvs, -0.75, -1.15*team(team_id)))
			{
				speed_regulation(cvs, 0, 0);
				strat->sub_state = RELEASE_TARGET;
			}
			break;
			
		case RELEASE_TARGET:
			outputs->flag_release = 1;
			
			strat->last_t = inputs->t;
			strat->main_state = CALIBRATE;
			break;
	}
}

void calibrate(CtrlStruct *cvs)
{
	// variables declaration
	Strategy *strat;
	CtrlIn *inputs;
	RobotPosition *rob_pos;
	KalmanStruct *pos_kalman;
	
	// variables initialization
	strat  = cvs->strat;
	inputs = cvs->inputs;
	rob_pos = cvs->rob_pos;
	pos_kalman = cvs->kalman_pos;
	
	speed_regulation(cvs, 0, 0);
	
	if (inputs->t - strat->last_t > 0.65)
	{		
		rob_pos->x = pos_kalman->x;
		rob_pos->y = pos_kalman->y;
		//rob_pos->theta = pos_kalman->theta;
		
		if (strat->current_action == strat->nb_targets-1)
		{
			strat->main_state = FUNNY_ACTION;
		}
		else
		{
			strat->current_action += 1;
			strat->sub_state = TRAJECTORY;
			strat->main_state = FIRST_TARGET;
		}
	}
	
	return;
}

void update_goal(CtrlStruct *cvs)
{
	
	OpponentsPosition *opp_pos;
	opp_pos = cvs->opp_pos;
	
	if(cvs->strat->goal_determination == false)
	{
		
		double dist_pts; double dist_base; double t = cvs->inputs->t;
		
		
		for(int i = GOAL0; i <= GOAL7; i++)
		{
			dist_pts = sqrtf((pow((cvs->rob_pos->x*1000+1562-(cvs->strat->goal_tab[i][COORD_X]+1562)),2)
			+ pow((cvs->rob_pos->y*1000+1062-(cvs->strat->goal_tab[i][COORD_Y]+1062)),2)));
			
			dist_base = sqrtf((pow((FINAL_POS_X+1562-(cvs->strat->goal_tab[i][COORD_X]+1562)),2)
			+ pow((FINAL_POS_Y+1062-(cvs->strat->goal_tab[i][COORD_Y]+1062)),2)));
			
			if(cvs->strat->goal_tab[i][TAKE] == false)
				cvs->strat->goal_tab[i][WEIGHT] = COEFF_PTS*cvs->strat->goal_tab[i][VALUE]
				+ (TPS_MAX+t)/TPS_MAX*(DIST_MAX/dist_pts)*COEFF_DIST_TPS
				+ (TPS_MAX+t)/TPS_MAX*(DIST_MAX/dist_base)*COEFF_BASE_TPS;
			else
				cvs->strat->goal_tab[i][WEIGHT] = 0;
		}
		
		int new_goal = 0;
		
		for (int i = GOAL1; i <= GOAL7; i++)
			if(cvs->strat->goal_tab[i][WEIGHT] > cvs->strat->goal_tab[new_goal][WEIGHT])
				new_goal = i;
			
			printf("New Goal lol: (%f,%f)\n", cvs->strat->goal_tab[new_goal][COORD_X],cvs->strat->goal_tab[new_goal][COORD_Y]);
		
		cvs->strat->goal = new_goal;
		
		cvs->strat->goal_determination = true;
	}
	
	if(   (opp_pos->x[0]*1000 <= cvs->strat->goal_tab[cvs->strat->goal][COORD_X]+AREA)
		&&(opp_pos->x[0]*1000 >= cvs->strat->goal_tab[cvs->strat->goal][COORD_X]-AREA)
		&&(opp_pos->y[0]*1000 <= cvs->strat->goal_tab[cvs->strat->goal][COORD_Y]+AREA)
		&&(opp_pos->y[0]*1000 >= cvs->strat->goal_tab[cvs->strat->goal][COORD_Y]-AREA))
	{
		cvs->strat->goal_determination = false;
		cvs->strat->goal_tab[cvs->strat->goal][TAKE] = true;
		
		update_goal(cvs); // si opponent, redéfinition du goal
	}
	
	return;
}

NAMESPACE_CLOSE();
