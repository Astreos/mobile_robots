//-------------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2008 by JF Collard
// Last update : 11/06/2015
//-------------------------------
//

#include "mbs_aux.h"
#include "mbs_data.h"
#include "mbs_tool.h"
#include "nrutil.h"

MbsAux * initMbsAux(MbsData *s)
{
    MbsAux *mbs_aux = (MbsAux*) malloc(sizeof(MbsAux));
    int i;

    #ifdef SENSORKIN
    int j;
    #endif

#if defined DIRDYNARED || defined INVDYNARED
    int njoint, nqu, nqv, nqc, nquc, Ncons, Nuserc;

    #ifdef INVDYNARED
    int nqa;
    #endif

    njoint = s->njoint;
    nqu = s->nqu;
    nqv = s->nqv;
    nqc = s->nqc;

    #ifdef INVDYNARED
    nqa = s->nqa;
    #endif

    nquc = nqu + nqc;
    Nuserc = s->Nuserc;
    Ncons = s->Ncons;

    mbs_aux->NRerr = s->NRerr;
    mbs_aux->MAX_NR_ITER = 100;

    if (Ncons>0)
    {
        mbs_aux->h = mbs_vector(Ncons);
        mbs_aux->Jac = mbs_matrix(Ncons,njoint);
        mbs_aux->bp = mbs_vector(Ncons);
//      mbs_aux->bp = mbs_matrix(Ncons,1);
        mbs_aux->mJv = mbs_matrix(Ncons,nqv);
        mbs_aux->ind_mJv = mbs_ivector(Ncons);
        mbs_aux->mJv_h = mbs_vector(nqv);
//      mbs_aux->mJv_h = mbs_matrix(nqv,1);

#ifdef MATLAB_MEX_FILE
        if (nquc != njoint - Ncons)
            mexErrMsgTxt("nquc != njoint-Ncons");
#endif

        mbs_aux->Juct = mbs_matrix(nquc,Ncons); // inversion des lignes et des colonnes
//      mbs_aux->Juc = mbs_matrix(Ncons,nquc);
        mbs_aux->Bvuc = mbs_matrix(nqv,nquc);
        mbs_aux->jdqd = mbs_vector(Ncons);
    }
    else
    {
        mbs_aux->h = NULL;
        mbs_aux->Jac = NULL;
        mbs_aux->bp = NULL;
        mbs_aux->mJv = NULL;
        mbs_aux->ind_mJv = NULL;
        mbs_aux->mJv_h = NULL;
        mbs_aux->Juct = NULL;
        mbs_aux->Bvuc = NULL;
        mbs_aux->jdqd = NULL;
    }

    if (Nuserc>0)
    {
        mbs_aux->huserc = mbs_vector(Nuserc);
        mbs_aux->Juserc = mbs_matrix(Nuserc,njoint);
        mbs_aux->jdqduserc = mbs_vector(Nuserc);
    }
    else
    {
        mbs_aux->huserc = NULL;
        mbs_aux->Juserc = NULL;
        mbs_aux->jdqduserc = NULL;
    }

    // 11/1/2008 JFC : modification
    if (nquc)
    {
        mbs_aux->iquc = mbs_ivector(nquc);
        mbs_aux->iquc[0]=nquc;
        for(i=1;i<=nqu;i++)
            mbs_aux->iquc[i] = s->qu[i];
        for(i=1;i<=nqc;i++)
            mbs_aux->iquc[i+nqu] = s->qc[i];
    }
    else
        mbs_aux->iquc = NULL;

    allocate_sensor(mbs_aux->psens, njoint);
    init_sensor(mbs_aux->psens, njoint);

#elif defined SENSORKIN
    if (nsensor) {
        mbs_aux->psensorStruct = (MbsSensor**) calloc(nsensor+1,sizeof(MbsSensor*));
        mbs_aux->psensorStruct[0] = NULL;
        for (i=1;i<=nsensor;i++) {
            mbs_aux->psensorStruct[i] = (MbsSensor*) malloc(sizeof(MbsSensor));
            //double *J[7]; //attention: nécessite allocation dynamique en J[7][njoint+1]
            mbs_aux->psensorStruct[i]->J[0] = NULL;
            for (j=1;j<=6;j++) {
                mbs_aux->psensorStruct[i]->J[j] = (double*) calloc(s->njoint+1,sizeof(double));
                mbs_aux->psensorStruct[i]->J[j][0] = s->njoint;
            }
        }
    }
    else
        mbs_aux->psensorStruct = NULL;
#endif

#ifdef DIRDYNARED
    mbs_aux->M = mbs_matrix(njoint,njoint);
    mbs_aux->c = mbs_vector(njoint);
    mbs_aux->F = mbs_vector(njoint);

    if (Ncons>0)
    {
        mbs_aux->BtMvu = mbs_matrix(nquc,nquc);
        mbs_aux->BtMvv = mbs_matrix(nquc,nqv);
        mbs_aux->BtMB = mbs_matrix(nquc,nquc);

        mbs_aux->BtFv = mbs_vector(nquc);
        mbs_aux->MBMb = mbs_vector(nquc); // JFC 4/02/2008 : correction de nqu en nquc
        mbs_aux->Jvtlambda = mbs_vector(nqv);
        mbs_aux->Jvt = mbs_matrix(nqv,nqv);
        mbs_aux->ind_Jvt = mbs_ivector(nqv);
    }

    mbs_aux->Mruc = mbs_matrix(nquc,nquc); // alloué désalloué dans le code : y a pas de raison ?
    mbs_aux->Fruc = mbs_vector(nquc); // alloué désalloué dans le code : y a pas de raison ?

    mbs_aux->Mr = mbs_matrix(nqu,nqu); // alloué désalloué dans le code : idem
    mbs_aux->Fr = mbs_vector(nqu);

    mbs_aux->p_Mr = mbs_vector(nqu);
//  mbs_aux->p_Mr = mbs_ivector(nqu);
    mbs_aux->Qc = mbs_vector(nqc);

#elif defined INVDYNARED
/*
    if (Ncons>0)
    {
        mbs_aux->lambda = mbs_vector(Ncons);
        mbs_aux->mJvt = mbs_matrix(nqv,Ncons);
        mbs_aux->ind_mJvt = mbs_ivector(Ncons);
    }
    else
    {
        mbs_aux->lambda = NULL;
        mbs_aux->mJvt = NULL;
        mbs_aux->ind_mJvt = NULL;
    }
*/
    mbs_aux->phi = mbs_vector(njoint);
    mbs_aux->Qact = mbs_vector(nqa);
    mbs_aux->Qc = mbs_vector(nqc);

    mbs_aux->A = mbs_matrix(nqu,nqa);
    mbs_aux->ind_A = mbs_ivector(nqa);
    mbs_aux->b = mbs_vector(nqu);

    mbs_aux->w = mbs_vector(nqa);
    mbs_aux->v = mbs_matrix(nqa,nqa);
#endif

#ifndef MBS_SIMULINK
    mbs_aux->y = dvector(1,2*nqu);
    mbs_aux->dydx = dvector(1,2*nqu);

    for(i=1;i<=s->nqu;i++)
    {
        mbs_aux->y[i-1] = s->q[s->qu[i]];
        mbs_aux->y[i+s->nqu-1] = s->qd[s->qu[i]];
        mbs_aux->dydx[i-1] = s->qd[s->qu[i]];
        mbs_aux->dydx[i+s->nqu-1] = s->udd[i];
    }
#endif
    /**/
    return mbs_aux;
}

