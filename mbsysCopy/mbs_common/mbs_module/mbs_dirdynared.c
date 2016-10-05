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
// 01/10/2008 : JFC : Bug n°41
//

#ifdef DIRDYNARED

#include "MBSfun.h"
#include "math.h"
#include "nrfct.h"
#include "mbs_project_interface.h"

int dirdynared(MbsAux *mbs_aux,MbsData *s)
{
    int i,j,k;
    int iter=0;
    int nL,nC,nk;

    double term;

    int has_a_line_of_zeros;

    // Compute driven variable -> user function
    if (s->nqc>0) user_DrivenJoints(s,s->tsim);

    // Solve constraints
    if (s->Ncons > 0)
    {
        // Geometric resolution
        iter = mbs_close_geo(s, mbs_aux);

        if (iter>=mbs_aux->MAX_NR_ITER)
        {
            return -1;
        }

        // Kinematic resolution
        mbs_close_kin(s, mbs_aux);

    // Acceleration dependence -> jdqd
    mbs_calc_jdqd(s, mbs_aux);

    }//if(s->Ncons > 0)



  // compute forces on the system
  mbs_calc_force(s);


    // calcul de la matrice de masse et du vecteur des forces non linéaires
    mbs_dirdyna(mbs_aux->M,mbs_aux->c,s,s->tsim);

    for(i=1;i<=s->njoint;i++)
    {
        mbs_aux->F[i] = mbs_aux->c[i] - s->Qq[i];
    }

    // calcul de la matrice de masse et du vecteur des forces reduites (DAE => ODE)
    if (s->nqv>0)
    {
        //  Mr_uc = Muc_uc + Muc_v*Bvuc + Bvuc'*Mv_uc + Bvuc'*Mvv*Bvuc;
        //  Mr_uc = Muc_uc + Muc_v*Bvuc + (Muc_v*Bvuc)' + (Bvuc'*Mvv)*Bvuc;

        //  Fr_uc = Fuc + Muc_v*bprim + Bvuc'*Fv + Bvuc'*Mvv*bprim;
        //  Fr_uc = Fuc + (Muc_v+ Bvuc'*Mvv)*bprim + Bvuc'*Fv ;

        // BtMvu = Bvuc'*Mv_uc
        nL = nC = mbs_aux->iquc[0];
        nk = s->nqv;
        for(i=1;i<=nL;i++)
        {
            for(j=1;j<=nC;j++)
            {
                term = 0.0;
                for(k=1;k<=nk;k++)
                {
                    term += mbs_aux->Bvuc[k][i] * mbs_aux->M[s->qv[k]][mbs_aux->iquc[j]];
                }
                mbs_aux->BtMvu[i][j] = term;
            }
        }

        // BtMvv = Bvuc'*Mvv
        nL = mbs_aux->iquc[0];
        nC = nk = s->nqv;
        for(i=1;i<=nL;i++)
        {
            for(j=1;j<=nC;j++)
            {
                term = 0.0;
                for(k=1;k<=nk;k++)
                {
                    term += mbs_aux->Bvuc[k][i] * mbs_aux->M[s->qv[k]][s->qv[j]];
                }
                mbs_aux->BtMvv[i][j] = term;
            }
        }

        // BtFv = Bvuc'*Fv
        nL = mbs_aux->iquc[0];
        nk = s->nqv;
        for(i=1;i<=nL;i++)
        {
            term = 0.0;
            for(k=1;k<=nk;k++)
            {
                term += mbs_aux->Bvuc[k][i] * mbs_aux->F[s->qv[k]];
            }
            mbs_aux->BtFv[i] = term;
        }

        // BtMB = BtMvv*Bvuc
        nL = nC = mbs_aux->iquc[0];
        nk = s->nqv;
        for(i=1;i<=nL;i++)
        {
            for(j=1;j<=nC;j++)
            {
                term = 0.0;
                for(k=1;k<=nk;k++)
                {
                    term += mbs_aux->BtMvv[i][k] * mbs_aux->Bvuc[k][j];
                }
                mbs_aux->BtMB[i][j] = term;
            }
        }

        // MBMb = (Muv+Bvuc'*Mvv)*bprim
        nL = mbs_aux->iquc[0];
        nk = s->nqv;
        for(i=1;i<=nL;i++)
        {
            term = 0.0;
            for(k=1;k<=nk;k++)
            {
                term += (mbs_aux->M[mbs_aux->iquc[i]][s->qv[k]] + mbs_aux->BtMvv[i][k]) * mbs_aux->bp[k];
            }
            mbs_aux->MBMb[i] = term;
        }

/*
        nL = mbs_aux->iquc[0];
        nk = s->nqv;
        for(i=1;i<=nL;i++)
        {
            // BtMvu = Bvuc'*Mvu
            nC = mbs_aux->iquc[0];
            for(j=1;j<=nC;j++)
            {
                term = 0.0;
                for(k=1;k<=nk;k++)
                {
                    term += mbs_aux->Bvuc[k][i] * mbs_aux->M[s->qv[k]][mbs_aux->iquc[j]];
                }
                mbs_aux->BtMvu[i][j] = term;
            }

            // BtMvv = Bvuc'*Mvv
            nC = s->nqv;
            for(j=1;j<=nC;j++)
            {
                term = 0.0;
                for(k=1;k<=nk;k++)
                {
                    term += mbs_aux->Bvuc[k][i] * mbs_aux->M[s->qv[k]][s->qv[j]];
                }
                mbs_aux->BtMvv[i][j] = term;
            }

            // BtFv = Bvuc'*Fv
            term = 0.0;
            for(k=1;k<=nk;k++)
            {
                term += mbs_aux->Bvuc[k][i] * mbs_aux->F[s->qv[k]];
            }
            mbs_aux->BtFv[i] = term;

            // BtMB = BtMvv*Bvuc
            nC = mbs_aux->iquc[0];
            for(j=1;j<=nC;j++)
            {
                term = 0.0;
                for(k=1;k<=nk;k++)
                {
                    term += mbs_aux->BtMvv[i][k] * mbs_aux->Bvuc[k][j];
                }
                mbs_aux->BtMB[i][j] = term;
            }

            // MBMb = (Muv+Bvuc'*Mvv)*bprim
            nC = s->nqv;
            term = 0.0;
            for(k=1;k<=nk;k++)
            {
                term += (mbs_aux->M[mbs_aux->iquc[i]][s->qv[k]] + mbs_aux->BtMvv[i][k]) * mbs_aux->bp[k];
            }
            mbs_aux->MBMb[i] = term;
        }
*/

        // Mruc Fruc
        nL = nC = mbs_aux->iquc[0];
        for(i=1;i<=nL;i++)
        {
            for(j=1;j<=nC;j++)
            {
                mbs_aux->Mruc[i][j] = mbs_aux->M[mbs_aux->iquc[i]][mbs_aux->iquc[j]] +
                    mbs_aux->BtMvu[i][j] + mbs_aux->BtMvu[j][i] +
                    mbs_aux->BtMB[i][j];
            }

            mbs_aux->Fruc[i] = mbs_aux->F[mbs_aux->iquc[i]] + mbs_aux->MBMb[i] + mbs_aux->BtFv[i];
        }
    }
    else
    {
        // Mruc Fruc
        nL = nC = mbs_aux->iquc[0];
        for(i=1;i<=nL;i++)
        {
            for(j=1;j<=nC;j++)
            {
                mbs_aux->Mruc[i][j] = mbs_aux->M[mbs_aux->iquc[i]][mbs_aux->iquc[j]];
            }
            mbs_aux->Fruc[i] = mbs_aux->F[mbs_aux->iquc[i]];
        }
    }

    // Mr Fr
    nL = nC = s->nqu;

    for(i=1;i<=nL;i++)
    {
        has_a_line_of_zeros = 1;
        for(j=1;j<=nC;j++)
        {
            mbs_aux->Mr[i][j] = mbs_aux->Mruc[i][j]; // Muu
            if (mbs_aux->Mr[i][j]>1e-16)
                has_a_line_of_zeros = 0;
        }
        if (has_a_line_of_zeros)
        {
            printf("The line %d of the reduced mass matrix, associated to q(%d), is full of zeros\n",i,mbs_aux->iquc[i]);
            for(k=1;k<=nC;k++)
                printf("mbs_aux->Mr[%d][%d] = %e;\n",i,k,mbs_aux->Mr[i][k]);
            fprintf(stderr,"The reduced mass matrix has a line of zeros\n");
        }

        term = 0.0;
        for(k=1;k<=s->nqc;k++)
        {
            term += mbs_aux->Mruc[i][s->nqu+k] * s->qdd[mbs_aux->iquc[s->nqu+k]];
        }
        mbs_aux->Fr[i] = -(mbs_aux->Fruc[i] + term);
    }

    // calcul des accelerations reduites : 'resolution' du systeme ODE = Mr*udd = Fr;
	choldc(mbs_aux->Mr,s->nqu,mbs_aux->p_Mr);
    cholsl(mbs_aux->Mr,s->nqu,mbs_aux->p_Mr,mbs_aux->Fr,s->udd);

    nL = s->nqu;
    for(i=1;i<=nL;i++)
    {
        s->qdd[s->qu[i]] = s->udd[i];
    }

	if (s->nqv>0)
	{
		double d;
        // qdd_v = Bvuc*qdd_u + bp
		nL = s->nqv;
		nk = s->nqu;
		for(i=1;i<=nL;i++)
		{
			term = 0.0;
			for(k=1;k<=nk;k++)
			{
				term += mbs_aux->Bvuc[i][k] * s->udd[k];
			}
			s->qdd[s->qv[i]] = term + mbs_aux->bp[i];
		}

		// Jvlambda = M_v_uc * qdd_uc + M_v_v * qdd_v + cv - Qv     term += (mbs_aux->M[mbs_aux->iquc[i]][s->qv[k]] + mbs_aux->BtMvv[i][k]) * mbs_aux->bp[k];
        for(i=1;i<=nL;i++)
		{
			term = 0.0;
			// M_v_uc * qdd_uc
            for(k=1;k<=nk;k++)
			{
				//       M_v_uc                   *    qdd_uc
                term += mbs_aux->M[s->qv[i]][mbs_aux->iquc[k]] * s->qdd[mbs_aux->iquc[k]];
			}

            // M_v_v * qdd_v
            for(k=1;k<=nL;k++)
			{
				//      M_v_v * qdd_v
                term += mbs_aux->M[s->qv[i]][s->qv[k]] * s->qdd[s->qv[k]];
			}

            // c - Qv
            term += mbs_aux->c[s->qv[i]] - s->Qq[s->qv[i]];

            mbs_aux->Jvtlambda[i] = term;
            s->lambda[i] = term;

            // copy Jv in mbs_aux->Jvt
            for(j=1;j<=nL;j++){
                mbs_aux->Jvt[j][i]=mbs_aux->Jac[s->hu[i]][s->qv[j]];
            }
		}

        // compute lambda
        ludcmp(mbs_aux->Jvt,s->nqv,mbs_aux->ind_Jvt,&d);
        lubksb(mbs_aux->Jvt,s->nqv,mbs_aux->ind_Jvt,s->lambda);
	}

    // Qc = Mruc_cu * udd + Mruc_cc * qddc + Fruc_c
    if (s->nqc>0)
    {
        nL = s->nqc;
        for(i=1;i<=nL;i++)
        {
            term = 0.0;

            nk = s->nqu;
            for(k=1;k<=nk;k++)
            {
                term += mbs_aux->Mruc[s->nqu+i][k] * s->udd[k];
            }

            nk = s->nqc;
            for(k=1;k<=nk;k++)
            {
                term += mbs_aux->Mruc[s->nqu+i][s->nqu+k] * s->qdd[mbs_aux->iquc[s->nqu+k]];
            }

            mbs_aux->Qc[i] = term + mbs_aux->Fruc[s->nqu+i];
			s->Qc[s->qc[i]] = mbs_aux->Qc[i];
        }
    }


    return iter;
}
#endif
