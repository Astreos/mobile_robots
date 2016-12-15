#include "triangulation_gr18.h"
#include "useful_gr18.h"
#include "init_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief set the fixed beacons positions, depending on the team
 * 
 * \param[in] team_id ID of the team ('TEAM_A' or 'TEAM_B')
 * \param[out] x_beac_1 first beacon x position [m]
 * \param[out] y_beac_1 first beacon y position [m]
 * \param[out] x_beac_2 second beacon x position [m]
 * \param[out] y_beac_2 second beacon y position [m]
 * \param[out] x_beac_3 third beacon x position [m]
 * \param[out] y_beac_3 third beacon y position [m]
 *
 * This function can be adapted, depending on the map.
 */
void fixed_beacon_positions(int team_id, double *x_beac_1, double *y_beac_1, double *x_beac_2, double *y_beac_2, double *x_beac_3, double *y_beac_3)
{
	switch (team_id)
	{
		case TEAM_A:
			*x_beac_1 = 1.062;
			*y_beac_1 = 1.562;
			
			*x_beac_2 = -1.062;
			*y_beac_2 = 1.562;
			
			*x_beac_3 = 0.0;
			*y_beac_3 = -1.562;
			break;
			
		case TEAM_B:
			*x_beac_1 = 1.062;
			*y_beac_1 = -1.562;
			
			*x_beac_2 = -1.062;
			*y_beac_2 = -1.562;
			
			*x_beac_3 = 0;
			*y_beac_3 = 1.562;
			break;
			
		default:
			printf("Error unknown team ID (%d) !\n", team_id);
			exit(EXIT_FAILURE);
	}
}

/*! \brief get the index of the best angle prediction
 * 
 * \param[in] alpha_predicted angle to reach [rad]
 * \param[in] alpha_a angle computed for A [rad]
 * \param[in] alpha_b angle computed for B [rad]
 * \param[in] alpha_c angle computed for C [rad]
 * \return best index (0, 1, or 2)
 */
int index_predicted(double alpha_predicted, double alpha_a, double alpha_b, double alpha_c)
{
	double pred_err_a, pred_err_b, pred_err_c;

	pred_err_a = fabs(limit_angle(alpha_a - alpha_predicted));
	pred_err_b = fabs(limit_angle(alpha_b - alpha_predicted));
	pred_err_c = fabs(limit_angle(alpha_c - alpha_predicted));

	return (pred_err_a < pred_err_b) ? ((pred_err_a < pred_err_c) ? 0 : 2) : ((pred_err_b < pred_err_c) ? 1 : 2);
}

/*! \brief triangulation main algorithm
 * 
 * \param[in] cvs controller main structure
 *
 * computation found here: http://www.telecom.ulg.ac.be/triangulation/
 */
