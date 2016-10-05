
#include "WallShape.hh"
#include <cstddef>

#define PI_2 (M_PI/2.0)

/// constructor
WallShape::WallShape(Vector2D const& init_center, double angle, double wrapping_radius)
{
	Pose init_pose(init_center, angle);

	pose.set_new_pose(init_pose);

	this->wrapping_radius = wrapping_radius;

	index_type = WALL_TYPE;

	seg_wall.set_vector(get_pos_dist_angle(init_center, wrapping_radius, limit_angle(angle+PI_2)), 
		get_pos_dist_angle(init_center, wrapping_radius, limit_angle(angle-PI_2)), get_nul_vector(), get_nul_vector());
}

/// destructor
WallShape::~WallShape()
{

}

/*! \brief update segments
 *
 *  nothing happens for a wall
 */
void WallShape::update_segments()
{

}

/*! \brief check if it contains a point
 * 
 * \param[in] point point to check if it belongs to this circle
 * \return 1 if the point belongs to the circle, 0 otherwise
 */
int WallShape::contains_point(Vector2D const& point)
{
	double dist_wall;
	Orientation wall_orient;
	Vector2D diff_vec;

	diff_vec = point - get_pos();

	if (diff_vec.get_dist() > wrapping_radius)
	{
		return 0;
	}

	wall_orient = get_orient();
	
	dist_wall = wall_orient.get_c_alpha() * diff_vec.get_vec(0) + wall_orient.get_s_alpha() * diff_vec.get_vec(1);

	return (dist_wall < 0.0);
}

/*! \brief check if it intersects a segment
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int WallShape::intersects_segment(Segment const& seg)
{
	Vector2D diff_vec = seg.get_center() - get_pos();

	if (diff_vec.get_dist() >= wrapping_radius + seg.get_wrapping_radius())
	{
		return 0;
	}
	
	return is_intersection(seg, seg_wall);
}
