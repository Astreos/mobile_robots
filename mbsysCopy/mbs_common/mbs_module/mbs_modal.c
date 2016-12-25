/**
* @file mbs_modal.c
*
* This file implements the  functions of the
* equil module in C.
*
*
* Creation date: 26/11/2015
* @author Quentin Docquier (based on the work of Aubain Verle and Paul Fisette)
*
*
* (c) Universite catholique de Louvain
*/

#include "mbs_modal.h"
#include "gsl/gsl_math.h" // QD? necessary for the Pi expression... should be removed in order to remove the mbs_module dependency on the gsl lib
#include "gslfct.h"

MbsModal* mbs_new_modal(MbsData* mbs_data)
{
	MbsAux* mbs_aux;

	// Initialize the local data struct
	mbs_aux = initMbsAux(mbs_data);

	return mbs_new_modal_aux(mbs_data, mbs_aux);
}

MbsModal* mbs_new_modal_aux(MbsData* mbs_data, MbsAux* mbs_aux)
{
	MbsModal *mo;			// short for modal general structure : mbs_modal
	MbsModalOptions *opts;

	// Initialize the modal structure
	mo = (MbsModal*)malloc(sizeof(MbsModal));

	// keep the pointer to the auxiliary data structure
	mo->aux = mbs_aux;

	// Initialize the options struct with default options
	opts = (MbsModalOptions*)malloc(sizeof(MbsModalOptions));
	opts->time = 0.0;
	opts->lintol = 1e-3;
	opts->relincr = 1e-2;
	opts->absincr = 1e-3;
	opts->mode_ampl = 0.2;
	opts->equitol = 1e-6;
	opts->save_mat = 0;
	opts->save_result = 0;
	opts->save_anim = 0;
	opts->renamefile = 0;
	opts->verbose = 1;
	opts->clearmbsglobal = 1;
	opts->qui = NULL;
	opts->nqui = 0;
	opts->resfilename = NULL;
	opts->respath = NULL;
	opts->animpath = NULL;

	mo->options = opts;

	// Initialize the Lpk structure with default options
	mo->lpk = (MbsLpk*)malloc(sizeof(MbsLpk));
	mo->lpk->n_diverge = 0;
	mo->lpk->maxcomp = 0.0;

	return mo;
}


void mbs_run_modal(MbsModal *mo, MbsData *s)
{
	if (mo->options->verbose)
	{
		printf(">>MODAL>> ***** mbs_run_modal begin *****\n");
	}

	mbs_modal_init(mo, s);


	mbs_modal_loop(mo, s);


	mbs_modal_finish(mo, s);

}


