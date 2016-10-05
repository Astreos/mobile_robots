/*! 
 * \author Nicolas Van der Noot
 * \file RollingGestion.hh
 * \brief RollingGestion class
 */

#ifndef _ROLLING_GESTION_HH_
#define _ROLLING_GESTION_HH_

#include <vector>
#include "RollingRobot.hh"
#include "mbs_data.h"

/*! \brief rolling without slipping: gestion of all robots
 */
class RollingGestion
{
	public:
		RollingGestion(MbsData *mbs_data);
		~RollingGestion();

		void hJ_compute(double *h, double **Jac);
		void jdqd_compute(double *jdqd);

		void integrate_pos();
		void set_der_to_integrate();

	private:
		std::vector<RollingRobot> rolling_list; ///< list with all the robots constraints
		MbsData *mbs_data; ///< Robotran structure
};

#endif
