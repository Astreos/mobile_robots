
#include "Segment.hh"

#include <cmath>

#define EPSILON 1.0e-6

/// constructor
Segment::Segment(): X1(0.0, 0.0), X2(1.0, 0.0), X1_P(0.0, 0.0), X2_P(0.0, 0.0)
{
	flag_vert = 0;

	slope  = 0.0;
	offset = 0.0;

	slope_p  = 0.0;
	offset_p = 0.0;

	min_x = 0.0;
	max_x = 1.0;
	min_y = 0.0;
	max_y = 0.0;

	center.set_vector(0.5, 0.0);
	wrapping_radius = 0.5;
}

/// constructor
Segment::Segment(double x1, double y1, double x2, double y2): X1(x1, y1), X2(x2, y2), X1_P(0.0, 0.0), X2_P(0.0, 0.0)
{
	set_slope_offset();
	set_min_max();
	compute_wrapping_center();
}

/// destructor
Segment::~Segment()
{

}

/*! \brief set a new vector
 * 
 * \param[in] x1 x coordinate of the first point
 * \param[in] y1 y coordinate of the first point
 * \param[in] x2 x coordinate of the second point
 * \param[in] y2 y coordinate of the second point
 * \param[in] x1_p x velocity component of the first point
 * \param[in] y1_p y velocity component of the first point
 * \param[in] x2_p x velocity component of the second point
 * \param[in] y2_p y velocity component of the second point
 */
void Segment::set_vector(double x1, double y1, double x2, double y2, double x1_p, double y1_p, double x2_p, double y2_p)
{
	X1.set_vector(x1, y1);
	X2.set_vector(x2, y2);

	X1_P.set_vector(x1_p, y1_p);
	X2_P.set_vector(x2_p, y2_p);

	set_slope_offset();
	set_min_max();
	compute_wrapping_center();
}

/*! \brief set a new vector
 * 
 * \param[in] XA position of the first point
 * \param[in] XB position of the second point
 * \param[in] XA_P position derivative of the first point
 * \param[in] XB_P position derivative of the second point
 */
void Segment::set_vector(Vector2D const& XA, Vector2D const& XB, Vector2D const& XA_P, Vector2D const& XB_P)
{
	X1.set_vector(XA);
	X2.set_vector(XB);

	X1_P.set_vector(XA_P);
	X2_P.set_vector(XB_P);

	set_slope_offset();
	set_min_max();
	compute_wrapping_center();
}

/*! \brief compute the slope and the offset 
 * 
 */
void Segment::set_slope_offset()
{
	double delta_x, delta_y, delta_x_p, delta_y_p;

	delta_x = X2.get_vec(0) - X1.get_vec(0);
	delta_y = X2.get_vec(1) - X1.get_vec(1);

	if (!delta_x)
	{
		flag_vert = 1;

		slope  = 0.0;
		offset = X1.get_vec(0);

		slope_p  = 0.0;
		offset_p = X1_P.get_vec(0);
	}
	else
	{
		flag_vert = 0;

		slope  = delta_y / delta_x;
		offset = X1.get_vec(1) - slope*X1.get_vec(0);

		delta_x_p = X2_P.get_vec(0) - X1_P.get_vec(0);
		delta_y_p = X2_P.get_vec(1) - X1_P.get_vec(1);

		slope_p = (delta_y_p*delta_x - delta_y*delta_x_p) / (delta_x*delta_x);

		offset_p = X1_P.get_vec(1) - slope_p*X1.get_vec(0) - slope*X1_P.get_vec(0);
	}
}

/*! \brief compute the minima and maxima of the extrema points
 */
void Segment::set_min_max()
{
	if (X1.get_vec(0) < X2.get_vec(0))
	{
		min_x = X1.get_vec(0);
		max_x = X2.get_vec(0);
	}
	else
	{
		min_x = X2.get_vec(0);
		max_x = X1.get_vec(0);
	}

	if (X1.get_vec(1) < X2.get_vec(1))
	{
		min_y = X1.get_vec(1);
		max_y = X2.get_vec(1);
	}
	else
	{
		min_y = X2.get_vec(1);
		max_y = X1.get_vec(1);
	}
}

/*! \brief update the segment center
 */
