/*! 
 * \author Nicolas Van der Noot
 * \file PositionGestion.hh
 * \brief PositionGestion class
 */

#ifndef __POSITION_GESTION__HH__
#define __POSITION_GESTION__HH__

#include "Gestion.hh"
#include "mbs_data.h"

/*! \brief Position gestion module
 */
class PositionGestion: public Gestion
{
	public:
		PositionGestion(ListElements *list_elem, MbsData *mbs_data);
		virtual ~PositionGestion();

		virtual void compute();

		void update_moving_obj_position();
		void update_robots_positions();
		void update_segments();

		double get_damping_torque(MovingObject *cur_moving);

	private:
		MbsData *mbs_data;             ///< Robotran structure
		ListPhysicElements *list_phys_elem; ///< list with all physical elements

		// function prototypes
		Vector2D get_friction_force(MovingObject *cur_moving);

		int elem_contact(PhysicElements *cur_elem);
};

#endif
