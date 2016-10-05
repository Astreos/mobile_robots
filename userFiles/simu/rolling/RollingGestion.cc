#include "RollingGestion.hh"
#include "user_all_id.h"
#include "robot_id.h"

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
RollingGestion::RollingGestion(MbsData *mbs_data)
{
	double wheel_radius, semi_axle;

	wheel_radius = 0.03;
	semi_axle = 0.1125;

	// RollingRobot(mbs_data, wheel_radius, semi_axle, id_robot, id_T1, id_T2, id_R3, id_RW, id_LW)
	rolling_list.push_back(RollingRobot(mbs_data, wheel_radius, semi_axle, ROBOT_B, FJ_T1_robot_B_id, FJ_T2_robot_B_id, FJ_R3_robot_B_id, Right_wheel_joint_B_id, Left_wheel_joint_B_id));
	rolling_list.push_back(RollingRobot(mbs_data, wheel_radius, semi_axle, ROBOT_R, FJ_T1_robot_R_id, FJ_T2_robot_R_id, FJ_R3_robot_R_id, Right_wheel_joint_R_id, Left_wheel_joint_R_id));
	rolling_list.push_back(RollingRobot(mbs_data, wheel_radius, semi_axle, ROBOT_Y, FJ_T1_robot_Y_id, FJ_T2_robot_Y_id, FJ_R3_robot_Y_id, Right_wheel_joint_Y_id, Left_wheel_joint_Y_id));
	rolling_list.push_back(RollingRobot(mbs_data, wheel_radius, semi_axle, ROBOT_W, FJ_T1_robot_W_id, FJ_T2_robot_W_id, FJ_R3_robot_W_id, Right_wheel_joint_W_id, Left_wheel_joint_W_id));

	this->mbs_data = mbs_data;
}

/*! \brief destructor
 */
RollingGestion::~RollingGestion()
{

}

/*! \brief compute the h vector and the Jacobian matrix for all robots
 * 
 * \param[out] h h vector
 * \param[out] Jac Jacobian matrix
 */
void RollingGestion::hJ_compute(double *h, double **Jac)
{
	for(unsigned int i=0; i<rolling_list.size(); i++)
	{
		rolling_list[i].hJ_compute(h, Jac);
	}
}

/*! \brief compute the jdqd vector for all robots
 * 
 * \param[out] jdqd jdqd vector
 */
void RollingGestion::jdqd_compute(double *jdqd)
{
	for(unsigned int i=0; i<rolling_list.size(); i++)
	{
		rolling_list[i].jdqd_compute(jdqd);
	}
}

/*! \brief integrate the position for all robots
 */
void RollingGestion::integrate_pos()
{
	for(unsigned int i=0; i<rolling_list.size(); i++)
	{
		rolling_list[i].integrate_pos();
	}
}

/*! \brief set the derivative of the position to integrate for all robots
 */
void RollingGestion::set_der_to_integrate()
{
	for(unsigned int i=0; i<rolling_list.size(); i++)
	{
		rolling_list[i].set_der_to_integrate();
	}
}
