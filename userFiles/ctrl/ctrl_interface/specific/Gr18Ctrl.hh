/*! 
 * \author Nicolas Van der Noot
 * \file Gr18Ctrl.hh
 * \brief Gr18Ctrl class
 */

#ifndef _GR18_CTRL_HH_
#define _GR18_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr18.h"

/*! \brief Group 18 controller
 */
class Gr18Ctrl: public Controller
{
	public:
		Gr18Ctrl(int robot_id, int nb_opp);
		virtual ~Gr18Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr18::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr18::CtrlStruct *cvs; ///< controller main structure
};

#endif
