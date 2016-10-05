/*! 
 * \author Nicolas Van der Noot
 * \file Gr17Ctrl.hh
 * \brief Gr17Ctrl class
 */

#ifndef _GR17_CTRL_HH_
#define _GR17_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr17.h"

/*! \brief Group 17 controller
 */
class Gr17Ctrl: public Controller
{
	public:
		Gr17Ctrl(int robot_id, int nb_opp);
		virtual ~Gr17Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr17::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr17::CtrlStruct *cvs; ///< controller main structure
};

#endif
