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

#include "cpp_interface_gestion.h"
#include "user_IO.h"

void user_cons_jdqd(double *jdqd, MbsData *mbs_data, double tsim)
{
	if (!mbs_data->user_IO->cpp_int_created)
	{
		simu_jdqd_init(mbs_data, jdqd);
	}
	else
	{
		simu_jdqd_compute(mbs_data, jdqd);
	}
}
