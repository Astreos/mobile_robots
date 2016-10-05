#include "Basis.hh"

/*! \brief constructor
 *
 * \param[in] x_rect rectangle center x coordinate [m]
 * \param[in] y_rect rectangle center y coordinate [m]
 * \param[in] angle_rect angle describing the rectangle orientation [rad]
 * \param[in] semi_width_rect semi width of the rectangle [m]
 * \param[in] semi_height_rect semi height of the rectangle [m]
 * \param[in] team_id ID of the team
 */
Basis::Basis(double x_rect, double y_rect, double angle_rect, double semi_width_rect, double semi_height_rect, int team_id)
{
	this->x_rect = x_rect;
	this->y_rect = y_rect;

	this->angle_rect = angle_rect;

	this->semi_width_rect  = semi_width_rect;
	this->semi_height_rect = semi_height_rect;

	this->team_id = team_id;
}

/*! \brief destructor
 */
Basis::~Basis()
{
	
}

/*! \brief check if a target is fully inside the corresponding basis
 * 
 * \param[in] cur_target current target to check
 * \return 1 if fully inside, 0 otherwise
 */
int Basis::target_full_inside(Target *cur_target)
{
	return cur_target->full_inside_rectangle(x_rect, y_rect, angle_rect, semi_width_rect, semi_height_rect);
}
