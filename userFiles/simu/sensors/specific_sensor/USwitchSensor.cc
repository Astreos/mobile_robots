#include "USwitchSensor.hh"
#include "Robot.hh"
#include "Orientation.hh"

/*! \brief contructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] robot_id robot ID
 * \param[in] dist_x distance along the x axis from the robot center to the u-switch [m] (if the robot orientation is 0.0)
 * \param[in] dist_y distance along the y axis from the robot center to the u-switch [m] (if the robot orientation is 0.0)
 */
USwitchSensor::USwitchSensor(MbsData *mbs_data, int robot_id, double dist_x, double dist_y): Sensor(mbs_data)
{
	this->robot_id = robot_id;

	u_switch_pos = get_nul_vector();

	this->dist_x = dist_x;
	this->dist_y = dist_y;

	u_switch_flag = 0;
}

/*! \brief destructor
 */
USwitchSensor::~USwitchSensor()
{
	
}

/*! \brief update the sensor
 */
void USwitchSensor::update()
{
	double c_alpha, s_alpha;

	Vector2D robot_pos;
	Orientation rob_or;

	ListPhysicElements *list_phys_elem;
	GeometryShape *cur_geom;

	// get robot characteristics
	robot_pos = get_robot(robot_id)->get_pos();

	rob_or = get_robot_shape(robot_id)->get_orient();

	c_alpha = rob_or.get_c_alpha();
	s_alpha = rob_or.get_s_alpha();

	// update u-switch position
	u_switch_pos = robot_pos + get_vec_amp_angle(dist_x, rob_or) + get_vec_amp_angle(dist_y, -s_alpha, c_alpha);

	// check if there is a contact
	u_switch_flag = 0;

	list_phys_elem = get_list_phys_elem();

	for(int i=0; i<list_phys_elem->get_nb_full(); i++)
	{
		cur_geom = list_phys_elem->get_full(i)->get_geometry();

		if (cur_geom->is_uSwitch())
		{
			if (cur_geom->contains_point(u_switch_pos))
			{
				u_switch_flag = 1;
				break;
			}
		}
	}
}
