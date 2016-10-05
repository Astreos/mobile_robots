#include "simu_functions.hh"
#include <cmath>

#define DOUBLE_PI (2.0*M_PI)

/*! \brief limit a value to be bounded in a given range
 * 
 * \param[in] value value to bound in the range
 * \param[in] min_val minimal bound of the range
 * \param[in] max_val maximal bound of the range
 * \return bounded value
 */
double limit_range(double value, double min_val, double max_val)
{
	if (value < min_val)
	{
		return min_val;
	}
	else if (value > max_val)
	{
		return max_val;
	}
	else
	{
		return value;
	}
}

/*! \brief check if a value belongs to a range
 * 
 * \param[in] value value to check in the range
 * \param[in] min_val minimal bound of the range
 * \param[in] max_val maximal bound of the range
 * \return 1 if value in range, 0 otherwise
 */
int is_in_range(double value, double min_val, double max_val)
{
	return ( (min_val <= value) && (value <= max_val) );
}

/*! \brief limit the angle in the range ]-PI ; PI]
 * 
 * \param[in] value angle to bound
 * \return bounded angle
 */
double limit_angle(double value)
{
	while (value > M_PI)
	{
		value -= DOUBLE_PI;
	}
	while (value <= -M_PI)
	{
		value += DOUBLE_PI;
	}

	return value;
}

/*! \brief return the interpolation between two points
 * 
 * \attention if x is outside the bounds, it equals the closest bound
 * \pre x2 > x1
 * \param[in] x requested abscissa
 * \param[in] x1 first abscissa
 * \param[in] x2 last abscissa
 * \param[in] y1 first ordinate
 * \param[in] y2 last ordinate
 * \return the requested interpolation
 */
double linear_interp(double x, double x1, double x2, double y1, double y2)
{
	double delta_x, delta_y;

	if (x <= x1)
	{
		return y1;
	}
	else if (x >= x2)
	{
		return y2;
	}
	else
	{
		delta_x = x2 - x1;
		delta_y = y2 - y1;

		if (delta_x > 0.0)
		{
			return ( (x - x1) / delta_x ) * delta_y + y1;
		}
		else
		{
			return y1;
		}
	}
}

/*! \brief return the interpolation between two 3D points
 * 
 * \attention if x is outside the bounds, it equals the closest bound
 * \pre x2 > x1
 * \param[in] x requested abscissa
 * \param[in] x1 first abscissa
 * \param[in] x2 last abscissa
 * \param[in] y1 first ordinate (3D point)
 * \param[in] y2 last ordinate (3D point)
 * \return the requested interpolation (3D point)
 */
Vector3D linear_interp_vec_3D(double x, double x1, double x2, Vector3D Y1, Vector3D Y2)
{
	Vector3D result;

	for (int i=0; i<3; i++)
	{
		result.set_index_vector(i, linear_interp(x, x1, x2, Y1.get_vec(i), Y2.get_vec(i)));	
	}

	return result;
}
