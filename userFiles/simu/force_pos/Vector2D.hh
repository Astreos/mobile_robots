/*! 
 * \author Nicolas Van der Noot
 * \file Vector2D.hh
 * \brief Vector2D class
 */

#ifndef __VECTOR_2_D__HH__
#define __VECTOR_2_D__HH__

#include <cmath>
#include <iostream>

#include "Orientation.hh"
#include "simu_functions.hh"
#include "Vector.hh"

/*! \brief Description of a 2D vector
 */
class Vector2D: public Vector
{
	public:
		Vector2D(double x, double y);
		Vector2D(double *X);
		Vector2D();

		virtual ~Vector2D();

		/*! \brief get a vector element
		 * 
		 * \param[in] index of the requested component
		 * \return distance between the two points [m]
		 */
		double get_vec(int index) const { return vector[index]; }

		/*! \brief return first vector element
		 * 
		 * \return x component
		 */
		double get_x() const { return vector[0]; }

		/*! \brief return second vector element
		 * 
		 * \return y component
		 */
		double get_y() const { return vector[1]; }

		/// get the distance of the vector
		double get_dist() { return sqrt(vector[0]*vector[0] + vector[1]*vector[1]); }

		/// get norm of the square
		double get_square_norm() { return vector[0]*vector[0] + vector[1]*vector[1]; }

		/*! \brief get the distance between this point and another one
		 * 
		 * \param[in] pos2D position of the other point
		 * \return distance between the two points [m]
		 */
		double get_dist_point(Vector2D const& pos2D)
		{
			double delta_x, delta_y;

			delta_x = vector[0] - pos2D.get_vec(0);
			delta_y = vector[1] - pos2D.get_vec(1);

			return sqrt(delta_x*delta_x + delta_y*delta_y);
		}

		/*! \brief set a component of the vector
		 * 
		 * \param[in] index index of the component 
		 * \param[in] value new value to set
		 *
		 * comments
		 */
		void set_index_vector(int index, double value)
		{
			vector[index] = value;
		}

		/// set all the vector components to 0
		void reset()
		{
			for (int i=0; i<2; i++)
			{
				vector[i] = 0.0;
			}
		}

		/*! \brief set new values for the vector
		 * 
		 * \param[in] pos2D new vector to copy
		 */
		void set_vector(Vector2D const& pos2D)
		{
			for (int i=0; i<2; i++)
			{
				vector[i] = pos2D.get_vec(i);
			}
		}

		/*! \brief set new values for the vector
		 * 
		 * \param[in] x x component of the vector
		 * \param[in] y y component of the vector
		 */
		void set_vector(double x, double y)
		{
			vector[0] = x;
			vector[1] = y;
		}

		/*! \brief set new values for the vector
		 * 
		 * \pre X must be a tabular of two elements
		 * \param[in] X tabular to copy
		 */
		void set_vector(double *X)
		{
			for (int i=0; i<2; i++)
			{
				vector[i] = X[i];
			}
		}

		/*! \brief set new values computed from an origin point
		 * 
		 * \param[in] init_point origin point position
		 * \param[in] dist distance from the origin to the new point [m]
		 * \param[in] theta angle describing the orientation between the two points [rad]
		 */
		void set_vector_dist_point(Vector2D const& init_point, double dist, double theta)
		{
			vector[0] = init_point.get_vec(0) + dist * cos(theta);
			vector[1] = init_point.get_vec(1) + dist * sin(theta);
		}

		/*! \brief increment the vector
		 * 
		 * \param[in] pos2D position to increment [m]
		 */
		void inc_vector(Vector2D const& pos2D)
		{
			for (int i=0; i<2; i++)
			{
				vector[i] += pos2D.get_vec(i);
			}
		}

		/*! \brief increment the vector
		 * 
		 * \param[in] x x component of the position to increment [m]
		 * \param[in] y y component of the position to increment [m]
		 */
		void inc_vector(double x, double y)
		{
			vector[0] += x;
			vector[1] += y;
		}

		/*! \brief set function with a =
		 * 
		 * \param[in] pos2D new position
		 */
		void operator=(Vector2D const& pos2D)
		{
			for (int i=0; i<2; i++)
			{
				vector[i] = pos2D.get_vec(i);
			}
		}

