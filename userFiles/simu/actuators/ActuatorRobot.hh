/*! 
 * \author Nicolas Van der Noot
 * \file ActuatorRobot.hh
 * \brief ActuatorRobot class
 */

#ifndef _ACTUATOR_ROBOT_HH_
#define _ACTUATOR_ROBOT_HH_

#include <vector>

#include "Actuator.hh"
#include "mbs_data.h"

/*! \brief Actuators for one robot
 */
class ActuatorRobot
{
	public:
		ActuatorRobot(MbsData *mbs_data, int id_robot, int id_RW, int id_LW, int id_TW);
		~ActuatorRobot();

		void update_voltages(double command_RW, double command_LW, double command_TW);
		void compute_torques();
		void apply_torques();

	private:
		int id_robot; ///< ID of the robot

		int id_RW; ///< joint ID of the right wheel
		int id_LW; ///< joint ID of the left wheel
		int id_TW; ///< joint ID of the tower

		std::vector<Actuator*> actuator_list; ///< list of actuators
		MbsData *mbs_data; ///< Robotran structure
};

#endif
