#include "MiniBotShape.hh"

enum{MAIN_RECTANGLE, MAIN_CIRCLE};

/*! \brief constructor
 */
MiniBotShape::MiniBotShape(Vector2D const& init_pos, double init_angle, double wrapping_radius): CircleCutShape(init_pos, init_angle, wrapping_radius, 0.07)
{
	set_index_elem(ROBOT_TYPE);
}

/*! \brief destructor
 */
MiniBotShape::~MiniBotShape()
{

}

/*! \brief check if it lies inside a given rectangle
 *
 * \param[in] rect rextangle to check
 * \return 1 if fully inside the rectangle, 0 otherwise
 */
int MiniBotShape::inside_rect(RectangleShape *rect)
{
	if(!rect->contains_point(rect_cut.get_point(1)))
	{
		return 0;
	}
	else if(!rect->contains_point(rect_cut.get_point(2)))
	{
		return 0;
	}
	else if(!rect->contains_point(get_pos_dist_angle(get_pos(), radius, get_alpha()-(M_PI/3.0))))
	{
		return 0;
	}
	else if(!rect->contains_point(get_pos_dist_angle(get_pos(), radius, get_alpha())))
	{
		return 0;
	}
	else if(!rect->contains_point(get_pos_dist_angle(get_pos(), radius, get_alpha()+(M_PI/3.0))))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
