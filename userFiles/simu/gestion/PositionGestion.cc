
#include "PositionGestion.hh"
#include "simu_functions.hh"
#include "Robot.hh"
#include "MovingObject.hh"

#define NB_ROBOTS 1 ///< number of robots

// damping coefficients
#define D_FRIC_LIN 10.0 ///< linear friction coefficient [-]
#define D_FRIC_ROT 1.0e-3  ///< angular friction coefficient [-]

/// constructor
PositionGestion::PositionGestion(ListElements *list_elem, MbsData *mbs_data): Gestion(list_elem)
{
	this->mbs_data = mbs_data;

	list_phys_elem = list_elem->get_list_phys_elem();
}

/// destructor
PositionGestion::~PositionGestion()
{

}

/*! \brief main position gestion function
 */
void PositionGestion::compute()
{
	update_robots_positions();

	update_moving_obj_position();

	update_segments();
}

/*! \brief update the moving objects position
 */
void PositionGestion::update_moving_obj_position()
{
	double cur_torque;
	Vector2D cur_force;
	MovingObject *cur_moving;

	for (int i=0; i<list_phys_elem->get_nb_moving(); i++)
	{
		cur_moving = list_phys_elem->get_moving(i);

		cur_force = cur_moving->get_force();

		cur_force = cur_moving->get_force() + get_friction_force(cur_moving);
		
		cur_torque = cur_moving->get_torque() + get_damping_torque(cur_moving);

		cur_moving->update_from_acc(division_vector(cur_force, cur_moving->get_mass()), cur_torque / cur_moving->get_inertia(), mbs_data->dt0);
	}
}

/*! \brief update the robots position
 */
void PositionGestion::update_robots_positions()
{
	int cur_T1, cur_T2, cur_R3;

	Robot *cur_robot;

	for (int i=0; i<list_phys_elem->get_nb_robot(); i++)
	{
		cur_robot = list_phys_elem->get_robot(i);

		if (elem_contact(cur_robot))
		{
			cur_T1 = cur_robot->get_T1_id();
			cur_T2 = cur_robot->get_T2_id();
			cur_R3 = cur_robot->get_R3_id();

			cur_robot->update_new_pose(mbs_data->q[cur_T1], mbs_data->q[cur_T2], mbs_data->qd[cur_T1], 
				mbs_data->qd[cur_T2], mbs_data->q[cur_R3], mbs_data->qd[cur_R3]);
		}	
	}
}

/*! \brief update the segments state of all physical elements containing rectangles
 */
void PositionGestion::update_segments()
{
	PhysicElements *cur_elem;

	for (int i=0; i<list_phys_elem->get_nb_full(); i++)
	{
		cur_elem = list_phys_elem->get_full(i);

		if (elem_contact(cur_elem))
		{
			cur_elem->update_segments();
		}
	}
}

/*! \brief compute damping on the torques applied
 * 
 * \param[in] cur_moving moving object
 * \return torque damping contribution
 */
double PositionGestion::get_damping_torque(MovingObject *cur_moving)
{
	return -D_FRIC_ROT * cur_moving->get_alpha_p();
}

/*! \brief compute the friction forces applied on moving objects
 * 
 * \param[in] cur_moving moving object
 * \return friction force
 */
Vector2D PositionGestion::get_friction_force(MovingObject *cur_moving)
{
	double fr, denom;

	Vector2D result, pos_p;

	pos_p = cur_moving->get_pos_p();

	result = mult_vector(pos_p, -D_FRIC_LIN);

	// check if each contact point is slipping or not
	fr = get_dist_point2D(result);

	if(fr > cur_moving->get_kinetic_friction())
	{
		fr = cur_moving->get_kinetic_friction();
		
		denom = 1.0 / get_dist_point2D(pos_p);

		result.set_index_vector(0, -fr * pos_p.get_vec(0) * denom);
		result.set_index_vector(1, -fr * pos_p.get_vec(1) * denom);
	}

	return result;
}

/*! \brief check if the element can have contacts
 * 
 * \param[in] cur_elem current element to check
 * \return 1 if possible contact, 0 otherwise
 */
int PositionGestion::elem_contact(PhysicElements *cur_elem)
{
	int robot_id;

	if (cur_elem->is_robot(robot_id))
	{
		if (!static_cast<Robot*>(cur_elem)->get_ctrl()->is_ctrl())
		{
			return 0;
		}
	}

	return 1;
}
