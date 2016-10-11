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
	
        int Kp = 40;
        int Ti = 1;
        
        sp_reg->int_error_r = ((r_sp_ref - r_sp)*dt + sp_reg->int_error_r) / sp_reg->last_t;
        sp_reg->int_error_l = ((l_sp_ref - l_sp)*dt + sp_reg->int_error_l) / sp_reg->last_t;

	// wheel commands
	outputs->wheel_commands[0] = Kp * (r_sp_ref - r_sp) + Kp * 1/Ti * limit_range(sp_reg->int_error_r, -fabs(r_sp_ref - r_sp), fabs(r_sp_ref - r_sp));
	outputs->wheel_commands[1] = Kp * (l_sp_ref - l_sp) + Kp * 1/Ti * limit_range(sp_reg->int_error_l, -fabs(l_sp_ref - l_sp), fabs(l_sp_ref - l_sp));

	// ----- Wheels regulation computation end ----- //

	// last update time
	sp_reg->last_t = inputs->t;
}

NAMESPACE_CLOSE();