void triangulation(CtrlStruct *cvs)
{
	// variables declaration
	RobotPosition *pos_tri, *rob_pos;
	CtrlIn *inputs;

	int alpha_1_index, alpha_2_index, alpha_3_index;
	int rise_index_1, rise_index_2, rise_index_3;
	int fall_index_1, fall_index_2, fall_index_3;

	double alpha_a, alpha_b, alpha_c;
	double alpha_1, alpha_2, alpha_3;
	double alpha_1_predicted, alpha_2_predicted, alpha_3_predicted;
	double x_beac_1, y_beac_1, x_beac_2, y_beac_2, x_beac_3, y_beac_3;
	double x_mod_beac_1, x_mod_beac_2, x_mod_beac_3, y_mod_beac_1, y_mod_beac_2, y_mod_beac_3;
	double cot_1_2, cot_2_3, cot_3_1;
	double x_circ_1_2, y_circ_1_2, x_circ_2_3, y_circ_2_3, x_circ_3_1, y_circ_3_1;
	double k_3_1, diam_tri;
	
	double delta_t;
	double old_pos_tri_x, old_pos_tri_y, old_pos_tri_theta;
	
	int team_id;

	// variables initialization
	pos_tri = cvs->triang_pos;
	rob_pos = cvs->rob_pos;
	inputs  = cvs->inputs;
	
	team_id = cvs->team_id;

	// safety
	if ((inputs->rising_index_fixed < 0) || (inputs->falling_index_fixed < 0))
	{
		return;
	}

	// known positions of the beacons
	fixed_beacon_positions(cvs->team_id, &x_beac_1, &y_beac_1, &x_beac_2, &y_beac_2, &x_beac_3, &y_beac_3);

	// indexes fot the angles detection
	rise_index_1 = inputs->rising_index_fixed;
	rise_index_2 = (rise_index_1 - 1 < 0) ? NB_STORE_EDGE-1 : rise_index_1 - 1;
	rise_index_3 = (rise_index_2 - 1 < 0) ? NB_STORE_EDGE-1 : rise_index_2 - 1;

	fall_index_1 = inputs->falling_index_fixed;
	fall_index_2 = (fall_index_1 - 1 < 0) ? NB_STORE_EDGE-1 : fall_index_1 - 1;
	fall_index_3 = (fall_index_2 - 1 < 0) ? NB_STORE_EDGE-1 : fall_index_2 - 1;

	// beacons angles measured with the laser (to compute)
	alpha_a = (inputs->last_rising_fixed[rise_index_1] + inputs->last_falling_fixed[fall_index_1])/2;
	alpha_b = (inputs->last_rising_fixed[rise_index_2] + inputs->last_falling_fixed[fall_index_2])/2;
	alpha_c = (inputs->last_rising_fixed[rise_index_3] + inputs->last_falling_fixed[fall_index_3])/2;
        
	//set_plot(alpha_a, "alpha_a");
	//set_plot(alpha_b, "alpha_b");
	//set_plot(alpha_c, "alpha_c");

	// beacons angles predicted thanks to odometry measurements (to compute)
	alpha_1_predicted = limit_angle(M_PI/2.0*team(team_id) - team(team_id)*atan(fabs(rob_pos->x - x_beac_1)/fabs(rob_pos->y - y_beac_1)) - rob_pos->theta);
	alpha_2_predicted = limit_angle(M_PI/2.0*team(team_id) + team(team_id)*atan(fabs(rob_pos->x - x_beac_2)/fabs(rob_pos->y - y_beac_2)) - rob_pos->theta);
	if (rob_pos->x >= 0)
	{
		alpha_3_predicted = limit_angle(M_PI + M_PI/2.0*team(team_id) - team(team_id)*atan(fabs(rob_pos->x - x_beac_3)/fabs(rob_pos->y - y_beac_3)) - rob_pos->theta);
	}
	else
	{
		alpha_3_predicted = limit_angle(M_PI + M_PI/2.0*team(team_id) + team(team_id)*atan(fabs(rob_pos->x - x_beac_3)/fabs(rob_pos->y - y_beac_3)) - rob_pos->theta);
	}
	
	// indexes of each beacon
	alpha_1_index = index_predicted(alpha_1_predicted, alpha_a, alpha_b, alpha_c);
	alpha_2_index = index_predicted(alpha_2_predicted, alpha_a, alpha_b, alpha_c);
	alpha_3_index = index_predicted(alpha_3_predicted, alpha_a, alpha_b, alpha_c);

	// safety
	if ((alpha_1_index == alpha_2_index) || (alpha_1_index == alpha_3_index) || (alpha_2_index == alpha_3_index))
	{
		return;
	}

	// angle of the first beacon
	switch (alpha_1_index)
	{
		case 0: alpha_1 = alpha_a; break;
		case 1: alpha_1 = alpha_b; break;
		case 2: alpha_1 = alpha_c; break;
	
		default:
			printf("Error: unknown index %d !\n", alpha_1_index);
			exit(EXIT_FAILURE);
	}

	// angle of the second beacon
	switch (alpha_2_index)
	{
		case 0: alpha_2 = alpha_a; break;
		case 1: alpha_2 = alpha_b; break;
		case 2: alpha_2 = alpha_c; break;
	
		default:
			printf("Error: unknown index %d !\n", alpha_2_index);
			exit(EXIT_FAILURE);
	}

	// angle of the third beacon
	switch (alpha_3_index)
	{
		case 0: alpha_3 = alpha_a; break;
		case 1: alpha_3 = alpha_b; break;
		case 2: alpha_3 = alpha_c; break;
	
		default:
			printf("Error: unknown index %d !\n", alpha_3_index);
			exit(EXIT_FAILURE);
	}
	
	// ----- triangulation computation start ----- //

	// Compute the modified beacon coordinates :
	x_mod_beac_1 = x_beac_1 - x_beac_2;
	y_mod_beac_1 = y_beac_1 - y_beac_2;

	x_mod_beac_3 = x_beac_3 - x_beac_2;
	y_mod_beac_3 = y_beac_3 - y_beac_2;
	
	// Compute the three cot(.) :
	cot_1_2 = cos(alpha_2 - alpha_1)/sin(alpha_2 - alpha_1);
	cot_2_3 = cos(alpha_3 - alpha_2)/sin(alpha_3 - alpha_2);
	cot_3_1 = (1.0 - cot_1_2*cot_2_3)/(cot_1_2 + cot_2_3);

	// Compute de modified circle center coordinates :
	x_circ_1_2 = x_mod_beac_1 + cot_1_2*y_mod_beac_1;
	y_circ_1_2 = y_mod_beac_1 - cot_1_2*x_mod_beac_1;

	x_circ_2_3 = x_mod_beac_3 - cot_2_3*y_mod_beac_3;
	y_circ_2_3 = y_mod_beac_3 + cot_2_3*x_mod_beac_3;

	x_circ_3_1 = x_mod_beac_3 + x_mod_beac_1 + cot_3_1*(y_mod_beac_3 - y_mod_beac_1);
	y_circ_3_1 = y_mod_beac_3 + y_mod_beac_1 - cot_3_1*(x_mod_beac_3 - x_mod_beac_1);

	// Compute k'31
	k_3_1 = x_mod_beac_1*x_mod_beac_3 + y_mod_beac_1*y_mod_beac_3 + cot_3_1*(x_mod_beac_1*y_mod_beac_3 - x_mod_beac_3*y_mod_beac_1);

	// Compute D
	diam_tri = (x_circ_1_2 - x_circ_2_3)*(y_circ_2_3 - y_circ_3_1) - (y_circ_1_2 - y_circ_2_3)*(x_circ_2_3 - x_circ_3_1);
        
	if(diam_tri == 0)
	{
		printf("Error!\n");
		return;
	}
	
	old_pos_tri_x = pos_tri->x;
	old_pos_tri_y = pos_tri->y;
	old_pos_tri_theta = pos_tri->theta;
	
	// robot position
	pos_tri->x = x_beac_2 + k_3_1*(y_circ_1_2 - y_circ_2_3)/diam_tri - 0.083*cos(rob_pos->theta);
	pos_tri->y = y_beac_2 + k_3_1*(x_circ_2_3 - x_circ_1_2)/diam_tri - 0.083*sin(rob_pos->theta);

	// robot orientation
	if (pos_tri->x >= 0)
	{
		pos_tri->theta = limit_angle(3.0*M_PI/2.0 - atan(fabs(pos_tri->x - x_beac_3)/fabs(pos_tri->y - y_beac_3)) - alpha_3);
	}
	else
	{
		pos_tri->theta = limit_angle(3.0*M_PI/2.0 + atan(fabs(pos_tri->x - x_beac_3)/fabs(pos_tri->y - y_beac_3)) - alpha_3);
	}
	
	// low pass filter time increment ('delta_t' is the last argument of the 'first_order_filter' function)
	delta_t = inputs->t - pos_tri->last_t;
	
	pos_tri->x = first_order_filter(old_pos_tri_x, pos_tri->x, 0.9, delta_t);
	pos_tri->y = first_order_filter(old_pos_tri_y, pos_tri->y, 0.9, delta_t);
	pos_tri->theta = first_order_filter(old_pos_tri_theta, pos_tri->theta, 0.9, delta_t);
	
	pos_tri->last_t = inputs->t;
	
	// ----- triangulation computation end ----- //
        
	//set_plot(pos_tri->x, "x_tri_[m]");
	//set_plot(pos_tri->y, "y_tri_[m]");
    //set_plot(pos_tri->theta, "theta_tri_[rad]");
}

NAMESPACE_CLOSE();
