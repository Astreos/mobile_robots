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
#include "kalman_gr18.h"

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
	if (cvs == NULL) {exit(0);}

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
	if (cvs->rob_pos == NULL) {exit(0);}

	cvs->rob_pos->x = 0.0;
	cvs->rob_pos->y = 0.0;
	cvs->rob_pos->theta  = 0.0;
	
	cvs->rob_pos->last_t = 0.0;

	// triangulation position
	cvs->triang_pos = (RobotPosition*) malloc(sizeof(RobotPosition));
	if (cvs->triang_pos == NULL) {exit(0);}

	cvs->triang_pos->x = 0.0;
	cvs->triang_pos->y = 0.0;
	cvs->triang_pos->theta  = 0.0;
	
	cvs->triang_pos->last_t = 0.0;

	// Kalman position
	cvs->kalman_pos = (KalmanStruct*) malloc(sizeof(KalmanStruct));
	if (cvs->kalman_pos == NULL) {exit(0);}
	
	cvs->kalman_pos->x = 0.0;
	cvs->kalman_pos->y = 0.0;
	cvs->kalman_pos->theta  = 0.0;
	
	cvs->kalman_pos->last_t = 0.0;
	
	cvs->kalman_pos->P = (double*) malloc(9*sizeof(double));
	if (cvs->kalman_pos->P == NULL) {exit(0);}
	
	for(i=0; i<9; i++)
	{
		cvs->kalman_pos->P[i] = 0.0;
	}
	
	// opponents position
	cvs->opp_pos = (OpponentsPosition*) malloc(sizeof(OpponentsPosition));
	if (cvs->opp_pos == NULL) {exit(0);}

	for(i=0; i<2; i++)
	{
		cvs->opp_pos->x[i] = 0.0;
		cvs->opp_pos->y[i] = 0.0;
	}

	cvs->opp_pos->last_t = 0.0;
	cvs->opp_pos->nb_opp = inputs->nb_opponents;

	// speed regulation
	cvs->sp_reg = (SpeedRegulation*) malloc(sizeof(SpeedRegulation));
	if (cvs->sp_reg == NULL) {exit(0);}

	cvs->sp_reg->int_error_r = 0.0;
	cvs->sp_reg->int_error_l = 0.0;

	cvs->sp_reg->last_t = 0.0;
        
	// position regulation
	cvs->pos_reg = (PosRegulation*) malloc(sizeof(PosRegulation));
	if (cvs->pos_reg == NULL) {exit(0);}

	cvs->pos_reg->int_error_r = 0.0;
	cvs->pos_reg->int_error_l = 0.0;

	cvs->pos_reg->last_t = 0.0;

	// calibration
	cvs->calib = (RobotCalibration*) malloc(sizeof(RobotCalibration));
	if (cvs->calib == NULL) {exit(0);}

	cvs->calib->flag = 0;
	cvs->calib->t_flag = 0.0;

	// strategy
	cvs->strat = init_strategy();

	// path-planning
	cvs->path = init_path_planning();

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
	free(cvs->kalman_pos);
	free(cvs);
	
	return;
}

NAMESPACE_CLOSE();
