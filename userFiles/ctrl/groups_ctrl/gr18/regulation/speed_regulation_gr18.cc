#include "speed_regulation_gr18.h"
#include "useful_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/*! \brief wheel speed regulation
 *
 * \param[in,out] cvs controller main structure
 * \parem[in] r_sp_ref right wheel speed reference [rad/s]
 * \parem[in] l_sp_ref left wheel speed reference [rad/s]
 */
void speed_regulation(CtrlStruct *cvs, double r_sp_ref, double l_sp_ref)
{
    double r_sp, l_sp;
    double dt;

    // variables declaration
    CtrlIn *inputs;
    CtrlOut *outputs;
    SpeedRegulation *sp_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    sp_reg  = cvs->sp_reg;

    // wheel speeds
    r_sp = inputs->r_wheel_speed;
    l_sp = inputs->l_wheel_speed;

    // time
    dt = inputs->t - sp_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    float Kp = 30.0; //Kc = 90 => Kopt = 40.5 //29
    float Ti = 0.05; //Tosc = 0.0057143 s //0.07

    sp_reg->int_error_r = (r_sp_ref - r_sp)*dt + limit_range(sp_reg->int_error_r, -fabs(r_sp_ref), fabs(r_sp_ref));
    sp_reg->int_error_l = (l_sp_ref - l_sp)*dt + limit_range(sp_reg->int_error_l, -fabs(l_sp_ref), fabs(l_sp_ref));

    // wheel commands
    outputs->wheel_commands[0] = Kp*(r_sp_ref - r_sp) + (Kp/Ti)*sp_reg->int_error_r;
    outputs->wheel_commands[1] = Kp*(l_sp_ref - l_sp) + (Kp/Ti)*sp_reg->int_error_l;

    // ----- Wheels regulation computation end ----- //

    //set_plot(r_sp, "R_wheel_reg_[rad/s]");
    //set_plot(l_sp, "L_wheel_reg_[rad/s]");

    // last update time
    sp_reg->last_t = inputs->t;
	
	return;
}

NAMESPACE_CLOSE();