void Segment::compute_center()
{
	center = get_mid_vector(X1, X2);
}

/*! \brief update the segment center and its wrapping radius
 */
void Segment::compute_wrapping_center()
{
	compute_center();

	wrapping_radius = get_dist_points2D(X1, X2);
}

/*! \brief compute the intersection between a segment (supposed infinite) and a circle (without speed)
 * 
 * \param[in] a segment
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \param[in] center_p velocity of the circle center [m/s]
 * \param[out] x1 x coordinate of the first intersection point [m]
 * \param[out] y1 y coordinate of the first intersection point [m]
 * \param[out] x2 x coordinate of the second intersection point [m]
 * \param[out] y2 y coordinate of the second intersection point [m]
 * \return the number of intersections found
 */	
int get_circle_line_intersection(Segment const& a, double radius, Vector2D const& center, double & x1, double & y1, double & x2, double & y2)
{
	double expr1, expr2, expr3;
	double R_2;
	double x, y, delta_x;
	double A, B, C;

	R_2 = radius * radius;

	if (a.get_flag_vert())
	{
		x   = a.get_offset();

		delta_x = x - center.get_vec(0);

		expr1 = R_2 - delta_x*delta_x;

		if (expr1 <= 0.0)
		{
			x1 = 0.0;
			y1 = 0.0;
			x2 = 0.0;
			y2 = 0.0;

			return 0;
		}
		else
		{
			expr2 = sqrt(expr1);

			x1 = x;
			x2 = x;

			y = center.get_vec(1);

			y1 = y - expr2;
			y2 = y + expr2;

			return 2;
		}
	}

	expr1 = a.get_offset() - center.get_vec(1);
	
	A = 1.0 + a.get_slope() * a.get_slope();
	B = 2.0 * ( a.get_slope()*expr1 - center.get_vec(0) );
	C = center.get_vec(0)*center.get_vec(0) + expr1*expr1 - R_2;

	expr2 = B*B - 4.0*A*C;

	if (expr2 <= 0.0)
	{
		x1 = 0.0;
		y1 = 0.0;
		x2 = 0.0;
		y2 = 0.0;

		return 0;
	}

	expr3 = sqrt(expr2);

	x1 = (-B - expr3) / (2.0*A);
	x2 = (-B + expr3) / (2.0*A);

	y1 = a.get_slope() * x1 + a.get_offset();
	y2 = a.get_slope() * x2 + a.get_offset();

	return 2;
}

/*! \brief compute the intersection between a segment (supposed infinite) and a circle (with speed)
 * 
 * \param[in] a segment
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \param[in] center_p velocity of the circle center [m/s]
 * \param[out] x1 x coordinate of the first intersection point [m]
 * \param[out] y1 y coordinate of the first intersection point [m]
 * \param[out] x2 x coordinate of the second intersection point [m]
 * \param[out] y2 y coordinate of the second intersection point [m]
 * \param[out] x1_p x velocity component of the first intersection point [m/s]
 * \param[out] y1_p y velocity component of the first intersection point [m/s]
 * \param[out] x2_p x velocity component of the second intersection point [m/s]
 * \param[out] y2_p y velocity component of the second intersection point [m/s]
 * \return the number of intersections found
 */	
