/*! 
 * \author Nicolas Van der Noot
 * \file Gr9Ctrl.hh
 * \brief Gr9Ctrl class
 */

#ifndef _GR9_CTRL_HH_
#define _GR9_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr9.h"

/*! \brief Group 9 controller
 */
class Gr9Ctrl: public Controller
{
	public:
		Gr9Ctrl(int robot_id, int nb_opp);
		virtual ~Gr9Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr9::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr9::CtrlStruct *cvs; ///< controller main structure
};

#endif
