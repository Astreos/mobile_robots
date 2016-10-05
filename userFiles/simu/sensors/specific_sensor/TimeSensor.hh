/*! 
 * \author Nicolas Van der Noot
 * \file TimeSensor.hh
 * \brief TimeSensor class
 */

#ifndef _TIME_SENSOR_HH_
#define _TIME_SENSOR_HH_

#include "Sensor.hh"
#include "TimeGestion.hh"

/*! \brief sensor for timing
 */
class TimeSensor: public Sensor
{
	public:
		TimeSensor(MbsData *mbs_data);
		virtual ~TimeSensor();

		virtual void update();

		double get_t() const { return t; }

	private:
		double t; ///< time [s]

		int flag_init; ///< 1 if initialization done, 0 otherwise

		TimeGestion *time_gestion; ///< time gestion
};

#endif