int get_circle_line_intersection(Segment const& a, double radius, Vector2D const& center, Vector2D const& center_p, double & x1, double & y1, double & x2, double & y2, double & x1_p, double & y1_p, double & x2_p, double & y2_p)
{
	double expr1, expr2, expr3;
	double expr1_p, expr2_p, expr3_p;
	double R_2;
	double x, y, delta_x;
	double x_p, y_p, delta_x_p;
	double A, B, C;
	double A_p, B_p, C_p;

	R_2 = radius * radius;

	if (a.get_flag_vert())
	{
		x   = a.get_offset();
		x_p = a.get_offset_p();

		delta_x = x - center.get_vec(0);

		expr1 = R_2 - delta_x*delta_x;

		if (expr1 <= 0.0)
		{
			x1 = 0.0;
			y1 = 0.0;
			x2 = 0.0;
			y2 = 0.0;

			x1_p = 0.0;
			y1_p = 0.0;
			x2_p = 0.0;
			y2_p = 0.0;

			return 0;
		}
		else
		{
			expr2 = sqrt(expr1);

			delta_x_p = x_p - center_p.get_vec(0);

			expr1_p = -2.0 * delta_x * delta_x_p;

			expr2_p = expr1_p / (2.0 * expr2);

			x1 = x;
			x2 = x;

			x1_p = x_p;
			x2_p = x_p;

			y   = center.get_vec(1);
			y_p = center_p.get_vec(1);

			y1 = y - expr2;
			y2 = y + expr2;

			y1_p = y_p - expr2_p;
			y2_p = y_p + expr2_p;

			return 2;
		}
	}

	expr1 = a.get_offset() - center.get_vec(1);
	
	A = 1.0 + a.get_slope() * a.get_slope();
	B = 2.0 * ( a.get_slope()*expr1 - center.get_vec(0) );
	C = center.get_vec(0)*center.get_vec(0) + expr1*expr1 - R_2;

	expr2 = B*B - 4.0*A*C;

	if (expr2 <= 0.0)
	{
		x1 = 0.0;
		y1 = 0.0;
		x2 = 0.0;
		y2 = 0.0;

		x1_p = 0.0;
		y1_p = 0.0;
		x2_p = 0.0;
		y2_p = 0.0;

		return 0;
	}

	expr3 = sqrt(expr2);

	expr1_p = a.get_offset_p() - center_p.get_vec(1);

	A_p = 2.0 * a.get_slope() * a.get_slope_p();
	B_p = 2.0 * ( a.get_slope_p()*expr1 + a.get_slope()*expr1_p - center_p.get_vec(0) );
	C_p = 2.0*center.get_vec(0)*center_p.get_vec(0) + 2.0*expr1*expr1_p;

	expr2_p = 2.0*B*B_p - 4.0*(A_p*C + A*C_p);

	expr3_p = expr2_p / (2.0 * expr3);

	x1 = (-B - expr3) / (2.0*A);
	x2 = (-B + expr3) / (2.0*A);

	y1 = a.get_slope() * x1 + a.get_offset();
	y2 = a.get_slope() * x2 + a.get_offset();

	x1_p = ( (-B_p - expr3_p)*A - (-B - expr3)*A_p ) / (2.0*A*A);
	x2_p = ( (-B_p + expr3_p)*A - (-B + expr3)*A_p ) / (2.0*A*A);

	y1_p = a.get_slope_p()*x1 + a.get_slope()*x1_p + a.get_offset_p();
	y2_p = a.get_slope_p()*x2 + a.get_slope()*x2_p + a.get_offset_p();

	return 2;
}

/*! \brief compute the intersection between a segment and a circle (without speed)
 * 
 * \param[in] a segment
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \param[out] x1 x coordinate of the first intersection point [m]
 * \param[out] y1 y coordinate of the first intersection point [m]
 * \param[out] x2 x coordinate of the second intersection point [m]
 * \param[out] y2 y coordinate of the second intersection point [m]
 * \return the number of intersections found
 */	
int get_circle_intersection(Segment const& a, double radius, Vector2D const& center, double & x1, double & y1, double & x2, double & y2)
{
	int count;

	if ( !get_circle_line_intersection(a, radius, center, x1, y1, x2, y2) )
	{
		return 0;
	}

	count = 0;

	if(    ( (a.get_min_x() <= x1) && (x1 <= a.get_max_x()) )
		&& ( (a.get_min_y() <= y1) && (y1 <= a.get_max_y()) ) )
	{
		count++;

		if(    ( (a.get_min_x() <= x2) && (x2 <= a.get_max_x()) )
			&& ( (a.get_min_y() <= y2) && (y2 <= a.get_max_y()) ) )
		{
			count++;
		}
		else
		{
			x2 = 0.0;
			y2 = 0.0;
		}
	}
	else
	{
		if(    ( (a.get_min_x() <= x2) && (x2 <= a.get_max_x()) )
			&& ( (a.get_min_y() <= y2) && (y2 <= a.get_max_y()) ) )
		{
			count++;

			x1 = x2;
			y1 = y2;

			x2 = 0.0;
			y2 = 0.0;
		}
		else
		{
			x1 = 0.0;
			y1 = 0.0;

			x2 = 0.0;
			y2 = 0.0;
		}
	}

	return count;
}

