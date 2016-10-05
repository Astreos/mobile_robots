/*! 
 * \author Nicolas Van der Noot
 * \file Gr2Ctrl.hh
 * \brief Gr2Ctrl class
 */

#ifndef _GR2_CTRL_HH_
#define _GR2_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr2.h"

/*! \brief Group 2 controller
 */
class Gr2Ctrl: public Controller
{
	public:
		Gr2Ctrl(int robot_id, int nb_opp);
		virtual ~Gr2Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr2::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr2::CtrlStruct *cvs; ///< controller main structure
};

#endif
