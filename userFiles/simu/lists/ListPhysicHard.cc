
#include "ListPhysicHard.hh"
#include "WallShape.hh"
#include "RectangleShape.hh"
#include "UnionShape.hh"
#include "Vector2D.hh"
#include "MovingObject.hh"
#include "FixedObject.hh"
#include "Robot.hh"
#include "CppInterface.hh"
#include "robot_id.h"
#include <iostream>

#define PI_2 (M_PI/2.0)

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
ListPhysicHard::ListPhysicHard(MbsData *mbs_data): ListPhysicElements(mbs_data)
{
	Vector2D init_pos;

	// walls: , angle, wrapping radius
	init_pos.set_vector(-1.0, 0.0);
	add_wall(init_pos, 0.0, 1.5);

	init_pos.set_vector(1.0, 0.0);
	add_wall(init_pos, M_PI, 1.5);

	init_pos.set_vector(0.0, -1.5);
	add_wall(init_pos, PI_2, 1.0);

	init_pos.set_vector(0.0, 1.5);
	add_wall(init_pos, -PI_2, 1.0);

	// salamander: coordinate, radius
	init_pos.set_vector(0.0, 0.0);
	add_fixed_circle(init_pos, 0.2);

	// initial bases: coordinate, orientation, semi_width, semi_height
	init_pos.set_vector(0.49, -1.25);
	add_fixed_rect(init_pos, 0.0, 0.01, 0.25);

	init_pos.set_vector(0.49, 1.25);
	add_fixed_rect(init_pos, 0.0, 0.01, 0.25);

	// target places: coordinate, orientation, semi_width, semi_height
	init_pos.set_vector(-0.75, -1.01);
	add_fixed_rect(init_pos, 0.0, 0.25, 0.01);

	init_pos.set_vector(-0.75, -0.34);
	add_fixed_rect(init_pos, 0.0, 0.25, 0.01);

	init_pos.set_vector(-0.75, 1.01);
	add_fixed_rect(init_pos, 0.0, 0.25, 0.01);

	init_pos.set_vector(-0.75, 0.34);
	add_fixed_rect(init_pos, 0.0, 0.25, 0.01);

	// rosaces: coordinate, radius, T1, T2, R3
	init_pos.set_vector(0.0, -0.8);
	add_moving_circle(init_pos, 0.075, 25, 26, 27);

	init_pos.set_vector(0.0, 0.8);
	add_moving_circle(init_pos, 0.075, 28, 29, 30);

	// salamander: coordinate, orientation, semi_width, semi_height, T1, T2, R3
	init_pos.set_vector(0.7, 0.0);
	add_moving_rect(init_pos, 0.0, 0.085, 0.085, 31, 32, 33);
}

/*! \brief destructor
 */
ListPhysicHard::~ListPhysicHard()
{

}