/*! \brief compute the intersection between a segment and a circle (with speed)
 * 
 * \param[in] a segment
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \param[in] center_p velocity of the circle center [m/s]
 * \param[out] x1 x coordinate of the first intersection point [m]
 * \param[out] y1 y coordinate of the first intersection point [m]
 * \param[out] x2 x coordinate of the second intersection point [m]
 * \param[out] y2 y coordinate of the second intersection point [m]
 * \param[out] x1_p x velocity component of the first intersection point [m/s]
 * \param[out] y1_p y velocity component of the first intersection point [m/s]
 * \param[out] x2_p x velocity component of the second intersection point [m/s]
 * \param[out] y2_p y velocity component of the second intersection point [m/s]
 * \return the number of intersections found
 */	
int get_circle_intersection(Segment const& a, double radius, Vector2D const& center, Vector2D const& center_p, double & x1, double & y1, double & x2, double & y2, double & x1_p, double & y1_p, double & x2_p, double & y2_p)
{
	int count;

	if ( !get_circle_line_intersection(a, radius, center, center_p, x1, y1, x2, y2, x1_p, y1_p, x2_p, y2_p) )
	{
		return 0;
	}

	count = 0;

	if(    ( (a.get_min_x() <= x1) && (x1 <= a.get_max_x()) )
		&& ( (a.get_min_y() <= y1) && (y1 <= a.get_max_y()) ) )
	{
		count++;

		if(    ( (a.get_min_x() <= x2) && (x2 <= a.get_max_x()) )
			&& ( (a.get_min_y() <= y2) && (y2 <= a.get_max_y()) ) )
		{
			count++;
		}
		else
		{
			x2 = 0.0;
			y2 = 0.0;

			x2_p = 0.0;
			y2_p = 0.0;
		}
	}
	else
	{
		if(    ( (a.get_min_x() <= x2) && (x2 <= a.get_max_x()) )
			&& ( (a.get_min_y() <= y2) && (y2 <= a.get_max_y()) ) )
		{
			count++;

			x1 = x2;
			y1 = y2;

			x1_p = x2_p;
			y1_p = y2_p;

			x2 = 0.0;
			y2 = 0.0;

			x2_p = 0.0;
			y2_p = 0.0;
		}
		else
		{
			x1 = 0.0;
			y1 = 0.0;

			x1_p = 0.0;
			y1_p = 0.0;

			x2 = 0.0;
			y2 = 0.0;

			x2_p = 0.0;
			y2_p = 0.0;
		}
	}

	return count;
}

/*! \brief compute the intersection between a list of segments and a circle
 * 
 * \param[in] seg_list list of segments
 * \param[in] list_size number of segments in 'seg_list'
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \param[in] center_p velocity of the circle center [m/s]
 * \param[out] X1 coordinates (x, y) of the first intersection point [m]
 * \param[out] X2 coordinates (x, y) of the second intersection point [m]
 * \param[out] X1_p velocity coordinates (x, y) of the first intersection point [m]
 * \param[out] X2_p velocity coordinates (x, y) of the second intersection point [m]
 * \return the number of intersections found
 */	
int get_circle_seg_list_two_points(Segment *seg_list, int list_size, double radius, Vector2D const& center, Vector2D const& center_p, Vector2D & X1, Vector2D & X2, Vector2D & X1_P, Vector2D & X2_P)
{
	int count;
	int cur_nb_int;
	double cur_x1, cur_y1, cur_x1_p, cur_y1_p;
	double cur_x2, cur_y2, cur_x2_p, cur_y2_p;

	Segment cur_seg;

	count = 0;

	X1.reset();
	X2.reset();

	X1_P.reset();
	X2_P.reset();


	for (int i=0; i<list_size; i++)
	{
		cur_seg = seg_list[i];	

		cur_nb_int = get_circle_intersection(cur_seg, radius, center, center_p, cur_x1, cur_y1, cur_x2, cur_y2, cur_x1_p, cur_y1_p, cur_x2_p, cur_y2_p);

		if (cur_nb_int)
		{
			if (!count)
			{
				X1.set_vector(cur_x1, cur_y1);
				X1_P.set_vector(cur_x1_p, cur_y1_p);
				count++;

				if (cur_nb_int > 1)
				{
					X2.set_vector(cur_x2, cur_y2);
					X2_P.set_vector(cur_x2_p, cur_y2_p);
					count++;
					
					return count;
				}
			}
			else if (count == 1)
			{
				X2.set_vector(cur_x1, cur_y1);
				X2_P.set_vector(cur_x1_p, cur_y1_p);
				count++;

				return count;
			}
		}
	}

	return count;
}

