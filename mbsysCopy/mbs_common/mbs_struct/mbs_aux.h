//-------------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2008 by JF Collard
// Last update : 01/10/2008
//-------------------------------
//
// Gestion via Bugzilla :
// 01/10/2008 : JFC : Bug n°39
//

#ifndef mbs_aux_h
#define mbs_aux_h
/*--------------------*/

#include "mbs_sensor.h"
#include "mbs_data.h"

#ifndef ACCELRED
 
typedef struct MbsAux_tag
{
#if defined DIRDYNARED || defined INVDYNARED
        double norm_h;
        double NRerr;
        int MAX_NR_ITER;

        double *h, **Jac;
        double *huserc, **Juserc;
        double **mJv;
        int *ind_mJv;

        double *mJv_h;
//      double **mJv_h;
        
        int *iquc;
        double **Juct; // JFC: 15/01/2008 : changement nom
//      double **Juc; // JFC: ajout temporaire

        double **Bvuc;
		
		double *Jvtlambda; ///< store Jv^T * lambda (before computing lambda associated to constraints)
        double **Jvt;
        int *ind_Jvt;
        

        double *jdqd;
        double *jdqduserc;
        
        double * bp;
//      double **bp;

        MbsSensor psens[1]; // temporary sensor pointer

#elif defined SENSORKIN
    MbsSensor **psensorStruct;
#endif
/**/

#ifdef DIRDYNARED
        double *c, **M;
        double *F;

        double **BtMvu,**BtMvv,**BtMB;
        double *BtFv,*MBMb;

        double **Mruc;
        double *Fruc;

        double **Mr;
        double *Fr;

        double *p_Mr;
//      int *p_Mr;
        double *Qc;
#endif

#ifdef INVDYNARED
        double *phi;
        double *Qact;
        double *Qc;

        double **A;
        int *ind_A;
        double *b;

        double *w;
        double **v;

//      double **mJvt;
//      int    *ind_mJvt;
//      double *lambda;
#endif

#ifndef MBS_SIMULINK
        double x, *y, *dydx;
#endif
} MbsAux;


MbsAux* initMbsAux(MbsData *mbs_data);

void freeMbsAux(MbsAux* mbs_aux, MbsData *mbs_data);

#else
typedef void* MbsAux;
#endif
#endif
