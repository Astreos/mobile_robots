/*! 
 * \author Nicolas Van der Noot
 * \file WheelsSensor.hh
 * \brief WheelsSensor class
 */

#ifndef _WHEEL_SENSOR_HH_
#define _WHEEL_SENSOR_HH_

#include "Sensor.hh"
#include "Noise.hh"

/*! \brief sensor for timing
 */
class WheelsSensor: public Sensor
{
	public:
		WheelsSensor(MbsData *mbs_data, int RW_id, int LW_id);
		virtual ~WheelsSensor();

		virtual void update();

		double get_r_wheel_sp() const { return r_wheel_sp; }
		double get_l_wheel_sp() const { return l_wheel_sp; }

	private:
		double t; ///< time [s]

		int RW_id; ///< RW_id right wheel joint ID
		int LW_id; ///< LW_id left wheel joint ID

		Noise *r_noise; ///< right wheel noise
		Noise *l_noise; ///< left wheel noise

		double r_wheel_sp; ///< right wheel speed [rad/s]
		double l_wheel_sp; ///< left wheel speed [rad/s]
};

#endif