	private:
		double vector[2]; ///< two components of the vector
};

/*! \brief sum two vectors with the + opration
 * 
 * \param[in] a first vector to sum
 * \param[in] b second vector to sum 
 * \return sum of the two vectors
 */
inline Vector2D operator+(Vector2D const& a, Vector2D const& b)
{
	Vector2D result;

	for (int i=0; i<2; i++)
	{
		result.set_index_vector(i, a.get_vec(i) + b.get_vec(i));
	}

	return result;
}

/*! \brief difference of two vectors with the - opration
 * 
 * \param[in] a first vector
 * \param[in] b second vector 
 * \return difference of the two vectors
 */
inline Vector2D operator-(Vector2D const& a, Vector2D const& b)
{
	Vector2D result;

	for (int i=0; i<2; i++)
	{
		result.set_index_vector(i, a.get_vec(i) - b.get_vec(i));
	}

	return result;
}

/*! \brief return a vector with only zero values
 * 
 * \return vector
 */
inline Vector2D get_nul_vector()
{
	Vector2D result(0.0, 0.0);

	return result;
}

/*! \brief return a vector
 * 
 * \param[in] x x component
 * \param[in] y y component
 * \return vector with x and y components
 */
inline Vector2D get_vector(double x, double y)
{
	Vector2D result(x, y);

	return result;
}

/*! \brief get a position computed from an origin
 * 
 * \param[in] init_point origin point
 * \param[in] dist distance from origin to requested point [m]
 * \param[in] orientation between requested point and origin [rad] 
 * \return requested position
 */
inline Vector2D get_pos_dist_angle(Vector2D const& init_point, double dist, double theta)
{
	Vector2D result;

	result.set_index_vector( 0, init_point.get_vec(0) + dist * cos(theta));
	result.set_index_vector( 1, init_point.get_vec(1) + dist * sin(theta));

	return result;
}

/*! \brief return a vector with a requested orientation
 * 
 * \param[in] dist distance of the vector [m]
 * \param[in] theta of the vector [rad] 
 * \return requested vector
 */
inline Vector2D get_vec_dist_angle(double dist, double theta)
{
	Vector2D result;

	result.set_index_vector( 0, dist * cos(theta));
	result.set_index_vector( 1, dist * sin(theta));

	return result;
}

/*! \brief return a rotated vector
 * 
 * \param[in] init_vec initial vector
 * \param[in] angle orientation requested [rad]
 * \return requested vector
 */
inline Vector2D get_rotated_vector(Vector2D const& init_vec, double angle)
{
	Vector2D result;

	Orientation orient(angle);

	result.set_index_vector( 0, init_vec.get_vec(0) * orient.get_c_alpha() - init_vec.get_vec(1) * orient.get_s_alpha() );
	result.set_index_vector( 1, init_vec.get_vec(0) * orient.get_s_alpha() + init_vec.get_vec(1) * orient.get_c_alpha() );

	return result;
}

/*! \brief return a rotated vector
 * 
 * \param[in] init_vec initial vector
 * \param[in] c_angle cosine of the orientation
 * \param[in] s_angle sine of the orientation
 * \return requested vector
 */
inline Vector2D get_rotated_vector(Vector2D const& init_vec, double c_angle, double s_angle)
{
	Vector2D result;

	result.set_index_vector( 0, init_vec.get_vec(0) * c_angle - init_vec.get_vec(1) * s_angle );
	result.set_index_vector( 1, init_vec.get_vec(0) * s_angle + init_vec.get_vec(1) * c_angle );

	return result;
}

/*! \brief return the opposite vector
 * 
 * \param[in] init_vec initial vector
 * \return requested vector
 */
inline Vector2D get_opp_vector(Vector2D const& init_vec)
{
	Vector2D result;

	result.set_index_vector( 0, -init_vec.get_vec(0));
	result.set_index_vector( 1, -init_vec.get_vec(1));

	return result;
}

/*! \brief performs 'omega I3 x (a I1 + b I2)' (cross product)
 * 
 * \param[in] init_vec initial vector (a I1 + b I2)
 * \param[in] omega omega value (omega I3)
 * \return requested vector
 */
