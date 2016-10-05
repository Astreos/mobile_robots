/*! 
 * \author Nicolas Van der Noot
 * \file PlayerCtrl.hh
 * \brief PlayerCtrl class
 */
#ifndef _PLAYER_CTRL_HH_
#define _PLAYER_CTRL_HH_

#include "Controller.hh"
#include "user_realtime.h"
#include <iostream>

/*! \brief Controller Allowing A human player to control a robot
 */
class PlayerCtrl: public Controller
{
	public:
		PlayerCtrl(int robot_id, int nb_opp);
		virtual ~PlayerCtrl();

		virtual void init_controller();
		virtual void close_controller();

		void set_wheel_commands();

	protected:
		double speed_ref[2];     ///< speed wheel references [rad/s]
		double input_command[2]; ///< input comming from user [-]
};

#endif
