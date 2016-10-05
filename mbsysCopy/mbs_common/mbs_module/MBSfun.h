//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef MBSfun_h
#define MBSfun_h
/*--------------------*/

 
#include "mbs_data.h"
#include "mbs_aux.h"
#include "mbs_sensor.h"



//void checkMBSdata(MbsData *MBSdata);

//void storeMBSdata(MbsData *s, char *s_name);
					  
/* dirdynared */

#ifdef DIRDYNARED
int dirdynared(MbsAux *mbs_aux,MbsData *s);
#endif

/* invdynared */

#ifdef INVDYNARED
int invdynared(MbsAux *mbs_aux, MbsData *s);
#endif

/**
* Compute the reduced Phi vector of size nquc=nqu+nqc for qu and qc variables (Robotran Basics Eq(7,20))
* necessary for 
*		- equilibrium (while acceleration are set to 0 beforehand)
*		- linearization of MBS system: get the tangent matrices (for control or modal analysis) 
*
* \p Fruc		the force vector in which the force must be returned
* \p aux		the local data structure
* \p s		the MbsData of the system
*/
int mbs_calc_Fruc(double Fruc[], MbsAux *aux, MbsData *s);

int mbs_close_geo(MbsData *s, MbsAux *mbs_aux);

void mbs_close_kin(MbsData *s, MbsAux *mbs_aux);

void mbs_calc_hJ(MbsData *s, MbsAux *mbs_aux);

void mbs_calc_jdqd(MbsData *s, MbsAux *mbs_aux);

void mbs_calc_force(MbsData *s);
					  
/*--------------------*/
#endif