void mbs_modal_init(MbsModal *mo, MbsData *s)
{
	int i, j, ind_x, ind1_x, ind2_x;

	// 1.1 test if a Modal analysis is needed AND if, a equilibrium has been performed beforehand
	if (s->DoneEquil == 0)
	{
		printf(">>MODAL>> mbs_modal_init : please perform an equilibrium at first !\n");
		exit(EXIT_FAILURE);
	}
	if (s->nqu == 0) // no Modal analysis needed
	{
		printf(">>MODAL>> mbs_modal_init : no independent variable - irrelevant process !\n");
		exit(EXIT_FAILURE);
	}

	// 2. mbsPad initial values Saving
	mo->q_saved = get_dvec_0(s->njoint);
	copy_dvec_0(&(s->q[1]), mo->q_saved, s->njoint);			// save the mbsPad initial values
	mo->qd_saved = get_dvec_0(s->njoint);
	copy_dvec_0(&(s->qd[1]), mo->qd_saved, s->njoint);		// save the mbsPad initial values

	// 3.1 Memory allocation for modal variables (mainly based on nx)
	mo->Mr = get_dmat_0(s->nqu, s->nqu);
	mo->Bvuc = get_dmat_0(s->nqv + 1, s->nqu + s->nqc + 1);
	mo->nx = s->nqu - mo->options->nqui;

	mo->Mrzz = get_dmat_0(mo->nx, mo->nx);
	mo->Kr = get_dmat_0(mo->nx, mo->nx);
	mo->Gr = get_dmat_0(mo->nx, mo->nx);
	mo->A = get_dmat_0(2 * mo->nx, 2 * mo->nx);
	mo->vec_a = get_dvec_0(2 * mo->nx);
	mo->vec_b = get_dvec_0(2 * mo->nx);
	mo->mat_r = get_dmat_0(2 * mo->nx, 2 * mo->nx);
	mo->mat_phi = get_dmat_0(2 * mo->nx, 2 * mo->nx);

	mo->nMode = 2 * mo->nx; // might be changed !
	// Memory allocation for mode List
	mo->modeList = (MbsModalMode**)malloc(2 * mo->nx*sizeof(MbsModalMode*));
	for (i = 0; i < 2 * mo->nx; i++)
	{
		mo->modeList[i] = (MbsModalMode*)malloc(sizeof(MbsModalMode));
		mo->modeList[i]->type = 0;
		mo->modeList[i]->eval_a = 0.0;
		mo->modeList[i]->eval_b = 0.0;
		mo->modeList[i]->ksi = 0.0;
		mo->modeList[i]->nu_0 = 0.0;
		mo->modeList[i]->nu = 0.0;
		mo->modeList[i]->evec_r = get_dvec_0(mo->nx);
		mo->modeList[i]->evec_phi = get_dvec_0(mo->nx);
	}
	mo->iModeSorted = get_ivec_0(2 * mo->nx);

	// 3.2 Memory allocation for lpk variables
	mo->lpk = mbs_new_lpk(s);



	// 3.3 file handling
	// set the path to result files if not specified
	if (mo->options->respath == NULL) {
		mo->options->respath = "../../resultsR";
	}
	if (mo->options->resfilename == NULL) {
		mo->options->resfilename = "modal";
	}
	if (mo->options->animpath == NULL) {
		mo->options->animpath = "../../animationR";
	}

	//---------------------------------------------------------------------------

	// 4. reduced Mass matrix computation, Mr (Robotran Basics Eq(42))
	if (mo->options->verbose)
	{
		printf(">>MODAL(MBS)>> :  Computation of the mass matrix Mr\n");
	}
	dirdynared(mo->aux, s); // ??QD not sure it is the best solution as we might have extra calculation...
	// In cholc algorithm (call in dyrdynared) lower part of Mr is destroyed !
	for (i = 0; i < s->nqu; i++)
	{
		for (j = i; j < s->nqu; j++)
		{
			mo->Mr[i][j] = mo->aux->Mr[i + 1][j + 1]; // from Pad convention (1) to numerics (0)
			mo->Mr[j][i] = mo->aux->Mr[i + 1][j + 1]; // rebuild the lower part
		}
	}
	// Bvuc matrix (necessary for animation in order to close the loops !...)
	for (i = 1; i <= s->nqv; i++)
	{
		for (j = 1; j <= (s->nqu + s->nqc); j++)
		{
			mo->Bvuc[i][j] = mo->aux->Bvuc[i][j];
		}
	}
	// Fruc vector (Robotran Basics Eq(20))
	ind_x = 0;
	for (i = 0; i < s->nqu; i++)
	{
		if (find_ivec_0(mo->options->qui, mo->options->nqui, s->qu[i]) == -1)
		{
			mo->lpk->F_equil[ind_x] = mo->aux->Fr[s->qu[i] + 1];
			ind_x++;
		}
	}
	// remove the ignored variables specified by the user in Mr (but keep all in mo->Mr)
	ind1_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
		if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qu[i]) == -1)
		{
			ind2_x = 0;
			for (j = 1; j <= s->nqu; j++)
			{
				if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qu[j]) == -1)
				{
					mo->Mr[ind1_x][ind2_x] = mo->Mr[i - 1][j - 1]; // because loop are i=1, j=1 for the find_int...
					ind2_x++;
				}
			}
			ind1_x++;
		}
	}
}
void mbs_modal_loop(MbsModal *mo, MbsData *s)
{
	int i, j, PosOrVel, lin_fail;

	char *filename;

	int iMode; // for post-process

	int *iLU;
	double d; // needed for system solving with LU method (see Numerics)
	double *columnGr, *columnKr; // for matrix A calculation...

	int next_type, ind_cur;
	double min_a, min_b;		// necessary for post process ordering

	// 1.---------- Linearization ----------//

	// Kr = delta Fr/delta u
	PosOrVel = 1;
	if (mo->options->verbose)
	{
		printf(">>MODAL>> Computation of the tangent matrix Kr\n");
	}
	lin_fail = mbs_linearipk(mo->Kr, mo->lpk, mo->aux, s, PosOrVel);

	//    Gr = delta Fr/delta ud
	PosOrVel = 2;
	if (mo->options->verbose)
	{
		printf(">>MODAL>> Computation of the tangent matrix Gr\n");
	}
	lin_fail = mbs_linearipk(mo->Gr, mo->lpk, mo->aux, s, PosOrVel);
	copy_dvec_0(mo->q_saved, &(s->q[1]), s->njoint);  // ? might be removed because done in linearipk
	copy_dvec_0(mo->qd_saved, &(s->qd[1]), s->njoint); // might be removed because done in linearipk

	if (lin_fail > 0)
	{
		printf(">>MODAL>> One of the linearization has failed...\n");
	}

	// save the Linear tangent Matrices: Kr, Mr, Gr  because modified in 2.0
	filename = (char *)malloc(1 + strlen(mo->options->respath) + strlen("/") + strlen(mo->options->resfilename) + 30);
	if (mo->options->save_mat)
	{
		sprintf(filename, "%s/%s_%s.res", mo->options->respath, mo->options->resfilename, "Mr");
		save_dmat_0(mo->Mr, mo->nx, mo->nx, filename);
		sprintf(filename, "%s/%s_%s.res", mo->options->respath, mo->options->resfilename, "Kr");
		save_dmat_0(mo->Kr, mo->nx, mo->nx, filename);
		sprintf(filename, "%s/%s_%s.res", mo->options->respath, mo->options->resfilename, "Gr");
		save_dmat_0(mo->Gr, mo->nx, mo->nx, filename);
	}
//	printf("\n Kr \n");
//	print_dmat_0(mo->Kr, mo->nx, mo->nx);
//	printf("\n Gr \n");
//	print_double_tab(mo->Gr, mo->nx, mo->nx);
//
	// 2.---------- Compution of matrix A----------// (Robotran Basics Eq(46))

	// initialization
	columnKr = get_dvec_0(mo->nx);
	columnGr = get_dvec_0(mo->nx);
	iLU = get_ivec_0(mo->nx);

	// LU decomposition of Mr
	ludcmp_0(mo->Mr, mo->nx, iLU, &d);

	// inversion and solving by column (Robotran Basics Eq(??))
	for (i = 0; i < mo->nx; i++)
	{
		for (j = 0; j < mo->nx; j++) // write down each column of Gr and Kr
		{
			columnGr[j] = mo->Gr[j][i];
			columnKr[j] = mo->Kr[j][i];
		}
		// solve a system for each column of Gr and Kr (implicitly calculate Mr-1 Gr , Mr-1 Kr)
		lubksb_0(mo->Mr, mo->nx, iLU, columnGr);
		lubksb_0(mo->Mr, mo->nx, iLU, columnKr); // --> the solution is stored in the column__ vectors!

		for (j = 0; j < mo->nx; j++)
		{
			// left and right upper blocks
			mo->A[j][i] = -columnGr[j];
			mo->A[j][i + mo->nx] = -columnKr[j];
		}
		// left and right lower blocks
		mo->A[mo->nx + i][i] = 1.0;
		mo->A[mo->nx + i][mo->nx + i] = 0.0;
	}

	free_dvec_0(columnGr);
	free_dvec_0(columnKr);
	free_ivec_0(iLU);




	// 3.---------- Modal analysis ----------//

	// some magics related to gsl libraries... ?Q   calculate lambda_i, v_i and put results in modal structure
	eig_0(mo->A, 2 * mo->nx, mo->vec_a, mo->vec_b, mo->mat_r, mo->mat_phi);


	// 4.---------- Post processing ----------//

	// 4.1 EigenModes  (Robotran Basics sec 5.4.2 ??))
	iMode = 0;
	for (i = 0; i < 2 * mo->nx; i++) // loop on the eigen values...
	{
		if (fabs(mo->vec_b[i]) < 1e-6)				// if imaginary part 0
		{
			if (mo->vec_a[i] < -1e-6)					// if real part -
			{
				mo->modeList[iMode]->type = 1;
			}
			else if (mo->vec_a[i] < 1e-6)				// if real part 0
			{
				mo->modeList[iMode]->type = 2;
			}
			else									// if real part +
			{
				mo->modeList[iMode]->type = 3;
			}
		}

		else										// if imaginary part non zero
		{
			if (mo->vec_a[i] < -1e-6)					// if real part -
			{
				mo->modeList[iMode]->type = 4;
				mo->modeList[iMode]->ksi = cos(atan(mo->vec_b[i] / mo->vec_a[i]));
				mo->modeList[iMode]->nu_0 = sqrt(mo->vec_a[i] * mo->vec_a[i] + mo->vec_b[i] * mo->vec_b[i]);
				mo->modeList[iMode]->nu = mo->modeList[iMode]->nu_0 * sqrt(1.0 - mo->modeList[iMode]->ksi*mo->modeList[iMode]->ksi);
			}
			else if (mo->vec_a[i] < 1e-6)				// if real part 0
			{
				mo->modeList[iMode]->type = 5;
				mo->modeList[iMode]->ksi = 0.0;
				mo->modeList[iMode]->nu_0 = fabs(mo->vec_b[i]);
				mo->modeList[iMode]->nu = fabs(mo->vec_b[i]);
			}
			else									// if real part +
			{
				mo->modeList[iMode]->type = 6;

			}
		}
		mo->modeList[iMode]->eval_a = mo->vec_a[i];
		mo->modeList[iMode]->eval_b = mo->vec_b[i];
		mbs_modal_evecPos(mo->mat_r, mo->mat_phi, mo->nx, i, mo->modeList[iMode]->evec_r, mo->modeList[iMode]->evec_phi);
		iMode++;
	}
	mo->nMode = iMode;

	// 4.2 EigenModes ordering  by types and frequencies...!
	iMode = 0;
	for (i = 0; i < mo->nMode; i++)
	{
		mo->iModeSorted[i] = -1;
	}
	for (i = 1; i <= 6; i++)
	{
		next_type = 0;
		while (next_type != 1)
		{
			min_a = DBL_MAX;
			min_b = DBL_MAX;
			ind_cur = -1;
			for (j = 0; j < mo->nMode; j++) //
			{
				if (mo->modeList[j]->type == i && find_ivec_0(mo->iModeSorted, iMode, j) == -1)
				{
					if (mo->modeList[j]->eval_a < min_a)
					{
						min_a = mo->modeList[j]->eval_a;
						min_b = mo->modeList[j]->eval_b;
						ind_cur = j;
					}
					else if (mo->modeList[j]->eval_a == min_a)
					{
						if (mo->modeList[j]->eval_b <= min_b)
						{
							min_b = mo->modeList[j]->eval_b;
							ind_cur = j;
						}
					}
				}
			}
			if (ind_cur == -1)
			{
				next_type = 1;
			}
			else
			{
				mo->iModeSorted[iMode] = ind_cur;
				iMode++;
			}
		}
	}


	for (i = 0; i < mo->nMode; i++)
	{
		mo->iModeSorted[i] = i;
	}

}
void mbs_modal_finish(MbsModal *mo, MbsData *s)
{
	char *filename;

	filename = (char *)malloc(1 + strlen(mo->options->respath) + strlen("/") + strlen(mo->options->resfilename) + 30);
	if (mo->options->save_mat)
	{// note: matrices Gr,Kr,Mr are saved just before computation of A
		sprintf(filename, "%s/%s_%s.res", mo->options->respath, mo->options->resfilename, "A");
		save_dmat_0(mo->A, 2 * mo->nx, 2 * mo->nx, filename);
	}
	if (mo->options->save_result)
	{
		sprintf(filename, "%s/%s_%s.res", mo->options->respath, mo->options->resfilename, "result");
		mbs_modal_save_result(mo, s, filename);
	}
	if (mo->options->save_anim)
	{
		sprintf(filename, "%s/", mo->options->animpath);
		mbs_modal_save_anim(mo, s, filename); // actually filepath ...
	}
	free(filename);

	if (mo->options->verbose)
	{
		printf(">>MODAL>> ***** mbs_run_modal end *****\n");
	}


	s->DoneModal = 1;
}

