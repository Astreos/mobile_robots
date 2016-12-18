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
	CtrlIn *inputs;
	RobotPosition *rob_pos;
	OpponentsPosition *opp_pos;
	
	int nb_opp;
	int rise_index_1, rise_index_2, fall_index_1, fall_index_2;
	double rise_1, rise_2, fall_1, fall_2;
	
	double delta_t;
	int team_id;
	
	double old_opp_pos_x[2];
	double old_opp_pos_y[2];

	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;
	opp_pos = cvs->opp_pos;

	nb_opp = opp_pos->nb_opp;
	team_id = cvs->team_id;

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
	
	//printf("(rise_1, fall_1, rise_2, fall_2) = (%f, %f, %f, %f) \n", rise_1, fall_1, rise_2, fall_2);
	
	if (inputs->t <= -5.0) {
		opp_pos->x[0] = 0.65;
		opp_pos->y[0] = -1.25*team(team_id);
		opp_pos->x[1] = 0.65;
		opp_pos->y[1] = -1.25*team(team_id);
	}
	
	if (nb_opp == 1)
	{
		old_opp_pos_x[0] = opp_pos->x[0];
		old_opp_pos_y[0] = opp_pos->y[0];
		
		single_opp_tower(cvs, rise_1, fall_1, opp_pos->x, opp_pos->y);
		
		opp_pos->x[0] = first_order_filter(old_opp_pos_x[0], opp_pos->x[0], 1.0, delta_t);
		opp_pos->y[0] = first_order_filter(old_opp_pos_y[0], opp_pos->y[0], 1.0, delta_t);
	}
	else
	{/*
		previous_rise_index = rise_index_1 - 2;
		
		if (previous_rise_index < 0)
		{
			previous_rise_index = NB_STORE_EDGE-1 - 1*rise_index_1;
		}
		
		previous_fall_index = fall_index_1 - 2;
		
		if (previous_fall_index < 0)
		{
			previous_fall_index = NB_STORE_EDGE-1 - 1*fall_index_1;
		}*/
		/*
		for (int i = 0; i < NB_STORE_EDGE; i++)
		{
			printf("(last_rise, last_fall) = (%f, %f) \n", inputs->last_rising[i], inputs->last_falling[i]);
		}
		printf("\n");
		*/
		
		//printf("(nb_rising, nb_falling) = (%d, %d) \n", inputs->nb_rising, inputs->nb_falling);
		
		//printf("(rise_index_1, fall_index_1, previous_rise_index, previous_fall_index) = (%d, %d, %d, %d) \n", rise_index_1, fall_index_1, previous_rise_index, previous_fall_index);
		//printf("(rise_1, fall_1, previous_rise, previous_fall) = (%f, %f, %f, %f) \n", rise_1, fall_1, inputs->last_rising[previous_rise_index], inputs->last_falling[previous_fall_index]);
		
		/*
		if ((fabs(rise_1 - inputs->last_rising[previous_rise_index]) > 0.1) || (fabs(fall_1 - inputs->last_falling[previous_fall_index]) > 0.1))
		{
			opp_pos->opp_switch = !opp_pos->opp_switch;
		}*/
		
		if ((inputs->nb_rising == 2) && (opp_pos->previous_nb_rising == 1) && !opp_pos->switch_check)
		{
			opp_pos->previous_nb_rising = inputs->nb_rising;
			opp_pos->switch_check = true;
		}
		
		if ((inputs->nb_rising == 1) && (opp_pos->previous_nb_rising == 2) && opp_pos->switch_check)
		{
			opp_pos->previous_nb_rising = inputs->nb_rising;
			opp_pos->previous_nb_falling = inputs->nb_falling;
			opp_pos->opp_switch = !opp_pos->opp_switch;
			
			opp_pos->switch_check = false;
		}
		
		//printf("switch = %d \n", opp_pos->opp_switch);
		
		old_opp_pos_x[0] = opp_pos->x[0];
		old_opp_pos_y[0] = opp_pos->y[0];
		old_opp_pos_x[1] = opp_pos->x[1];
		old_opp_pos_y[1] = opp_pos->y[1];
		
		if (inputs->nb_rising == 1)
		{
			opp_pos->x[0] = old_opp_pos_x[0];
			opp_pos->y[0] = old_opp_pos_y[0];
			opp_pos->x[1] = old_opp_pos_x[1];
			opp_pos->y[1] = old_opp_pos_y[1];
		}
		else
		{
			if (opp_pos->opp_switch)
			{
				/*
				old_opp_pos_x[0] = opp_pos->x[1];
				old_opp_pos_y[0] = opp_pos->y[1];
				old_opp_pos_x[1] = opp_pos->x[0];
				old_opp_pos_y[1] = opp_pos->y[0];
				*/
				single_opp_tower(cvs, rise_2, fall_2, opp_pos->x, opp_pos->y);
				single_opp_tower(cvs, rise_1, fall_1, opp_pos->x + 1, opp_pos->y + 1);
			}
			else
			{/*
				old_opp_pos_x[0] = opp_pos->x[0];
				old_opp_pos_y[0] = opp_pos->y[0];
				old_opp_pos_x[1] = opp_pos->x[1];
				old_opp_pos_y[1] = opp_pos->y[1];
				*/
				single_opp_tower(cvs, rise_1, fall_1, opp_pos->x, opp_pos->y);
				single_opp_tower(cvs, rise_2, fall_2, opp_pos->x + 1, opp_pos->y + 1);
			}
			
			opp_pos->x[0] = first_order_filter(old_opp_pos_x[0], opp_pos->x[0], 1.0, delta_t);
			opp_pos->y[0] = first_order_filter(old_opp_pos_y[0], opp_pos->y[0], 1.0, delta_t);
			opp_pos->x[1] = first_order_filter(old_opp_pos_x[1], opp_pos->x[1], 1.0, delta_t);
			opp_pos->y[1] = first_order_filter(old_opp_pos_y[1], opp_pos->y[1], 1.0, delta_t);
		}
		
		//printf("(opp_0_x, opp_0_y, old_opp_0_x, old_opp_0_y) = (%f, %f, %f, %f) and (opp_1_x, opp_1_y, old_opp_1_x, old_opp_1_y) = (%f, %f, %f, %f)\n", opp_pos->x[0], opp_pos->y[0], old_opp_pos_x[0], old_opp_pos_y[0], opp_pos->x[1], opp_pos->y[1], old_opp_pos_x[1], old_opp_pos_y[1]);
	}
	
	check_opp_front(cvs);

	// ----- opponents position computation end ----- //
        
	//set_plot(opp_pos->x[0], "opp1_x_[m]");
	//set_plot(opp_pos->y[0], "opp1_y_[m]");
        
	//set_plot(opp_pos->x[1], "opp2_x_[m]");
	//set_plot(opp_pos->y[1], "opp2_y_[m]");
        
	//set_plot(opp_pos->opp_front, "detection");
	
	opp_pos->last_t = inputs->t;
	
	return;
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
int single_opp_tower(CtrlStruct *cvs, double last_rise, double last_fall, double *new_x_opp, double *new_y_opp)
{
	// variables declaration
	RobotPosition *rob_pos;
	
	double R, delta;
	double d, phi;
	
	// variables initialization
	rob_pos = cvs->rob_pos;
	
	R = 0.040;
	delta = 0.083;
	
	if ((last_fall < -M_PI/2.0) && (last_fall > -M_PI) && (last_rise < M_PI) && (last_rise > M_PI/2.0))
	{
		d = R/sin(fabs(last_fall + 2.0*M_PI - last_rise)/2.0);
		phi = M_PI/2.0 - (last_fall + 2.0*M_PI + last_rise)/2.0 - rob_pos->theta;
	}
	else
	{
		d = R/sin(fabs(last_fall - last_rise)/2.0);
		phi = M_PI/2.0 - (last_fall+last_rise)/2.0 - rob_pos->theta;
	}
	
	*new_x_opp = rob_pos->x + delta*cos(rob_pos->theta) + d*sin(phi);
	*new_y_opp = rob_pos->y + delta*sin(rob_pos->theta) + d*cos(phi);

	return 1;
}

