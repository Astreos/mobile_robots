/*! 
 * \author Nicolas Van der Noot
 * \file Gr15Ctrl.hh
 * \brief Gr15Ctrl class
 */

#ifndef _GR15_CTRL_HH_
#define _GR15_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr15.h"

/*! \brief Group 15 controller
 */
class Gr15Ctrl: public Controller
{
	public:
		Gr15Ctrl(int robot_id, int nb_opp);
		virtual ~Gr15Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr15::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr15::CtrlStruct *cvs; ///< controller main structure
};

#endif
