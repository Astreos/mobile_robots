//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef mbs_linearipk_h
#define mbs_linearipk_h
/*--------------------*/

#include "mbs_linearipk_struct.h"
#include "mbs_data.h"
#include "mbs_aux.h"
#include "useful_functions.h"
#include "MBSfun.h"

int mbs_lineari_fct(double *x, int nx, double *Fx, MbsAux *aux, MbsData *s, int PosOrVel);
int mbs_linearipk(double **GK, MbsLpk *lpk, MbsAux *aux, MbsData *s, int PosOrVel);
MbsLpk* mbs_new_lpk(MbsData *s);
void mbs_delete_lpk(MbsLpk *lpk);


/*--------------------*/
#endif
