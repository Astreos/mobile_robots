/*! 
 * \author Nicolas Van der Noot
 * \file Gr1Ctrl.hh
 * \brief Gr1Ctrl class
 */

#ifndef _GR1_CTRL_HH_
#define _GR1_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr1.h"

/*! \brief Group 1 controller
 */
class Gr1Ctrl: public Controller
{
	public:
		Gr1Ctrl(int robot_id, int nb_opp);
		virtual ~Gr1Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr1::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr1::CtrlStruct *cvs; ///< controller main structure
};

#endif