/*! \brief compute the intersection between a segment (supposed infinite) and a wall
 * 
 * \param[in] a segment
 * \param[in] wall_center center of the wall
 * \param[in] wall_or orientation of the wall
 * \param[out] x x coordinate of the intersection point [m]
 * \param[out] y y coordinate of the intersection point [m]
 * \param[out] x_p x velocity component of the intersection point [m/s]
 * \param[out] y_p y velocity component of the intersection point [m/s]
 * \return the number of intersections found
 */	
int get_wall_line_intersection(Segment const& a, Vector2D const& wall_center, Orientation const& wall_or, double & x, double & y, double & x_p, double & y_p)
{
	double wall_alpha, wall_slope, wall_offset;
	double diff_slope, diff_offset, diff_slope_p, diff_offset_p;

	wall_alpha = wall_or.get_alpha();

	if ( (!wall_alpha) || (wall_alpha == M_PI) ) // vertical wall
	{
		if (a.get_flag_vert())
		{
			x = 0.0;
			y = 0.0;

			x_p = 0.0;
			y_p = 0.0;

			return 0;
		}

		x = wall_center.get_vec(0);
		y = a.get_slope() * x + a.get_offset();

		x_p = 0.0;
		y_p = a.get_slope_p() * x + a.get_slope() * x_p + a.get_offset_p();

		return 1;
	}

	wall_slope  = tan((M_PI/2.0) - wall_alpha);

	wall_offset = wall_center.get_vec(1) - wall_slope * wall_center.get_vec(0);

	if (a.get_slope() == wall_slope)
	{
		x = 0.0;
		y = 0.0;
		
		x_p = 0.0;
		y_p = 0.0;

		return 0;
	}

	if (a.get_flag_vert())
	{
		x = a.get_offset();
		y = wall_slope * x + wall_offset;

		x_p = a.get_offset_p();
		y_p = wall_slope * x_p;

		return 1;
	}

	diff_slope  = wall_slope - a.get_slope();
	diff_offset = a.get_offset() - wall_offset;

	diff_slope_p  = -a.get_slope_p();
	diff_offset_p =  a.get_offset_p();

	x = diff_offset / diff_slope;

	y = a.get_slope() * x + a.get_offset();

	x_p = (diff_offset_p*diff_slope - diff_offset*diff_slope_p) / (diff_slope*diff_slope);

	y_p = a.get_slope_p() * x + a.get_slope() * x_p + a.get_offset_p();

	return 1;
}

/*! \brief compute the intersection between a segment and a wall
 * 
 * \param[in] a segment
 * \param[in] wall_center center of the wall
 * \param[in] wall_or orientation of the wall
 * \param[out] x x coordinate of the intersection point [m]
 * \param[out] y y coordinate of the intersection point [m]
 * \param[out] x_p x velocity component of the intersection point [m/s]
 * \param[out] y_p y velocity component of the intersection point [m/s]
 * \return the number of intersections found
 */	
int get_wall_intersection(Segment const& a, Vector2D const& wall_center, Orientation const& wall_or, double & x, double & y, double & x_p, double & y_p)
{
	if ( !get_wall_line_intersection(a, wall_center, wall_or , x, y, x_p, y_p) )
	{
		return 0;
	}

	if(    ( (a.get_min_x() <= x) && (x <= a.get_max_x()) )
		&& ( (a.get_min_y() <= y) && (y <= a.get_max_y()) ) )
	{
		return 1;
	}
	else
	{
		x = 0.0;
		y = 0.0;

		x_p = 0.0;
		y_p = 0.0;

		return 0;
	}
}

