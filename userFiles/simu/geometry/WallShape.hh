/*! 
 * \author Nicolas Van der Noot
 * \file WallShape.hh
 * \brief WallShape class
 */

#ifndef _WALL_HH_
#define _WALL_HH_

#include "BasicShape.hh"

/*! \brief WallShape geometry shape
 */
class WallShape: public BasicShape
{
	public:
		WallShape(Vector2D const& init_center, double angle, double wrapping_radius);

		virtual ~WallShape();

		virtual void update_segments();

		virtual int contains_point(Vector2D const& point);
		virtual int intersects_segment(Segment const& seg);

		Segment get_seg_wall() const { return seg_wall; }

		virtual void print_identity() { std::cout << "WallShape" << std::endl; }

	private:
		Segment seg_wall; ///< segment describing the wall position
};

#endif
