
#include "Orientation.hh"
#include "simu_functions.hh"

#include <cmath>

/// constructor
Orientation::Orientation()
{
	alpha   = 0.0;
	c_alpha = 1.0;
	s_alpha = 0.0;
}

/// constructor
Orientation::Orientation(double alpha)
{
	set_orientation(alpha);
}

/// destructor
Orientation::~Orientation()
{

}

/*! \brief Set a new orientation
 * 
 * \param[in] new_alpha angle [rad]
 */
void Orientation::set_orientation(double new_alpha)
{
	alpha   = new_alpha;
	c_alpha = cos(new_alpha);
	s_alpha = sin(new_alpha);
}

/*! \brief Increment the orientation
 * 
 * \param[in] inc_alpha angle to increment [rad] 
 */
void Orientation::inc_orientation(double inc_alpha)
{
	double new_alpha = limit_angle(alpha + inc_alpha);

	set_orientation(new_alpha);
}