/*! \brief compute the intersection between a list of segments and a wall (max 2 intersections)
 * 
 * \param[in] seg_list list of segments
 * \param[in] list_size number of segments in 'seg_list'
 * \param[in] wall_center center of the wall
 * \param[in] wall_or orientation of the wall
 * \param[out] X1 x coordinates (x, y) of the first intersection point [m]
 * \param[out] X2 x coordinates (x, y) of the second intersection point [m]
 * \param[out] X1_p x velocity coordinates (x, y) of the first intersection point [m]
 * \param[out] X2_p x velocity coordinates (x, y) of the second intersection point [m]
 * \return the number of intersections found
 */	
int get_wall_seg_list_two_points(Segment *seg_list, int list_size, Vector2D const& wall_center, Orientation const& wall_or, Vector2D & X1, Vector2D & X2, Vector2D & X1_P, Vector2D & X2_P)
{
	int count;
	int cur_nb_int;
	double cur_x, cur_y, cur_x_p, cur_y_p;

	Segment cur_seg;

	count = 0;

	X1.reset();
	X2.reset();

	X1_P.reset();
	X2_P.reset();

	for (int i=0; i<list_size; i++)
	{
		cur_seg = seg_list[i];

		cur_nb_int = get_wall_intersection(cur_seg, wall_center, wall_or, cur_x, cur_y, cur_x_p, cur_y_p);

		if (cur_nb_int)
		{
			if (!count)
			{
				X1.set_vector(cur_x, cur_y);
				X1_P.set_vector(cur_x_p, cur_y_p);
				count++;
			}
			else if (count == 1)
			{
				X2.set_vector(cur_x, cur_y);
				X2_P.set_vector(cur_x_p, cur_y_p);
				count++;

				return count;
			}
		}
	}

	return count;
}

/*! \brief check if there is an intersection between two segments
 * 
 * \param[in] a first segment
 * \param[in] b second segment
 * \return 1 if intersection, 0 otherwise
 */
