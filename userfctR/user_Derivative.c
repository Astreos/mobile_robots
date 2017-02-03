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

void user_Derivative(MbsData *mbs_data)
{
	rolling_integrate_pos(mbs_data);
	rolling_set_der(mbs_data);
}
