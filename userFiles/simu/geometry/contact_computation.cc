
#include "contact_computation.hh"
#include "GeometryShape.hh"
#include "WallShape.hh"
#include "CircleShape.hh"
#include "CircleCutShape.hh"
#include "RectangleShape.hh"
#include "Vector2D.hh"
#include "user_realtime.h"
#include <stdlib.h>

// prototypes of the contact functions
int contact_wall_wall(WallShape *wall_1, WallShape *wall_2);
int contact_wall_circle(WallShape *wall_1, CircleShape *circle_2);
int contact_wall_circle_cut(WallShape *wall_1, CircleCutShape *circle_cut_2);
int contact_wall_rectangle(WallShape *wall_1, RectangleShape *rectangle_2);
int contact_circle_circle(CircleShape *circle_1, CircleShape *circle_2);
int contact_circle_circle_cut(CircleShape *circle_1, CircleCutShape *circle_cut_2);
int contact_circle_rectangle(CircleShape *circle_1, RectangleShape *rectangle_2);
int contact_circle_cut_circle_cut(CircleCutShape *circle_cut_1, CircleCutShape *circle_cut_2);
int contact_circle_cut_rectangle(CircleCutShape *circle_cut_1, RectangleShape *rectangle_2);
int contact_rectangle_rectangle(RectangleShape *rectangle_1, RectangleShape *rectangle_2);

// additional functions
void triangle_area(Vector2D const& point_A, Vector2D const& point_B, Vector2D const& point_C, Vector2D const& point_A_P, Vector2D const& point_B_P, Vector2D const& point_C_P, double & area, double & area_p);
void lens_area(double R, double d, double d_p, double &area, double &area_p);
double force_config(double area, double area_p, BasicShape *basic_1, BasicShape *basic_2);

int contact_intersec_circles(CircleShape *circ_1, CircleShape *circ_2, double &x_a, double &x_b, double &y_a, double &y_b);
int contact_intersec_circles(double x1, double x2, double y1, double y2, double R1, double R2, double &x_a, double &x_b, double &y_a, double &y_b);
int contact_intersec_wall_circle(WallShape *wall, CircleShape *circle, double &x_cont, double &y_cont);

/// square computation
inline double square(double x) { return x*x; }

/*! \brief configuration of the repulsive force
 *
 * \param[in] area intersection area [m]
 * \param[in] area_p time derivative of 'area' [m/s]
 * \param[in] basic_1 first geometry shape
 * \param[in] basic_2 second geometry shape
 * \return repulsive force [N]
 *
 * This function can be edited if needed
 */
double force_config(double area, double area_p, BasicShape *basic_1, BasicShape *basic_2)
{
	if (basic_1->is_moving() || basic_2->is_moving())
	{
		return 1.0e4 * area + 1.0e2 * area_p;
	}
	else
	{
		return 1.0e6 * area + 1.0e4 * area_p;
	}
}

/*! \brief main gate to choose the type of contact computation
 * 
 * \param[in] basic_1 first geometry shape
 * \param[in] basic_2 second geometry shape
 *
 * This is decided according to the geometry shapes type
 */
