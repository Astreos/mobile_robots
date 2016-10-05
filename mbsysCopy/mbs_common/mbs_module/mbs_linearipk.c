//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Q.D.
//
// Creation : 14/06/2016
// Last update : 14/06/2016
//---------------------------

#include "mbs_linearipk.h"



int mbs_linearipk(double **GK, MbsLpk *lpk , MbsAux *aux, MbsData *s, int PosOrVel)
{
	int i, j, k; // fctlin,qoqp_equi,fctlin_equi,posvit,epsmod,delta_ua,delta_ur,linpa
	int ind_x;
	int rep, compt, test, li;

	int lin_fail = 0;

	double max_GK_k, max_GK_prem_div_k;
	double increment;

	// step 1 : save the mbs_data value !
	copy_dvec_0(&(s->q[1]),lpk->q_saved , s->njoint);
	copy_dvec_0(&(s->qd[1]),lpk->qd_saved, s->njoint);
	copy_dvec_0(&(s->Qq[1]), lpk->Qq_saved, s->njoint);

	// step 2 : linearization computation (by modifying s->q and s->qd during the process)
	lpk->n_diverge = 0;
	lpk->maxcomp = 0.0;
	ind_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
			if (PosOrVel == 1) // check whether the linearization is on the velocities or the positions
			{
				lpk->x_equil[ind_x] = s->q[s->qu[i]];
			}
			else
			{
				lpk->x_equil[ind_x] = s->qd[s->qu[i]];
			}
			ind_x++;
	}

	// loop on the matrix column !  (the linearized matrix is build column by column...)
	for (k = 0; k<lpk->nx; k++)//
	{

		for (i = 0; i<lpk->nx; i++)
		{
			for (j = 0; j<lpk->nx; j++)
			{
				lpk->GK_comp[i][j] = 100.0;
			}
		}
		rep = 1;
		compt = 1;
		copy_dvec_0(lpk->x_equil, lpk->x_ext, lpk->nx);



		// ideal increment computation
		if (fabs(lpk->x_ext[k] * lpk->relincr) < lpk->absincr)
		{
			if (lpk->x_ext[k] == 0.0)
			{
				increment = lpk->absincr;
			}
			else
			{
				increment = sign(lpk->x_ext[k]) * lpk->absincr;
			}
		}
		else
		{
			increment = lpk->x_ext[k] * lpk->relincr;
		}

		// Parabolic fitting

			// 1. Extreme Point
		lpk->x_ext[k] = lpk->x_equil[k] + increment;


		copy_dvec_0(lpk->q_saved, &(s->q[1]), s->njoint);
		copy_dvec_0(lpk->qd_saved, &(s->qd[1]), s->njoint);
		copy_dvec_0(lpk->Qq_saved, &(s->Qq[1]), s->njoint);
		lin_fail = mbs_lineari_fct(lpk->x_ext, lpk->nx, lpk->F_ext, aux, s, PosOrVel);


		copy_dvec_0(lpk->x_ext, lpk->x_mid, lpk->nx);

		while (rep)
		{
			// 2. Middle Point
			li = 0;
			lpk->x_mid[k] = lpk->x_equil[k] + increment / 2;
			lin_fail = mbs_lineari_fct(lpk->x_mid, lpk->nx, lpk->F_mid, aux, s, PosOrVel);

			for (i = 0; i<lpk->nx; i++)
			{
				// if there is no influence on equation Fi(x)=0 , put it to zero...
				if (fabs(lpk->F_ext[i]) < lpk->equitol && fabs(lpk->F_mid[i]) < lpk->equitol)
				{
					li++;
					GK[i][k] = 0.0;
				}
				// otherwise compute the slope
				else
				{
					// slope at point x_equil
					GK[i][k] = (-lpk->F_ext[i] + 4.0 * lpk->F_mid[i] - 3.0 * 0.0) / increment;  // to demonstrate solve for a centered parabola (then shift it back)

					// first iteration in the while loop
					if (compt == 1)
					{
						lpk->GK_prem[i][k] = GK[i][k];
					}
				}
			}
			// if all entries are non sensitive, Column k with no influence (i.e. variable non sensitive)
			if (li == lpk->nx)
			{
				if (lpk->verbose)
				{
					printf(">>LINEARIPK>> Column %d/%d    (no influence)\n", k + 1, lpk->nx);
				}
				rep = 0; // stop the iterations on the column : no influence
			}
			// otherwise, compute and iterate !
			else
			{
				max_GK_k = 0.0;
				for (i = 0; i<lpk->nx; i++)
				{
					max_GK_k = MAX(max_GK_k, fabs(GK[i][k]));
				}

				test = 1;
				for (i = 0; i<lpk->nx; i++)
				{
					if (fabs(GK[i][k] / max_GK_k) > 1e-6 && fabs(GK[i][k]) > 1e-8);
					{
						if (fabs((lpk->GK_comp[i][k] - GK[i][k]) / GK[i][k]) > lpk->lintol)
						{
							test = 0; // accuracy tolerance for linearization is not reached
						}
					}
				}
				// continue to iterate : x_mid->x_ext  & increment->increment/2
				if (test)
				{
					for (i = 0; i<lpk->nx; i++)
					{
						lpk->GK_comp[i][k] = GK[i][k];
					}
					increment = increment / 2.0;
					copy_dvec_0(lpk->F_mid, lpk->F_ext, lpk->nx);     //// ????????????? 8888

					compt++; // The linearization is diverging ...
					if (compt > lpk->itermax)
					{
						if (lpk->verbose)
						{
							printf(">>LINEARIPK>> Column %d/%d    (waiting...)\n", k + 1, lpk->nx);
						}
						lpk->diverge_ind[lpk->n_diverge] = k;
						lpk->n_diverge++;
						rep = 0; // stop the iterations on the column : has diverged
					}
				}
				// stop to iterate: linearization tolerance reached
				else
				{
					if (lpk->maxcomp < max_GK_k)
					{
						lpk->maxcomp = max_GK_k; // save the maximum GK entry value over all columns
					}
					if (lpk->verbose)
					{
						printf(">>LINEARIPK>> Column %d/%d    (%d iterations)\n", k + 1, lpk->nx, compt);
					}
					rep = 0; // stop the iterations on the column : has converged
				}
			}
		}

		// step 3 : restore the exact value for mbs_data->q and mbs_data->qd and Qq
		copy_dvec_0(lpk->q_saved, &(s->q[1]), s->njoint);
		copy_dvec_0(lpk->qd_saved, &(s->qd[1]), s->njoint);
		copy_dvec_0(lpk->Qq_saved, &(s->Qq[1]), s->njoint);
	}

	// Processing the waiting columns: based on maxcomp we possibly accept them
	if (lpk->n_diverge > 0)
	{
		if (lpk->verbose)
		{
			printf(">>LINEARIPK>> Processing the waiting columns\n");
		}
		// check for all diverging columns  (new loop on diverging columns)
		for (k = 0; k<lpk->n_diverge; k++)
		{
			max_GK_prem_div_k = 0.0;
			for (i = 0; i<lpk->nx; i++)
			{
				max_GK_prem_div_k = MAX(max_GK_prem_div_k, fabs(lpk->GK_prem[i][lpk->diverge_ind[k]]));
			}
			// waiting column k is accepted
			if (max_GK_prem_div_k < 1e-5 * lpk->maxcomp)
			{
				for (i = 0; i<lpk->nx; i++)
				{
					GK[i][lpk->diverge_ind[k]] = 0.0;
				}
				if (lpk->verbose)
				{
					printf(">>LINEARIPK>> Column %d/%d   has converged \n", k + 1, lpk->nx);
				}
			}
			// waiting column k is still not acepted
			else
			{
				printf(">>LINEARIPK>> Column %d/%d   no convergence after %d iterations\n", k + 1, lpk->nx, lpk->itermax);
				printf(">>LINEARIPK>> Restart with other parameters !\n");
				lin_fail = 1;
				return lin_fail;
			}
		}
	}
	lin_fail = 0;
	return lin_fail;
}





