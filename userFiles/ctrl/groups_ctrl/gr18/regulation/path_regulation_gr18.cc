#include "path_regulation_gr18.h"
#include "useful_gr18.h"
#include "speed_regulation_gr18.h"
#include "init_pos_gr18.h"
#include "path_planning_gr18.h"
#include "opp_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief follow a given path
 *
 * \param[in,out] cvs controller main structure
 */
int follow_path(CtrlStruct *cvs, double goal_x, double goal_y)
{
	//variable declaration
	PathPlanning *path;
	int team_id;
	
	// variables initialization
    path = cvs->path;
	team_id = cvs->team_id;
    
    if (path->count_actions <= path->nb_goals-5)
	{
		if (run(cvs, X_to_x(path->list_goal[path->count_actions][0]), Y_to_y(path->list_goal[path->count_actions][1]), 66, 0.20))
		{
			path->count_actions++;
		}
		
		return 0;
	}
	else
	{
		if (((goal_x == -0.70) && (goal_y == -1.15*team(team_id))) || ((goal_x == 0.10) && (goal_y == 0*team(team_id))))
		{
			if (run(cvs, goal_x, goal_y, M_PI, 0.003))
			{				
				speed_regulation(cvs, 0, 0);
				
				path->flag_trajectory = 0;
				path->count_actions = 1;
				
				return 1;
			}
		}
		else
		{
			if (run(cvs, goal_x, goal_y, 66, 0.003))
			{
				speed_regulation(cvs, 0, 0);
				
				path->flag_trajectory = 0;
				path->count_actions = 1;
				
				return 1;
			}
		}
	}
}

