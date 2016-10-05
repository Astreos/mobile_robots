/*! 
 * \author Nicolas Van der Noot
 * \file Gr14Ctrl.hh
 * \brief Gr14Ctrl class
 */

#ifndef _GR14_CTRL_HH_
#define _GR14_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr14.h"

/*! \brief Group 14 controller
 */
class Gr14Ctrl: public Controller
{
	public:
		Gr14Ctrl(int robot_id, int nb_opp);
		virtual ~Gr14Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr14::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr14::CtrlStruct *cvs; ///< controller main structure
};

#endif
