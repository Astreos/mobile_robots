/*! 
 * \author Nicolas Van der Noot
 * \file CircleShape.hh
 * \brief CircleShape class
 */
#ifndef __CIRCLE__HH__
#define __CIRCLE__HH__


#include "BasicShape.hh"

/*! \brief CircleShape geometry
 */
class CircleShape: public BasicShape
{
	public:
		CircleShape(Vector2D const& init_center, double radius);

		virtual ~CircleShape();

		virtual void update_segments();

		double get_radius() { return radius; }

		virtual int contains_point(Vector2D const& point);
		virtual int intersects_segment(Segment const& seg);

		int intersects_line(Segment const& seg);

		virtual void print_identity() { std::cout << "CircleShape" << std::endl; }

	protected:
		double radius; ///< radius of the circle [m]
};

#endif
