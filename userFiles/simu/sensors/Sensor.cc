#include "Sensor.hh"
#include "CppInterface.hh"
#include "Robot.hh"
#include "user_model.h"
#include "robot_id.h"

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
Sensor::Sensor(MbsData *mbs_data)
{
	this->mbs_data = mbs_data;
}

/*! \brief destructor
 */
Sensor::~Sensor()
{

}

/*! \brief get the list of physical ListPhysicElements
 * 
 * \return corresponding list
 */
ListPhysicElements* Sensor::get_list_phys_elem()
{
	CppInterface *cpp_interface;
	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	return cpp_interface->get_global_gestion()->get_list_elem()->get_list_phys_elem();
}

/*! \brief get the corresponding robot
 * 
 * \param[in] robot_id ID of the robot
 * \return corresponding robot
 */
Robot* Sensor::get_robot(int robot_id)
{
	return get_list_phys_elem()->get_robot(robot_id);
}

/*! \brief get the current robot shape
 * 
 * \param[in] robot_id ID of the robot
 * \return current robot shape
 */
MiniBotShape* Sensor::get_robot_shape(int robot_id)
{
	return static_cast<MiniBotShape*>(get_robot(robot_id)->get_geometry());
}

/*! \brief get an opponent robot
 *
 * \param[in] robot_id ID of the robot
 * \param[in] opp_id 0 or 1
 * \return requested opponent
 */
Robot* Sensor::get_opponent(int robot_id, int opp_id)
{
	switch (robot_id)
	{
		// team A
		case ROBOT_B:
		case ROBOT_R:
			return opp_id ? get_robot(ROBOT_Y) : get_robot(ROBOT_W);

		// team B
		case ROBOT_W:
		case ROBOT_Y:
			return opp_id ? get_robot(ROBOT_R) : get_robot(ROBOT_B);
	
		default:
			std::cout << "Error: unknown robot ID (" << robot_id << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}
}
