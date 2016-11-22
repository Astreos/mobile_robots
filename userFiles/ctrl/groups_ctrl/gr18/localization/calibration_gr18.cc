#include "calibration_gr18.h"
#include "speed_regulation_gr18.h"
#include "odometry_gr18.h"
#include "useful_gr18.h"
#include "init_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

#define DEG_TO_RAD (M_PI/180.0) ///< convertion from degrees to radians

// calibration states
enum {CALIB_START, CALIB_STATE_A, CALIB_STATE_B, CALIB_STATE_C, CALIB_STATE_D, CALIB_STATE_E, CALIB_FINISH};

/*! \brief calibration of the robot to calibrate its position
 * 
 * \param[in,out] cvs controller main structure
 * 
 * This FSM can be adapted, depending on the map and on the robots initial position.
 */
void calibration(CtrlStruct *cvs)
{
	// variables declaration
	int team_id;
	double t;

	CtrlIn *inputs;
	RobotCalibration *calib;
	RobotPosition *rob_pos;

	// variables initialization
	inputs = cvs->inputs;
	calib  = cvs->calib;

	rob_pos = cvs->rob_pos;

	t = inputs->t;
	team_id = cvs->team_id;

	// finite state machine (FSM)        
	switch (calib->flag)
	{
		case CALIB_START: // start calibration

                    speed_regulation(cvs, 0, 0);

                    calib->flag = CALIB_STATE_A;
                    calib->t_flag = t;

                    break;
                    
                case CALIB_STATE_A: // state A
                    
                    speed_regulation(cvs, -7, -7);

                    if ((inputs->u_switch[0] == 1) && (inputs->u_switch[1] == 1) && (t - calib->t_flag > 2.0)) {
                        speed_regulation(cvs, 0, 0);

                        rob_pos->y = 1.44*team(team_id);
                        rob_pos->theta = -M_PI/2.0*team(team_id);
                        
                        calib->flag = CALIB_STATE_B;
                        calib->t_flag = t;
                    }

                    break;

		case CALIB_STATE_B: // state B

                    speed_regulation(cvs, 7, 7);

                    if ((rob_pos->y <= 1.15 && !team_id) || (rob_pos->y >= -1.15 && team_id)) {
                        speed_regulation(cvs, 0, 0);

                        calib->flag = CALIB_STATE_C;
                        calib->t_flag = t;
                    }

                    break;

		case CALIB_STATE_C: // state C

                    speed_regulation(cvs, -7*team(team_id), 7*team(team_id));

                    if (((rob_pos->theta > 0) && (rob_pos->theta <= M_PI) && !team_id) || ((rob_pos->theta < 0) && (rob_pos->theta <= M_PI) && team_id)) {
			speed_regulation(cvs, 0, 0);

                        calib->flag = CALIB_STATE_D;
                        calib->t_flag = t;
                    }

                    break;

		case CALIB_STATE_D: // state D
                    
                    speed_regulation(cvs, -7, -7);

                    if ((inputs->u_switch[0] == 1) && (inputs->u_switch[1] == 1) && (t - calib->t_flag > 2.0)) {
                        speed_regulation(cvs, 0, 0);
                        
                        rob_pos->x = 0.94;
                        
                        calib->flag = CALIB_STATE_E;
                        calib->t_flag = t;
                    }

                    break;
                    
                case CALIB_STATE_E: // state E
                    
                    speed_regulation(cvs, 7, 7);

                    if (rob_pos->x <= 0.65) {
			speed_regulation(cvs, 0, 0);
                        
                        calib->flag = CALIB_FINISH;
                        calib->t_flag = t;
                    }

                    break;

		case CALIB_FINISH: // wait before the match is starting
                    
                    speed_regulation(cvs, 7*team(team_id), -7*team(team_id));
                    
                    if (((rob_pos->theta < 0) && (rob_pos->theta >= -M_PI/2.0) && !team_id) || ((rob_pos->theta > 0) && (rob_pos->theta <= M_PI/2.0) && team_id)) {
			speed_regulation(cvs, 0, 0);
                        
                        cvs->main_state = WAIT_INIT_STATE;
                    }
                    
                    break;

		default:
			printf("Error: unknown state : %d !\n", calib->flag);
			exit(EXIT_FAILURE);
	}
}

NAMESPACE_CLOSE();

/*
    // go to state B after 2 seconds
    if (t - calib->t_flag > 2.0)
    {
        calib->flag = CALIB_STATE_B;
        calib->t_flag = t;
    }
    break;
*/
