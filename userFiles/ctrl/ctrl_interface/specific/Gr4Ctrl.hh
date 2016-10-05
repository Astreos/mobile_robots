/*! 
 * \author Nicolas Van der Noot
 * \file Gr4Ctrl.hh
 * \brief Gr4Ctrl class
 */

#ifndef _GR4_CTRL_HH_
#define _GR4_CTRL_HH_

#include "Controller.hh"
#include "CtrlStruct_gr4.h"

/*! \brief Group 4 controller
 */
class Gr4Ctrl: public Controller
{
	public:
		Gr4Ctrl(int robot_id, int nb_opp);
		virtual ~Gr4Ctrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();

		inline ctrlGr4::CtrlStruct* get_cvs() const { return cvs; }

	private:
		ctrlGr4::CtrlStruct *cvs; ///< controller main structure
};

#endif
