/*! 
 * \author Nicolas Van der Noot
 * \file Gr10Ctrl.hh
 * \brief Gr10Ctrl class
 */

#ifndef _GR10_CTRL_HH_
#define _GR10_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr10.h"

/*! \brief Group 10 controller
 */
class Gr10Ctrl: public Controller
{
	public:
		Gr10Ctrl(int robot_id, int nb_opp);
		virtual ~Gr10Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr10::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr10::CtrlStruct *cvs; ///< controller main structure
};

#endif