int contact_basic_shapes(BasicShape *basic_1, BasicShape *basic_2)
{
	int contact_bodies;

	contact_bodies = 0;

	switch ( basic_1->get_index_type() ) 
	{
		// basic 1: wall
		case WALL_TYPE:
			switch ( basic_2->get_index_type() ) 
			{
				case WALL_TYPE:
					contact_bodies = contact_wall_wall(static_cast<WallShape*>(basic_1), static_cast<WallShape*>(basic_2));
					break;

				case CIRCLE_TYPE:
					contact_bodies = contact_wall_circle(static_cast<WallShape*>(basic_1), static_cast<CircleShape*>(basic_2));
					break;

				case CIRCLE_CUT_TYPE:
					contact_bodies = contact_wall_circle_cut(static_cast<WallShape*>(basic_1), static_cast<CircleCutShape*>(basic_2));
					break;

				case RECTANGLE_TYPE:
					contact_bodies = contact_wall_rectangle(static_cast<WallShape*>(basic_1), static_cast<RectangleShape*>(basic_2));
					break;
			
				default:
					std::cout << "Error: unknown ID for basic shape: " << basic_2->get_index_type() << "" << std::endl;
					exit(EXIT_FAILURE);
			}
			break;

		// basic 1: circle
		case CIRCLE_TYPE:
			switch ( basic_2->get_index_type() ) 
			{
				case WALL_TYPE:
					contact_bodies = contact_wall_circle(static_cast<WallShape*>(basic_2), static_cast<CircleShape*>(basic_1));
					break;

				case CIRCLE_TYPE:
					contact_bodies = contact_circle_circle(static_cast<CircleShape*>(basic_1), static_cast<CircleShape*>(basic_2));
					break;

				case CIRCLE_CUT_TYPE:
					contact_bodies = contact_circle_circle_cut(static_cast<CircleShape*>(basic_1), static_cast<CircleCutShape*>(basic_2));
					break;

				case RECTANGLE_TYPE:
					contact_bodies = contact_circle_rectangle(static_cast<CircleShape*>(basic_1), static_cast<RectangleShape*>(basic_2));
					break;
			
				default:
					std::cout << "Error: unknown ID for basic shape: " << basic_2->get_index_type() << "" << std::endl;
					exit(EXIT_FAILURE);
			}
			break;

		// basic 1: circle cut
		case CIRCLE_CUT_TYPE:
			switch ( basic_2->get_index_type() ) 
			{
				case WALL_TYPE:
					contact_bodies = contact_wall_circle_cut(static_cast<WallShape*>(basic_2), static_cast<CircleCutShape*>(basic_1));
					break;

				case CIRCLE_TYPE:
					contact_bodies = contact_circle_circle_cut(static_cast<CircleShape*>(basic_2), static_cast<CircleCutShape*>(basic_1));
					break;

				case CIRCLE_CUT_TYPE:
					contact_bodies = contact_circle_cut_circle_cut(static_cast<CircleCutShape*>(basic_1), static_cast<CircleCutShape*>(basic_2));
					break;

				case RECTANGLE_TYPE:
					contact_bodies = contact_circle_cut_rectangle(static_cast<CircleCutShape*>(basic_1), static_cast<RectangleShape*>(basic_2));
					break;
			
				default:
					std::cout << "Error: unknown ID for basic shape: " << basic_2->get_index_type() << "" << std::endl;
					exit(EXIT_FAILURE);
			}
			break;

		// basic 1: rectangle
		case RECTANGLE_TYPE:
			switch ( basic_2->get_index_type() ) 
			{
				case WALL_TYPE:
					contact_bodies = contact_wall_rectangle(static_cast<WallShape*>(basic_2), static_cast<RectangleShape*>(basic_1));
					break;

				case CIRCLE_TYPE:
					contact_bodies = contact_circle_rectangle(static_cast<CircleShape*>(basic_2), static_cast<RectangleShape*>(basic_1));
					break;

				case CIRCLE_CUT_TYPE:
					contact_bodies = contact_circle_cut_rectangle(static_cast<CircleCutShape*>(basic_2), static_cast<RectangleShape*>(basic_1));
					break;

				case RECTANGLE_TYPE:
					contact_bodies = contact_rectangle_rectangle(static_cast<RectangleShape*>(basic_1), static_cast<RectangleShape*>(basic_2));
					break;
			
				default:
					std::cout << "Error: unknown ID for basic shape: " << basic_2->get_index_type() << "" << std::endl;
					exit(EXIT_FAILURE);
			}
			break;
	
		default:
			std::cout << "Error: unknown ID for basic shape: " << basic_1->get_index_type() << "" << std::endl;
			exit(EXIT_FAILURE);
	}

	return contact_bodies;
}

/*! \brief contact between two walls
 * 
 * \param[in] wall_1 first wall
 * \param[in] wall_2 second wall
 * \return 1 if contact, 0 otherwise
 */
int contact_wall_wall(WallShape *wall_1, WallShape *wall_2)
{
	// nothing happens: contact between two fixed bodies
	return 0;
}

/*! \brief contact between a wall and a circle
 * 
 * \param[in] wall_1 wall
 * \param[in] circle_2 circle
 * \return 1 if contact, 0 otherwise
 */
int contact_wall_circle(WallShape *wall_1, CircleShape *circle_2)
{
	double d, d_p, F;
	double dist_wall;
	double delta_x, delta_y, delta_x_p, delta_y_p;
	double c_alpha, s_alpha;
	double area, area_p;
	double radius;

	Orientation wall_orient;
	Vector2D F_vec_1, F_vec_2;
	Pose pose_wall, pose_circle;

	pose_wall   = wall_1->get_pose();
	pose_circle = circle_2->get_pose();

	wall_orient = wall_1->get_orient();

	c_alpha = wall_orient.get_c_alpha();
	s_alpha = wall_orient.get_s_alpha();

	delta_x = pose_circle.get_x() - pose_wall.get_x();
	delta_y = pose_circle.get_y() - pose_wall.get_y();

	delta_x_p = pose_circle.get_x_p() - pose_wall.get_x_p();
	delta_y_p = pose_circle.get_y_p() - pose_wall.get_y_p();

	dist_wall = c_alpha * delta_x + s_alpha * delta_y;

	radius = circle_2->get_radius();

	d = radius - fabs(dist_wall);

	if (d > 0)
	{
		d_p = -c_alpha * delta_x_p - s_alpha * delta_y_p;

		if (dist_wall < 0.0)
		{
			d_p = -d_p;
		}

		lens_area(radius, radius - d, -d_p, area, area_p);

		F = force_config(area, area_p, wall_1, circle_2);

		F_vec_1.set_index_vector(0, -F * c_alpha);
		F_vec_1.set_index_vector(1, -F * s_alpha);

		F_vec_2 = get_opp_vector(F_vec_1);

		wall_1->inc_force(F_vec_1);
		circle_2->inc_force(F_vec_2);

		return 1;
	}

	return 0;
}

