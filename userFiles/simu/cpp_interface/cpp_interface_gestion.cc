#include "cpp_interface_gestion.h"
#include "CppInterface.hh"
#include "robot_init_pos.hh"
#include "Robot.hh"
#include "RobotGestion.hh"
#include "user_model.h"
#include "user_IO.h"

/*! \brief return the C++ interface
 * 
 * \param[in] mbs_data Robotran structure
 * \return C++ interface
 */
CppInterface *get_cppInterface(MbsData* mbs_data)
{
	return static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);
}

/*! \brief initialization of the simulation C++ structure
 * 
 * \param[in] mbs_data Robotran structure
 */
void simu_init(MbsData* mbs_data)
{
	mbs_data->user_model->user.cpp_interface = new CppInterface(mbs_data);
	mbs_data->user_IO->cpp_int_created = 1;
}

/*! \brief run the main loop of the simulation C++ structure
 * 
 * \param[in] mbs_data Robotran structure
 */
void simu_loop(MbsData* mbs_data)
{
	get_cppInterface(mbs_data)->compute();
}

/*! \brief last call of the simulation C++ structure
 * 
 * \param[in] mbs_data Robotran structure
 */
void simu_finish(MbsData* mbs_data)
{
	CppInterface *cpp_interface;

	cpp_interface = get_cppInterface(mbs_data);
	
	static_cast<RobotGestion*>(cpp_interface->get_global_gestion()->get_gestion(ROBOT_GESTION))->close_controllers();

	delete cpp_interface;
}

/*! \brief compute the h vector and the Jacobian matrix for all robots
 *
 * This function is used at simulation initiation, during the coordinate partitioning
 *
 * \param[in] mbs_data Robotran structure
 * \param[out] h h vector
 * \param[out] Jac Jacobian matrix
 */
void simu_hJ_init(MbsData* mbs_data, double *h, double **Jac)
{
	RollingGestion init_rolling(mbs_data);
	init_rolling.hJ_compute(h, Jac);
}

/*! \brief compute the jdqd vector for all robots
 *
 * This function is used at simulation initiation, during the coordinate partitioning
 *
 * \param[in] mbs_data Robotran structure
 * \param[out] jdqd jdqd vector
 */
void simu_jdqd_init(MbsData* mbs_data, double *jdqd)
{
	RollingGestion init_rolling(mbs_data);
	init_rolling.jdqd_compute(jdqd);
}
/*! \brief compute the h vector and the Jacobian matrix for all robots
 *
 * \param[in] mbs_data Robotran structure
 * \param[out] h h vector
 * \param[out] Jac Jacobian matrix
 */
void simu_hJ_compute(MbsData* mbs_data, double *h, double **Jac)
{
	get_cppInterface(mbs_data)->get_rolling()->hJ_compute(h, Jac);
}

/*! \brief compute the jdqd vector for all robots
 *
 * \param[in] mbs_data Robotran structure
 * \param[out] jdqd jdqd vector
 */
void simu_jdqd_compute(MbsData* mbs_data, double *jdqd)
{
	get_cppInterface(mbs_data)->get_rolling()->jdqd_compute(jdqd);
}

/*! \brief integrate the position for all robots
 *
 * \param[in] mbs_data Robotran structure
 */
void rolling_integrate_pos(MbsData* mbs_data)
{
	get_cppInterface(mbs_data)->get_rolling()->integrate_pos();
}

/*! \brief set the derivative of the position to integrate for all robots
 *
 * \param[in] mbs_data Robotran structure
 */
void rolling_set_der(MbsData* mbs_data)
{
	get_cppInterface(mbs_data)->get_rolling()->set_der_to_integrate();
}

/*! \brief set the robot initial position
 *
 * \param[in,out] mbs_data Robotran structure
 */
void set_robot_init_pos(MbsData* mbs_data)
{
	robot_init_pos(mbs_data->ux);
}

/*! \brief update the contact model
 *
 * \param[in,out] mbs_data Robotran structure
 */
void upate_position_contact(MbsData *mbs_data)
{
	GlobalGestion   *glob_gest;
	PositionGestion *pos_gest;
	ContactGestion  *contact_gest;

	glob_gest = get_cppInterface(mbs_data)->get_global_gestion();

	pos_gest     = glob_gest->get_pos_gestion();
	contact_gest = glob_gest->get_contact_gestion();

	pos_gest->compute();
	contact_gest->compute();
}

/*! \brief Get the robot external torques and forces
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] robot_index index of the current robot
 * \param[out] *Fx pointer to the x component of the force to return
 * \param[out] *Fy pointer to the y component of the force to return
 * \param[out] *Mz pointer to the z component of the moment to return
 */
void get_robot_force_torque(MbsData *mbs_data, int robot_index, double *Fx, double *Fy, double *Mz)
{
	double cur_torque;

	Vector2D cur_force;
	Robot *cur_robot;

	cur_robot = get_cppInterface(mbs_data)->get_global_gestion()->get_list_elem()->get_list_phys_elem()->get_robot(robot_index);

	cur_force = cur_robot->get_force();

	cur_torque = cur_robot->get_torque();

	// get the force and moment
	*Fx = cur_force.get_vec(0);
	*Fy = cur_force.get_vec(1);
	*Mz = cur_torque;
}
