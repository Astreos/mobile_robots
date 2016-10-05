#include "Actuator.hh"
#include "simu_functions.hh"

#define RND_PERIOD 0.75

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] rho motor reduction ratio [-]
 * \param[in] k_phi torque constant [Nm/A]
 * \param[in] R motor resistance [ohm]
 * \param[in] L motor inductance (neglected, not used) [H]
 * \param[in] min_u_m minimum voltage [V]
 * \param[in] max_u_m maximum voltage [V]
 * \param[in] fric_cst friction constant [Nm]
 */
Actuator::Actuator(MbsData *mbs_data, int joint_id, double rho, double k_phi, double R, double L, double min_u_m, double max_u_m, double fric_cst, double max_noise)
{
	this->mbs_data = mbs_data;

	this->joint_id = joint_id;

	this->rho = rho;
	this->k_phi = k_phi;
	this->R = R;
	this->L = L;

	this->min_u_m = min_u_m;
	this->max_u_m = max_u_m;

	this->fric_cst = fric_cst;

	mid_u_m       = 0.5 * (min_u_m + max_u_m);
	diff_u_m      = max_u_m - min_u_m;
	semi_diff_u_m = 0.5 * diff_u_m;

	omega_l = mbs_data->qd[joint_id];
	omega_m = rho * omega_l;

	u_m = 0.0;
	i_m = 0.0;

	tau_m = 0.0;
	tau_l = 0.0;

	// Noise(t, max_noise, t_period)
	noise = new Noise(mbs_data->tsim, max_noise, RND_PERIOD);
	this->max_noise = max_noise;
}

/*! \brief destructor
 */
Actuator::~Actuator()
{
	delete noise;
}

/*! \brief compute the torque provided to the wheel
 */
void Actuator::compute_torque()
{
	omega_l = mbs_data->qd[joint_id];
	omega_m = rho * omega_l;

	i_m = (u_m - k_phi*omega_m) / R;

	tau_m = k_phi * i_m;
	tau_l = rho * tau_m;

	// add noise
	tau_l += tau_l*noise->update_and_get(mbs_data->tsim);

	// add friction
	tau_l -= friction_torque();
}

/*! \brief apply the wheel torque to the corrsponding Robotran joint
 */
void Actuator::apply_torque()
{
	mbs_data->Qq[joint_id] = tau_l;
}

/*! \brief update the voltage sent to the motors
 * 
 * \param[in] command a command bounded in [-100 ; 100]
 *
 * The command [-100 ; 100] is proportional to the voltage
 *
 *     command: -100   ->   corresponding to 5% of the PWM
 *     command:    0   ->   corresponding to 50% of the PWM
 *     command:  100   ->   corresponding to 95% of the PWM
 */
void Actuator::update_voltage(double command)
{
	u_m = mid_u_m + 0.9 * semi_diff_u_m * 0.01 * limit_range(command, -100.0, 100.0);
}

/*! \brief compute the wheel torque friction
 * 
 * \return torque friction [Nm]
 */
double Actuator::friction_torque()
{
	return (omega_l < 0.0) ? -fric_cst : (omega_l > 0.0) ? fric_cst : 0.0;
}
