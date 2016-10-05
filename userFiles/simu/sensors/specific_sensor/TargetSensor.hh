/*! 
 * \author Nicolas Van der Noot
 * \file TargetSensor.hh
 * \brief TargetSensor class
 */

#ifndef _TARGET_SENSOR_HH_
#define _TARGET_SENSOR_HH_

#include "Sensor.hh"
#include "ListTargets.hh"

/*! \brief sensor for target
 */
class TargetSensor: public Sensor
{
	public:
		TargetSensor(MbsData *mbs_data, int robot_id);
		virtual ~TargetSensor();

		virtual void update();

		int get_flag_detect() const { return flag_detect; }

	private:
		ListTargets *targets; ///< list of targets
		Robot *rob; ///< robot related to this sensor

		int flag_init;   ///< 1 if initialization done, 0 otherwise
		int robot_id;    ///< ID of the robot
		int flag_detect; ///< 1 if beacon detected, 0 otherwise

		double rob_x; ///< robot x position [m]
		double rob_y; ///< robot y position [m]
};

#endif
