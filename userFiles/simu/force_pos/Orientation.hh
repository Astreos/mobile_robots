/*! 
 * \author Nicolas Van der Noot
 * \file Orientation.hh
 * \brief Orientation class
 * 
 * Details
 */

#ifndef _ORIENTATION_HH_
#define _ORIENTATION_HH_

/*! \brief Angle describing an orientation
 */
class Orientation
{
	public:
		Orientation();
		Orientation(double alpha);
		
		~Orientation();

		void set_orientation(double new_alpha);
		void inc_orientation(double inc_alpha);

		double get_alpha() const { return alpha; }

		double get_c_alpha() { return c_alpha; }
		double get_s_alpha() { return s_alpha; }

	private:
		double alpha;   ///< angle [rad]
		double c_alpha; ///< cosine of the angle [-]
		double s_alpha; ///< cosine of the angle [-]
};

#endif
