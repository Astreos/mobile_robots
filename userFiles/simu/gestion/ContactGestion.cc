
#include "ContactGestion.hh"
#include "Robot.hh"
#include "contact_computation.hh"
#include "MovingObject.hh"
#include "CppInterface.hh"
#include "user_model.h"

#define REPULSE_RECT 10.0

/*! \brief constructor
 */
ContactGestion::ContactGestion(ListElements *list_elem, MbsData *mbs_data): Gestion(list_elem)
{
	this->mbs_data = mbs_data;
	list_phys_elem = list_elem->get_list_phys_elem();
	flag_init = 0;
}

/*! \brief destructor
 */
ContactGestion::~ContactGestion()
{

}

/*! \brief main contact function
 */
void ContactGestion::compute()
{
	CppInterface *cpp_interface;

	if (!flag_init)
	{
		flag_init = 1;
		cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);
		score_gestion = static_cast<ScoreGestion*>(cpp_interface->get_global_gestion()->get_gestion(SCORE_GESTION));
	}

	reset_force_torque_elements();

	iterate_elements();

	update_unions();
}

/*! \brief set all forces and torques to 0
 */
void ContactGestion::reset_force_torque_elements()
{
	for(int i=0; i<list_phys_elem->get_nb_full(); i++)
	{
		list_phys_elem->get_full(i)->reset_force_torque();
	}
}

/*! \brief iterations on all contact elements
 *
 * each possible combination is tested
 */
void ContactGestion::iterate_elements()
{
	int nb_full;
	int robot_id_1, robot_id_2;
	GeometryShape *cur_geom;
	Vector2D cur_pos;

	PhysicElements *elem_1, *elem_2;

	nb_full = list_phys_elem->get_nb_full();

	for(int i=0; i<nb_full; i++)
	{
		elem_1 = list_phys_elem->get_full(i);
		elem_1->is_robot(robot_id_1);

		if (elem_contact(elem_1))
		{
			for(int j=i+1; j<nb_full; j++)
			{
				elem_2 = list_phys_elem->get_full(j);
				elem_2->is_robot(robot_id_2);

				if (elem_contact(elem_2))
				{
					iterate_shapes(elem_1->get_geometry(), elem_2->get_geometry(), robot_id_1, robot_id_2);
				}
			}
		}
	}
}

/*! \brief iteration on two GeometryShape shapes
 * 
 * \param[in] geom_1 first geometry shape
 * \param[in] geom_2 second geometry shape
 */
void ContactGestion::iterate_shapes(GeometryShape* geom_1, GeometryShape* geom_2, int robot_id_1, int robot_id_2)
{
	int contact;

	UnionShape *union_1, *union_2;
	BasicShape *basic_1, *basic_2;
		
	if (contact_possible(geom_1, geom_2))
	{
		if (geom_1->is_union())
		{
			union_1 = static_cast<UnionShape*>(geom_1);

			for (int i=0; i<union_1->nb_items(); i++)
			{
				iterate_shapes(union_1->get_item(i), geom_2, robot_id_1, robot_id_2);	
			}
		}
		else if(geom_2->is_union())
		{
			union_2 = static_cast<UnionShape*>(geom_2);

			for (int i=0; i<union_2->nb_items(); i++)
			{
				iterate_shapes(geom_1, union_2->get_item(i), robot_id_1, robot_id_2);	
			}
		}
		else
		{
			basic_1 = static_cast<BasicShape*>(geom_1);
			basic_2 = static_cast<BasicShape*>(geom_2);

			contact = contact_basic_shapes(basic_1, basic_2);

			// contacts between robots
			if (contact && basic_1->is_robot() && basic_2->is_robot())
			{
				score_gestion->set_contact(geom_1, geom_2, robot_id_1, robot_id_2);
			}
		}
	}
}

/*! \brief check if a contact is possible between two GeometryShape shapes
 * 
 * \param[in] geom_1 first geometry shape
 * \param[in] geom_2 second geometry shape
 * \return 1 if a contact is possible, 0 otherwise
 */
int ContactGestion::contact_possible(GeometryShape* geom_1, GeometryShape* geom_2)
{
	// if one of the two bodies cannot generate any contact, return 0
	if ( (!geom_1->is_contact()) || (!geom_2->is_contact()) )
	{
		return 0;
	}

	// if the two bodies are fixed, return 0
	if (geom_1->is_fixed() && geom_2->is_fixed())
	{
		return 0;
	}

	// check wrapping circle
	return ( get_dist_geom(geom_1, geom_2) < geom_1->get_wrapping_radius() + geom_2->get_wrapping_radius() );
}

/*! \brief update forces and torques for the unions
 */
void ContactGestion::update_unions()
{
	for(int i=0; i<list_phys_elem->get_nb_full(); i++)
	{
		list_phys_elem->get_full(i)->update_force_torque();
	}
}

/*! \brief check if the element can have contacts
 * 
 * \param[in] cur_elem current element to check
 * \return 1 if possible contact, 0 otherwise
 */
int ContactGestion::elem_contact(PhysicElements *cur_elem)
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
