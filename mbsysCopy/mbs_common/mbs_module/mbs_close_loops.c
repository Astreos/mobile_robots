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
// 01/10/2008 : JFC : Bug n°40
//

#include "MBSfun.h"
#include "nrfct.h"
#include "mbs_project_interface.h"


double norm_vector(double *v, int n);
double norminf_vector(double *v, int n);



int mbs_close_geo(MbsData *s, MbsAux *mbs_aux)
{
    int i,j;
    int iter=0;
    int nL,nC;

    double d;

    iter = 0;
    mbs_aux->norm_h=1.0;
    while((mbs_aux->norm_h > mbs_aux->NRerr) && (iter++ <= mbs_aux->MAX_NR_ITER))
    {

        // Calcul des contraintes et de la Jacobienne
        mbs_calc_hJ(s,mbs_aux);



        // Norme des contraintes (en supposant que toutes les contraintes indépendantes sont au début ???)
        mbs_aux->norm_h = norminf_vector(mbs_aux->h,s->nhu);

        // -Jv
        nL = s->nhu;
        nC = s->nqv;
        for(i=1;i<=nL;i++)
        {
            for(j=1;j<=nC;j++)
            {
                mbs_aux->mJv[i][j] = -mbs_aux->Jac[s->hu[i]][s->qv[j]];
            }
        }
		
        // Décomposition LU de la matrice -Jv
        ludcmp(mbs_aux->mJv,s->nqv,mbs_aux->ind_mJv,&d);

        if(mbs_aux->norm_h > mbs_aux->NRerr)
        {
            // err
            for(i=1;i<=s->nhu;i++)
            {
                mbs_aux->mJv_h[i] = mbs_aux->h[s->hu[i]];
//              mbs_aux->mJv_h[i][1] = mbs_aux->h[s->hu[i]];
            }
            lubksb(mbs_aux->mJv,s->nqv,mbs_aux->ind_mJv,mbs_aux->mJv_h);
//          gaussj(mbs_aux->mJv, s->nqv, mbs_aux->mJv_h, 1);

            // Correction des qv
            for(i=1;i<=s->nhu;i++)
            {
                s->q[s->qv[i]] += mbs_aux->mJv_h[i];
//              s->q[s->qv[i]] += mbs_aux->mJv_h[i][1];
            }
        }
    }
    return iter;
}

void mbs_close_kin(MbsData *s, MbsAux *mbs_aux)
{
    int i,j,k;
    int nL,nC,nk;

    double term;

    nL = s->nhu;
    nC = mbs_aux->iquc[0];
    for(i=1;i<=nL;i++)
    {
        for(j=1;j<=nC;j++)
        {
            mbs_aux->Juct[j][i] = mbs_aux->Jac[s->hu[i]][mbs_aux->iquc[j]];
//          mbs_aux->Juc[i][j] = mbs_aux->Jac[s->hu[i]][mbs_aux->iquc[j]];
        }
    }

    // calcul de la matrice de couplage des vitesses
    for (j=1;j<=nC;j++)
    {
        lubksb(mbs_aux->mJv,s->nqv,mbs_aux->ind_mJv,mbs_aux->Juct[j]);
    }
//  gaussj(mbs_aux->mJv, s->nqv, mbs_aux->Juc, nC);
    for(i=1;i<=nL;i++)
    {
        for(j=1;j<=nC;j++)
        {
            mbs_aux->Bvuc[i][j] = mbs_aux->Juct[j][i];
//          mbs_aux->Bvuc[i][j] = mbs_aux->Juc[i][j];
        }
    }

    // calcul des vitesses dépendantes (qdv = Bvuc * qduc)
    nL = s->nqv;
    nk = mbs_aux->iquc[0];
    for(i=1;i<=nL;i++)
    {
        term = 0.0;
        for(k=1;k<=nk;k++)
        {
            term += mbs_aux->Bvuc[i][k]*s->qd[mbs_aux->iquc[k]];
        }
        s->qd[s->qv[i]] = term;
    }

}

void mbs_calc_hJ(MbsData *s, MbsAux *mbs_aux)
{
    int i,j;


    // contraintes de fermeture de boucles
    mbs_cons_hJ(mbs_aux->h,mbs_aux->Jac,s,s->tsim);
    // contraintes user
    if (s->Nuserc>0) {
        user_cons_hJ(mbs_aux->huserc, mbs_aux->Juserc, s, s->tsim);

        // ajout des contraintes user aux contraintes de fermeture
        for (i=1;i<=s->Nuserc;i++) {
            mbs_aux->h[s->Nloopc+i] = mbs_aux->huserc[i];
            for (j=1;j<=s->njoint;j++)
                mbs_aux->Jac[s->Nloopc+i][j] = mbs_aux->Juserc[i][j];
        }
    }
}

void mbs_calc_jdqd(MbsData *s, MbsAux *mbs_aux)
{
  int i;

  // compute contribution of symbolic constraints to jdqd
  mbs_cons_jdqd(mbs_aux->jdqd,s,s->tsim);

  // compute contribution of user constraints to jdqd
  if (s->Nuserc>0) {
    user_cons_jdqd(mbs_aux->jdqduserc, s, s->tsim);

    // ajout des contraintes user aux contraintes de fermeture
    for (i=1;i<=s->Nuserc;i++) {
      mbs_aux->jdqd[s->Nloopc+i] = mbs_aux->jdqduserc[i];
    }
  }

  // bp = (-Jv)\jdqd
  for(i=1;i<=s->nhu;i++)
  {
    mbs_aux->bp[i] = mbs_aux->jdqd[s->hu[i]];
  }
  lubksb(mbs_aux->mJv,s->nqv,mbs_aux->ind_mJv,mbs_aux->bp);

}
