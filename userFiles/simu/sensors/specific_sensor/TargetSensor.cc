#include "TargetSensor.hh"
#include "CppInterface.hh"
#include "Robot.hh"
#include "user_model.h"

/*! \brief contructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] robot_id ID of the robot
 */
TargetSensor::TargetSensor(MbsData *mbs_data, int robot_id): Sensor(mbs_data)
{
	flag_init = 0;
	this->robot_id = robot_id;

	rob_x = 0.0;
	rob_y = 0.0;

	flag_detect = 0;
}

/*! \brief destructor
 */
TargetSensor::~TargetSensor()
{
	
}

/*! \brief update the sensor
 */
void TargetSensor::update()
{
	double cur_x, cur_y, cur_z;
	CppInterface *cpp_interface;
	Target *cur_target;

	// get list of targets
	if (!flag_init)
	{
		flag_init = 1;

		cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

		targets = cpp_interface->get_global_gestion()->get_list_elem()->get_list_targets();

		rob = get_robot(robot_id);
	}

	flag_detect = 0;

	rob_x = rob->get_pose_x();
	rob_y = rob->get_pose_y();

	for(int i=0; i<targets->get_nb_targets(); i++)
	{
		cur_target = targets->get_target(i);

		if (cur_target->on_ground()) // current target on the ground
		{
			if (cur_target->point_inside(rob_x, rob_y))
			{
				flag_detect = 1;
			}
		}	
	}
}
