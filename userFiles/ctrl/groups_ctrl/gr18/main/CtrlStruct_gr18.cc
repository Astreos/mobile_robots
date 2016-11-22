#include "CtrlStruct_gr18.h"
#include "namespace_ctrl.h"
#include "init_pos_gr18.h"
#include "odometry_gr18.h"
#include "opp_pos_gr18.h"
#include "speed_regulation_gr18.h"
#include "path_regulation_gr18.h"
#include "calibration_gr18.h"
#include "path_planning_gr18.h"
#include "strategy_gr18.h"

NAMESPACE_INIT(ctrlGr18);

/*! \brief initialize the controller structure
 * 
 * \param[in] inputs inputs of the controller
 * \param[in] outputs outputs of the controller
 * \return controller main structure
 */
CtrlStruct* init_CtrlStruct(CtrlIn *inputs, CtrlOut *outputs)
{
	int i;
	CtrlStruct *cvs;

	cvs = (CtrlStruct*) malloc(sizeof(CtrlStruct));

	// io
	cvs->inputs  = inputs;
	cvs->outputs = outputs;

	// main state
	cvs->main_state = CALIB_STATE;

	// IDs (will be erased in the initialization)
	cvs->robot_id = ROBOT_B;
	cvs->team_id  = TEAM_A;

	// robot position
	cvs->rob_pos = (RobotPosition*) malloc(sizeof(RobotPosition));

	cvs->rob_pos->x = 0.0;
	cvs->rob_pos->y = 0.0;

	cvs->rob_pos->theta  = 0.0;
	cvs->rob_pos->last_t = 0.0;

	// triangulation position
	cvs->triang_pos = (RobotPosition*) malloc(sizeof(RobotPosition));

	cvs->triang_pos->x = 0.0;
	cvs->triang_pos->y = 0.0;

	cvs->triang_pos->theta  = 0.0;
	cvs->triang_pos->last_t = 0.0;

	// opponents position
	cvs->opp_pos = (OpponentsPosition*) malloc(sizeof(OpponentsPosition));

	for(i=0; i<2; i++)
	{
		cvs->opp_pos->x[i] = 0.0;
		cvs->opp_pos->y[i] = 0.0;
	}

	cvs->opp_pos->last_t = 0.0;
	cvs->opp_pos->nb_opp = inputs->nb_opponents;

	// speed regulation
	cvs->sp_reg = (SpeedRegulation*) malloc(sizeof(SpeedRegulation));

	cvs->sp_reg->int_error_r = 0.0;
	cvs->sp_reg->int_error_l = 0.0;

	cvs->sp_reg->last_t = 0.0;
        
        // position regulation
	cvs->pos_reg = (PosRegulation*) malloc(sizeof(PosRegulation));

	cvs->pos_reg->int_error_r = 0.0;
	cvs->pos_reg->int_error_l = 0.0;

	cvs->pos_reg->last_t = 0.0;

	// calibration
	cvs->calib = (RobotCalibration*) malloc(sizeof(RobotCalibration));

	cvs->calib->flag = 0;
	cvs->calib->t_flag = 0.0;

	// strategy
	cvs->strat = init_strategy();

	// path-planning
	cvs->path = init_path_planning(cvs, cvs->rob_pos);

	return cvs;
}

/*! \brief release controller main structure memory
 * 
 * \param[in] cvs controller main structure
 */
void free_CtrlStruct(CtrlStruct *cvs)
{
	free_path_planning(cvs->path);
	free_strategy(cvs->strat);
	free(cvs->calib);
	free(cvs->sp_reg);
    free(cvs->pos_reg);
	free(cvs->opp_pos);
	free(cvs->rob_pos);
	free(cvs->triang_pos);

	free(cvs);
}

NAMESPACE_CLOSE();
