#include "path_regulation_gr18.h"
#include "useful_gr18.h"
#include "speed_regulation_gr18.h"
#include "init_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief follow a given path
 *
 * \param[in,out] cvs controller main structure
 */
void follow_path(CtrlStruct *cvs)
{

}

int turn(CtrlStruct *cvs, double theta_ref, int sens)
{
    double dt;
    double error;
    int team_id;
    int dir;
    int sens_opt;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    team_id = cvs->team_id;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //
    
	if (theta_ref*rob_pos->theta < 0)
	{		
		if ((2.0*M_PI - fabs(theta_ref) - fabs(rob_pos->theta)) <= (fabs(theta_ref) + fabs(rob_pos->theta)))
		{
			sens_opt = -1;
		}
		else
		{
			sens_opt = 1;
		}
	}
	else
	{
		if (theta_ref < rob_pos->theta)
		{
			sens_opt = -1;
		}
		else
		{
			sens_opt = 1;
		}
	}
	
	float Kp = 20;
    float Ti = 5;
	
	if (sens == 0)
	{		
		dir = team(team_id)*sens_opt;
		
		if (sens_opt = 1)
		{
			if ((theta_ref*rob_pos->theta < 0) && (theta_ref < 0))
			{
				error = 2.0*M_PI - fabs(theta_ref - rob_pos->theta);
				
				pos_reg->int_error_r = error*dt + pos_reg->int_error_r;
				pos_reg->int_error_l = error*dt + pos_reg->int_error_l;
				
				outputs->wheel_commands[0] = (Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
				outputs->wheel_commands[1] = -(Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
			}
			else
			{
				error = fabs(theta_ref - rob_pos->theta);
				
				pos_reg->int_error_r = error*dt + pos_reg->int_error_r;
				pos_reg->int_error_l = error*dt + pos_reg->int_error_l;
				
				outputs->wheel_commands[0] = (Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
				outputs->wheel_commands[1] = -(Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
			}
		}
		else
		{
			if ((theta_ref*rob_pos->theta < 0) && (theta_ref > 0))
			{
				error = 2.0*M_PI - fabs(theta_ref - rob_pos->theta);
				
				pos_reg->int_error_r = error*dt + pos_reg->int_error_r;
				pos_reg->int_error_l = error*dt + pos_reg->int_error_l;
				
				outputs->wheel_commands[0] = (Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
				outputs->wheel_commands[1] = -(Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
			}
			else
			{
				error = fabs(theta_ref - rob_pos->theta);
				
				pos_reg->int_error_r = error*dt + pos_reg->int_error_r;
				pos_reg->int_error_l = error*dt + pos_reg->int_error_l;
				
				outputs->wheel_commands[0] = (Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
				outputs->wheel_commands[1] = -(Kp*error + (Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI, M_PI))*dir;
			}
		}
	}
	
/*
	// tourner par 0 OU tourner par pi, theta et theta_ref même signe
	outputs->wheel_commands[0] = (Kp*fabs(theta_ref - rob_pos->theta) + 0*(Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI/4.0, M_PI/4.0))*dir;
	outputs->wheel_commands[1] = -(Kp*fabs(theta_ref - rob_pos->theta) + 0*(Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI/4.0, M_PI/4.0))*dir;
	
	// tourner par pi, theta et theta_ref signe contraire
	outputs->wheel_commands[0] = (Kp * (2.0*M_PI - fabs(theta_ref - rob_pos->theta)) + 0*(Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI/4.0, M_PI/4.0))*dir;
	outputs->wheel_commands[1] = -(Kp * (2.0*M_PI - fabs(theta_ref - rob_pos->theta)) + 0*(Kp/Ti)*limit_range(pos_reg->int_error_r, -M_PI/4.0, M_PI/4.0))*dir;
*/

    // ----- Wheels regulation computation end ----- //
    
    set_plot(fabs(theta_ref - rob_pos->theta), "error");
    
    set_plot(outputs->wheel_commands[0], "R_wheel_[V]");
    set_plot(outputs->wheel_commands[1], "L_wheel_[V]");
    
    // last update time
    pos_reg->last_t = inputs->t;
    
    if (fabs(theta_ref - rob_pos->theta) < 0.01)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int run_x(CtrlStruct *cvs, double x_ref)
{
    double dt;
    double r_sp, l_sp;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;
    
    r_sp = inputs->r_wheel_speed;
    l_sp = inputs->l_wheel_speed;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    float Kp = 70.0;
    float Ti = 1.5;

    pos_reg->int_error_r = (x_ref - rob_pos->x)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = (x_ref - rob_pos->x)*dt + pos_reg->int_error_l;
    
    outputs->wheel_commands[0] = -Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.5, 0.5);
    outputs->wheel_commands[1] = -Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.5, 0.5);

    //speed_regulation(cvs, -Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.3, 0.3), -Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.3, 0.3));

    // ----- Wheels regulation computation end ----- //
    
    set_plot(r_sp, "R_wheel_reg_[rad/s]");
    set_plot(l_sp, "L_wheel_reg_[rad/s]");

	// last update time
    pos_reg->last_t = inputs->t;
    
    if (fabs(x_ref - rob_pos->x) < 0.001)
    {
        return 1;
    }
    else
    {
        return 0;
    }

/*
    if ((fabs(x_ref - rob_pos->x) < 0.001) && (fabs(Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.3, 0.3)) < 0.75))
    {
        return 1;
    }
    else
    {
        return 0;
    }
    */
}

int run_y(CtrlStruct *cvs, double y_ref)
{
    double dt;
    double r_sp, l_sp;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;
    
    r_sp = inputs->r_wheel_speed;
    l_sp = inputs->l_wheel_speed;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    float Kp = 70.0;
    float Ti = 1.5;

    pos_reg->int_error_r = (y_ref - rob_pos->y)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = (y_ref - rob_pos->y)*dt + pos_reg->int_error_l;
    
    outputs->wheel_commands[0] = -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.5, 0.5);
    outputs->wheel_commands[1] = -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_l, -0.5, 0.5);
    
    set_plot(r_sp, "R_wheel_reg_[rad/s]");
    set_plot(l_sp, "L_wheel_reg_[rad/s]");

    //speed_regulation(cvs, -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.3, 0.3), -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_l, -0.3, 0.3));

    // ----- Wheels regulation computation end ----- //
    
    // last update time
    pos_reg->last_t = inputs->t;
    
    if (fabs(y_ref - rob_pos->y) < 0.001)
    {
        return 1;
    }
    else
    {
        return 0;
    }

/*
    if ((fabs(y_ref - rob_pos->y) < 0.001) && (fabs(-Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.3, 0.3)) < 0.75))
    {
        return 1;
    }
    else
    {
        return 0;
    }
    */
}

NAMESPACE_CLOSE();
