/*! 
 * \author Nicolas Van der Noot
 * \file USwitchSensor.hh
 * \brief USwitchSensor class
 */

#ifndef _U_SWITCH_SENSOR_HH_
#define _U_SWITCH_SENSOR_HH_

#include "Sensor.hh"
#include "Vector2D.hh"

class Robot;

/*! \brief sensor for timing
 */
class USwitchSensor: public Sensor
{
	public:
		USwitchSensor(MbsData *mbs_data, int robot_id, double dist_x, double dist_y);
		virtual ~USwitchSensor();

		virtual void update();

		int get_u_switch_flag() const { return u_switch_flag; }

	private:
		int robot_id; ///< robot ID

		int u_switch_flag; ///< 1 if contact, 0 otherwise

		double dist_x; ///< distance along the x axis from the robot center to the u-switch [m] (if the robot orientation is 0.0)
		double dist_y; ///< distance along the y axis from the robot center to the u-switch [m] (if the robot orientation is 0.0)

		Vector2D u_switch_pos; ///< micro-switch position
};

#endif
