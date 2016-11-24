#include "path_regulation_gr18.h"
#include "useful_gr18.h"
#include "speed_regulation_gr18.h"
#include "init_pos_gr18.h"
#include <math.h>

//enum {RUN, TURN, CONTINUE}

#define SENS_BAS 0
#define SENS_HAUT 1
#define SENS_GAUCHE 2
#define SENS_DROITE 3

NAMESPACE_INIT(ctrlGr18);

/*! \brief follow a given path
 *
 * \param[in,out] cvs controller main structure
 */
 /*
void follow_path(CtrlStruct *cvs)
{
	if(goal_flag == 0) 
	{
		switch (direction)
		{
		case SENS_BAS:
			switch(flag)
			{
				case TURN:
					if (turn(cvs,-M_PI/2.0, 1) == 1)
					{
						flag = RUN;
					}
					break;
				case RUN:
					if (run_y(cvs, Y_to_y(y-1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
						
					}
					break;
				case CONTINUE:
					
				flag = TURN;
				xy_to_XY(cvs);
				trajectory(cvs, x, y);
				break;
			}
				printf("bas \n");
				
			break;
			
		case SENS_HAUT:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, M_PI/2.0, 1) == 1)
					{
						flag = RUN;
						
					}
					break;
				case RUN:
					if (run_y(cvs, Y_to_y(y+1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
					}
					break;
					case CONTINUE:
				flag = TURN;
				xy_to_XY(cvs);
				trajectory(cvs, x, y-1);
				break;
					
			}
			printf("haut \n");
			
			break;

		case SENS_GAUCHE:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, -M_PI, 1) == 1)
					{
						flag = RUN;
						cvs->rob_pos->theta = M_PI;
					}
					break;
				case RUN:
					if (run_x(cvs, X_to_x(x-1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
					}
					break;
					case CONTINUE:
					xy_to_XY(cvs);
					flag = TURN;
					trajectory(cvs, x, y);
				break;
					
			}
			printf("gauche \n");
			
			break;

		case SENS_DROITE:
		switch(flag)
			{
				case TURN:
					if (turn(cvs, 0, 1) == 1)
					{
						flag = RUN;
					}
					break;
				case RUN:
					if (run_x(cvs, X_to_x(x+1)) == 1)
					{
						xy_to_XY(cvs);
						flag = TURN;
						trajectory(cvs, x, y);
					}
					break;
					case CONTINUE:
						xy_to_XY(cvs);
				flag = TURN;
					trajectory(cvs, x, y-1);
				break;
					
			}
		printf("droite \n");
				
			break;
		}
	}
}
* */

int turn(CtrlStruct *cvs, double theta_ref, int sens)
{
    double dt;
    int team_id;
    int dir;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    team_id = cvs->team_id;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    if (sens == 0)
    {
        dir = team(team_id);
    }
    else
    {
        dir = team(team_id)*sens;
    }

    // ----- Wheels regulation computation start ----- //

    int Kp = 20; //Kc = 90
    float Ti = 100; //Tosc = 0.0057143 s

    pos_reg->int_error_r = (theta_ref - rob_pos->theta)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = -(theta_ref - rob_pos->theta)*dt + pos_reg->int_error_l;

    speed_regulation(cvs, (Kp*(theta_ref - rob_pos->theta) + (Kp/Ti)*limit_range(pos_reg->int_error_r, -2.0*M_PI, 2.0*M_PI))*dir, -(Kp*(theta_ref - rob_pos->theta) + (Kp/Ti)*limit_range(pos_reg->int_error_r, -2.0*M_PI, 2.0*M_PI))*dir);

    // ----- Wheels regulation computation end ----- //
    
    // last update time
    pos_reg->last_t = inputs->t;

    if (fabs(theta_ref - rob_pos->theta) < 0.001)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int run_x(CtrlStruct *cvs, double x_ref)
{
    double dt;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    int Kp = 50;
    float Ti = 100;

    pos_reg->int_error_r = (x_ref - rob_pos->x)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = (x_ref - rob_pos->x)*dt + pos_reg->int_error_l;

    speed_regulation(cvs, -Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -3.0, 3.0), -Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -3.0, 3.0));

    // ----- Wheels regulation computation end ----- //
    
    // last update time
    pos_reg->last_t = inputs->t;

    if (fabs(x_ref - rob_pos->x) < 0.0001)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int run_y(CtrlStruct *cvs, double y_ref)
{
    double dt;
    int flag = 0;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    int Kp = 50;
    float Ti = 100;

    pos_reg->int_error_r = (y_ref - rob_pos->y)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = (y_ref - rob_pos->y)*dt + pos_reg->int_error_l;

    speed_regulation(cvs, -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -0.5, 0.5), -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_l, -0.5, 0.5));
    
    //printf("%f \n", y_ref - rob_pos->y);

    // ----- Wheels regulation computation end ----- //
    
    // last update time
    pos_reg->last_t = inputs->t;

    if (fabs(y_ref - rob_pos->y) < 0.0001)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

NAMESPACE_CLOSE();
