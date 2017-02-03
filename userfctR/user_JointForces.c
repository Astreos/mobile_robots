//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include "math.h" 

#include "MBSdef.h"
#include "mbs_data.h"
#include "user_model.h"

#include "set_output.h"

double* user_JointForces(MbsData *mbs_data, double tsim)
{
	return mbs_data->Qq;
}