/*! \brief check if there is an opponent in front of the robot
 * 
 * \param[in] cvs controller main structure
 * \return 1 if opponent robot in front of the current robot
 */
void check_opp_front(CtrlStruct *cvs)
{
	// variables declaratio
	CtrlIn *inputs;
	OpponentsPosition *opp_pos;
	RobotPosition *rob_pos;
	
	int i, nb_opp;
	int rise_index[2], fall_index[2];
	double rise[2], fall[2];

	// variables initialization
	inputs  = cvs->inputs;
	rob_pos = cvs->rob_pos;
	opp_pos = cvs->opp_pos;
	nb_opp = opp_pos->nb_opp;

	// no opponent
	if (!nb_opp)
	{
		opp_pos->opp_front = 0;
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
		opp_pos->opp_front = ((norm_dist(rob_pos->x - opp_pos->x[0], rob_pos->y - opp_pos->y[0]) < 0.65) && (((rise[0] <= M_PI/5.0) && (rise[0] >= -M_PI/5.0)) || ((fall[0] <= M_PI/5.0) && (fall[0] >= -M_PI/5.0))));
	}
	else
	{
		opp_pos->opp_front = ((norm_dist(rob_pos->x - opp_pos->x[0], rob_pos->y - opp_pos->y[0]) < 0.65) && (((rise[0] <= M_PI/5.0) && (rise[0] >= -M_PI/5.0)) || ((fall[0] <= M_PI/5.0) && (fall[0] >= -M_PI/5.0))))
		|| ((norm_dist(rob_pos->x - opp_pos->x[1], rob_pos->y - opp_pos->y[1]) < 0.65) && (((rise[1] <= M_PI/5.0) && (rise[1] >= -M_PI/5.0)) || ((fall[1] <= M_PI/5.0) && (fall[1] >= -M_PI/5.0))));
	}
            
	// ----- opponents check computation end ----- //
	
	return;
}

NAMESPACE_CLOSE();