void mbs_delete_modal(MbsModal *mo, MbsData *s)
{
	int i;
	// ALL the get_double man !
	// user free
	if (mo != NULL)
	{

		free_dvec_0(mo->q_saved);
		free_dvec_0(mo->qd_saved);

		free_dmat_0(mo->Mr);
		free_dmat_0(mo->Gr);
		free_dmat_0(mo->Kr);
		free_dmat_0(mo->Bvuc);

		free_dmat_0(mo->A);
		free_dvec_0(mo->vec_a);
		free_dvec_0(mo->vec_b);
		free_dmat_0(mo->mat_r);
		free_dmat_0(mo->mat_phi);

		// the mode list...
		for (i = 0; i < 2 * mo->nx; i++)
		{
			free(mo->modeList[i]->evec_phi);
			free(mo->modeList[i]->evec_r);
			free(mo->modeList[i]);
		}
		free(mo->modeList);
		free(mo->iModeSorted);

		// lpk structure (for linearization)
		mbs_delete_lpk(mo->lpk);
		/*
		free(mo->lpk->diverge_ind);
		free(mo->lpk->x_equil);
		free(mo->lpk->x_mid);
		free(mo->lpk->x_ext);
		free(mo->lpk->F_equil);
		free(mo->lpk->F_mid);
		free(mo->lpk->F_ext);
		free_dmat_0(mo->lpk->GK_comp,mo->nx);
		free_dmat_0(mo->lpk->GK_prem,mo->nx);
		free(mo->lpk);*/

		freeMbsAux(mo->aux, s);

		free(mo);
	}
}