int mbs_lineari_fct(double *x, int nx, double *Fx, MbsAux *aux, MbsData *s, int PosOrVel)
{
	int fail = 0;
	int i;

	int ind_x;
	double *Fruc;
	Fruc = get_dvec_0(s->nqu + s->nqc + 1);

	ind_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
			if (PosOrVel == 1)
			{
				s->q[s->qu[i]] = x[ind_x];
			}
			else
			{
				s->qd[s->qu[i]] = x[ind_x];
			}
			ind_x++;
	}

	// Compute the Fruc vector of size nquc=nqu+nqc for qu and qc variables (Robotran Basics Eq(20))
	fail = mbs_calc_Fruc(Fruc, aux, s);

	ind_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
			Fx[ind_x] = Fruc[i]; // from Rob to Num convention
			ind_x++;
	}
	free(Fruc);

	return fail;
}

MbsLpk* mbs_new_lpk(MbsData *s)
{
	MbsLpk *lpk;

	lpk=(MbsLpk*)malloc(sizeof(MbsLpk));

	lpk->nx = s->nqu;

	lpk->diverge_ind = get_ivec_0(lpk->nx);
	lpk->n_diverge = 0;

	lpk->maxcomp = 0.0;

	lpk->q_saved=  get_dvec_0(s->njoint);
	lpk->qd_saved= get_dvec_0(s->njoint);
	lpk->Qq_saved = get_dvec_0(s->njoint);

	lpk->x_equil = get_dvec_0(lpk->nx);
	lpk->x_mid = get_dvec_0(lpk->nx);
	lpk->x_ext = get_dvec_0(lpk->nx);

	lpk->F_equil = get_dvec_0(lpk->nx);
	lpk->F_mid = get_dvec_0(lpk->nx);
	lpk->F_ext = get_dvec_0(lpk->nx);

	lpk->GK_comp = get_dmat_0(lpk->nx, lpk->nx);
	lpk->GK_prem = get_dmat_0(lpk->nx, lpk->nx);

	lpk->verbose=0;

	lpk->itermax=10;
	lpk->relincr=1e-2;
	lpk->absincr=1e-3;
	lpk->equitol=1e-6;
	lpk->lintol= 1e-3;

	return lpk;
}

void mbs_delete_lpk(MbsLpk *lpk)
{
	free(lpk->diverge_ind);

	free(lpk->q_saved);
	free(lpk->qd_saved);
	free(lpk->Qq_saved);

	free(lpk->x_equil);
	free(lpk->x_mid);
	free(lpk->x_ext);

	free(lpk->F_equil);
	free(lpk->F_mid);
	free(lpk->F_ext);

	free_dmat_0(lpk->GK_comp);
	free_dmat_0(lpk->GK_prem);

	free(lpk);
}
