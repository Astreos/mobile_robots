/*===========================================================================*
	*
	*  user_sf_IO.c
	*    
	*  Project:    PendulumSpringC
	* 
	*  Generation date: 14-Nov-2014 18:28:15
	* 
	*  (c) Universite catholique de Louvain
	*      2, Place du Levant 
	*      1348 Louvain-la-Neuve 
	*  http://www.robotran.be// 
	*  
 /*===========================================================================*/

#include <stdlib.h>
#include "user_IO.h" 

/*! \brief initialize the UserIO struct
 * 
 * \param[in] ioInfoinformation about UserIO
 * \return initialized structure
 */
UserIO* mbs_new_user_IO(UserIoInfo* ioInfo)
{
	UserIO *uvs;

	uvs = (UserIO*) malloc(sizeof(UserIO));

	uvs->cpp_int_created = 0;
	
	return uvs;
}

void mbs_delete_user_IO(UserIO *uvs)
{
	free(uvs);
}



