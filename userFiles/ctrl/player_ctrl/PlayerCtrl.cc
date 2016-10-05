
#include "PlayerCtrl.hh"

#define KP_CTRL 5.0e2 ///< proportional (p) gain
#define K_FORWARD 15.0 ///< forward gain
#define K_TURN    7.5 ///< turn gain

inline double limit_min_max(double x, double min, double max) { return (x < min) ? min : (x > max) ? max : x; }

/// constructor
PlayerCtrl::PlayerCtrl(int robot_id, int nb_opp): Controller(PLAYER_CTRL, robot_id, nb_opp)
{
	for(int i=0; i<2; i++)
	{
		speed_ref[i] = 0.0;
		input_command[i] = 0.0;
	}
}

/// destructor
PlayerCtrl::~PlayerCtrl()
{

}

/*! \brief controller initialization
 */
void PlayerCtrl::init_controller()
{

}

/*! \brief controller motor commands gestion
 */
void PlayerCtrl::set_wheel_commands()
{
	// velocity references for the right (r) and left (l) wheels [rad/s]
	speed_ref[R_ID] = K_FORWARD * input_command[0] + K_TURN * input_command[1];
	speed_ref[L_ID] = K_FORWARD * input_command[0] - K_TURN * input_command[1];

	// motor commands [-] bounded in [-100 ; 100]
	outputs->wheel_commands[R_ID] = KP_CTRL * (speed_ref[R_ID] - inputs->r_wheel_speed);
	outputs->wheel_commands[L_ID] = KP_CTRL * (speed_ref[L_ID] - inputs->l_wheel_speed);

	outputs->wheel_commands[R_ID] = limit_min_max( outputs->wheel_commands[R_ID], -100.0, 100.0);
	outputs->wheel_commands[L_ID] = limit_min_max( outputs->wheel_commands[L_ID], -100.0, 100.0);

	// tower speed [rad/s], bounded in [-20 ; 20]
	outputs->tower_command = 15.0;
}

/*! \brief controller close operations
 */
void PlayerCtrl::close_controller()
{

}