inline Vector2D get_omega_vector(Vector2D const& init_vec, double omega)
{
	Vector2D result;

	result.set_index_vector( 0, -omega * init_vec.get_vec(1) );
	result.set_index_vector( 1,  omega * init_vec.get_vec(0) );

	return result;
}

/*! \brief multiply a vector by a constant
 * 
 * \param[in] init_vec initial vector
 * \param[in] factor multiplication factor
 * \return requested vector
 */
inline Vector2D mult_vector(Vector2D const& init_vec, double factor)
{
	Vector2D result;

	result.set_index_vector( 0, factor * init_vec.get_vec(0) );
	result.set_index_vector( 1, factor * init_vec.get_vec(1) );

	return result;
}

/*! \brief divide a vector by a constant
 * 
 * \param[in] init_vec initial vector
 * \param[in] factor division factor
 * \return requested vector
 */
inline Vector2D division_vector(Vector2D const& init_vec, double factor)
{
	Vector2D result;

	result.set_index_vector( 0, init_vec.get_vec(0) / factor );
	result.set_index_vector( 1, init_vec.get_vec(1) / factor );

	return result;
}

/*! \brief get a vector with a requested amplitude and orientation
 * 
 * \param[in] amplitude amplitude of the vector
 * \param[in] angle orientation (angle) of the vector
 * \return requested vector
 */
inline Vector2D get_vec_amp_angle(double amplitude, double angle)
{
	Vector2D result;

	result.set_index_vector( 0, amplitude * cos(angle) );
	result.set_index_vector( 1, amplitude * sin(angle) );

	return result;
}

/*! \brief get a vector with a requested amplitude and orientation
 * 
 * \param[in] amplitude amplitude of the vector
 * \param[in] orient orientation of the vector
 * \return requested vector
 */
inline Vector2D get_vec_amp_angle(double amplitude, Orientation orient)
{
	Vector2D result;

	result.set_index_vector( 0, amplitude * orient.get_c_alpha() );
	result.set_index_vector( 1, amplitude * orient.get_s_alpha() );

	return result;
}

/*! \brief get a vector with a requested amplitude and orientation
 * 
 * \param[in] amplitude amplitude of the vector
 * \param[in] c_angle cosine of the orientation
 * \param[in] s_angle sine of the orientation
 * \return requested vector
 */
inline Vector2D get_vec_amp_angle(double amplitude, double c_angle, double s_angle)
{
	Vector2D result;

	result.set_index_vector( 0, amplitude * c_angle );
	result.set_index_vector( 1, amplitude * s_angle );

	return result;
}

/*! \brief get the mid point of two points
 * 
 * \param[in] a first point
 * \param[in] b second point
 * \return requested point
 */
inline Vector2D get_mid_vector(Vector2D const& a, Vector2D const& b)
{
	Vector2D result;

	result.set_index_vector( 0, 0.5 * (a.get_vec(0) + b.get_vec(0)) );
	result.set_index_vector( 1, 0.5 * (a.get_vec(1) + b.get_vec(1)) );

	return result;
}

/*! \brief cross product: i I3 x (a I1 + b I2) = i*a I2 - i*b I1
 * 
 * \param[in] comp_I3 i component
 * \param[in] vec vector (a I1 + b I2)
 * \return requested vector
 */
inline Vector2D cross_product(double comp_I3, Vector2D const& vec)
{
	Vector2D result;

	result.set_index_vector( 0, -comp_I3 * vec.get_vec(1) );
	result.set_index_vector( 1,  comp_I3 * vec.get_vec(0) );

	return result;
}

/*! \brief get the orientation between two points
 * 
 * \param[in] a first point
 * \param[in] b second point
 * \return requested orientation
 */
inline Orientation get_orientation(Vector2D const& a, Vector2D const& b)
{
	Orientation result;

	Vector2D diff_vec = b - a;

	result.set_orientation(atan2(diff_vec.get_vec(1), diff_vec.get_vec(0)));

	return result;
}

/*! \brief get the perpendicular orientation between two vectors
 * 
 * \param[in] a first point
 * \param[in] b second point
 * \return requested orientation
 */
