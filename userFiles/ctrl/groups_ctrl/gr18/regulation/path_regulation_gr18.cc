#include "path_regulation_gr18.h"
#include "useful_gr18.h"
#include "speed_regulation_gr18.h"
#include "init_pos_gr18.h"
#include "path_planning_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief follow a given path
 *
 * \param[in,out] cvs controller main structure
 */
void follow_path(CtrlStruct *cvs)
{
	//variable declaration
	PathPlanning *path;
	int i;
	
	// variables initialization
    path = cvs->path;
    
    for (i=0; i <= path->nb_goals; i++)
    {
		if (path->list_goal[i][0] < path->rob_pos_XY->X)
		{
			if (path->list_goal[i][1] < path->rob_pos_XY->Y)
			{
				if (turn(cvs, -3.0*M_PI/4.0, 0)) {}
			}
			else if (path->list_goal[i][1] = path->rob_pos_XY->Y)
			{
				if (turn(cvs, M_PI, 0)) {}
			}
			else if (path->list_goal[i][1] > path->rob_pos_XY->Y)
			{
				if (turn(cvs, 3.0*M_PI/4.0, 0)) {}
			}
		}
		else if (path->list_goal[i][0] = path->rob_pos_XY->X)
		{
			if (path->list_goal[i][1] < path->rob_pos_XY->Y)
			{
				if (turn(cvs, -M_PI/2.0, 0)) {run_y(cvs, Y_to_y(path->list_goal[i][1])); printf("%f", Y_to_y(path->list_goal[i][1]));}
			}
			else if (path->list_goal[i][1] > path->rob_pos_XY->Y)
			{
				if (turn(cvs, M_PI/2.0, 0)) {}
			}
		}
		else if (path->list_goal[i][0] > path->rob_pos_XY->X)
		{
			if (path->list_goal[i][1] < path->rob_pos_XY->Y)
			{
				if (turn(cvs, -M_PI/4.0, 0)) {}
			}
			else if (path->list_goal[i][1] = path->rob_pos_XY->Y)
			{
				if (turn(cvs, 0, 0)) {}
			}
			else if (path->list_goal[i][1] > path->rob_pos_XY->Y)
			{
				if (turn(cvs, M_PI/4.0, 0)) {}
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
    int team_id;
    int dir;
    int sens_opt;

    // variables initialization
    inputs  = cvs->inputs;
    rob_pos = cvs->rob_pos;
    team_id = cvs->team_id;
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
		dir = team(team_id)*sens_opt;
		
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
	
	speed_regulation(cvs, (Kp*error + (Kp/Ti)*pos_reg->int_error_r)*dir, -(Kp*error + (Kp/Ti)*pos_reg->int_error_r)*dir);

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

NAMESPACE_CLOSE();
