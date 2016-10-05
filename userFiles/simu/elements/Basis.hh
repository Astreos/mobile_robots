/*! 
 * \author Nicolas Van der Noot
 * \file Basis.hh
 * \brief Basis class
 */

#ifndef _BASIS_HH_
#define _BASIS_HH_

#include "robot_id.h"
#include "Target.hh"

/*! \brief basis of a team
 */
class Basis
{
	public:
		Basis(double x_rect, double y_rect, double angle_rect, double semi_width_rect, double semi_height_rect, int team_id);
		~Basis();

		int target_full_inside(Target *cur_target);

	private:
		double x_rect; ///< rectangle center x coordinate [m]
		double y_rect; ///< rectangle center y coordinate [m]
		double angle_rect; ///< angle describing the rectangle orientation [rad]
		double semi_width_rect; ///< semi width of the rectangle [m]
		double semi_height_rect; ///< semi height of the rectangle [m]

		int team_id; ///< ID of the team
};

#endif
