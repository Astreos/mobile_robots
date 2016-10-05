#include "Target.hh"
#include "Vector2D.hh"
#include <cmath>

/// get square
inline double square(double x) { return x*x; }

/// get distance, given x and y components
inline double dist(double dx, double dy) { return sqrt(square(dx) + square(dy)); }

/*! \brief constructor
 *
 * \param[in] point_value number of points for this target [-]
 * \param[in] T1 initial value of the x position [m]
 * \param[in] T2 initial value of the y position [m]
 * \param[in] T3 initial value of the z position [m]
 * \param[in] T1_id ID of the T1 joint in the anim .mbs file
 * \param[in] T2_id ID of the T2 joint in the anim .mbs file
 * \param[in] T3_id ID of the T3 joint in the anim .mbs file
 */
Target::Target(double T1, double T2, double T3, int T1_id, int T2_id, int T3_id, int point_value): pos(T1, T2, T3)
{
	this->T1_id = T1_id;
	this->T2_id = T2_id;
	this->T3_id = T3_id;

	this->point_value = point_value;

	state = TARGET_GROUND;

	radius = 0.1;
}

/*! \brief destructor
 */
Target::~Target()
{
	
}

/*! \brief detect if point inside
 *
 * \param[in] x x position of the point [m]
 * \param[in] y y position of the point [m]
 * \return 1 if point inside, 0 otherwise
 */
int Target::point_inside(double x, double y)
{
	return (dist(x - get_x(), y - get_y()) < radius);
}

/*! \brief detect if the target is entirely inside a given rectangle
 *
 * \param[in] x_rect rectangle center x coordinate [m]
 * \param[in] y_rect rectangle center y coordinate [m]
 * \param[in] angle_rect angle describing the rectangle orientation [rad]
 * \param[in] semi_width_rect semi width of the rectangle [m]
 * \param[in] semi_height_rect semi height of the rectangle [m]
 * \return 1 if point inside, 0 otherwise
 */
int Target::full_inside_rectangle(double x_rect, double y_rect, double angle_rect, double semi_width_rect, double semi_height_rect)
{
	Vector2D diff_vec(get_x() - x_rect, get_y() - y_rect);

	if (angle_rect)
	{
		diff_vec = get_rotated_vector(diff_vec, -angle_rect);
	}

	return ((fabs(diff_vec.get_x()) + radius) <= semi_width_rect) && ((fabs(diff_vec.get_y()) + radius) <= semi_height_rect);
}

/*! \brief release on the ground
 */
void Target::set_ground()
{
	pos.set_z(0.0);

	state = TARGET_GROUND;
}

/*! \brief release on the ground, and set it to fixed position
 */
void Target::set_fixed()
{
	pos.set_z(0.0);

	state = TARGET_FIXED;
}
