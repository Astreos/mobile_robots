/*! 
 * \author Nicolas Van der Noot
 * \file RestingCtrl.hh
 * \brief RestingCtrl class
 */

#ifndef _RESTING_CTRL_HH_
#define _RESTING_CTRL_HH_

#include "Controller.hh"

/*! \brief Basic controller: robot stands still
 */
class RestingCtrl: public Controller
{
	public:
		RestingCtrl(int robot_id, int nb_opp);
		~RestingCtrl();

		virtual void init_controller();
		virtual void loop_controller();
		virtual void close_controller();
};

#endif
