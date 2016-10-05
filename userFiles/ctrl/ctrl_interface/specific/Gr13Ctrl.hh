/*! 
 * \author Nicolas Van der Noot
 * \file Gr13Ctrl.hh
 * \brief Gr13Ctrl class
 */

#ifndef _GR13_CTRL_HH_
#define _GR13_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr13.h"

/*! \brief Group 13 controller
 */
class Gr13Ctrl: public Controller
{
	public:
		Gr13Ctrl(int robot_id, int nb_opp);
		virtual ~Gr13Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr13::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr13::CtrlStruct *cvs; ///< controller main structure
};

#endif
