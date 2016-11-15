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

void turn(CtrlStruct *cvs, double theta_ref, int sens)
{
	// variables declaration
        int team_id;
        int dir;
        
        RobotPosition *rob_pos;
	CtrlIn *inputs;
	CtrlOut *outputs;

	// variables initialization
	inputs  = cvs->inputs;
	outputs = cvs->outputs;
        rob_pos = cvs->rob_pos;
        team_id = cvs->team_id;
        
        if (rob_pos->theta*theta_ref >= 0)
        {
            dir = team(team_id);
        }

	// ----- Wheels regulation computation start ----- //

	int Kp = 10;

	speed_regulation(cvs, Kp * (rob_pos->theta - theta_ref) * dir, -Kp * (rob_pos->theta - theta_ref) * dir);

	// ----- Wheels regulation computation end ----- //
        
        //set_plot(r_sp, "R_wheel_reg_[rad/s]");
	//set_plot(l_sp, "L_wheel_reg_[rad/s]");
}

NAMESPACE_CLOSE();
