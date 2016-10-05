/*! 
 * \author Nicolas Van der Noot
 * \file RollingRobot.hh
 * \brief RollingRobot class
 */

#ifndef _ROLLING_ROBOT_HH_
#define _ROLLING_ROBOT_HH_

#include "mbs_data.h"

/*! \brief rolling without slipping for one robot
 */
class RollingRobot
{
	public:
		RollingRobot(MbsData *mbs_data, double wheel_radius, double semi_axle, int id_robot, int id_T1, int id_T2, int id_R3, int id_RW, int id_LW);
		~RollingRobot();

		void hJ_compute(double *h, double **Jac);
		void jdqd_compute(double *jdqd);

		void integrate_pos();
		void set_der_to_integrate();

	private:
		MbsData *mbs_data; ///< Robotran structure

		int id_T1; ///< T1 floating base joint ID
		int id_T2; ///< T2 floating base joint ID
		int id_R3; ///< R3 floating base joint ID

		int id_RW; ///< right wheel joint ID
		int id_LW; ///< left wheel joint ID

		int id_robot; ///< ID of the robot
		int id_inc; ///< increment for the indexes

		double wheel_radius; ///< radius of the wheels [m]
		double semi_axle; ///< half of the distance between the two wheels [m]
};

#endif