int mbs_modal_linearipk(double **GK, MbsModal *mo, MbsAux *aux, MbsData *s, int PosOrVel)
{
	int i, j, k; // fctlin,qoqp_equi,fctlin_equi,posvit,epsmod,delta_ua,delta_ur,linpa
	int ind_x;
	int rep, compt, test, li;

	int lin_fail = 0;

	double max_GK_k, max_GK_prem_div_k;
	double increment;

	// step 1 : save the mbs_data value !
	copy_dvec_0(&(s->q[1]), mo->q_saved, s->njoint);
	copy_dvec_0(&(s->qd[1]), mo->qd_saved, s->njoint);

	// step 2 : linearization computation (by modifying s->q and s->qd during the process)
	mo->lpk->n_diverge = 0;
	mo->lpk->maxcomp = 0.0;

	ind_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
		if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qu[i]) == -1)
		{
			if (PosOrVel == 1) // check whether the linearization is on the velocities or the positions
			{
				mo->lpk->x_equil[ind_x] = s->q[s->qu[i]];
			}
			else
			{
				mo->lpk->x_equil[ind_x] = s->qd[s->qu[i]];
			}
			ind_x++;
		}
	}

	// loop on the matrix column !  (the linearized matrix is build column by column...)
	for (k = 0; k < mo->nx; k++)//
	{
		for (i = 0; i < mo->nx; i++)
		{
			for (j = 0; j < mo->nx; j++)
			{
				mo->lpk->GK_comp[i][j] = 100.0;
			}
		}
		rep = 1;
		compt = 1;
		copy_dvec_0(mo->lpk->x_equil, mo->lpk->x_ext, mo->nx);

		copy_dvec_0(mo->q_saved, &(s->q[1]), s->njoint);
		copy_dvec_0(mo->qd_saved, &(s->qd[1]), s->njoint);

		// ideal increment computation
		if (fabs(mo->lpk->x_ext[k] * mo->options->relincr) < mo->options->absincr)
		{
			if (mo->lpk->x_ext[k] == 0.0)
			{
				increment = mo->options->absincr;
			}
			else
			{
				increment = sign(mo->lpk->x_ext[k]) * mo->options->absincr;
			}
		}
		else
		{
			increment = mo->lpk->x_ext[k] * mo->options->relincr;
		}

		// Parabolic fitting

		// 1. Extreme Point

		mo->lpk->x_ext[k] = mo->lpk->x_equil[k] + increment;
		lin_fail = mbs_modal_fct(mo->lpk->x_ext, mo->nx, mo->lpk->F_ext, mo, aux, s, PosOrVel);


		copy_dvec_0(mo->lpk->x_ext, mo->lpk->x_mid, mo->nx);

		while (rep)
		{
			// 2. Middle Point
			li = 0;
			mo->lpk->x_mid[k] = mo->lpk->x_equil[k] + increment / 2;
			lin_fail = mbs_modal_fct(mo->lpk->x_mid, mo->nx, mo->lpk->F_mid, mo, aux, s, PosOrVel);

			for (i = 0; i < mo->nx; i++)
			{
				// if there is no influence on equation Fi(x)=0 , put it to zero...
				if (fabs(mo->lpk->F_ext[i]) < mo->options->equitol && fabs(mo->lpk->F_mid[i]) < mo->options->equitol)
				{
					li++;
					GK[i][k] = 0.0;
				}
				// otherwise compute the slope
				else
				{
					// slope at point x_equil
					GK[i][k] = (-mo->lpk->F_ext[i] + 4.0 * mo->lpk->F_mid[i] - 3.0 * 0.0) / increment;

					// first iteration in the while loop
					if (compt == 1)
					{
						mo->lpk->GK_prem[i][k] = GK[i][k];
					}
				}
			}
			// if all entries are non sensitive, Column k with no influence (i.e. variable non sensitive)
			if (li == mo->nx)
			{
				if (mo->options->verbose)
				{
					printf(">>MODAL>> Column %d/%d    (no influence)\n", k + 1, mo->nx);
				}
				rep = 0; // stop the iterations on the column : no influence
			}
			// otherwise, compute and iterate !
			else
			{
				max_GK_k = 0.0;
				for (i = 0; i<mo->nx; i++)
				{
					max_GK_k = MAX(max_GK_k, fabs(GK[i][k]));
				}

				test = 1;
				for (i = 0; i<mo->nx; i++)
				{
					if (fabs(GK[i][k] / max_GK_k) > 1e-6 && fabs(GK[i][k]) > 1e-8);
					{
						if (fabs((mo->lpk->GK_comp[i][k] - GK[i][k]) / GK[i][k]) > mo->options->lintol)
						{
							test = 0; // accuracy tolerance for linearization is not reached
						}
					}
				}
				// continue to iterate : x_mid->x_ext  & increment->increment/2
				if (test)
				{
					for (i = 0; i < mo->nx; i++)
					{
						mo->lpk->GK_comp[i][k] = GK[i][k];
					}
					increment = increment / 2.0;
					copy_dvec_0(mo->lpk->F_mid, mo->lpk->F_ext, mo->nx);
					//copy_dvec_0(lpk->F_mid, lpk->F_ext, lpk->nx);

					compt++; // The linearization is diverging ...
					if (compt > mo->lpk->itermax)
					{
						if (mo->options->verbose)
						{
							printf(">>MODAL>> Column %d/%d    (waiting...)\n", k + 1, mo->nx);
						}
						mo->lpk->diverge_ind[mo->lpk->n_diverge] = k;
						mo->lpk->n_diverge++;
						rep = 0; // stop the iterations on the column : has diverged
					}
				}
				// stop to iterate: linearization tolerance reached
				else
				{
					if (mo->lpk->maxcomp < max_GK_k)
					{
						mo->lpk->maxcomp = max_GK_k; // save the maximum GK entry value over all columns
					}
					if (mo->options->verbose)
					{
						printf(">>MODAL>> Column %d/%d    (%d iterations)\n", k + 1, mo->nx, compt);
					}
					rep = 0; // stop the iterations on the column : has converged
				}
			}
		}
		// step 3 : restore the exact value for mbs_data->q and mbs_data->qd
		copy_dvec_0(mo->q_saved, &(s->q[1]), s->njoint);
		copy_dvec_0(mo->qd_saved, &(s->qd[1]), s->njoint);
	}

	// Processing the waiting columns: based on maxcomp we possibly accept them
	if (mo->lpk->n_diverge > 0)
	{
		if (mo->options->verbose)
		{
			printf(">>MODAL>> Processing the waiting columns\n");
		}
		// check for all diverging columns  (new loop on diverging columns)
		for (k = 0; k < mo->lpk->n_diverge; k++)
		{
			max_GK_prem_div_k = 0.0;
			for (i = 0; i < mo->nx; i++)
			{
				max_GK_prem_div_k = MAX(max_GK_prem_div_k, fabs(mo->lpk->GK_prem[i][mo->lpk->diverge_ind[k]]));
			}
			// waiting column k is accepted
			if (max_GK_prem_div_k < 1e-5 * mo->lpk->maxcomp)
			{
				for (i = 0; i < mo->nx; i++)
				{
					GK[i][mo->lpk->diverge_ind[k]] = 0.0;
				}
				if (mo->options->verbose)
				{
					printf(">>MODAL>> Column %d/%d   has converged \n", k + 1, mo->nx);
				}
			}
			// waiting column k is still not acepted
			else
			{
				printf(">>MODAL>> Column %d/%d   no convergence after %d iterations\n", k + 1, mo->nx, mo->lpk->itermax);
				printf(">>MODAL>> Restart with other parameters !\n");
				lin_fail = 1;
				return lin_fail;
			}
		}
	}
	lin_fail = 0;
	return lin_fail;
}