int turn(CtrlStruct *cvs, double theta_ref, int sens)
{
    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    PosRegulation *pos_reg;
    
    double dt;
    double error;
    int sens_opt;

    // variables initialization
    inputs  = cvs->inputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //
    
	if (theta_ref*rob_pos->theta < 0)
	{		
		if ((2.0*M_PI - fabs(theta_ref) - fabs(rob_pos->theta)) >= (fabs(theta_ref) + fabs(rob_pos->theta)))
		{
			if (theta_ref >= rob_pos->theta)
			{
				sens_opt = 1;
			}
			else
			{
				sens_opt = -1;
			}
		}
		else
		{
			if (theta_ref <= rob_pos->theta)
			{
				sens_opt = 1;
			}
			else
			{
				sens_opt = -1;
			}
		}
	}
	else
	{
		if (theta_ref >= rob_pos->theta)
		{
			sens_opt = 1;
		}
		else
		{
			sens_opt = -1;
		}
	}
	
	float Kp = 5.0;
    float Ti = 10.0;
	
	if (sens == 0)
	{
		if (sens_opt == 1)
		{
			if ((theta_ref*rob_pos->theta < 0) && (theta_ref < 0))
			{
				error = 2.0*M_PI - fabs(theta_ref - rob_pos->theta);
			}
			else
			{
				error = fabs(theta_ref - rob_pos->theta);
			}
		}
		else
		{
			if ((theta_ref*rob_pos->theta < 0) && (theta_ref > 0))
			{
				error = 2.0*M_PI - fabs(theta_ref - rob_pos->theta);
			}
			else
			{
				error = fabs(theta_ref - rob_pos->theta);
			}
		}
	}
	
	pos_reg->int_error_r = error*dt + limit_range(pos_reg->int_error_r, 0, M_PI/2.0);
	pos_reg->int_error_l = error*dt + limit_range(pos_reg->int_error_l, 0, M_PI/2.0);
	
	speed_regulation(cvs, (Kp*error + (Kp/Ti)*pos_reg->int_error_r)*sens_opt, -(Kp*error + (Kp/Ti)*pos_reg->int_error_r)*sens_opt);

    // ----- Wheels regulation computation end ----- //
    
    //set_plot(error, "error");
    //set_plot(pos_reg->int_error_r, "int_error");
    
    // last update time
    pos_reg->last_t = inputs->t;
    
    if (error <= 0.00011)
    {
		pos_reg->int_error_r = 0;
		pos_reg->int_error_l = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

int run(CtrlStruct *cvs, double x_ref, double y_ref, double theta_ref, float epsilon)
{
	// variables declaration
	RobotPosition *rob_pos;
	
	PosRegulation *pos_reg;
	OpponentsPosition *opp_pos;
	
	double rho, alpha, beta;
	double dt;
	float K_rho, K_alpha, K_beta;
	
	// variables initialization
	rob_pos = cvs->rob_pos;
	pos_reg  = cvs->pos_reg;
	opp_pos = cvs->opp_pos;
	
	// ----- Wheels regulation computation start ----- //
	
	if (epsilon >= 0.1)
	{
		K_rho = 20.0*7; // K_rho > 0
		K_alpha = 21.0*7; // K_alpha > K_rho
		K_beta = -12.0*7; // K_beta < 0
	}
	else if (epsilon < 0.1)
	{
		K_rho = 20.0*3; // K_rho > 0
		K_alpha = 21.0*3; // K_alpha > K_rho
		K_beta = -12.0*3; // K_beta < 0
	}
	
	if (theta_ref == 66)
	{
		K_beta = 0;
	}
	
	rho = sqrt(pow((x_ref - rob_pos->x), 2) + pow(y_ref - rob_pos->y, 2));
	alpha = limit_angle(-rob_pos->theta + atan2(y_ref - rob_pos->y, x_ref - rob_pos->x));
	beta = limit_angle(theta_ref - rob_pos->theta - alpha);
	
	if (opp_pos->opp_front)
	{
		speed_regulation(cvs, K_alpha*alpha, -K_alpha*alpha);
	}
	else
	{
		speed_regulation(cvs, K_rho*rho + (K_alpha*alpha + K_beta*beta), K_rho*rho - (K_alpha*alpha + K_beta*beta));
	}
	
	// ----- Wheels regulation computation end ----- //
	
	if (rho < epsilon)
	{		
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
int run_x(CtrlStruct *cvs, double x_ref)
{
	// variables declaration
	RobotPosition *rob_pos;
	CtrlIn *inputs;
	PosRegulation *pos_reg;
	
	double dt;
	double error;
	
	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;
	pos_reg  = cvs->pos_reg;
	
	// time
	dt = inputs->t - pos_reg->last_t; // time interval since last call
	
	// ----- Wheels regulation computation start ----- //
	
	float Kp = 35.0;
	float Ti = 20.0;
	
	error = x_ref - rob_pos->x;
	
	pos_reg->int_error_r = error*dt + limit_range(pos_reg->int_error_r, -1.0, 1.0);
	pos_reg->int_error_l = error*dt + limit_range(pos_reg->int_error_l, -1.0, 1.0);
	
	speed_regulation(cvs, -(Kp*error + (Kp/Ti)*pos_reg->int_error_r), -(Kp*error + (Kp/Ti)*pos_reg->int_error_r));
	
	// ----- Wheels regulation computation end ----- //
	
	// last update time
	pos_reg->last_t = inputs->t;
	
	if (fabs(error) < 0.001)
	{
		pos_reg->int_error_r = 0;
		pos_reg->int_error_l = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

int run_y(CtrlStruct *cvs, double y_ref)
{
	// variables declaration
	RobotPosition *rob_pos;
	CtrlIn *inputs;
	PosRegulation *pos_reg;
	
	double dt;
	double error;
	
	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;
	pos_reg  = cvs->pos_reg;
	
	// time
	dt = inputs->t - pos_reg->last_t; // time interval since last call
	
	// ----- Wheels regulation computation start ----- //
	
	float Kp = 35.0;
	float Ti = 20.0;
	
	error = y_ref - rob_pos->y;
	
	pos_reg->int_error_r = error*dt + limit_range(pos_reg->int_error_r, -1.0, 1.0);
	pos_reg->int_error_l = error*dt + limit_range(pos_reg->int_error_l, -1.0, 1.0);
	
	speed_regulation(cvs, -(Kp*error + (Kp/Ti)*pos_reg->int_error_r), -(Kp*error + (Kp/Ti)*pos_reg->int_error_l));
	
	// ----- Wheels regulation computation end ----- //
	
	// last update time
	pos_reg->last_t = inputs->t;
	
	if (fabs(error) < 0.001)
	{
		pos_reg->int_error_r = 0;
		pos_reg->int_error_l = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
*/

NAMESPACE_CLOSE();
