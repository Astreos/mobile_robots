#include "TimeSensor.hh"
#include "CppInterface.hh"
#include "user_model.h"

/*! \brief contructor
 * 
 * \param[in] mbs_data Robotran structure
 */
TimeSensor::TimeSensor(MbsData *mbs_data): Sensor(mbs_data)
{
	t = 0.0;

	flag_init = 0;
}

/*! \brief destructor
 */
TimeSensor::~TimeSensor()
{
	
}

/*! \brief update the sensor
 */
void TimeSensor::update()
{
	CppInterface *cpp_interface;

	if (!flag_init)
	{
		flag_init = 1;

		cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

		time_gestion = static_cast<TimeGestion*>(cpp_interface->get_global_gestion()->get_gestion(TIME_GESTION));
	}

	t = time_gestion->get_t();
}