int mbs_modal_fct(double *x, int nx, double *Fx, MbsModal *mo, MbsAux *aux, MbsData *s, int PosOrVel)
{
	int fail = 0;
	int i;

	int ind_x;
	double *Fruc;
	Fruc = get_dvec_0(s->nqu + s->nqc + 1);

	ind_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
		if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qu[i]) == -1) // for ignored variables
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
	}



	// Compute the Fruc vector of size nquc=nqu+nqc for qu and qc variables (Robotran Basics Eq(20))
	fail = mbs_calc_Fruc(Fruc, aux, s);

	ind_x = 0;
	for (i = 1; i <= s->nqu; i++)
	{
		if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qu[i]) == -1) // for ignored variables
		{
			Fx[ind_x] = Fruc[i]; // from Rob to Num convention
			ind_x++;
		}
	}
	free(Fruc);

	return fail;
}

int mbs_modal_evecPos(double** mat_r, double** mat_phi, int nx, int ind, double *evec_r, double *evec_phi)
{
	int i;
	double r_max, phi_r_max;
	int ind_r_max;
	ind_r_max = 0;
	r_max = 0.0;
	for (i = 0; i<nx; i++)
	{
		if (fabs(mat_r[nx + i][ind])>r_max)
		{
			r_max = fabs(mat_r[nx + i][ind]);
			ind_r_max = i;
		}
	}
	r_max = mat_r[nx + ind_r_max][ind];
	phi_r_max = mat_phi[nx + ind_r_max][ind];
	for (i = 0; i < nx; i++)
	{
		evec_r[i] = mat_r[nx + i][ind] / r_max;
		evec_phi[i] = mat_phi[nx + i][ind] - phi_r_max;
	}
	return 0;
}

