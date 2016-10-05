/*! 
 * \author Nicolas Van der Noot
 * \file Sensor.hh
 * \brief Sensor class
 */

#ifndef _SENSOR_HH_
#define _SENSOR_HH_

#include "mbs_data.h"
#include "MiniBotShape.hh"
#include "ListPhysicElements.hh"
#include <iostream>
#include <stdlib.h>
#include "user_realtime.h"

/*! \brief single sensor of a robot
 */
class Sensor
{
	public:
		Sensor(MbsData *mbs_data);
		virtual ~Sensor();

		virtual void update() = 0;

	protected:
		MbsData *mbs_data; ///< Robotran structure

		Robot* get_robot(int robot_id);
		Robot* get_opponent(int robot_id, int opp_id);
		MiniBotShape* get_robot_shape(int robot_id);
		ListPhysicElements* get_list_phys_elem();
};

#endif
