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
        
        if (inputs->t <= (-15 + delta_t)) {
            opp_pos->x[0] = 0.65;
            opp_pos->y[0] = -1.10;
        }
        
        double old_opp_pos_x = opp_pos->x[0];
        double old_opp_pos_y = opp_pos->y[0];

	single_opp_tower(rise_1, fall_1, rob_pos->x, rob_pos->y, rob_pos->theta, opp_pos->x, opp_pos->y);
        
        opp_pos->x[0] = first_order_filter(old_opp_pos_x, opp_pos->x[0], 0.7, delta_t);
        opp_pos->y[0] = first_order_filter(old_opp_pos_y, opp_pos->y[0], 0.7, delta_t);
        
        //set_plot(opp_pos->x[0], "opp_x_[m]");
        //set_plot(opp_pos->y[0], "opp_y_[m]");
        
        //set_plot(rise_1, "rise");
        //set_plot(fall_1, "fall");
        
        //set_plot(check_opp_front(cvs), "detection");

	// ----- opponents position computation end ----- //
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
            if ((last_rise >= 0) && (last_fall <= 0)) {
                last_fall = last_fall + 2.0*M_PI;
            }
            
            *new_x_opp = rob_x + 0.083*cos(rob_theta) + (0.04/sin((last_fall-last_rise)/2.0))*sin((last_fall+last_rise)/2.0);
            *new_y_opp = rob_y + 0.083*sin(rob_theta) - (0.04/sin((last_fall-last_rise)/2.0))*cos((last_fall+last_rise)/2.0);
            
            //set_plot((0.04/sin((last_fall-last_rise)/2.0))*sin((last_fall+last_rise)/2.0), "delta_x_[m]");
            //set_plot((0.04/sin((last_fall-last_rise)/2.0))*cos((last_fall+last_rise)/2.0) + (0.04/sin((last_fall-last_rise)/2.0))*cos((last_fall+last_rise)/2.0)*cos(rob_theta), "delta_y_[m]");
            //set_plot(rob_theta, "rob_theta_[rad]");
            
            set_plot(*new_x_opp, "opp_x_[m]");
            set_plot(*new_y_opp, "opp_y_[m]");

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

	for(i=0; i<nb_opp; i++)
	{
		// ----- opponents check computation start ----- //
            
            if ((norm_dist(rob_pos->x - opp_pos->x[i], rob_pos->y - opp_pos->y[i]) < 0.50) && (((rise[i] <= M_PI/6.0) && (rise[i] >= -M_PI/6.0)) || ((fall[i] <= M_PI/6.0) && (fall[i] >= -M_PI/6.0)))) {
                return 1;
            }
            
		// ----- opponents check computation end ----- //
	}

	return 0;
}

NAMESPACE_CLOSE();
