/*! 
 * \author Nicolas Van der Noot
 * \file JoystickCtrl.hh
 * \brief JoystickCtrl class
 */

#ifndef _JOYSTICK_CTRL_HH_
#define _JOYSTICK_CTRL_HH_

#include "PlayerCtrl.hh"

/*! \brief Control the robot with the joystick
 */
class JoystickCtrl: public PlayerCtrl
{
	public:
		JoystickCtrl(int robot_id, int nb_opp);
		virtual ~JoystickCtrl();

		virtual void loop_controller();

	protected:
		static int nb_joystick_ctrl; ///< number of joysticks
		int id_joystick;             ///< ID of this joystick

		double max_time_up;    ///< maximal time button up [s]
		double t_button_up[4]; ///< first time the corresponding button is seen to be 1 [s]
		int flag_up[4];        ///< 1 if corresponding button is up
};

#endif
