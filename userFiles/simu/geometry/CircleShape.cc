
#include "CircleShape.hh"
#include <cstddef>

/// constructor
CircleShape::CircleShape(Vector2D const& init_center, double radius)
{
	Pose init_pose(init_center, 0.0);

	pose.set_new_pose(init_pose);

	this->radius = radius;

	wrapping_radius = radius;

	index_type = CIRCLE_TYPE;
}

/// destructor
CircleShape::~CircleShape()
{

}

/*! \brief update segments
 *
 *  nothing happens for a circle
 */
void CircleShape::update_segments()
{

}

/*! \brief check if it contains a point
 * 
 * \param[in] point point to check if it belongs to this circle
 * \return 1 if the point belongs to the circle, 0 otherwise
 */
int CircleShape::contains_point(Vector2D const& point)
{
	Vector2D diff_vec = point - get_pos();

	return (diff_vec.get_dist() <= radius);
}

/*! \brief check if it intersects a segment
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int CircleShape::intersects_segment(Segment const& seg)
{
	Vector2D diff_vec = seg.get_center() - get_pos();

	if (diff_vec.get_dist() >= wrapping_radius + seg.get_wrapping_radius())
	{
		return 0;
	}

	return is_intersection(seg, radius, get_pos());
}


/*! \brief check if it intersects a segment (supposed infinite)
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int CircleShape::intersects_line(Segment const& seg)
{
	Vector2D diff_vec = seg.get_center() - get_pos();

	return is_line_intersection(seg, radius, get_pos());
}
