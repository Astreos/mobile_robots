#include "useful_gr18.h"
#include <math.h>
#include <stdlib.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief generate random number in [0,1]
 * 
 * \return random number in the [0,1] range
 */
double rnd()
{
	return ((double)rand())/((double)RAND_MAX);
}

/*! \brief get the normal distance, given two vector components
 * 
 * \param[in] dx x vector component
 * \param[in] dy y vector component
 * \return normal distance
 */
double norm_dist(double dx, double dy)
{
	return sqrt(dx * dx + dy * dy);
}

/*! \brief limit a function in a given range
 * 
 * \param[in] x value to limit
 * \param[in] min minimal bound of the range
 * \param[in] max maximal bound of the range
 * \return value limited in the given range
 */
double limit_range(double x, double min, double max)
{
	return (x < min) ? min : (x > max) ? max : x;
}

/*! \brief set an angle in the in ]-pi;pi] range
 * 
 * \param[in] x angle to limit
 * \return angle limited in ]-pi;pi]
 */
double limit_angle(double x)
{
	while (x <= -M_PI)
	{
		x += 2.0*M_PI;
	}
	while (x > M_PI)
	{
		x -= 2.0*M_PI;
	}

	return x;
}

/*! \brief first-order low-pass filter
 * 
 * \param[in] last_val last value
 * \param[in] new_val new value
 * \param[in] tau time constant [s]
 * \param[in] delta_t time increment since last call [s]
 * \return output of the low-pass filter
 */
double first_order_filter(double last_val, double new_val, double tau, double delta_t)
{
	double f = delta_t / tau;
	double frac = 1.0 / (1.0 + f);

	return f * frac * new_val + frac * last_val;
}

// Return 1 if TEAM_A and -1 if TEAM_B
int team(int team_id)
{
    if (team_id == 0)
    {
        team_id = 1;
    }
    else
    {
        team_id = -1;
    }
    
    return team_id;
}

NAMESPACE_CLOSE();
