
#include "RestingCtrl.hh"

/// constructor
RestingCtrl::RestingCtrl(int robot_id, int nb_opp): Controller(RESTING_CTRL, robot_id, nb_opp)
{

}

/// destructor
RestingCtrl::~RestingCtrl()
{

}

/*! \brief init function
 */
void RestingCtrl::init_controller()
{

}

/*! \brief loop function
 */
void RestingCtrl::loop_controller()
{
	for(int i=0; i<2; i++)
	{
		outputs->wheel_commands[i] = 0.0;
	}

	outputs->tower_command = 0.0;
}

/*! \brief close function
 */
void RestingCtrl::close_controller()
{

}
