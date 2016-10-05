/*! 
 * \author Nicolas Van der Noot
 * \file Gr8Ctrl.hh
 * \brief Gr8Ctrl class
 */

#ifndef _GR8_CTRL_HH_
#define _GR8_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr8.h"

/*! \brief Group 8 controller
 */
class Gr8Ctrl: public Controller
{
	public:
		Gr8Ctrl(int robot_id, int nb_opp);
		virtual ~Gr8Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr8::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr8::CtrlStruct *cvs; ///< controller main structure
};

#endif
