#include "NoCtrl.hh"

/*! \brief constructor
 */
NoCtrl::NoCtrl(int robot_id, int nb_opp): RestingCtrl(robot_id, nb_opp)
{
	flag_ctrl = 0;

	ctrl_type = NO_CTRL;
}

/*! \brief destructor
 */
NoCtrl::~NoCtrl()
{
	
}
