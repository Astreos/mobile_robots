/*! 
 * \author Nicolas Van der Noot
 * \file Gr11Ctrl.hh
 * \brief Gr11Ctrl class
 */

#ifndef _GR11_CTRL_HH_
#define _GR11_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr11.h"

/*! \brief Group 11 controller
 */
class Gr11Ctrl: public Controller
{
	public:
		Gr11Ctrl(int robot_id, int nb_opp);
		virtual ~Gr11Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr11::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr11::CtrlStruct *cvs; ///< controller main structure
};

#endif
