//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include "math.h"

#include "mbs_data.h"
#include "user_IO.h"

double user_LinkForces(double Z, double Zd, MbsData *mbs_data, double tsim, int ilnk)
{
	
	double Flink = 0;
	
/*-- Begin of user code --*/

	/* 
		// example for a spring-damper in link number 2
		if(ilnk==1){
			double K = 200;
			double C = 20;
			double Z0 = 0.1;

			double Fsrping = K*(Z-Z0);
			double Fdamp = C*Zd;
			Flink = K*(Z-Z0) + C*Zd;
		}
	*/
	

/*-- End of user code --*/    
	
	return  Flink;
}
