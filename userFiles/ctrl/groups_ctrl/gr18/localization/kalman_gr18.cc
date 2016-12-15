#include "kalman_gr18.h"
#include "odometry_gr18.h"
#include "triangulation_gr18.h"
#include "useful_gr18.h"

NAMESPACE_INIT(ctrlGr18);
 
/*! \brief follow a given path
* 
* \param[in,out] cvs controller main structure
*/
void kalman(CtrlStruct *cvs)
{
	// variables declaration
	CtrlIn *inputs;
	RobotPosition *rob_pos;
	RobotPosition *pos_tri;
	KalmanStruct *pos_kalman;
	
	// variables initialization
	rob_pos = cvs->rob_pos;
	pos_tri = cvs->triang_pos;
	pos_kalman = cvs->kalman_pos;
	inputs = cvs->inputs;

	// variables temporaires 
	int i;
	double tab1[9];
	double tab2[9];
	double tab3[9];
	double K[9],P_k_p[9];
	double Q[9],R[9];
	
	double old_kalman_x , old_kalman_y, old_kalman_theta, delta_t;
	
	for(i=0; i < 9; i++) 
	{
		tab1[i] = 0.0;
		tab2[i]=  0.0;
		tab3[i] = 0.0;
		K[i] =    0.0;
		P_k_p[i]= 0.0;
		Q[i] = 0.0;
		R[i] = 0.0;
	}
	
	// Initialization of the Odometry ERROR variances matrix
	Q[0] = 0.00000248;
	Q[4] = 0.00000198;
	Q[8] = 0.00000206;
	
	// Initialization of the Triangulation ERROR variances matrix R
	R[0] = 0.0000022;
	R[4] = 0.0000019;
	R[8] = 0.0000017;

	// Pkp = P_k_1 + Q
	
	if(pos_kalman->P[0] = 0.0)
	{
		pos_kalman->P[0] = Q[0];
		pos_kalman->P[4] = Q[4];
		pos_kalman->P[8] = Q[8];
	}
	
	P_k_p[0] = Q[0] + pos_kalman->P[0];
	P_k_p[4] = Q[4] + pos_kalman->P[4];
	P_k_p[8] = Q[8] + pos_kalman->P[8];
	//matrix_add_diag(Q,P,P_k_p);

	//Kalman coef K = P_kp / (P_k_p + R )
	//matrix_add_diag(P_k_p,R,tab2);
	tab2[0] = P_k_p[0] + R[0];
    tab2[4] = P_k_p[4] + R[4];
    tab2[8] = P_k_p[8] + R[8];
	
	// The inverse of this diagonal matrix
	//matrix_diag_invert(tab2, tab3);
	tab3[0] = 1.0/tab2[0];
	tab3[4] = 1.0/tab2[4];
	tab3[8] = 1.0/tab2[8];
	
	//matrix_multiply_diag(tab3,P_k_p,K);	
	K[0] = tab3[0]*P_k_p[0];
	K[4] = tab3[4]*P_k_p[4];
	K[8] = tab3[8]*P_k_p[8];
	
	//printf(" K[0] = %f and P[0] = %f , P_k_p[0] = %f & tab3[0] : %f & R[0] = %f & Q[0] = %f & tab2[0] : %f \n",K[0],P[0],P_k_p[0],tab3[0],R[0],Q[0],tab2[0]);

	// Nouveau Pk
	pos_kalman->P[0] = (1.0-K[0])*P_k_p[0];
	pos_kalman->P[4] = (1.0-K[4])*P_k_p[4];
	pos_kalman->P[8] = (1.0-K[8])*P_k_p[8];

	// Store the old values of the kalman filtered position 
	old_kalman_x = pos_kalman->x;
	old_kalman_y = pos_kalman->y ;
	old_kalman_theta = pos_kalman->theta;
	
	//operation finale : X_k = X_kp + k*[Yk-Xk_1]
	//matrices diag !
	pos_kalman->x = rob_pos->x + K[0]*(pos_tri->x - pos_kalman->x);
	pos_kalman->y = rob_pos->y + K[4]*(pos_tri->y - pos_kalman->y);
	pos_kalman->theta = rob_pos->theta + K[8]*(pos_tri->theta - pos_kalman->theta);
	
	// low pass filter time increment ('delta_t' is the last argument of the 'first_order_filter' function)
	delta_t = inputs->t - pos_kalman->last_t;
	
	pos_kalman->x = first_order_filter(old_kalman_x, pos_kalman->x, 0.3, delta_t);
	pos_kalman->y = first_order_filter(old_kalman_y, pos_kalman->y, 0.3, delta_t);
	pos_kalman->theta = first_order_filter(old_kalman_theta, pos_kalman->theta, 0.3, delta_t);
	
	pos_kalman->last_t = inputs->t;
	
	set_plot(pos_kalman->x, "x_kalm");
    set_plot(pos_kalman->y, "y_kalm");
    //set_plot(pos_kalman->theta, "theta_kalm");
	
	return;
}

NAMESPACE_CLOSE();
