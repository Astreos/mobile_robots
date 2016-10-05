/*! 
 * \author Nicolas Van der Noot
 * \file ContactGestion.hh
 * \brief ContactGestion class
 */

#ifndef __CONTACTGESTION__HH__
#define __CONTACTGESTION__HH__

#include "Gestion.hh"
#include "RectangleShape.hh"
#include "ScoreGestion.hh"
#include "mbs_data.h"

/*! \brief Gestion of the contacts
 */
class ContactGestion: public Gestion
{
	public:
		ContactGestion(ListElements *list_elem, MbsData *mbs_data);
		virtual ~ContactGestion();

		virtual void compute();

		void reset_force_torque_elements();
		void iterate_elements();
		void iterate_shapes(GeometryShape* geom_1, GeometryShape* geom_2, int robot_id_1, int robot_id_2);
		void update_unions();

		int contact_possible(GeometryShape* geom_1, GeometryShape* geom_2);
		int elem_contact(PhysicElements *cur_elem);

	protected:
		int flag_init; ///< 1 if initialization done, 0 otherwise

		MbsData *mbs_data; ///< Robotran structure
		ListPhysicElements *list_phys_elem; ///< list of all physical elements
		ScoreGestion *score_gestion; ///< score gestion
};

#endif
