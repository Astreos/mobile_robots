#include "opp_pos_gr18.h"
#include "init_pos_gr18.h"
#include "useful_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief compute the opponents position using the tower
 * 
 * \param[in,out] cvs controller main structure
 */
void opponents_tower(CtrlStruct *cvs)
{
	// variables declaration
	int nb_opp;
	int rise_index_1, rise_index_2, fall_index_1, fall_index_2;

	double delta_t;
	double rise_1, rise_2, fall_1, fall_2;

	CtrlIn *inputs;
	RobotPosition *rob_pos;
	OpponentsPosition *opp_pos;

	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;
	opp_pos = cvs->opp_pos;

	nb_opp = opp_pos->nb_opp;

	// no opponent
	if (!nb_opp)
	{
		return;
	}

	// safety
	if (nb_opp < 0 || nb_opp > 2)
	{
		printf("Error: number of opponents cannot be %d!\n", nb_opp);
		exit(EXIT_FAILURE);
	}

	// low pass filter time increment ('delta_t' is the last argument of the 'first_order_filter' function)
	delta_t = inputs->t - opp_pos->last_t;
	opp_pos->last_t = inputs->t;

	// indexes
	rise_index_1 = inputs->rising_index;
	fall_index_1 = inputs->falling_index;

	// rise and fall angles of the first opponent
	rise_1 = inputs->last_rising[rise_index_1];
	fall_1 = inputs->last_falling[fall_index_1];

	// rise and fall angles of the second opponent
	if (nb_opp == 2)
	{
		rise_index_2 = (rise_index_1-1 < 0) ? NB_STORE_EDGE-1 : rise_index_1-1;
		fall_index_2 = (fall_index_1-1 < 0) ? NB_STORE_EDGE-1 : fall_index_1-1;

		rise_2 = inputs->last_rising[rise_index_2];
		fall_2 = inputs->last_falling[fall_index_2];
	}

	// ----- opponents position computation start ----- //
        
	if (inputs->t <= -9.0) {
		opp_pos->x[0] = 0.65;
		opp_pos->y[0] = -1.10;
		opp_pos->x[1] = 0.65;
		opp_pos->y[1] = -1.10;
	}
        
	double old_opp_pos_x[2];
	double old_opp_pos_y[2];
        
	old_opp_pos_x[0] = opp_pos->x[0];
	old_opp_pos_y[0] = opp_pos->y[0];
	old_opp_pos_x[1] = opp_pos->x[1];
	old_opp_pos_y[1] = opp_pos->y[1];

	single_opp_tower(rise_1, fall_1, rob_pos->x, rob_pos->y, rob_pos->theta, opp_pos->x, opp_pos->y);
        
	opp_pos->x[0] = first_order_filter(old_opp_pos_x[0], opp_pos->x[0], 0.7, delta_t);
	opp_pos->y[0] = first_order_filter(old_opp_pos_y[0], opp_pos->y[0], 0.7, delta_t);
        
	if (nb_opp == 2)
	{
		single_opp_tower(rise_2, fall_2, rob_pos->x, rob_pos->y, rob_pos->theta, opp_pos->x + 1, opp_pos->y + 1);
            
		opp_pos->x[1] = first_order_filter(old_opp_pos_x[1], opp_pos->x[1], 0.7, delta_t);
		opp_pos->y[1] = first_order_filter(old_opp_pos_y[1], opp_pos->y[1], 0.7, delta_t);
	}

	// ----- opponents position computation end ----- //
        
	//set_plot(opp_pos->x[0], "opp1_x_[m]");
	//set_plot(opp_pos->y[0], "opp1_y_[m]");
        
	//set_plot(opp_pos->x[1], "opp2_x_[m]");
	//set_plot(opp_pos->y[1], "opp2_y_[m]");
        
	set_plot(check_opp_front(cvs), "detection");
}

