/*! 
 * \author Nicolas Van der Noot
 * \file Gr12Ctrl.hh
 * \brief Gr12Ctrl class
 */

#ifndef _GR12_CTRL_HH_
#define _GR12_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr12.h"

/*! \brief Group 12 controller
 */
class Gr12Ctrl: public Controller
{
	public:
		Gr12Ctrl(int robot_id, int nb_opp);
		virtual ~Gr12Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr12::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr12::CtrlStruct *cvs; ///< controller main structure
};

#endif
