#include "WheelsSensor.hh"
#include "config_functions.hh"
#include "config_file.h"

#define RND_PERIOD 0.75

/*! \brief contructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] RW_id right wheel joint ID
 * \param[in] LW_id left wheel joint ID
 */
WheelsSensor::WheelsSensor(MbsData *mbs_data, int RW_id, int LW_id): Sensor(mbs_data)
{
	FILE* config_file;
	double wheel_noise;

	config_file = open_config();

	#ifdef HARDCODED_CONFIG
	wheel_noise = WHEEL_NOISE;
	#else
	wheel_noise = config_double(config_file, "wheel_noise");
	#endif

	t = mbs_data->tsim;

	this->RW_id = RW_id;
	this->LW_id = LW_id;

	r_noise = new Noise(t, wheel_noise, RND_PERIOD);
	l_noise = new Noise(t, wheel_noise, RND_PERIOD);

	r_wheel_sp = mbs_data->qd[RW_id];
	l_wheel_sp = mbs_data->qd[LW_id];

	close_config(config_file);
}

/*! \brief destructor
 */
WheelsSensor::~WheelsSensor()
{
	delete r_noise;
	delete l_noise;
}

/*! \brief update the sensor
 */
void WheelsSensor::update()
{
	t = mbs_data->tsim;

	r_wheel_sp = mbs_data->qd[RW_id] * (1.0 + r_noise->update_and_get(t));
	l_wheel_sp = mbs_data->qd[LW_id] * (1.0 + l_noise->update_and_get(t));
}
