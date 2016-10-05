/*! 
 * \author Nicolas Van der Noot
 * \file KeyboardCtrl.hh
 * \brief KeyboardCtrl class
 */

#ifndef _KEYBOARD_CTRL_HH_
#define _KEYBOARD_CTRL_HH_

#include "PlayerCtrl.hh"

/*! \brief Control the robot with the keyboard
 */
class KeyboardCtrl: public PlayerCtrl
{
	public:
		KeyboardCtrl(int robot_id, int nb_opp);
		virtual ~KeyboardCtrl();

		virtual void loop_controller();

	protected:
		static int nb_keyboard_ctrl; ///< number of keyboards
		int id_keyboard;             ///< ID of this keyboard
};

#endif
