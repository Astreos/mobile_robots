#include "RollingRobot.hh"

#include <cmath>

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] wheel_radius radius of the wheels [m]
 * \param[in] double half of the distance between the two wheels [m]
 * \param[in] id_robot
 * \param[in] id_T1 T1 floating base joint ID
 * \param[in] id_T2 T2 floating base joint ID
 * \param[in] id_R3 R3 floating base joint ID
 * \param[in] id_RW right wheel joint ID
 * \param[in] id_LW left wheel joint ID
 */
RollingRobot::RollingRobot(MbsData *mbs_data, double wheel_radius, double semi_axle, int id_robot, int id_T1, int id_T2, int id_R3, int id_RW, int id_LW)
{
	this->mbs_data = mbs_data;

	this->id_T1 = id_T1;
	this->id_T2 = id_T2;
	this->id_R3 = id_R3;

	this->id_RW = id_RW;
	this->id_LW = id_LW;

	this->id_robot = id_robot;

	this->wheel_radius = wheel_radius;
	this->semi_axle = semi_axle;

	id_inc = 3*id_robot;
}

/*! \brief destructor
 */
RollingRobot::~RollingRobot()
{

}

/*! \brief compute the h vector and the Jacobian matrix
 * 
 * \param[out] h h vector
 * \param[out] Jac Jacobian matrix
 */
void RollingRobot::hJ_compute(double *h, double **Jac)
{
	double theta, c_theta, s_theta;

	theta   = mbs_data->q[id_R3];
	c_theta = cos(theta);
	s_theta = sin(theta);

	// computation of h and of the Jacobian
	h[1+id_inc] = 0.0;
	Jac[1+id_inc][id_T1] = -s_theta;
	Jac[1+id_inc][id_T2] = c_theta;
	Jac[1+id_inc][id_R3] = 0.0;
	Jac[1+id_inc][id_LW] = 0.0;
	Jac[1+id_inc][id_RW] = 0.0;
	
	h[2+id_inc] = 0.0;
	Jac[2+id_inc][id_T1] = c_theta;
	Jac[2+id_inc][id_T2] = s_theta;
	Jac[2+id_inc][id_R3] = -semi_axle;
	Jac[2+id_inc][id_LW] = -wheel_radius;
	Jac[2+id_inc][id_RW] = 0.0;
	
	h[3+id_inc] = 0.0;
	Jac[3+id_inc][id_T1] = c_theta;
	Jac[3+id_inc][id_T2] = s_theta;
	Jac[3+id_inc][id_R3] = semi_axle;
	Jac[3+id_inc][id_LW] = 0.0;
	Jac[3+id_inc][id_RW] = -wheel_radius;
}

/*! \brief compute the jdqd vector
 * 
 * \param[out] jdqd jdqd vector
 */
void RollingRobot::jdqd_compute(double *jdqd)
{
	double x_p, y_p;
	double theta, theta_p, c_theta, s_theta;
	double xp_tp, yp_tp;
	double xp_tp_c, xp_tp_s, yp_tp_c, yp_tp_s;

	x_p = mbs_data->qd[id_T1];
	y_p = mbs_data->qd[id_T2];

	theta   = mbs_data->q[id_R3];
	theta_p = mbs_data->qd[id_R3];
	c_theta = cos(theta);
	s_theta = sin(theta);

	xp_tp = x_p*theta_p;
	yp_tp = y_p*theta_p;

	xp_tp_c = xp_tp*c_theta;
	xp_tp_s = xp_tp*s_theta;
	yp_tp_c = yp_tp*c_theta;
	yp_tp_s = yp_tp*s_theta;

	jdqd[1+id_inc] = -xp_tp_c - yp_tp_s;
	jdqd[2+id_inc] = -xp_tp_s + yp_tp_c;
	jdqd[3+id_inc] = -xp_tp_s + yp_tp_c;
}

/*! \brief integrate the position of the robot
 */
void RollingRobot::integrate_pos()
{
	mbs_data->q[id_T1] = mbs_data->ux[1+id_inc];
	mbs_data->q[id_T2] = mbs_data->ux[2+id_inc];
	mbs_data->q[id_R3] = mbs_data->ux[3+id_inc];
}

/*! \brief set the derivative of the position to integrate
 */
void RollingRobot::set_der_to_integrate()
{
	mbs_data->uxd[1+id_inc] = mbs_data->qd[id_T1];
	mbs_data->uxd[2+id_inc] = mbs_data->qd[id_T2];
	mbs_data->uxd[3+id_inc] = mbs_data->qd[id_R3];
}
