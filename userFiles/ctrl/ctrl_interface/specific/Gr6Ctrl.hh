/*! 
 * \author Nicolas Van der Noot
 * \file Gr6Ctrl.hh
 * \brief Gr6Ctrl class
 */

#ifndef _GR6_CTRL_HH_
#define _GR6_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr6.h"

/*! \brief Group 6 controller
 */
class Gr6Ctrl: public Controller
{
	public:
		Gr6Ctrl(int robot_id, int nb_opp);
		virtual ~Gr6Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr6::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr6::CtrlStruct *cvs; ///< controller main structure
};

#endif
