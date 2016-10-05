/*! 
 * \author Nicolas Van der Noot
 * \file MiniBotShape.hh
 * \brief MiniBotShape class
 */

#ifndef _ROBOT_SHAPE_HH_
#define _ROBOT_SHAPE_HH_

#include "CircleCutShape.hh"

/*! \brief Full geometry shape describing a robot
 */
class MiniBotShape: public CircleCutShape
{
	public:
		MiniBotShape(Vector2D const& init_pos, double init_angle, double wrapping_radius);
		virtual ~MiniBotShape();

		virtual void print_identity() { std::cout << "MiniBotShape" << std::endl; }

		int inside_rect(RectangleShape *rect);
};

#endif
