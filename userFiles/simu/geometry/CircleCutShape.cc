
#include "CircleCutShape.hh"
#include "simu_functions.hh"
#include <cstddef>

#define EPSILON 1.0e-6
#define DEG_TO_RAD (M_PI / 180.0)
#define RAD_TO_DEG (180.0 / M_PI)
#define THRES_TORQUE_ANGLE (100.0 * DEG_TO_RAD)
#define CST_TORQUE_ARM 0.02

/*! \brief constructor
 * 
 * \param[in] init_center center of the robot [m]
 * \param[in] angle orientation of the robot [rad]
 * \param[in] radius radius of the robot [m]
 * \param[in] cut_length cutted length along a radius [m]
 *
 * the length cut_length will be bounded in the range [0 ; radius]
 */
CircleCutShape::CircleCutShape(Vector2D const& init_center, double angle, double radius, double cut_length): circ_full(init_center, radius)
{
	double semi_width, semi_height;

	cut_length = limit_range(cut_length, 0.0, radius);

	Pose init_pose(init_center, angle);

	pose.set_new_pose(init_pose);

	this->radius = radius;
	this->cut_length = cut_length;

	cut_center = radius - cut_length;

	gamma = acos(cut_center / radius);

	semi_width = 0.25*cut_center;
	semi_height = cut_center * tan(gamma);

	dist_rect = -cut_center + semi_width;
	rect_center = get_pos_dist_angle(init_center, dist_rect, angle);

	rect_cut.set_rectangle(rect_center, angle, semi_width, semi_height);

	wrapping_radius = radius;

	index_type = CIRCLE_CUT_TYPE;

	// same things for the elements in the list
	set_index_elem(index_elem);
	set_contact(flag_contact);
	set_uSwitch(flag_uSwitch);
}

/// destructor
CircleCutShape::~CircleCutShape()
{

}

/*! \brief set the forces and torques to 0
 */
void CircleCutShape::reset_force_torque()
{
	circ_full.reset_force_torque();
	rect_cut.reset_force_torque();

	force.reset();
	torque = 0.0;
}

/*! \brief update segments (and rectangle position)
 */
void CircleCutShape::update_segments()
{
	circ_full.set_new_pose(pose);

	rect_center = get_pos_dist_angle(pose.get_pos(), dist_rect, pose.get_alpha());
	rect_pose = pose;
	rect_pose.set_pos(rect_center);

	rect_cut.set_new_pose(rect_pose);
	rect_cut.update_segments();
}

/*! \brief check if it contains a point
 * 
 * \param[in] point point to check if its projection belongs to this circle
 * \return 1 if the point projetion belongs to the circle, 0 otherwise
 */
int CircleCutShape::contains_point(Vector2D const& point)
{
	double x_proj;

	Vector2D diff_vec = point - get_pos();

	if (diff_vec.get_dist() > radius + EPSILON)
	{
		return 0;
	}

	x_proj = diff_vec.get_x()*get_c_alpha() + diff_vec.get_y()*get_s_alpha();

	return (x_proj > - cut_center);
}

/*! \brief check if it intersects a segment
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int CircleCutShape::intersects_segment(Segment const& seg)
{
	int nb_int;
	double x1, y1, x2, y2;

	Vector2D diff_vec = seg.get_center() - get_pos();

	if (diff_vec.get_dist() >= wrapping_radius + seg.get_wrapping_radius())
	{
		return 0;
	}

	nb_int = get_circle_intersection(seg, radius, get_pos(), x1, y1, x2, y2);

	if (nb_int >= 1)
	{
		if (contains_point(get_vector(x1, y1)) || contains_point(get_vector(x2, y2)))
		{
			return 1;
		}
	}

	return rect_cut.intersects_segment(seg);
}

/*! \brief check if it intersects a segment (supposed infinite)
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int CircleCutShape::intersects_line(Segment const& seg)
{
	int nb_int;
	double x1, y1, x2, y2;

	nb_int = get_circle_line_intersection(seg, radius, get_pos(), x1, y1, x2, y2);

	if (nb_int >= 1)
	{
		if (contains_point(get_vector(x1, y1)) || contains_point(get_vector(x2, y2)))
		{
			return 1;
		}
	}

	return rect_cut.intersects_segment(seg);
}

/// modify the 'index_elem' value
void CircleCutShape::set_index_elem(int index_value)
{
	index_elem = index_value;

	circ_full.set_index_elem(index_value);
	rect_cut.set_index_elem(index_value);
}

/// modify the 'flag_contact' value
void CircleCutShape::set_contact(int flag_value)
{
	flag_contact = flag_value;

	circ_full.set_contact(flag_value);
	rect_cut.set_contact(flag_value);
}

/// modify the 'flag_uSwitch' value
void CircleCutShape::set_uSwitch(int flag_value)
{
	flag_uSwitch = flag_value;

	circ_full.set_uSwitch(flag_value);
	rect_cut.set_uSwitch(flag_value);
}

/*! \brief update the force and torque based on the circle
 *
 * \param[in] x_cont contact point x coordinate [m]
 * \param[in] y_cont contact point y coordinate [m]
 */
void CircleCutShape::inc_circ_full_torque(double x_cont, double y_cont)
{
	double cont_alpha, diff_alpha;
	Vector2D F, diff_vec, cont_point;

	F = circ_full.get_force();

	cont_point = get_vector(x_cont, y_cont);

	diff_vec = cont_point - get_pos();

	cont_alpha = atan2(diff_vec.get_y(), diff_vec.get_x());

	diff_alpha = get_alpha() - cont_alpha;

	// limit
	diff_alpha = (diff_alpha >   M_PI) ? diff_alpha - 2.0 * M_PI : diff_alpha;
	diff_alpha = (diff_alpha <= -M_PI) ? diff_alpha + 2.0 * M_PI : diff_alpha;

	if (diff_alpha > THRES_TORQUE_ANGLE) // rear right
	{
		circ_full.inc_torque(F.get_dist() * CST_TORQUE_ARM);
	}
	else if (diff_alpha < -THRES_TORQUE_ANGLE) // rear left
	{
		circ_full.inc_torque(-F.get_dist() * CST_TORQUE_ARM);
	}
}

/*! \brief update the force and torque based on the circle
 */
void CircleCutShape::set_force_torque_circle()
{
	set_force(circ_full.get_force());
	set_torque(circ_full.get_torque());
}

/*! \brief update the force and torque based on the rectangle
 */
void CircleCutShape::set_force_torque_rectangle()
{
	set_force(rect_cut.get_force());
	set_torque(rect_cut.get_torque() + cross_product(rect_center - get_pos(), rect_cut.get_force()));
}

/*! \brief update the force and torque based on the circle and the rectangle
 */
void CircleCutShape::set_all_force_torque()
{
	set_force(circ_full.get_force() + rect_cut.get_force());
	set_torque(circ_full.get_torque() + rect_cut.get_torque() + cross_product(rect_center - get_pos(), rect_cut.get_force()));
}
