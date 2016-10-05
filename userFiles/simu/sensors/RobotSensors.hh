/*! 
 * \author Nicolas Van der Noot
 * \file RobotSensors.hh
 * \brief RobotSensors class
 */

#ifndef _ROBOT_SENSOR_HH_
#define _ROBOT_SENSOR_HH_

#include <vector>

#include "mbs_data.h"
#include "ctrl_io.h"

#include "Sensor.hh"
#include "TimeSensor.hh"
#include "WheelsSensor.hh"
#include "USwitchSensor.hh"
#include "TowerSensor.hh"
#include "TargetSensor.hh"
#include "ColorSensor.hh"

// forward declaration
class Robot;

/*! \brief all sensors of one robot
 */
class RobotSensors
{
	public:
		RobotSensors(MbsData *mbs_data, int robot_id, int RW_id, int LW_id);
		~RobotSensors();

		virtual void update();
		void fill_inputs(CtrlIn *inputs);

	private:
		int robot_id; ///< ID of the robot

		MbsData *mbs_data; ///< Robotran structure

		TimeSensor   *timing; ///< time sensor
		WheelsSensor *wheels; ///< wheels sensor
		TowerSensor  *tower;  ///< tower sensor
		TargetSensor *target; ///< target sensor
		ColorSensor  *color;  ///< color sensor

		int init_flag; ///< 1 if initialization done, 0 otherwise

		Robot *rob; ///< robot

		std::vector<USwitchSensor*> u_switch_list; ///< list of micro-switches
};

#endif