/*! \brief contact between a wall and a cutted circle
 * 
 * \param[in] wall_1 wall
 * \param[in] circle_cut_2 cutted circle
 * \return 1 if contact, 0 otherwise
 */
int contact_wall_circle_cut(WallShape *wall_1, CircleCutShape *circle_cut_2)
{
	double x_cont, y_cont;
	RectangleShape *rect;
	CircleShape *circ;

	if (! circle_cut_2->intersects_line(wall_1->get_seg_wall()))
	{
		return 0;
	}
	else
	{
		rect = circle_cut_2->get_rect_cut();
		circ = circle_cut_2->get_circ_full();

		if (contact_wall_rectangle(wall_1, rect))
		{
			circle_cut_2->set_force_torque_rectangle();

			return 1;
		}
		else
		{
			if (contact_wall_circle(wall_1, circ))
			{
				if (contact_intersec_wall_circle(wall_1, circ, x_cont, y_cont))
				{
					circle_cut_2->inc_circ_full_torque(x_cont, y_cont);
				}
				
				circle_cut_2->set_force_torque_circle();

				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}

/*! \brief contact between a wall and a rectangle
 * 
 * \param[in] wall_1 wall
 * \param[in] rectangle_2 rectangle
 * \return 1 if contact, 0 otherwise
 */
int contact_wall_rectangle(WallShape *wall_1, RectangleShape *rectangle_2)
{
	int nb_contacts;

	double cur_area, cur_area_p, max_area, max_area_p, F;
	double c_alpha, s_alpha;
	double delta_x, delta_y, dist_wall;
	double tor_1, tor_2;

	Orientation wall_orient;
	Vector2D X1, X2, X1_P, X2_P;
	Vector2D F_vec_1, F_vec_2;
	Vector2D force_point;
	Vector2D wall_pos, rect_pos;
	Pose pose_wall, pose_rect;
	Vector2D inside_point, inside_point_p;
	Vector2D cur_point, cur_point_p;

	wall_orient = wall_1->get_orient();

	wall_pos = wall_1->get_pos();
	rect_pos = rectangle_2->get_pos();

	pose_wall = wall_1->get_pose();
	pose_rect = rectangle_2->get_pose();

	c_alpha = wall_orient.get_c_alpha();
	s_alpha = wall_orient.get_s_alpha();

	delta_x = pose_rect.get_x() - pose_wall.get_x();
	delta_y = pose_rect.get_y() - pose_wall.get_y();

	dist_wall = c_alpha * delta_x + s_alpha * delta_y;

	if (dist_wall > rectangle_2->get_wrapping_radius())
	{
		return 0;
	}

	nb_contacts = get_wall_seg_list_two_points(rectangle_2->get_segment_list(), rectangle_2->get_list_size(),wall_pos , wall_1->get_orient(), X1, X2, X1_P, X2_P);

	if (nb_contacts > 1)
	{
		max_area   = 0.0;
		max_area_p = 0.0;

		cur_area   = 0.0;
		cur_area_p = 0.0;

		for (int i=0; i<rectangle_2->get_list_size(); i++)
		{
			cur_point = rectangle_2->get_point(i);

			cur_point_p = get_rigid_pos_p(pose_rect, cur_point);

			if (wall_1->contains_point(cur_point))
			{
				triangle_area(cur_point, X1, X2, cur_point_p, X1_P, X2_P, cur_area, cur_area_p);
			}

			if (cur_area > max_area)
			{
				max_area   = cur_area;
				max_area_p = cur_area_p;
			}
		}

		F = force_config(max_area, max_area_p, wall_1, rectangle_2);

		force_point = get_mid_vector(X1, X2);

		F_vec_2 = get_vec_amp_angle(F, wall_orient.get_c_alpha(), wall_orient.get_s_alpha());

		F_vec_1 = get_opp_vector(F_vec_2);

		tor_1 = torque_from_force(F_vec_1, force_point, wall_pos);

		tor_2 = torque_from_force(F_vec_2, force_point, rect_pos);

		wall_1->inc_force(F_vec_1);
		rectangle_2->inc_force(F_vec_2);

		wall_1->inc_torque(tor_1);
		rectangle_2->inc_torque(tor_2);

		return 1;
	}

	return 0;
}

/*! \brief contact between two circles
 * 
 * \param[in] circle_1 first circle
 * \param[in] circle_2 second circle
 * \return 1 if contact, 0 otherwise
 */
int contact_circle_circle(CircleShape *circle_1, CircleShape *circle_2)
{
	double d, d_p, dist, dist_p, dist_center;
	double d1, d2, d1_p, d2_p;
	double delta_x, delta_y, delta_x_p, delta_y_p;
	double F;
	double denom_inv;
	double radius_1, radius_2;
	double num, denom;
	double area_1, area_2, area_1_p, area_2_p;

	Vector2D F_vec_1, F_vec_2;

	Pose pose_1, pose_2;

	pose_1 = circle_1->get_pose();
	pose_2 = circle_2->get_pose();

	radius_1 = circle_1->get_radius();
	radius_2 = circle_2->get_radius();

	dist_center = get_dist_pose(pose_1, pose_2);

	dist = radius_1 + radius_2 - dist_center;

	if (dist > 0.0)
	{	
		delta_x = pose_2.get_x() - pose_1.get_x();
		delta_y = pose_2.get_y() - pose_1.get_y();

		delta_x_p = pose_2.get_x_p() - pose_1.get_x_p();
		delta_y_p = pose_2.get_y_p() - pose_1.get_y_p();

		dist_p = -( delta_x*delta_x_p + delta_y*delta_y_p ) / dist_center;

		// area computation
		d = dist_center;
		d_p = -dist_p;

		num = square(d) - square(radius_2) + square(radius_1);
		denom = 2.0 * d;

		d1 = num / denom;
		d1_p = ((2.0*d*d_p)*denom - num*(2.0*d_p)) / square(denom);

		d2 = d - d1;
		d2_p = d_p - d1_p;

		lens_area(radius_1, d1, d1_p, area_1, area_1_p);
		lens_area(radius_2, d2, d2_p, area_2, area_2_p);

		F = force_config(area_1 + area_2, area_1_p + area_2_p, circle_1, circle_2);

		if (!delta_x)
		{
			F_vec_1.set_index_vector(0, 0.0);

			if (delta_y > 0.0)
			{
				F_vec_1.set_index_vector(1, -F);
			}
			else
			{
				F_vec_1.set_index_vector(1, F);
			}
		}
		else
		{
			denom_inv = 1.0 / sqrt(delta_x*delta_x + delta_y*delta_y);

			F_vec_1.set_index_vector(0, -F * delta_x * denom_inv);
			F_vec_1.set_index_vector(1, -F * delta_y * denom_inv);
		}

		F_vec_2 = get_opp_vector(F_vec_1);

		circle_1->inc_force(F_vec_1);
		circle_2->inc_force(F_vec_2);

		return 1;
	}

	return 0;
}

/*! \brief contact a circle and a cutted circle
 * 
 * \param[in] circle_1 circle
 * \param[in] circle_cut_2 cutted circle
 * \return 1 if contact, 0 otherwise
 */
int contact_circle_circle_cut(CircleShape *circle_1, CircleCutShape *circle_cut_2)
{
	int flag_contact, nb_contacts;
	double dist_center, d;
	double x_a, x_b, y_a, y_b, x_mid, y_mid;

	Vector2D point_A, point_B;

	RectangleShape *rect;
	CircleShape *circ;

	rect = circle_cut_2->get_rect_cut();
	circ = circle_cut_2->get_circ_full();

	// compute the circles distances
	dist_center = get_dist_pose(circle_1->get_pose(), circ->get_pose());

	d = circle_1->get_radius() + circ->get_radius() - dist_center;

	if (d < 0.0)
	{
		return 0;
	}

	// contact with the cutted edge
	flag_contact = circle_1->intersects_line(rect->get_segment(1));

	// contact with circle	
	nb_contacts = contact_intersec_circles(circle_1, circ, x_a, x_b, y_a, y_b);

	if (!flag_contact)
	{
		if (nb_contacts != 2)
		{
			return 0;
		}

		point_A.set_vector(x_a, y_a);
		point_B.set_vector(x_b, y_b);

		flag_contact = (circle_cut_2->contains_point(point_A) && circle_cut_2->contains_point(point_B));
	}

	if (!flag_contact)
	{
		return 0;
	}
	else
	{
		if (contact_circle_rectangle(circle_1, rect))
		{
			circle_cut_2->set_force_torque_rectangle();

			return 1;
		}
		else
		{
			if (contact_circle_circle(circle_1, circ))
			{
				if (nb_contacts == 2)
				{
					x_mid = 0.5 * (x_a + x_b);
					y_mid = 0.5 * (y_a + y_b);

					circle_cut_2->inc_circ_full_torque(x_mid, y_mid);
				}
				
				circle_cut_2->set_force_torque_circle();

				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}

/*! \brief contact between a circle and a rectangle
 * 
 * \param[in] circle_1 circle
 * \param[in] rectangle_2 rectangle
 * \return 1 if contact, 0 otherwise
 */
int contact_circle_rectangle(CircleShape *circle_1, RectangleShape *rectangle_2)
{
	int nb_contacts;
	double d, d_p;
	double tor_2;
	double F;
	double circle_radius;
	double cur_area, cur_area_p;
	double area_circle, area_circle_p;
	double max_area_rect, max_area_rect_p;
	double dist, dist_p;

	Vector2D X1, X2, X1_P, X2_P;
	Vector2D force_point;
	Vector2D F_vec_1, F_vec_2;
	Vector2D cur_point, cur_point_p;
	Pose pose_circle, pose_rect;
	Orientation force_or;

	pose_circle = circle_1->get_pose();
	pose_rect   = rectangle_2->get_pose();

	circle_radius = circle_1->get_radius();

	// useless safety, normally already done with 'contact_possible' in ContactGestion.cc.
	if (get_dist_pose(pose_circle, pose_rect) > circle_1->get_wrapping_radius() + rectangle_2->get_wrapping_radius())
	{
		return 0;
	}

	nb_contacts = get_circle_seg_list_two_points(rectangle_2->get_segment_list(), rectangle_2->get_list_size(), circle_radius, pose_circle.get_pos(), pose_circle.get_pos_p(), X1, X2, X1_P, X2_P);

	if (nb_contacts > 1)
	{
		d = get_dist_points2D(X1, X2);

		d_p = get_dist_points2D_der(X1, X1_P, X2, X2_P);

		// safety
		if (fabs(d) > 2.0*circle_radius)
		{
			return 0;
		}

		// area circle
		dist = sqrt(square(circle_radius) - 0.25*square(d));

		dist_p = -(d * d_p) / (4.0 * dist); 

		lens_area(circle_radius, dist, dist_p, area_circle, area_circle_p);

		// area rect
		max_area_rect   = 0.0;
		max_area_rect_p = 0.0;

		for (int i=0; i<rectangle_2->get_list_size(); i++)
		{
			cur_point = rectangle_2->get_point(i);

			cur_point_p = get_rigid_pos_p(pose_rect, cur_point);

			if (circle_1->contains_point(cur_point))
			{
				triangle_area(cur_point, X1, X2, cur_point_p, X1_P, X2_P, cur_area, cur_area_p);

				if (cur_area > max_area_rect)
				{
					max_area_rect   = cur_area;
					max_area_rect_p = cur_area_p;
				}
			}
		}

		F = force_config(area_circle + max_area_rect, area_circle_p + max_area_rect_p, circle_1, rectangle_2);

		force_point = get_mid_vector(X1, X2);

		force_or = get_perp_orientation(X1, X2);

		if (is_correct_direction(force_point, circle_1->get_pos(), force_or.get_alpha()))
		{
			F_vec_1 = get_vec_amp_angle(F, force_or.get_c_alpha(), force_or.get_s_alpha());
			F_vec_2 = get_opp_vector(F_vec_1);
		}
		else
		{
			F_vec_2 = get_vec_amp_angle(F, force_or.get_c_alpha(), force_or.get_s_alpha());
			F_vec_1 = get_opp_vector(F_vec_2);
		}		

		tor_2 = torque_from_force(F_vec_2, force_point, rectangle_2->get_pos());

		circle_1->inc_force(F_vec_1);
		rectangle_2->inc_force(F_vec_2);

		rectangle_2->inc_torque(tor_2);

		return 1;
	}

	return 0;
}

/*! \brief contact between two cutted circles
 * 
 * \param[in] circle_cut_1 first cutted circle
 * \param[in] circle_cut_2 second cutted circle
 * \return 1 if contact, 0 otherwise
 */
int contact_circle_cut_circle_cut(CircleCutShape *circle_cut_1, CircleCutShape *circle_cut_2)
{
	int flag_contact;
	double dist_center, d, nb_contacts;
	double x_a, x_b, y_a, y_b, x_mid, y_mid;

	Vector2D point_A, point_B;

	RectangleShape *rect_1, *rect_2;
	CircleShape *circ_1, *circ_2;

	rect_1 = circle_cut_1->get_rect_cut();
	circ_1 = circle_cut_1->get_circ_full();

	rect_2 = circle_cut_2->get_rect_cut();
	circ_2 = circle_cut_2->get_circ_full();

	// compute the circles distances
	dist_center = get_dist_pose(circ_1->get_pose(), circ_2->get_pose());

	d = circ_1->get_radius() + circ_2->get_radius() - dist_center;

	if (d < 0.0)
	{
		return 0;
	}

	if (contact_circle_cut_rectangle(circle_cut_1, rect_2))
	{
		circle_cut_2->set_force_torque_rectangle();

		return 1;
	}

	if (contact_circle_cut_rectangle(circle_cut_2, rect_1))
	{
		circle_cut_1->set_force_torque_rectangle();

		return 1;
	}

	nb_contacts = contact_intersec_circles(circ_1, circ_2, x_a, x_b, y_a, y_b);

	if (nb_contacts != 2)
	{
		return 0;
	}

	point_A.set_vector(x_a, y_a);
	point_B.set_vector(x_b, y_b);

	if (circle_cut_1->contains_point(point_A) && circle_cut_1->contains_point(point_B) &&
		circle_cut_2->contains_point(point_A) && circle_cut_2->contains_point(point_B))
	{
		if (contact_circle_circle(circ_1, circ_2))
		{
			x_mid = 0.5 * (x_a + x_b);
			y_mid = 0.5 * (y_a + y_b);

			circle_cut_1->inc_circ_full_torque(x_mid, y_mid);
			circle_cut_2->inc_circ_full_torque(x_mid, y_mid);

			circle_cut_1->set_force_torque_circle();
			circle_cut_2->set_force_torque_circle();

			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/*! \brief contact between a cutted circle and a rectangle
 * 
 * \param[in] circle_cut_1 cutted circle
 * \param[in] rectangle_2 rectangle
 * \return 1 if contact, 0 otherwise
 */
int contact_circle_cut_rectangle(CircleCutShape *circle_cut_1, RectangleShape *rectangle_2)
{
	int flag_contact, nb_contacts;
	double x_cont, y_cont;

	RectangleShape *rect;
	CircleShape *circ;

	Vector2D X1, X2, X1_P, X2_P;
	Pose pose_circle;

	rect = circle_cut_1->get_rect_cut();
	circ = circle_cut_1->get_circ_full();

	rect->update_segments();

	flag_contact = 0;

	for(int i=0; i<rectangle_2->get_list_size(); i++)
	{
		if (circle_cut_1->intersects_segment(rectangle_2->get_segment(i)))
		{
			flag_contact = 1;
			break;
		}
	}

	if (!flag_contact)
	{
		return 0;
	}
	else
	{
		if (contact_rectangle_rectangle(rect, rectangle_2))
		{
			circle_cut_1->set_force_torque_rectangle();

			return 1;
		}
		else
		{
			if (contact_circle_rectangle(circ, rectangle_2))
			{
				pose_circle = circ->get_pose();

				nb_contacts = get_circle_seg_list_two_points(rectangle_2->get_segment_list(), rectangle_2->get_list_size(), circ->get_radius(), pose_circle.get_pos(), pose_circle.get_pos_p(), X1, X2, X1_P, X2_P);

				if (nb_contacts > 1)
				{
					x_cont = 0.5 * (X1.get_x() + X2.get_x());
					y_cont = 0.5 * (X1.get_y() + X2.get_y());

					circle_cut_1->inc_circ_full_torque(x_cont, y_cont);
				}
				
				circle_cut_1->set_force_torque_circle();

				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}

/*! \brief contact between two rectangles
 * 
 * \param[in] rectangle_1 first rectangle
 * \param[in] rectangle_2 second rectangle
 * \return 1 if contact, 0 otherwise
 */
int contact_rectangle_rectangle(RectangleShape *rectangle_1, RectangleShape *rectangle_2)
{
	int nb_contacts;
	double F, tor_1, tor_2;
	double cur_area, cur_area_p;
	double max_area_1, max_area_p_1, max_area_2, max_area_p_2;
	double max_area, max_area_p;

	Vector2D X1, X2, X1_P, X2_P;
	Vector2D force_point, F_vec_1, F_vec_2;
	Vector2D cur_point, cur_point_p;
	Pose pose_1, pose_2;
	Orientation force_or;

	pose_1 = rectangle_1->get_pose();
	pose_2 = rectangle_2->get_pose();

	// useless safety, normally already done with 'contact_possible' in ContactGestion.cc...
	if (get_dist_pose(pose_1, pose_2) > rectangle_1->get_wrapping_radius() + rectangle_2->get_wrapping_radius())
	{
		return 0;
	}

	nb_contacts = get_two_seg_list_two_points(rectangle_1->get_segment_list(), rectangle_1->get_list_size(), rectangle_2->get_segment_list(), rectangle_2->get_list_size(), X1, X2, X1_P, X2_P);

	if (nb_contacts > 1)
	{		
		max_area_1 = 0.0;
		max_area_2 = 0.0;

		max_area_p_1 = 0.0;
		max_area_p_2 = 0.0;

		cur_area   = 0.0;
		cur_area_p = 0.0;

		// area rect 1
		for (int i=0; i<rectangle_1->get_list_size(); i++)
		{
			cur_point = rectangle_1->get_point(i);

			cur_point_p = get_rigid_pos_p(pose_1, cur_point);

			if (rectangle_2->contains_point(cur_point))
			{
				triangle_area(cur_point, X1, X2, cur_point_p, X1_P, X2_P, cur_area, cur_area_p);
			}

			if (cur_area > max_area_1)
			{
				max_area_1   = cur_area;
				max_area_p_1 = cur_area_p;
			}
		}

		// area rect 2
		for (int i=0; i<rectangle_2->get_list_size(); i++)
		{
			cur_point = rectangle_2->get_point(i);

			cur_point_p = get_rigid_pos_p(pose_2, cur_point);

			if (rectangle_1->contains_point(cur_point))
			{
				triangle_area(cur_point, X1, X2, cur_point_p, X1_P, X2_P, cur_area, cur_area_p);
			}

			if (cur_area > max_area_2)
			{
				max_area_2   = cur_area;
				max_area_p_2 = cur_area_p;
			}
		}

		max_area   = max_area_1   + max_area_2;
		max_area_p = max_area_p_1 + max_area_p_2;

		F = force_config(max_area, max_area_p, rectangle_1, rectangle_2);

		force_point = get_mid_vector(X1, X2);

		force_or = get_perp_orientation(X1, X2);

		if (is_correct_direction(force_point, rectangle_1->get_pos(), force_or.get_alpha()))
		{
			F_vec_1 = get_vec_amp_angle(F, force_or.get_c_alpha(), force_or.get_s_alpha());
			F_vec_2 = get_opp_vector(F_vec_1);
		}
		else
		{
			F_vec_2 = get_vec_amp_angle(F, force_or.get_c_alpha(), force_or.get_s_alpha());
			F_vec_1 = get_opp_vector(F_vec_2);
		}		

		tor_1 = torque_from_force(F_vec_1, force_point, rectangle_1->get_pos());
		tor_2 = torque_from_force(F_vec_2, force_point, rectangle_2->get_pos());

		rectangle_1->inc_force(F_vec_1);
		rectangle_2->inc_force(F_vec_2);

		rectangle_1->inc_torque(tor_1);
		rectangle_2->inc_torque(tor_2);

		return 1;
	}

	return 0;
}

/*! \brief compute the area of a triangle (and its derivative)
 * 
 * \param[in] point_A first corner position [m]
 * \param[in] point_B second corner position [m]
 * \param[in] point_C third corner position [m]
 * \param[in] point_A_P first corner position derivative [m/s]
 * \param[in] point_B_P second corner position derivative [m/s]
 * \param[in] point_C_P third corner position derivative [m/s]
 * \param[out] area area of the triangle [m^2]
 * \param[out] area_p area derivative of the triangle [m^2/s] 
 *
 * This is done given the position and velocity of its three corners
 */
void triangle_area(Vector2D const& point_A, Vector2D const& point_B, Vector2D const& point_C, Vector2D const& point_A_P, Vector2D const& point_B_P, Vector2D const& point_C_P, double & area, double & area_p)
{
	double delta_x1, delta_x2, delta_y1, delta_y2;
	double delta_x1_p, delta_x2_p, delta_y1_p, delta_y2_p;
	double expr;

	delta_x1 = point_B.get_vec(0) - point_A.get_vec(0);
	delta_x2 = point_C.get_vec(0) - point_A.get_vec(0);

	delta_y1 = point_C.get_vec(1) - point_A.get_vec(1);
	delta_y2 = point_B.get_vec(1) - point_A.get_vec(1);

	delta_x1_p = point_B_P.get_vec(0) - point_A_P.get_vec(0);
	delta_x2_p = point_C_P.get_vec(0) - point_A_P.get_vec(0);

	delta_y1_p = point_C_P.get_vec(1) - point_A_P.get_vec(1);
	delta_y2_p = point_B_P.get_vec(1) - point_A_P.get_vec(1);

	expr = delta_x1 * delta_y1 - delta_x2 * delta_y2;

	area = 0.5 * fabs(expr);

	area_p = 0.5 * (delta_x1_p*delta_y1 +  delta_x1*delta_y1_p - delta_x2_p*delta_y2 - delta_x2*delta_y2_p);

	if (expr < 0.0)
	{
		area_p = - area_p;
	}
}

/*! \brief compute the area of a lens from a circle
 * 
 * \param[in] R circle radius
 * \param[in] d distance from the center to the lens
 * \param[in] d_p time derivative of 'd'
 * \param[out] area area of the lens
 * \param[out] area_p derivative of 'area'
 */
void lens_area(double R, double d, double d_p, double &area, double &area_p)
{
	double R_square, sqrt_val;

	// safety
	if (fabs(d) >= R)
	{
		area   = 0.0;
		area_p = 0.0;
		return;
	}

	R_square = square(R);
	sqrt_val = sqrt(R_square - square(d));

	area = R_square * acos(d/R) - d * sqrt_val;

	area_p = -2.0 * d_p * sqrt_val;
}

/*! \brief compute the middle of the contact points between a wall and a circle
 * 
 * \param[in] wall wall
 * \param[in] circle circle
 * \param[out] x_cont requested point (x coordinate)
 * \param[out] y_cont requested point (y coordinate)
 * \return 1 if contact, 0 otherwise
 */
int contact_intersec_wall_circle(WallShape *wall, CircleShape *circle, double &x_cont, double &y_cont)
{
	double dist_wall;
	double delta_x, delta_y;
	double c_alpha, s_alpha;
	double radius;

	Orientation wall_orient;
	Pose pose_wall, pose_circle;

	x_cont = 0.0;
	y_cont = 0.0;

	pose_wall   = wall->get_pose();
	pose_circle = circle->get_pose();

	wall_orient = wall->get_orient();

	c_alpha = wall_orient.get_c_alpha();
	s_alpha = wall_orient.get_s_alpha();

	delta_x = pose_circle.get_x() - pose_wall.get_x();
	delta_y = pose_circle.get_y() - pose_wall.get_y();

	dist_wall = c_alpha * delta_x + s_alpha * delta_y;

	radius = circle->get_radius();

	if (radius < fabs(dist_wall))
	{
		return 0;
	}

	x_cont = pose_circle.get_x() - dist_wall * c_alpha;
	y_cont = pose_circle.get_y() - dist_wall * s_alpha;

	return 1;
}

/*! \brief compute the contact points between two circles
 * 
 * \param[in] circ_1 first circle
 * \param[in] circ_2 second circle
 * \param[out] x_a first  instersection x coordinate
 * \param[out] x_b second instersection x coordinate
 * \param[out] y_a first  instersection y coordinate
 * \param[out] y_b second instersection y coordinate
 * \return number of contacts
 */
int contact_intersec_circles(CircleShape *circ_1, CircleShape *circ_2, double &x_a, double &x_b, double &y_a, double &y_b)
{
	return contact_intersec_circles(circ_1->get_x(), circ_2->get_x(), circ_1->get_y(), circ_2->get_y(), circ_1->get_radius(), circ_2->get_radius(), x_a, x_b, y_a, y_b);
}

/*! \brief compute the contact points between two circles
 * 
 * \param[in] x1 first  circle x center
 * \param[in] x2 second circle x center
 * \param[in] y1 first  circle y center
 * \param[in] y2 second circle y center
 * \param[in] R1 first  circle radius
 * \param[in] R2 second circle radius
 * \param[out] x_a first  instersection x coordinate
 * \param[out] x_b second instersection x coordinate
 * \param[out] y_a first  instersection y coordinate
 * \param[out] y_b second instersection y coordinate
 * \return number of contacts
 */
int contact_intersec_circles(double x1, double x2, double y1, double y2, double R1, double R2, double &x_a, double &x_b, double &y_a, double &y_b)
{
	double R1_square, R2_square;
	double delta_x, delta_y, alpha;
	double val, k1, k2;
	double a, b, c;

	// outputs
	x_a = 0.0;
	x_b = 0.0;
	y_a = 0.0;
	y_b = 0.0;

	R1_square = square(R1);
	R2_square = square(R2);

	delta_x = x1 - x2;
	delta_y = y1 - y2;

	alpha = (R1_square - R2_square) - (square(x1) - square(x2)) - (square(y1) - square(y2));

	// delta_y == 0
	if (!delta_y)
	{
		x_a = -alpha / (2.0 * delta_x);

		val = R1_square - square(x_a - x1);

		if (val < 0.0)
		{
			x_a = 0.0;
			return 0;
		}
		else if(!val)
		{
			y_a = y1;
			return 1;
		}
		else
		{
			val = sqrt(val);
			x_b = x_a;
			y_a = y1 - val;
			y_b = y1 + val;
			return 2;
		}
	}

	// delta_x == 0
	if (!delta_x)
	{
		y_a = -alpha / (2.0 * delta_y);

		val = R1_square - square(y_a - y1);

		if (val < 0.0)
		{
			y_a = 0.0;
			return 0;
		}
		else if(!val)
		{
			x_a = x1;
			return 1;
		}
		else
		{
			val = sqrt(val);
			y_b = y_a;
			x_a = x1 - val;
			x_b = x1 + val;
			return 2;
		}
	}

	// (delta_x != 0) & (delta_y != 0)
	k1 = -alpha / (2.0 * delta_y);
	k2 = -delta_x / delta_y;

	a = 1.0 + square(k2);
	b = (k1 - y1)*k2 - x1;
	c = square(k1 - y1) + square(x1) - R1_square;

	if (!a)
	{
		if (!b)
		{
			return 0;
		}

		x_a = -c / (2.0 * b);
		y_a = k1 + k2*x_a;
		return 1;
	}

	val = square(b) - a*c;

	if (val < 0.0)
	{
		return 0;
	}
	else if (!val)
	{
		x_a = -b / a;
		y_a = k1 + k2*x_a;
		return 1;
	}
	else
	{
		val = sqrt(val);

		x_a = (-b - val) / a;
		x_b = (-b + val) / a;

		y_a = k1 + k2*x_a;
		y_b = k1 + k2*x_b;

		return 2;
	}
}
