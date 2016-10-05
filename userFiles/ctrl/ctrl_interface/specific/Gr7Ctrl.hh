/*! 
 * \author Nicolas Van der Noot
 * \file Gr7Ctrl.hh
 * \brief Gr7Ctrl class
 */

#ifndef _GR7_CTRL_HH_
#define _GR7_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr7.h"

/*! \brief Group 7 controller
 */
class Gr7Ctrl: public Controller
{
	public:
		Gr7Ctrl(int robot_id, int nb_opp);
		virtual ~Gr7Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr7::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr7::CtrlStruct *cvs; ///< controller main structure
};

#endif
