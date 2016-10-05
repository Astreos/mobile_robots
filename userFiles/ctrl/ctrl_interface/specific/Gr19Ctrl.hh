/*! 
 * \author Nicolas Van der Noot
 * \file Gr19Ctrl.hh
 * \brief Gr19Ctrl class
 */

#ifndef _GR19_CTRL_HH_
#define _GR19_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr19.h"

/*! \brief Group 19 controller
 */
class Gr19Ctrl: public Controller
{
	public:
		Gr19Ctrl(int robot_id, int nb_opp);
		virtual ~Gr19Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr19::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr19::CtrlStruct *cvs; ///< controller main structure
};

#endif