/*! \brief compute a single opponent position
 * 
 * \param[in] last_rise last rise relative angle [rad]
 * \param[in] last_fall last fall relative angle [rad]
 * \param[in] rob_x robot x position [m]
 * \param[in] rob_y robot y position [m]
 * \param[in] rob_theta robot orientation [rad]
 * \param[out] new_x_opp new known x opponent position
 * \param[out] new_y_opp new known y opponent position
 * \return 1 if computation successful, 0 otherwise
 */
int single_opp_tower(double last_rise, double last_fall, double rob_x, double rob_y, double rob_theta, double *new_x_opp, double *new_y_opp)
{                       
            *new_x_opp = rob_x + 0.083*cos(rob_theta) + (0.040/sin((last_fall - last_rise)/2.0))*sin(M_PI/2.0 - (last_fall+last_rise)/2.0 - rob_theta);
            *new_y_opp = rob_y + 0.083*sin(rob_theta) + (0.040/sin((last_fall - last_rise)/2.0))*cos(M_PI/2.0 - (last_fall+last_rise)/2.0 - rob_theta);

	return 1;
}

/*! \brief check if there is an opponent in front of the robot
 * 
 * \param[in] cvs controller main structure
 * \return 1 if opponent robot in front of the current robot
 */
int check_opp_front(CtrlStruct *cvs)
{
	// variables declaration
	int i, nb_opp;
	int rise_index[2], fall_index[2];
	
	double rise[2], fall[2];

	CtrlIn *inputs;
	OpponentsPosition *opp_pos;
	RobotPosition *rob_pos;

	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;
	opp_pos = cvs->opp_pos;
	nb_opp = opp_pos->nb_opp;

	// no opponent
	if (!nb_opp)
	{
		return 0;
	}

	// safety
	if (nb_opp < 0 || nb_opp > 2)
	{
		printf("Error: number of opponents cannot be %d!\n", nb_opp);
		exit(EXIT_FAILURE);
	}
	
	// indexes
	rise_index[0] = inputs->rising_index;
	fall_index[0] = inputs->falling_index;

	// rise and fall angles of the first opponent
	rise[0] = inputs->last_rising[rise_index[0]];
	fall[0] = inputs->last_falling[fall_index[0]];

	// rise and fall angles of the second opponent
	if (nb_opp == 2)
	{
		rise_index[1] = (rise_index[0]-1 < 0) ? NB_STORE_EDGE-1 : rise_index[0]-1;
		fall_index[1] = (fall_index[0]-1 < 0) ? NB_STORE_EDGE-1 : fall_index[0]-1;

		rise[1] = inputs->last_rising[rise_index[1]];
		fall[1] = inputs->last_falling[fall_index[1]];
	}

		// ----- opponents check computation start ----- //
            
	if (nb_opp == 1)
	{
		return ((norm_dist(rob_pos->x - opp_pos->x[0], rob_pos->y - opp_pos->y[0]) < 0.50) && (((rise[0] <= M_PI/4.0) && (rise[0] >= -M_PI/4.0)) || ((fall[0] <= M_PI/4.0) && (fall[0] >= -M_PI/4.0))));
	}
	else
	{
		return (((norm_dist(rob_pos->x - opp_pos->x[0], rob_pos->y - opp_pos->y[0]) < 0.50) && (((rise[0] <= M_PI/4.0) && (rise[0] >= -M_PI/4.0)) || ((fall[0] <= M_PI/4.0) && (fall[0] >= -M_PI/4.0))))
		|| ((norm_dist(rob_pos->x - opp_pos->x[1], rob_pos->y - opp_pos->y[1]) < 0.50) && (((rise[1] <= M_PI/4.0) && (rise[1] >= -M_PI/4.0)) || ((fall[1] <= M_PI/4.0) && (fall[1] >= -M_PI/4.0)))));
	}
            
		// ----- opponents check computation end ----- //
}

NAMESPACE_CLOSE();