int mbs_modal_save_result(MbsModal* mbs_modal, MbsData* s, char* filename)
{
	int i, j, k, ind_x;
	char *q_type_char = "x";

	double q_v_cur_a, q_v_cur_b;

	FILE  *fidR;

	// File declaration
	fidR = NULL; // internal filename

	// Opening file
	fidR = fopen(filename, "wt");

	// Fill the file
	if (fidR == NULL)
	{
		printf("error: cannot open file '%s'\n", filename);
		return EXIT_FAILURE;
	}

	// 1. Result file Heading
	fprintf(fidR, "\n-------------------------------------------------------\n");
	fprintf(fidR, "Project            : %s\n", "mbs_data->name");
	fprintf(fidR, "Date            : %s", get_time_machine());
	fprintf(fidR, "Process            : Modal Analysis\n");
	fprintf(fidR, "Computation        : Eigenmodes\n");
	fprintf(fidR, "-------------------------------------------------------\n");

	// 2. Eigen values and modes frequencies and damping coeffiencient
	fprintf(fidR, "|--------------------------------------------------------------------------------------------------------------|\n");
	fprintf(fidR, "|    Eigenvalues                                 |                                                             |\n");
	fprintf(fidR, "|    #            re                 im          |         nu(Hz)              ksi(%%)              nu0(Hz)     |\n");
	fprintf(fidR, "|--------------------------------------------------------------------------------------------------------------|\n");
	for (i = 0; i < mbs_modal->nMode; i++)
	{
		fprintf(fidR, "|   %2.d       % .3e        % .3e      |      % .3e         % .3e         % .3e    |\n", i + 1, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->eval_a, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->eval_b, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->nu / (2 * M_PI), mbs_modal->modeList[mbs_modal->iModeSorted[i]]->ksi *100.0, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->nu_0 / (2 * M_PI));
	}
	fprintf(fidR, "|--------------------------------------------------------------------------------------------------------------|\n");

	// Detailed modes description : for each mode : its type, the corresponding eigen vectors (in mbs system).
	fprintf(fidR, " \n\n");
	fprintf(fidR, " Details about modes: \n");
	fprintf(fidR, " --------------------\n\n");
	for (i = 0; i < mbs_modal->nMode; i++)
	{
		fprintf(fidR, "|--------------------------------------------------------------------------------\n");
		fprintf(fidR, "|    Eigenmode %d, (Eigen value # %d = %f + %f i )\n", i + 1, i + 1, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->eval_a, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->eval_b);
		switch (mbs_modal->modeList[mbs_modal->iModeSorted[i]]->type)
		{
		case 1:
			fprintf(fidR, "|    Type : unoscillating stable\n");
			break;
		case 2:
			fprintf(fidR, "|    Type : rigid\n");
			break;
		case 3:
			fprintf(fidR, "|    Type : unoscillating unstable\n");
			break;
		case 4:
			fprintf(fidR, "|    Type : oscillating damped\n");
			break;
		case 5:
			fprintf(fidR, "|    Type : oscillating-undamped\n");
			break;
		case 6:
			fprintf(fidR, "|    Type : oscillating unstable\n");
			break;
		}
		fprintf(fidR, "|                                                                                |\n");

		fprintf(fidR, "|    q-id                 q-name        |        q-type                 q-r                 q-phi             |\n");
		fprintf(fidR, "|--------------------------------------------------------------------------------|\n");
		q_type_char = "u";
		ind_x = 0;
		for (j = 1; j <= s->nqu; j++)
		{
			fprintf(fidR, "|    %4.d", s->qu[j]);
			fprintf(fidR, "             %10.s", "??");
			if (find_ivec_0(&(mbs_modal->options->qui[1]), mbs_modal->options->nqui, s->qu[j]) == -1)
			{
				fprintf(fidR, "                 %6s               %.3f               %.3f\n", q_type_char, mbs_modal->modeList[mbs_modal->iModeSorted[i]]->evec_r[ind_x], mbs_modal->modeList[mbs_modal->iModeSorted[i]]->evec_phi[ind_x]);
				ind_x++;
			}
			else
			{
				fprintf(fidR, "    %s-%s            %f        %f\n", q_type_char, "i", 0.0, 0.0);
			}
		}
		q_type_char = "v";
		for (j = 1; j <= s->nqv; j++)
		{
			fprintf(fidR, "|    %4.d", s->qv[j]);
			fprintf(fidR, "             %10.s", "??");
			q_v_cur_a = 0.0;
			q_v_cur_b = 0.0;
			ind_x = 0;
			for (k = 1; k <= s->nqu; k++)
			{
				if (find_ivec_0(&(mbs_modal->options->qui[1]), mbs_modal->options->nqui, s->qu[k]) == -1)
				{
					q_v_cur_a += mbs_modal->Bvuc[j][k] * mbs_modal->modeList[mbs_modal->iModeSorted[i]]->evec_r[ind_x] * cos(mbs_modal->modeList[mbs_modal->iModeSorted[i]]->evec_phi[ind_x]);
					q_v_cur_b += mbs_modal->Bvuc[j][k] * mbs_modal->modeList[mbs_modal->iModeSorted[i]]->evec_r[ind_x] * sin(mbs_modal->modeList[mbs_modal->iModeSorted[i]]->evec_phi[ind_x]);
					ind_x++;
				}
			}
			// need paul to know if c par of Bvuc is needed

			fprintf(fidR, "                 %6s               %.3f               %.3f\n", q_type_char, sqrt(q_v_cur_a*q_v_cur_a + q_v_cur_b*q_v_cur_b), atan2(q_v_cur_b, q_v_cur_a));
		}
		fprintf(fidR, "\n\n\n");
	}
	fclose(fidR);

	return EXIT_SUCCESS;
}

