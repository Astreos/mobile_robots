/*! 
 * \author Nicolas Van der Noot
 * \file NoCtrl.hh
 * \brief NoCtrl class
 */

#ifndef _NO_CTRL_HH_
#define _NO_CTRL_HH_

#include "RestingCtrl.hh"

/*! \brief no controller when the robot is not there
 */
class NoCtrl: public RestingCtrl
{
	public:
		NoCtrl(int robot_id, int nb_opp);
		virtual ~NoCtrl();

	private:
};

#endif
