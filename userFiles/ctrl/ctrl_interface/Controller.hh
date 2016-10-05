/*! 
 * \author Nicolas Van der Noot
 * \file Controller.hh
 * \brief Controller class
 */

#ifndef _CONTROLLER_HH_
#define _CONTROLLER_HH_

#include "ctrl_io.h"

enum {NO_CTRL, RESTING_CTRL, PLAYER_CTRL, EXPL_CTRL, SOL_CTRL, GR1_CTRL, GR2_CTRL, GR3_CTRL, GR4_CTRL, GR5_CTRL, GR6_CTRL, GR7_CTRL, GR8_CTRL,
	GR9_CTRL, GR10_CTRL, GR11_CTRL, GR12_CTRL, GR13_CTRL, GR14_CTRL, GR15_CTRL, GR16_CTRL, GR17_CTRL, GR18_CTRL, GR19_CTRL, GR20_CTRL};

/*! \brief Controller of a robot
 */
class Controller
{
	public:
		Controller(int ctrl_type, int robot_id, int nb_opp);
		virtual ~Controller();		

		virtual void init_controller()  = 0;
		virtual void loop_controller()  = 0;
		virtual void close_controller() = 0;

		int get_ctrl_type() const { return ctrl_type; }

		CtrlIn*  get_inputs()  const { return inputs;  }
		CtrlOut* get_outputs() const { return outputs; }

		int is_ctrl() const { return flag_ctrl; }

	protected:
		CtrlIn  *inputs;  ///< inputs of the controller
		CtrlOut *outputs; ///< outputs of the controller

		int ctrl_type; ///< controller type index
		int robot_id; ///< robot ID

		int flag_ctrl; ///< 1 if controller active, 0 otherwise
};

#endif
