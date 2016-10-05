/*! 
 * \author Nicolas Van der Noot
 * \file Gr5Ctrl.hh
 * \brief Gr5Ctrl class
 */

#ifndef _GR5_CTRL_HH_
#define _GR5_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr5.h"

/*! \brief Group 5 controller
 */
class Gr5Ctrl: public Controller
{
	public:
		Gr5Ctrl(int robot_id, int nb_opp);
		virtual ~Gr5Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr5::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr5::CtrlStruct *cvs; ///< controller main structure
};

#endif
