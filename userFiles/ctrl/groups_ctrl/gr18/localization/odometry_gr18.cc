#include "odometry_gr18.h"
#include "useful_gr18.h"
#include "init_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief update the robot odometry
 * 
 * \param[in,out] cvs controller main structure
 */
void update_odometry(CtrlStruct *cvs)
{
	// variables declaration
	RobotPosition *rob_pos;
	CtrlIn *inputs;
	
	double r_sp, l_sp;
	double dt;

	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;

	r_sp = inputs->r_wheel_speed; // right wheel speed
	l_sp = inputs->l_wheel_speed; // left wheel speed

	// time
	dt = inputs->t - rob_pos->last_t; // time increment since last call

	// safety
	if (dt <= 0.0)
	{
		return;
	}

	// ----- odometry computation start ----- //

	rob_pos->x = rob_pos->x + ((r_sp*0.030*dt + l_sp*0.030*dt) / 2) * cos(rob_pos->theta + (r_sp*0.030*dt - l_sp*0.030*dt) / (2*0.225));
	rob_pos->y = rob_pos->y + ((r_sp*0.030*dt + l_sp*0.030*dt) / 2) * sin(rob_pos->theta + (r_sp*0.030*dt - l_sp*0.030*dt) / (2*0.225));
	rob_pos->theta = limit_angle(rob_pos->theta + (r_sp*0.030*dt - l_sp*0.030*dt) / 0.225);

	// ----- odometry computation end ----- //
	
	//set_plot(rob_pos->x, "x_odo_[m]");
	//set_plot(rob_pos->y, "y_odo_[m]");
	//set_plot(rob_pos->theta, "theta_odo_[rad]");

	// last update time
	rob_pos->last_t = inputs->t;
	
	return;
}

NAMESPACE_CLOSE();