/******************************************************************************/
void freeMbsAux(MbsAux *mbs_aux, MbsData *s)
{

#if defined DIRDYNARED || defined INVDYNARED
    int njoint, nqu, nqv, nqc, nquc, Ncons, Nuserc;

    #ifdef INVDYNARED
    int nqa;
    #endif

    njoint = s->njoint;
    nqu = s->nqu;
    nqv = s->nqv;
    nqc = s->nqc;

    #ifdef INVDYNARED
    nqa = s->nqa;
    #endif

    nquc = nqu + nqc;
    Nuserc = s->Nuserc;
    Ncons = s->Ncons;
    /**/

    if (s->nqu + s->nqc)
    {
        free_mbs_ivector(mbs_aux->iquc);
    }

    if (Ncons>0)
    {
        free_mbs_vector(mbs_aux->h);
        free_mbs_matrix(mbs_aux->Jac,Ncons);
        free_mbs_matrix(mbs_aux->mJv,Ncons);
        free_mbs_ivector(mbs_aux->ind_mJv);
        free_mbs_vector(mbs_aux->mJv_h);
//      free_mbs_matrix(mbs_aux->mJv_h,nqv);
        free_mbs_matrix(mbs_aux->Juct,nquc);
//      free_mbs_matrix(mbs_aux->Juc,Ncons);
        free_mbs_matrix(mbs_aux->Bvuc,nqv);
        free_mbs_vector(mbs_aux->jdqd);
        free_mbs_vector(mbs_aux->bp);
//      free_mbs_matrix(mbs_aux->bp,Ncons);
    }
    if (Nuserc>0)
    {
        free_mbs_vector(mbs_aux->huserc);
        free_mbs_matrix(mbs_aux->Juserc,Nuserc);
        free_mbs_vector(mbs_aux->jdqduserc);
    }

    free_sensor(mbs_aux->psens);

#elif defined SENSORKIN
    int i,j;

    if (nsensor) {
        for (i=1;i<=nsensor;i++)
            for (j=1;j<=6;j++)
                free(mbs_aux->psensorStruct[i]->J[j]);
            free(mbs_aux->psensorStruct[i]);
    }

    free(mbs_aux->psensorStruct);
#endif

#ifdef DIRDYNARED
    free_mbs_matrix(mbs_aux->M,njoint);
    free_mbs_vector(mbs_aux->c);
    free_mbs_vector(mbs_aux->F);

    if (Ncons>0)
    {
        free_mbs_matrix(mbs_aux->BtMvu,nquc);
        free_mbs_matrix(mbs_aux->BtMvv,nquc);
        free_mbs_matrix(mbs_aux->BtMB,nquc);

        free_mbs_vector(mbs_aux->BtFv);
        free_mbs_vector(mbs_aux->MBMb);
        free_mbs_vector(mbs_aux->Jvtlambda);
        free_mbs_matrix(mbs_aux->Jvt, nqv);
        free_mbs_ivector(mbs_aux->ind_Jvt);
        
    }

    free_mbs_matrix(mbs_aux->Mruc,nquc);
    free_mbs_vector(mbs_aux->Fruc);

    free_mbs_matrix(mbs_aux->Mr,nqu);
    free_mbs_vector(mbs_aux->Fr);

    free_mbs_vector(mbs_aux->p_Mr);
//  free_mbs_ivector(mbs_aux->p_Mr);
    free_mbs_vector(mbs_aux->Qc);
#endif

#ifdef INVDYNARED
/*
    if (Ncons>0)
    {
        free_mbs_vector(mbs_aux->lambda);
        free_mbs_matrix(mbs_aux->mJvt,Ncons);
        free_mbs_ivector(mbs_aux->ind_mJvt);
    }
*/
    free_mbs_vector(mbs_aux->phi);
    free_mbs_vector(mbs_aux->Qact);
    free_mbs_vector(mbs_aux->Qc);

    free_mbs_matrix(mbs_aux->A,nqu);
    free_mbs_vector(mbs_aux->b);

    free_mbs_vector(mbs_aux->w);
    free_mbs_matrix(mbs_aux->v,nqa);
#endif

#ifndef MBS_SIMULINK
    free_vector(mbs_aux->y,1,2*nqu);
    free_vector(mbs_aux->dydx,1,2*nqu);
#endif
/**/
    free(mbs_aux);
}
