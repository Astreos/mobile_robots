/*! 
 * \author Nicolas Van der Noot
 * \file Actuator.hh
 * \brief Actuator class
 */

#ifndef _ACTUATOR_HH_
#define _ACTUATOR_HH_

#include <iostream>

#include "mbs_data.h"
#include "user_realtime.h"
#include "Noise.hh"

/*! \brief Single actuator
 */
class Actuator
{
	public:
		Actuator(MbsData *mbs_data, int joint_id, double rho, double k_phi, double R, double L, double min_u_m, double max_u_m, double fric_cst, double max_noise);
		~Actuator();

		void compute_torque();
		void apply_torque();
		void update_voltage(double command);

	private:
		int joint_id; ///< ID of the joint actuated by this motor

		double rho; ///< motor reduction [-]
		double k_phi; ///< torque constant [Nm/A]
		double R; ///< motor resistance [ohm]
		double L; ///< motor inductance (neglected, not used) [H]

		double tau_m; ///< motor torque [Nm]
		double tau_l; ///< load torque [Nm]
		double omega_m; ///< motor velocity [rad/s]
		double omega_l; ///< load velocity [rad/s]

		double fric_cst; ///< friction constant [Nm]

		double u_m; ///< motor voltage [V]
		double i_m; ///< motor current [A]

		double min_u_m; ///< minimum voltage [V]
		double max_u_m; ///< maximum voltage [V]

		double mid_u_m;       ///< mean of the voltage range [V]
		double diff_u_m;      ///< difference between the maximum and minimum voltages [V]
		double semi_diff_u_m; ///< half of diff_u_m [V]

		double max_noise; ///< maximal noise value [Nm]

		Noise *noise; ///< noise on the motor torque

		MbsData *mbs_data; ///< Robotran structure

		double friction_torque();
};

#endif
