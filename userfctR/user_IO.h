/*===========================================================================*
	*
	*  user_sf_IO.h
	*	
	*  Project:	PendulumSpringC
	* 
	*  Generation date: 14-Nov-2014 18:28:15
	* 
	*  (c) Universite catholique de Louvain
	*      D�partement de M�canique 
	*      Unit� de Production M�canique et Machines 
	*      2, Place du Levant 
	*      1348 Louvain-la-Neuve 
	*  http://www.robotran.be// 
	*  
 /*===========================================================================*/

#ifndef UsersfIO_h
#define UsersfIO_h
/*--------------------*/
 
#ifdef ACCELRED 
#define S_FUNCTION_NAME  mbs_sf_accelred_PendulumSpringC 
#elif defined DIRDYNARED 
#define S_FUNCTION_NAME  mbs_sf_dirdynared_PendulumSpringC 
#elif defined INVDYNARED 
#define S_FUNCTION_NAME  mbs_sf_invdynared_PendulumSpringC 
#elif defined SENSORKIN 
#define S_FUNCTION_NAME  mbs_sf_sensorkin_PendulumSpringC 
#endif 
 
#define SF_N_USER_INPUT 0 
#define SF_N_USER_OUTPUT 0 

#include "mbs_user_interface.h"

struct UserIO 
{
	int cpp_int_created; ///< 1 if C++ interface created, 0 otherwise
};



/*--------------------*/
#endif