inline Orientation get_perp_orientation(Vector2D const& a, Vector2D const& b)
{
	Orientation result;

	Vector2D diff_vec = b - a;

	result.set_orientation(atan2(-diff_vec.get_vec(0), diff_vec.get_vec(1)));

	return result;
}

/*! \brief return 1 if the supposed orientation is in an interval [-pi ; pi] of points difference orientation
 * 
 * \param[in] start_point first point
 * \param[in] end_point second point
 * \param[in] alpha supposed orientation
 * \return 1 if correct direction
 */
inline int is_correct_direction(Vector2D const& start_point, Vector2D const& end_point, double alpha)
{
	double alpha_2, diff_alpha;

	Vector2D diff_vec = end_point -start_point;

	alpha_2 = atan2(diff_vec.get_vec(1), diff_vec.get_vec(0));

	diff_alpha = limit_angle(alpha_2 - alpha);

	return (fabs(diff_alpha) <= (M_PI/2.0));
}

/*! \brief compute the cross product of two vectors
 * 
 * \param[in] a first vector
 * \param[in] b second vector
 * \return cross product
 */
inline double cross_product(Vector2D const& a, Vector2D const& b)
{
	return a.get_vec(0) * b.get_vec(1) - a.get_vec(1) * b.get_vec(0);
}

/*! \brief compute a torque from a force applied at a point
 * 
 * \param[in] force force vector
 * \param[in] applic_point force application point
 * \param[in] tor_point point where the torque is computed
 * \return cross product
 */
inline double torque_from_force(Vector2D const& force, Vector2D const& applic_point, Vector2D const& tor_point)
{
	return cross_product(applic_point - tor_point, force);
}

/*! \brief compute the distance of a vector
 * 
 * \param[in] pos2D vector
 * \return distance of pos2D
 */
inline double get_dist_point2D(Vector2D & pos2D)
{
	return pos2D.get_dist();
}

/*! \brief compute the distance between two points
 * 
 * \param[in] a first point
 * \param[in] b second point
 * \return distance between the two points
 */
inline double get_dist_points2D(Vector2D const& a, Vector2D const& b)
{
	Vector2D vec = a - b;

	return vec.get_dist();
}

/*! \brief compute the derivative of the distance between two points
 * 
 * \param[in] a first point
 * \param[in] a_p first point derivative
 * \param[in] b second point
 * \param[in] b_p second point derivative
 * \return derivative of the distance between the two points
 */
inline double get_dist_points2D_der(Vector2D const& a, Vector2D const& a_p, Vector2D const& b, Vector2D const& b_p)
{
	double delta_x, delta_y;
	double delta_x_p, delta_y_p;
	double dist;

	delta_x = b.get_vec(0) - a.get_vec(0);
	delta_y = b.get_vec(1) - a.get_vec(1);

	delta_x_p = b_p.get_vec(0) - a_p.get_vec(0);
	delta_y_p = b_p.get_vec(1) - a_p.get_vec(1);

	dist = sqrt(delta_x*delta_x + delta_y*delta_y);

	if (!dist)
	{
		return 0.0;
	}

	return (delta_x*delta_x_p + delta_y*delta_y_p) / dist;
}

/*! \brief get the orientation angle between two points
 * 
 * \param[in] a first point
 * \param[in] b second point
 * \return requested orientation
 */
inline double get_angle(Vector2D const& a, Vector2D const& b)
{
	Vector2D diff_vec = b - a;

	return atan2(diff_vec.get_vec(1), diff_vec.get_vec(0));
}

/*! \brief get the interpolation between to 2D
 * 
 * \param[in] a first 2D point
 * \param[in] b second 2D point
 * \param[in] ratio 0.0 for a, 1.0 for b, a value in [0.0 , 1.0] for a position between these tow bounds
 * \return requested interpolation
 */
inline Vector2D get_interpolation_vec_2D(Vector2D const& a, Vector2D const& b, double ratio)
{
	double x, y, ratio_opp;

	if (ratio < 0.0)
	{
		ratio = 0.0;
	}
	else if (ratio > 1.0)
	{
		ratio = 1.0;
	}

	ratio_opp = 1.0 - ratio;

	x = ratio_opp * a.get_x() + ratio * b.get_x();
	y = ratio_opp * a.get_y() + ratio * b.get_y();

	return get_vector(x, y);
}

#endif