int mbs_modal_save_anim(MbsModal* mo, MbsData* s, char* filepath)
{
	int i, j, k, l, ind_x;

	char filename[100]; // caution
	FILE  *fidR;
	double *q_print, *q_mode_r, *q_mode_phi;
	int N, n_cycle;

	double t, delta_t, tf;
	double q_v_cur_a, q_v_cur_b, alpha;



	q_mode_r = get_dvec_0(s->njoint + 1);
	q_mode_phi = get_dvec_0(s->njoint + 1);
	q_print = get_dvec_0(s->njoint + 1);

	for (i = 0; i < mo->nMode; i++)
	{

		sprintf(filename, "%smode_%d.anim", filepath, i + 1);


		// File declaration
		fidR = NULL; // internal filename

		// Opening file
		fidR = fopen(filename, "wt");


		// Fill the file
		if (fidR == NULL)
		{
			printf("error: cannot open file '%s'\n", filename);
			return EXIT_FAILURE;
		}



		t = 0.0;
		// decide mode parameters (frequencies, ...) based on the type of mode
		switch (mo->modeList[mo->iModeSorted[i]]->type)
		{
		case 1: // 1=unoscillating stable
			alpha = 5.0;
			delta_t = 0.001;
			tf = 15.0;
			break;
		case 2: // 2=rigid
			alpha = 0.0;
			delta_t = 0.001;
			tf = 2.0;
			break;
		case 3: // 3=unoscillating unstable
			alpha = 5.0;
			delta_t = 0.001;
			tf = 5.0;
			break;
		case 4: // 4=oscillating damped
		case 5: // 5=oscillating undamped
		case 6: // 6=oscillating unstable
			alpha = 2.0 * M_PI;
			delta_t = 0.001;
			tf = 15.0;
			break;
		}

		ind_x = 0;
		for (j = 1; j <= s->nqu; j++)
		{
			if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qu[j]) == -1)
			{



				q_mode_r[s->qu[j] - 1] = mo->modeList[mo->iModeSorted[i]]->evec_r[ind_x];
				q_mode_phi[s->qu[j] - 1] = mo->modeList[mo->iModeSorted[i]]->evec_phi[ind_x];
				ind_x++;
			}
			else
			{
				q_mode_r[s->qu[j]] = 0.0;
				q_mode_phi[s->qu[j]] = 0.0;
			}
		}
		for (j = 1; j <= s->nqv; j++)
		{
			q_v_cur_a = 0.0;
			q_v_cur_b = 0.0;
			ind_x = 0;
			for (k = 1; k <= s->nqu; k++)
			{
				if (find_ivec_0(&(mo->options->qui[1]), mo->options->nqui, s->qv[j]) == -1)
				{
					q_v_cur_a += mo->Bvuc[j][k] * mo->modeList[mo->iModeSorted[i]]->evec_r[ind_x] * cos(mo->modeList[mo->iModeSorted[i]]->evec_phi[ind_x]);
					q_v_cur_b += mo->Bvuc[j][k] * mo->modeList[mo->iModeSorted[i]]->evec_r[ind_x] * sin(mo->modeList[mo->iModeSorted[i]]->evec_phi[ind_x]);
					ind_x++;
				}
			}
			q_mode_r[s->qv[j] - 1] = sqrt(q_v_cur_a*q_v_cur_a + q_v_cur_b*q_v_cur_b);
			q_mode_phi[s->qv[j] - 1] = atan2(q_v_cur_b, q_v_cur_a);
		}
		for (j = 0; j < s->njoint; j++)
		{
			q_mode_r[j] *= mo->options->mode_ampl;
		}
		while (t < tf)
		{
			for (k = 0; k < s->njoint; k++)
			{
				switch (mo->modeList[mo->iModeSorted[i]]->type)
				{
				case 1: // 1=unoscillating stable
					q_print[k] = mo->q_saved[k] + q_mode_r[k] * cos(q_mode_phi[k]) * exp(-alpha * t);
					break;
				case 2: // 2=rigid
					q_print[k] = mo->q_saved[k] + q_mode_r[k] * t;
					break;
				case 3: // 2=unoscillating unstable
					q_print[k] = mo->q_saved[k] + q_mode_r[k] * cos(q_mode_phi[k]) * exp(alpha * (t - tf));
					break;
				case 4: // 2=oscillating damped
				case 5: // 2=oscillating undamped
				case 6: // 2=oscillating unstable
					q_print[k] = mo->q_saved[k] + q_mode_r[k] * cos(q_mode_phi[k] + alpha * t);
					break;
				}

				// Trifoulle !
				/*q_print[1]= t*4.0;
				q_print[8]= t*4.0/0.255193;
				q_print[16]= t*4.0/0.258591;
				q_print[22]= t*4.0/0.258591;*/



			}
			fprintf(fidR, "% 12.8f", t);
			for (k = 0; k < s->njoint; k++)
			{
				fprintf(fidR, "  %12.8f", q_print[k]);
			}
			fprintf(fidR, "\n");

			t += delta_t;
		}
		fclose(fidR);
	}
	free_dvec_0(q_mode_r);
	free_dvec_0(q_mode_phi);
	free_dvec_0(q_print);


	return 0; // signature should be changed if no return is needed
}
