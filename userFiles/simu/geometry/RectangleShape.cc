
#include "RectangleShape.hh"

#include <cstddef>

#define PI_2 (M_PI/2.0)

/// constructor
RectangleShape::RectangleShape()
{
	double semi_width, semi_height;

	semi_width  = 1.0;
	semi_height = 1.0;

	Pose init_pose(get_nul_vector(), 0.0);

	pose.set_new_pose(init_pose);

	this->semi_width  = semi_width;
	this->semi_height = semi_height;

	wrapping_radius = sqrt(semi_width*semi_width + semi_height*semi_height);

	update_seg();

	list_size = 4;

	index_type = RECTANGLE_TYPE;
}

/// constructor
RectangleShape::RectangleShape(Vector2D const& init_center, double angle, double semi_width, double semi_height)
{
	Pose init_pose(init_center, angle);

	pose.set_new_pose(init_pose);

	this->semi_width  = semi_width;
	this->semi_height = semi_height;

	wrapping_radius = sqrt(semi_width*semi_width + semi_height*semi_height);

	update_seg();

	list_size = 4;

	index_type = RECTANGLE_TYPE;
}

/// destructor
RectangleShape::~RectangleShape()
{

}

/*! \brief modify the rectangle to get a new one (as initialization)
 * 
 * \param[in] center center of the rectangle [m]
 * \param[in] angle orientation of the rectangle [rad]
 * \param[in] semi_width semi width of the rectangle [m]
 * \param[in] semi_height semi height of the rectangle [m]
 */
void RectangleShape::set_rectangle(Vector2D const& center, double angle, double semi_width, double semi_height)
{
	Pose cur_pose(center, angle);

	pose.set_new_pose(cur_pose);

	this->semi_width  = semi_width;
	this->semi_height = semi_height;

	wrapping_radius = sqrt(semi_width*semi_width + semi_height*semi_height);

	update_seg();

	list_size = 4;

	index_type = RECTANGLE_TYPE;
}

/*! \brief Update the four segments state (without check)
 */
void RectangleShape::update_seg()
{
	double alpha, alpha_plus, alpha_minus, alpha_opp;

	Vector2D Point_1, Point_2, Point_3, Point_4;
	Vector2D Point_1_P, Point_2_P, Point_3_P, Point_4_P;

	alpha = get_alpha();

	alpha_plus  = alpha + PI_2;
	alpha_minus = alpha - PI_2;
	alpha_opp   = alpha + M_PI;

	Point_1 = get_pos_dist_angle(pose.get_pos(), semi_width, alpha)     + get_vec_dist_angle(semi_height, alpha_plus);
	Point_2 = get_pos_dist_angle(pose.get_pos(), semi_width, alpha_opp) + get_vec_dist_angle(semi_height, alpha_plus);
	Point_3 = get_pos_dist_angle(pose.get_pos(), semi_width, alpha_opp) + get_vec_dist_angle(semi_height, alpha_minus);
	Point_4 = get_pos_dist_angle(pose.get_pos(), semi_width, alpha)     + get_vec_dist_angle(semi_height, alpha_minus);

	point_list[0] = Point_1;
	point_list[1] = Point_2;
	point_list[2] = Point_3;
	point_list[3] = Point_4;

	Point_1_P = get_rigid_pos_p(pose, Point_1);
	Point_2_P = get_rigid_pos_p(pose, Point_2);
	Point_3_P = get_rigid_pos_p(pose, Point_3);
	Point_4_P = get_rigid_pos_p(pose, Point_4);
	
	segment_list[0].set_vector(Point_1, Point_2, Point_1_P, Point_2_P);
	segment_list[1].set_vector(Point_2, Point_3, Point_2_P, Point_3_P);
	segment_list[2].set_vector(Point_3, Point_4, Point_3_P, Point_4_P);
	segment_list[3].set_vector(Point_4, Point_1, Point_4_P, Point_1_P);
}

/*! \brief Update the four segments state (with check)
 */
void RectangleShape::update_segments()
{
	if ( is_fixed() || ( (!is_contact()) && (!is_uSwitch()) ) )
	{
		return;
	}

	update_seg();
}

/*! \brief check if it contains a point
 * 
 * \param[in] point point to check if it belongs to this circle
 * \return 1 if the point belongs to the circle, 0 otherwise
 */
int RectangleShape::contains_point(Vector2D const& point)
{
	Vector2D diff_vec = point - get_pos();

	if (diff_vec.get_dist() > wrapping_radius)
	{
		return 0;
	}

	diff_vec = get_rotated_vector(diff_vec, -get_alpha());

	return ( (fabs(diff_vec.get_vec(0)) <= semi_width) && (fabs(diff_vec.get_vec(1)) <= semi_height) );
}

/*! \brief check if it intersects a segment
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int RectangleShape::intersects_segment(Segment const& seg)
{
	Vector2D diff_vec = seg.get_center() - get_pos();

	if (diff_vec.get_dist() >= wrapping_radius + seg.get_wrapping_radius())
	{
		return 0;
	}
	
	return is_intersection(seg, get_segment_list(), get_list_size());
}