/*! 
 * \author Nicolas Van der Noot
 * \file Gr3Ctrl.hh
 * \brief Gr3Ctrl class
 */

#ifndef _GR3_CTRL_HH_
#define _GR3_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr3.h"

/*! \brief Group 3 controller
 */
class Gr3Ctrl: public Controller
{
	public:
		Gr3Ctrl(int robot_id, int nb_opp);
		virtual ~Gr3Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr3::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr3::CtrlStruct *cvs; ///< controller main structure
};

#endif
