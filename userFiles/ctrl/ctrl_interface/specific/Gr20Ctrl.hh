/*! 
 * \author Nicolas Van der Noot
 * \file Gr20Ctrl.hh
 * \brief Gr20Ctrl class
 */

#ifndef _GR20_CTRL_HH_
#define _GR20_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr20.h"

/*! \brief Group 20 controller
 */
class Gr20Ctrl: public Controller
{
	public:
		Gr20Ctrl(int robot_id, int nb_opp);
		virtual ~Gr20Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr20::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr20::CtrlStruct *cvs; ///< controller main structure
};

#endif
