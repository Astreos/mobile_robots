//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//
//
//---------------------------

#include "math.h"

#include "MBSdef.h"
#include "mbs_data.h"
#include "mbs_project_interface.h"
#include "cpp_interface_gestion.h"
#include "user_all_id.h"
#include "robot_id.h"

double* user_ExtForces(double PxF[4], double RxF[4][4], 
					   double VxF[4], double OMxF[4], 
					   double AxF[4], double OMPxF[4], 
					   MbsData *mbs_data, double tsim,int ixF)
{
	double Fx=0.0, Fy=0.0, Fz=0.0;
	double Mx=0.0, My=0.0, Mz=0.0;
	double dxF[4] ={0.0, 0.0, 0.0, 0.0};

	double *SWr = mbs_data->SWr[ixF];

	// default application point of the force: anchor point to which it is attached
	int idpt = 0;
	int robot_id;
	idpt = mbs_data->xfidpt[ixF];
	dxF[1] = mbs_data->dpt[1][idpt];
	dxF[2] = mbs_data->dpt[2][idpt];
	dxF[3] = mbs_data->dpt[3][idpt];

	switch(ixF)
	{
		case Fsens_robot_B_id: robot_id = ROBOT_B; break;
		case Fsens_robot_R_id: robot_id = ROBOT_R; break;
		case Fsens_robot_Y_id: robot_id = ROBOT_Y; break;
		case Fsens_robot_W_id: robot_id = ROBOT_W; break;

		default:
			printf("Error: unknown robot Fsens ID: %d\n", ixF);
			exit(EXIT_FAILURE);
	}

	if (ixF == 1)
	{
		upate_position_contact(mbs_data);
	}
	
	get_robot_force_torque(mbs_data, robot_id, &Fx, &Fy, &Mz);

	SWr[1]=Fx;
	SWr[2]=Fy;
	SWr[3]=Fz;
	SWr[4]=Mx;
	SWr[5]=My;
	SWr[6]=Mz;
	SWr[7]=dxF[1];
	SWr[8]=dxF[2];
	SWr[9]=dxF[3];

	return SWr;
}