int is_intersection(Segment const& a, Segment const& b)
{
	double x, y;
	double diff_slope, diff_offset;

	// same slope
	if ( (a.get_flag_vert() && b.get_flag_vert()) || (a.get_slope() == b.get_slope()) )
	{
		return 0;
	}

	if (a.get_flag_vert())
	{
		x = a.get_offset();
		y = b.get_slope() * x + b.get_offset();
	}
	else if (b.get_flag_vert())
	{
		x = b.get_offset();
		y = a.get_slope() * x + a.get_offset();
	}
	else
	{
		diff_slope  = b.get_slope()  - a.get_slope();
		diff_offset = a.get_offset() - b.get_offset();

		x = diff_offset / diff_slope;
		y = a.get_slope() * x + a.get_offset();
	}

	if(    ( (a.get_min_x() <= x) && (x <= a.get_max_x()) )
		&& ( (b.get_min_x() <= x) && (x <= b.get_max_x()) )
		&& ( (a.get_min_y() <= y) && (y <= a.get_max_y()) )
		&& ( (b.get_min_y() <= y) && (y <= b.get_max_y()) ) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*! \brief check if there is an intersection between a segment and a list of segments
 * 
 * \param[in] a single segment
 * \param[in] seg_list list of segments
 * \param[in] list_size size of the list of segments
 * \return 1 if intersection, 0 otherwise
 */
int is_intersection(Segment const& a, Segment *seg_list, int list_size)
{
	for (int i=0; i<list_size; i++)
	{
		if ( is_intersection(a, seg_list[i]) )
		{
			return 1;
		}	
	}

	return 0;
}

/*! \brief check if there is an intersection between an infinite segment and a circle
 * 
 * \param[in] a segment
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \return 1 if intersection, 0 otherwise
 */	
int is_line_intersection(Segment const& a, double radius, Vector2D const& center)
{
	double expr1, expr2;
	double R_2;
	double x, y, delta_x;
	double A, B, C;

	R_2 = radius * radius;

	if (a.get_flag_vert())
	{
		x = a.get_offset();

		delta_x = x - center.get_vec(0);

		expr1 = R_2 - delta_x*delta_x;

		if (expr1 <= 0.0)
		{
			return 0;
		}
	}
	else
	{
		expr1 = a.get_offset() - center.get_vec(1);
		
		A = 1.0 + a.get_slope() * a.get_slope();
		B = 2.0 * ( a.get_slope()*expr1 - center.get_vec(0) );
		C = center.get_vec(0)*center.get_vec(0) + expr1*expr1 - R_2;

		expr2 = B*B - 4.0*A*C;

		if (expr2 <= 0.0)
		{
			return 0;
		}
	}

	return 1;
}

/*! \brief check if there is an intersection between a segment and a circle
 * 
 * \param[in] a segment
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \return 1 if intersection, 0 otherwise
 */	
int is_intersection(Segment const& a, double radius, Vector2D const& center)
{
	double expr1, expr2, expr3;
	double R_2;
	double x, y, delta_x;
	double A, B, C;
	double x1, x2, y1, y2;

	R_2 = radius * radius;

	if (a.get_flag_vert())
	{
		x = a.get_offset();

		delta_x = x - center.get_vec(0);

		expr1 = R_2 - delta_x*delta_x;

		if (expr1 <= 0.0)
		{
			return 0;
		}
		else
		{
			expr2 = sqrt(expr1);

			x1 = x;
			x2 = x;

			y   = center.get_vec(1);

			y1 = y - expr2;
			y2 = y + expr2;
		}
	}
	else
	{
		expr1 = a.get_offset() - center.get_vec(1);
		
		A = 1.0 + a.get_slope() * a.get_slope();
		B = 2.0 * ( a.get_slope()*expr1 - center.get_vec(0) );
		C = center.get_vec(0)*center.get_vec(0) + expr1*expr1 - R_2;

		expr2 = B*B - 4.0*A*C;

		if (expr2 <= 0.0)
		{
			return 0;
		}

		expr3 = sqrt(expr2);

		x1 = (-B - expr3) / (2.0*A);
		x2 = (-B + expr3) / (2.0*A);

		y1 = a.get_slope() * x1 + a.get_offset();
		y2 = a.get_slope() * x2 + a.get_offset();
	}

	if(    ( (a.get_min_x() <= x1) && (x1 <= a.get_max_x()) )
		&& ( (a.get_min_y() <= y1) && (y1 <= a.get_max_y()) ) )
	{
		return 1;
	}
	else
	{
		if(    ( (a.get_min_x() <= x2) && (x2 <= a.get_max_x()) )
			&& ( (a.get_min_y() <= y2) && (y2 <= a.get_max_y()) ) )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

/*! \brief check if there is an intersection between a list of segments and a circle
 * 
 * \param[in] seg_list list of segments
 * \param[in] list_size number of segments in 'seg_list'
 * \param[in] radius radius of the circle [m]
 * \param[in] center center of the circle
 * \return 1 if intersection, 0 otherwise
 */	
int is_intersection(Segment *seg_list, int list_size, double radius, Vector2D const& center)
{
	for (int i=0; i<list_size; i++)
	{
		if (is_intersection(seg_list[i], radius, center))
		{
			return 1;
		}	
	}

	return 0;
}

/*! \brief compute the intersection between two segments (supposed infinite)
 * 
 * \param[in] a first segment
 * \param[in] b second segment
 * \param[out] x x coordinate of the intersection point [m]
 * \param[out] y y coordinate of the intersection point [m]
 * \param[out] x_p x velocity component of the intersection point [m/s]
 * \param[out] y_p y velocity component of the intersection point [m/s]
 * \return the number of intersections found
 */	
int get_line_intersection(Segment const& a, Segment const& b, double & x, double & y, double & x_p, double & y_p)
{
	double diff_slope, diff_offset, diff_slope_p, diff_offset_p;

	if ( (a.get_flag_vert() && b.get_flag_vert()) || (a.get_slope() == b.get_slope()) )
	{
		x = 0.0;
		y = 0.0;

		x_p = 0.0;
		y_p = 0.0;

		return 0;
	}

	if (a.get_flag_vert())
	{
		x = a.get_offset();
		y = b.get_slope() * x + b.get_offset();

		x_p = a.get_offset_p();
		y_p = b.get_slope_p()*x + b.get_slope()*x_p + b.get_offset_p();

		return 1;
	}

	if (b.get_flag_vert())
	{
		x = b.get_offset();
		y = a.get_slope() * x + a.get_offset();

		x_p = b.get_offset_p();
		y_p = a.get_slope_p()*x + a.get_slope()*x_p + a.get_offset_p();

		return 1;
	}

	diff_slope  = b.get_slope()  - a.get_slope();
	diff_offset = a.get_offset() - b.get_offset();

	diff_slope_p  = b.get_slope_p()  - a.get_slope_p();
	diff_offset_p = a.get_offset_p() - b.get_offset_p();

	x = diff_offset / diff_slope;

	y = a.get_slope() * x + a.get_offset();

	x_p = (diff_offset_p*diff_slope - diff_offset*diff_slope_p) / (diff_slope*diff_slope);

	y_p = a.get_slope_p()*x + a.get_slope()*x_p + a.get_offset_p();

	return 1;
}

/*! \brief compute the intersection between two segments
 * 
 * \param[in] a first segment
 * \param[in] b second segment
 * \param[out] x x coordinate of the intersection point [m]
 * \param[out] y y coordinate of the intersection point [m]
 * \param[out] x_p x velocity component of the intersection point [m/s]
 * \param[out] y_p y velocity component of the intersection point [m/s]
 * \return the number of intersections found
 */	
int get_intersection(Segment const& a, Segment const& b, double & x, double & y, double & x_p, double & y_p)
{
	if ( !get_line_intersection(a, b, x, y, x_p, y_p) )
	{
		return 0;
	}

	if(    ( (a.get_min_x()-EPSILON <= x) && (x <= a.get_max_x()+EPSILON) )
		&& ( (b.get_min_x()-EPSILON <= x) && (x <= b.get_max_x()+EPSILON) )
		&& ( (a.get_min_y()-EPSILON <= y) && (y <= a.get_max_y()+EPSILON) )
		&& ( (b.get_min_y()-EPSILON <= y) && (y <= b.get_max_y()+EPSILON) ) )
	{
		return 1;
	}
	else
	{
		x = 0.0;
		y = 0.0;

		x_p = 0.0;
		y_p = 0.0;

		return 0;
	}
}

/*! \brief compute the intersection between two list of segments (max 2 intersections)
 * 
 * \param[in] seg_list_1 list of segments (first list)
 * \param[in] list_size_1 number of segments in 'seg_list_1'
 * \param[in] seg_list_2 list of segments (second list)
 * \param[in] list_size_2 number of segments in 'seg_list_2'
 * \param[out] X1 x coordinates (x, y) of the first intersection point [m]
 * \param[out] X2 x coordinates (x, y) of the second intersection point [m]
 * \param[out] X1_p x velocity coordinates (x, y) of the first intersection point [m]
 * \param[out] X2_p x velocity coordinates (x, y) of the second intersection point [m]
 * \return the number of intersections found
 */	
int get_two_seg_list_two_points(Segment *seg_list_1, int list_size_1, Segment *seg_list_2, int list_size_2, Vector2D & X1, Vector2D & X2, Vector2D & X1_P, Vector2D & X2_P)
{
	int count;
	int cur_nb_int;
	double cur_x, cur_y, cur_x_p, cur_y_p;

	Segment cur_seg_1, cur_seg_2;

	count = 0;

	X1.reset();
	X2.reset();

	X1_P.reset();
	X2_P.reset();

	for (int i=0; i<list_size_1; i++)
	{
		cur_seg_1 = seg_list_1[i];

		for(int j=0; j<list_size_2; j++)
		{
			cur_seg_2 = seg_list_2[j];

			cur_nb_int = get_intersection(cur_seg_1, cur_seg_2, cur_x, cur_y, cur_x_p, cur_y_p);

			if (cur_nb_int)
			{
				if (!count)
				{
					X1.set_vector(cur_x, cur_y);
					X1_P.set_vector(cur_x_p, cur_y_p);
					count++;
				}
				else if (count == 1)
				{
					X2.set_vector(cur_x, cur_y);
					X2_P.set_vector(cur_x_p, cur_y_p);
					count++;

					return count;
				}
			}
		}
	}

	return count;
}
