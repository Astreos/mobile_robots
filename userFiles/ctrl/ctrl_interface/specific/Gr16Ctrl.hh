/*! 
 * \author Nicolas Van der Noot
 * \file Gr16Ctrl.hh
 * \brief Gr16Ctrl class
 */

#ifndef _GR16_CTRL_HH_
#define _GR16_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr16.h"

/*! \brief Group 16 controller
 */
class Gr16Ctrl: public Controller
{
	public:
		Gr16Ctrl(int robot_id, int nb_opp);
		virtual ~Gr16Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr16::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr16::CtrlStruct *cvs; ///< controller main structure
};

#endif
