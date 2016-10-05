/*! 
 * \author Nicolas Van der Noot
 * \file ActuatorGestion.hh
 * \brief ActuatorGestion class
 */

#ifndef _ACTUATOR_GESTION_HH_
#define _ACTUATOR_GESTION_HH_

#include <vector>

#include "ActuatorRobot.hh"
#include "mbs_data.h"

/*! \brief Gestions of all robots actuators
 */
class ActuatorGestion
{
	public:
		ActuatorGestion(MbsData *mbs_data);
		~ActuatorGestion();

		void compute();

		ActuatorRobot* get_actuators_robot(int i) const { return robot_list[i]; }

	private:
		std::vector<ActuatorRobot*> robot_list; ///< list of robots
		MbsData *mbs_data; ///< Robotran structure

		void compute_torques();
		void apply_torques();
};

#endif
