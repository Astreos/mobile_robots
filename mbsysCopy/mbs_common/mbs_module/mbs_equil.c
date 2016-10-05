/**
* @file mbs_equil.c
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

#include <stdio.h>
#include "mbs_equil.h"
#include "gslfct.h"
#include "mbs_project_interface.h"
#include "mbs_equil_struct.h"
#include "MBSfun.h"

MbsEquil* mbs_new_equil(MbsData* mbs_data)
{
	MbsAux* mbs_aux;

	// Initialize the local data struct
	mbs_aux = initMbsAux(mbs_data);

	return mbs_new_equil_aux(mbs_data, mbs_aux);
}

MbsEquil* mbs_new_equil_aux(MbsData* mbs_data, MbsAux* mbs_aux)
{
	MbsEquil *equil;
	MbsEquilOptions *opts;

	// Initialize the equilibrium structure
	equil = (MbsEquil*)malloc(sizeof(MbsEquil));

	// keep the pointer to the auxiliary data structure
	equil->aux = mbs_aux;

	equil->nx = 0;
	equil->x = NULL;
	equil->nxs;
	equil->xs = NULL;
	equil->nxns = 0;
	equil->xns = NULL;
	equil->x_ptr = NULL;
	equil->norm_pk = 0.0;
	equil->qd_saved = NULL;
	equil->qdd_saved = NULL;


	// Initialize the options struct with default options
	opts = (MbsEquilOptions*)malloc(sizeof(MbsEquilOptions));
	opts->method = 1;
	opts->relax = 1.0;
	opts->smooth = 0;
	opts->senstol = 1e-06;
	opts->equitol = 1e-06;
	opts->devjac = 1e-06;
	opts->itermax = 30;
	opts->quasistatic = 0;
	opts->verbose = 1;
	opts->visualize = 0;
	opts->clearmbsglobal = 1;
	//opts->nqui=0;
	//opts->qui=NULL;  ??Q removed... !
	opts->nquch = 0;
	opts->quch = NULL;
	opts->xch_ptr = NULL;
	opts->nxe = 0;
	opts->xe_ptr = NULL;
	opts->fxe_ptr = NULL;

	equil->options = opts;

	return equil;
}

void mbs_run_equil(MbsEquil* eq, MbsData* mbs_data)
{
	// 1. Initialize the simulation
	// - - - - - - - - - - - - - -
	mbs_equil_init(eq, mbs_data);

	// 2. Run the simulation
	// - - - - - - - - - - -
	mbs_equil_loop(eq, mbs_data);  // another name might be more appropriate ??Q

	// 3. Finish the simulation
	// - - - - - - - - - - - -
	mbs_equil_finish(eq, mbs_data);
}

void mbs_delete_equil(MbsEquil* eq, MbsData* mbs_data)
{
	if (eq != NULL)
	{

		//printf("le 1 : %s \n",)

		free(eq->x_ptr);

		free_dvec_0(eq->x);
		free_ivec_0(eq->xs);
		free_ivec_0(eq->xns);

		free(eq->qd_saved);
		free(eq->qdd_saved);
		//-------
		//if(eq->options->nqui !=0){free(eq->options->qui);} // malloc in mbs_equil_ignorance
		if (eq->options->nquch != 0) { free(eq->options->xch_ptr); free(eq->options->quch); } // corresponding to malloc in mbs_equil_substitution
		if (eq->options->nxe != 0) { free(eq->options->xe_ptr); } // corresponding to malloc in mbs_equil_addition ??Q Fxe ?
		free(eq->options);
		//-------
		freeMbsAux(eq->aux, mbs_data);
		free(eq);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_equil_init(MbsEquil* eq, MbsData* s)
{
	int test, i, ind_x, ind_c;
	int ind_c_test;


	// 1.1 test if an Equilibrium is needed AND if, in a case of a closed-loop system, partionning has been made
	if (s->nqu == 0)
	{
		printf(">>EQUIL>> mbs_equil_init : no independent variable - irrelevant process !\n");
		exit(EXIT_FAILURE);
	}
	if (s->DonePart == 0 && s->Ncons > 0)
	{
		printf(">>EQUIL>> mbs_equil_init : please perform a partitioning at first !\n");
		exit(EXIT_FAILURE);
	}

	// 1.2 Verify whether a static equilibrium has been performed before a quasistatic equilibrium

	if (eq->options->quasistatic == 1 && s->DoneEquil != 1)
	{
		printf(">>EQUIL>> mbs_equil_init : It might be appropriate to make a static equilibrium before a quasistatic one !\n");
	}

	// 2.1 For static equilibrium, set velocities to 0 when necessary
	eq->qd_saved = (double*)malloc(s->njoint*sizeof(double));
	copy_dvec_0(&(s->qd[1]), eq->qd_saved, s->njoint);  // save the mbsPad initial values
	if (eq->options->quasistatic == 0)// for static equilibrium
	{
		test = 0;
		for (i = 1; i <= s->njoint; i++)
		{
			if (s->qd[i] != 0.0)
			{
				test = 1;
				s->qd[i] = 0.0;
			}
		}
		if (test && eq->options->verbose)
		{
			printf(">>EQUIL>> Non-zero initial velocities for all joints have been forced to 0 !\n");
		}
	}

	// 2.2 For ANY equilibrium: set all acceleration to zero.
	eq->qdd_saved = (double*)malloc(s->njoint*sizeof(double));
	copy_dvec_0(&(s->qdd[1]), eq->qdd_saved, s->njoint);
	test = 0;
	for (i = 1; i <= s->njoint; i++)
	{
		if (s->qdd[i] != 0.0)
		{
			test = 1;
			s->qdd[i] = 0.0;
		}
	}
	if (test && eq->options->verbose)
	{
		printf(">>EQUIL>> Non-zero initial accelerations for all joints have been forced to 0 !\n");
	}

	// 3. Memory allocation for equilibrium variables
	eq->nx = s->nqu + eq->options->nxe; // - eq->options->nqui    ??Q
	eq->x = get_dvec_0(eq->nx);
	eq->xs = get_ivec_0(eq->nx);  // oversized
	eq->xns = get_ivec_0(eq->nx); // oversized
	eq->x_ptr = (double**)malloc((eq->nx + 1)*sizeof(double*));

	// 4. Definition of equilibrium variables: x, initial values and x_ptr, pointers
	ind_x = 1;
	ind_c = 0;
	// 4.1 Independent variables and substitution variables, qu and quch(->xch)
	for (i = 1; i <= s->nqu; i++)
	{
		//if (find_int_vec(&(eq->options->qui[1]), eq->options->nqui, s->qu[i]) == -1) // if the variable is not a ignored variable
		//{
		ind_c = find_ivec_1(eq->options->quch, eq->options->nquch, s->qu[i]);

		if (ind_c == -1) // if the variable is not a changed variable
		{
			eq->x_ptr[ind_x - 1] = &(s->q[s->qu[i]]);
			eq->x[ind_x - 1] = (s->q[s->qu[i]]);
		}
		else			// if the variable is a changed variable, it is replaced by an exchange variable
		{

			ind_c = ind_c;
			eq->x_ptr[ind_x - 1] = eq->options->xch_ptr[ind_c];		// get the exchange variable
			eq->x[ind_x - 1] = *(eq->options->xch_ptr[ind_c]);		// get the substituation variable initial value

																	// ?Q might add test to check whether non null on ptr
		}
		ind_x++;
		//}
	}
	// 4.2 Added extra equilibrium variables, xe
	for (i = 1; i <= eq->options->nxe; i++)
	{
		eq->x_ptr[ind_x - 1] = eq->options->xe_ptr[i];
		eq->x[ind_x - 1] = *(eq->options->xe_ptr[i]);    // might add test to check whether non null on ptr
		ind_x++;
	}
	eq->options->fxe_ptr = &(user_equil_fxe); // extra equilibrium functions, Fxe(xe)=0
	// might add test to check whether non null on ptr
}

void mbs_equil_loop(MbsEquil* eq, MbsData* s)
{
	int err = 0;

	switch (eq->options->method)
	{
	case 1:
	{
		err = mbs_equil_fsolvepk(&(mbs_equil_fct), eq, eq->aux, s);
		break;
	}
	case 2:
	{
		//err = EQUIL_fsolve_GSL_multiroot((void*)lmstr);
		break;
	}
	case 3:
	{
		//err = EQUIL_fsolve_GSL_multimin((void*)lmstr);
		break;
	}
	}
	if (err > 0)
	{
		printf(">>EQUIL>> mbs_run_equil : Error Code : %d !\n", err);
	}


}

void mbs_equil_finish(MbsEquil* eq, MbsData* s)
{


	// ?Q check All free in all the code !!!

	// free qd_saved qd_saved
	// free x xs xns
	if (!(eq->options->quasistatic))  // if static equilibrium: get the velocities and acceleration back from the pad.
	{
		copy_dvec_0(eq->qd_saved, &(s->qd[1]), s->njoint);
		copy_dvec_0(eq->qdd_saved, &(s->qdd[1]), s->njoint);
		s->DoneEquil = 1;
	}
	else
	{
		s->DoneEquil = 1;
	}
	//user_equil_finish ! (with the user free ! )
	if (eq->options->verbose)
	{
		printf(">>EQUIL>> ***** mbs_run_equil end *****\n");
	}


}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_equil_exchange(MbsEquilOptions *options)
{
	int i;
	options->quch = (int*)malloc((options->nquch + 1) *sizeof(int));
	options->xch_ptr = (double**)malloc((options->nquch + 1) *sizeof(double*));

	for (i = 0; i <= options->nquch; i++)
	{
		options->quch[i] = 0;
		options->xch_ptr[i] = NULL;
	}
	options->quch[0] = options->nquch;
}

void mbs_equil_addition(MbsEquilOptions *options)
{
	int i;
	options->xe_ptr = (double**)malloc((options->nxe + 1) *sizeof(double*));

	for (i = 0; i <= options->nxe; i++)
	{
		options->xe_ptr[i] = NULL;
	}
}

void mbs_equil_ignorance(MbsEquilOptions *options) // useless function without the qui
{
	/*
	int i;
	options->qui=(int*) malloc((options->nqui+1)*sizeof(int));
	for(i=0;i<=options->nxe;i++)
	{
		options->qui[i]=0;
	}
	*/
}

void mbs_equil_save(MbsEquil* eq, MbsData *mbs_data, int n)
{
	// save the equilibrium steps ?


	/* int i;
	#ifdef JAVA
	Simu_realtime *realtime;
	Realtime_java *java;
	#endif

	for (i=0 ; i < dd->bufferNb ; i++){
	mbs_buffer_save(dd->buffers[i], t, dd->savedArrays[i]);
	}

	mbs_growing_buffer_save(dd->user_buffer, t);

	if (dd->options->save_visu)
	{
	#ifdef JAVA
	realtime = (Simu_realtime*) mbs_data->realtime;
	java = realtime->ext->java;

	user_realtime_visu(mbs_data, java->nb_q, java->anim_q);
	mbs_buffer_save(dd->buffer_visu, t, java->anim_q);
	#endif
	}*/
}

void user_equil_fxe(MbsData *mbs_data, double* f)  // ?Q might need to be moved in a a new .c (user_equil_init.c) + need for extra safety test
{
	//f[x] must be zero after the equilibrium, (x = 1 2 ... nxe)

	//f[1]= qu[1] - qu[2]   // symmetry constraint
	//f[2]=   ;
	//...
	//f[nxe]= ;


	f[1] = mbs_data->q[21] - 0.0450076;
	f[2] = mbs_data->q[15] + 0.0450076;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



int mbs_equil_fct(double x[], int nx, double fx[], MbsEquil *eq, MbsAux *aux, MbsData *s)
{
	int err = 0;
	int i, j;
	int ind_x;
	double *Fruc;

	Fruc = get_dvec_0(s->nqu + s->nqc + 1);

	// 1. Update equilibrium variables (in mbs_data) through the equilibrium pointers
	for (i = 0; i < nx; i++) // numerical process convention (starts at 0)
	{
		eq->x_ptr[i][0] = x[i];
	}

	// 2. Compute Fruc
	err = mbs_calc_Fruc(Fruc, aux, s);

	// 3. Express the equilibrium function, f(x) (i.e. the vector that has to equals 0)
	ind_x = 0;
	// 3.1 Express Fru, the force vector associated to the independant variables (Robotran Basics Eq(37) )
	for (i = 0; i < s->nqu; i++)
	{
		//if (find_int_vec(eq->options->qui,eq->options->nqui, s->qu[i]) == -1)
		//{
		fx[ind_x] = Fruc[i + 1]; //  +1 necessary to match Num and Rob convention
		ind_x++;
		//} // removed qui
	}
	// 3.2 Express Fxe, the extra equilibrium equations added by the user
	if (eq->options->nxe != 0)
	{
		(*eq->options->fxe_ptr)(s, &(fx[ind_x-1])); // -1 necessary to match Num and Rob convention
	}
	// 0. Free
	free_dvec_1(Fruc);
	return err;
}

int mbs_equil_fsolvepk(int(*fun_ptr)(double*, int, double*, MbsEquil*, MbsAux*, MbsData*), MbsEquil *eq, MbsAux *aux, MbsData *s)
{
	int i, j, k;
	int err = 0;
	int isSensitive;

	void* grad_fun_ptr = NULL; // in case of Analythical expression for the gradient

	int nx, nf;
	double  *xd, *f, *fd;
	double **grad;

	double norm_pk, normR, norm_Q;
	int it_cou, r_cou;

	int ind;

	double **grads;
	double *xds, *xs, *pgrads, *fs, *xdR, *xR;
	double norm1, norm2, inorm2, dnorm2, norm3, sc_prdct;

	int *indx;
	double d;




	// 0. initialization (and allocation)
	nx = eq->nx;
	nf = nx;
	xd = get_dvec_0(nx);
	f = get_dvec_0(nx);
	fd = get_dvec_0(nx);
	grad = get_dmat_0(nf, nx);	// Grad(f) [nf*nx]  with f [nf*1] and x [nx*1]

	// 0.1 Solver might either use a analythical evaluation or a numerical estimation for the Jacobian
	// in case of analythical evaluation a additional test might be necessary

	// 1. Gradient test - search for sensitive variables
	err = (*fun_ptr)(eq->x, nx, f, eq, aux, s);
	if (err) { return err; }

	// 1.1 Gradient estimation for f(x) :
	if (grad_fun_ptr != NULL)		// analytical evaluation
	{
		//[grad] = mygrad(x);// ?Q could be implemented if a symbolic expression is available
	}
	else							// numerical estimation
	{

		copy_dvec_0(eq->x, xd, nx);
		for (i = 0; i<nx; i++)			    //xd is, here, a small deviation from x
		{
			if (fabs(eq->x[i])>1e-2)				// if x[i] !=0
			{
				xd[i] = eq->x[i] * (1.0 + eq->options->devjac);

			}
			else								// if x[i] ~=0
			{
				xd[i] = eq->x[i] + eq->options->devjac;
			}
			err = (*fun_ptr)(xd, nx, fd, eq, aux, s);



			if (err) { return err; }
			for (j = 0; j < nf; j++)					// gradient estimation
			{
				grad[j][i] = (fd[j] - f[j]) / (xd[i] - eq->x[i]);
			}
			xd[i] = eq->x[i];
		}
	}

	// 1.2. Gradient testing (removal of non-sensitive variables)
	eq->nxs = 0;
	eq->nxns = 0;
	for (i = 0; i < nx; i++)			// test all equilibrium variables
	{
		isSensitive = 0;
		for (j = 0; j<nf; j++)
		{
			if (fabs(grad[j][i]) > fabs(eq->options->senstol)) // sensitive if there is a non-zero element among the ith column
			{
				isSensitive = 1;
				break;
			}
		}
		if (isSensitive)			// the ith variable is sensitive
		{
			eq->xs[eq->nxs] = i;		// write the vector, eq->xs, with the index of sensitive variables
			(eq->nxs)++;				// increment the number of sensitive variables
		}
		else					// the ith variable is non sensitive (all the column is 0)
		{
			eq->xns[eq->nxns] = i;	// write the vector, eq->xs, with the index of non sensitive variables
			(eq->nxns)++;			// increment the number of non sensitive variables
			for (j = 0; j < nf; j++)
			{
				grad[j][i] = 0.0;	// set gradient line for the non sensitive variables to 0
			}
		}
	}

	// 1.3 Additional Tests (numerical gradient singularity and existence of a sensitive variable)
	if (rank_0(grad, nf, nx) != eq->nxs)
	{
		printf(">>EQUIL>> mbs_equil_fsolvepk: After eliminating the non sensitive variables, the numerical gradient is still singular !\n");
		err = 1;
		return err;
	}
	if (eq->nxs == 0)
	{
		printf(">>EQUIL>> mbs_equil_fsolvepk: none of the proposed parameter are sensitive !\n");
		err = 0;
		return err;
	}

	// 2. Second initialization (and allocation)
	grads = get_dmat_0(eq->nxs, eq->nxs);
	xds = get_dvec_0(eq->nxs);
	xs = get_dvec_0(eq->nxs);
	pgrads = get_dvec_0(eq->nxs);
	fs = get_dvec_0(eq->nxs);
	xdR = get_dvec_0(eq->nxs);
	xR = get_dvec_0(eq->nxs);

	// 3. Solution searching

	// Initial values for Iterative procedure
	err = (*fun_ptr)(eq->x, nx, f, eq, aux, s);
	if (err) { return err; }
	eq->norm_pk = norm_dvec_0(f, nf);
	it_cou = 0;
	r_cou = 0;
	normR = DBL_MAX;
	// Iterate as long as the equilibrium tolerance is not reached
	while ((eq->norm_pk > eq->options->equitol) && (it_cou < eq->options->itermax))
	{
		if (eq->options->verbose)
		{
			printf(">>EQUIL>> ---> x: "); print_dvec_0(eq->x, eq->nx);
			printf(">>EQUIL>> ---> f: "); print_dvec_0(f, eq->nx);
			printf(">>EQUIL>> ---> Residue = %f\n", eq->norm_pk);
		}

		// 3.1 Gradient Estimation
		zeros_dmat_0(grad, nf, nx);			// initialize the gradient
		if (grad_fun_ptr != NULL)				// analytical evaluation
		{
			//[grad] = mygrad(x);
		}
		else									// numerical estimation (similar to 1.1)
		{
			for (j = 0; j < eq->nxns; j++)			//xd is here a small deviation from x
			{
				xd[eq->xns[j]] = eq->x[eq->xns[j]];
			}
			for (j = 0; j < eq->nxs; j++)
			{
				xd[eq->xs[j]] = eq->x[eq->xs[j]];
			}
			for (i = 0; i<eq->nxs; i++)				// only sensitive variables
			{
				ind = eq->xs[i];
				// xd as a neighboring value of x
				if (fabs(eq->x[ind])>1e-2)		// if x[i] !=0
				{
					xd[ind] = eq->x[ind] * (1 + eq->options->devjac);
				}
				else							// if x[i] ~=0
				{
					xd[ind] = eq->x[ind] + eq->options->devjac;
				}
				// gradient calculation
				err = (*fun_ptr)(xd, nx, fd, eq, aux, s);
				if (err) { return err; }
				for (j = 0; j < nf; j++)
				{
					grad[j][ind] = (fd[j] - f[j]) / (xd[ind] - eq->x[ind]);  // Gradient Estimation (all variables) :  [nf * nx]
				}
				xd[ind] = eq->x[ind];// changing back again
			}
		}
		// Gradient Resizing (sensitive variables only) :  [nfs * nx]
		for (i = 0; i < eq->nxs; i++)
		{
			for (j = 0; j < eq->nxs; j++)
			{
				grads[i][j] = grad[eq->xs[i]][eq->xs[j]];
			}
		}

		// 3.2 xd = f(x)/J(x) calculation
		slct_dvec_0(f, nf, eq->xs, eq->nxs, fs);				// resizing f->fs
		indx = get_ivec_0(eq->nxs);							//
		copy_dvec_0(fs, xds, eq->nxs);					//
		ludcmp_0(grads, eq->nxs, indx, &d);					// those functions make the matrix division (inversion)
		lubksb_0(grads, eq->nxs, indx, xds);				//

		free_ivec_0(indx);
		//save_double_vec(xd_x_sens, eq->nxs,"xd_sens.txt");
		for (i = 0; i < eq->nxs; i++)
		{
			xd[eq->xs[i]] = xds[i];							// now xd contains f(x)/Japprox(x)
		}


		// 3.3 softing (some crazy Polish algorithm ... !) QD
		if (eq->options->smooth)
		{
			slct_dvec_0(eq->x, nx, eq->xs, eq->nxs, xs);
			norm1 = norm_dvec_0(xs, eq->nxs);
			norm2 = norm_dvec_0(xds, eq->nxs);
			inorm2 = norm2;
			dnorm2 = norm2;
			if (inorm2< 1e-6)
			{
				norm2 = 1.0;
				dnorm2 = 1.0;
			}
			if ((norm1< 1e-2) && (inorm2>1e-1))
			{
				dnorm2 = 1e-1;
			}
			if ((norm1 >= 1e-2) && ((inorm2 / norm1)>0.8))
			{
				sc_prdct = 0.0;
				for (i = 0; i < nx; i++)
				{
					sc_prdct += xd[i] * eq->x[i];
				}
				norm3 = fabs(sc_prdct / (norm1*norm2));    // error perpendicular to actual position
				if (norm3 < 0.3)
				{
					dnorm2 = norm1 * 1e-1;
				}
				else
				{
					dnorm2 = norm1 * 0.8;
				}
			}

			if ((norm1 >= 1e-2) && (inorm2 / norm1>300))
			{
				dnorm2 = 1e-1 * norm1;       // "grad" singularity possible
			}
			for (i = 0; i < eq->nxs; i++)
			{
				xd[eq->xs[i]] = (dnorm2 / norm2) * xd[eq->xs[i]];
			}
			slct_dvec_0(xd, nx, eq->xs, eq->nxs, xdR);
		}
		else
		{
			slct_dvec_0(xd, nx, eq->xs, eq->nxs, xdR);
			norm2 = norm_dvec_0(xdR, eq->nxs);
			dnorm2 = norm2;
		}

		// 3.4 Newton-Raphson Iteration:  x_(k+1)=x_(k) - xd
		for (i = 0; i < eq->nxs; i++)
		{
			eq->x[eq->xs[i]] = eq->x[eq->xs[i]] - xd[eq->xs[i]];  // with now, xd=f(x_k)/J(x_k)
		}

		// 3.5 Relaxation
		err = (*fun_ptr)(eq->x, nx, f, eq, aux, s); if (err) { return err; }
		eq->norm_pk = norm_dvec_0(f, nf);
		norm_Q = eq->norm_pk;
		while (eq->norm_pk > normR && r_cou < 10)
		{

			for (i = 0; i < eq->nxs; i++)
			{
				xdR[i] = eq->options->relax * xdR[i];
				eq->x[eq->xs[i]] = xR[i] - xdR[i];
			}
			r_cou = r_cou + 1;
			err = (*fun_ptr)(eq->x, nx, f, eq, aux, s); if (err) { return err; }
			eq->norm_pk = norm_dvec_0(f, nf);
			if (eq->options->verbose)
			{
				printf(">>EQUIL>> ----> ... Relaxation - Residue = %f\n", eq->norm_pk);
			}
		}
		it_cou = it_cou + 1;
		if (dnorm2 == norm2)
		{
			normR = 0.9*eq->norm_pk;
		}
		else
		{
			normR = eq->norm_pk;
		}
		slct_dvec_0(eq->x, nx, eq->xs, eq->nxs, xR);
		r_cou = 0;          // iteration counter actualisation

		if (eq->norm_pk > norm_Q)
		{
			eq->norm_pk = norm_Q;
		}
	}

	free_dmat_0(grads);

	free_dvec_0(xd);
	free_dvec_0(f);
	free_dvec_0(fd);

	free_dvec_0(xds);
	free_dvec_0(xs);
	free_dvec_0(pgrads);
	free_dvec_0(fs);
	free_dvec_0(xdR);
	free_dvec_0(xR);

	free_dmat_0(grad);

	if (eq->options->verbose)
	{
		printf(">>EQUIL>> ---> Residue = %f\n", eq->norm_pk);
	}
	if (it_cou >= eq->options->itermax)
	{
		printf(">>EQUIL>> mbs_equil_fsolvepk : Equilibrium has not converged after %d iterations\n", eq->options->itermax);
		err = 1;
		return err;
	}

	return err;
}

























/*

void choldc_0(double **a, int n, double *p)
{
	//void nrerror(char error_text[]);
	int i, j, k;
	double sum;

	for (i = 0; i < n; i++)
	{
		for (j = i; j < n; j++)
		{
			for (sum = a[i][j], k = i - 1; k >= 0; k--)
			{
				sum -= a[i][k] * a[j][k];
			}
			if (i == j)
			{
				if (sum <= 0.0)
				{
					//nrerror("choldc failed");
					printf("choldc failed/n");
				}
				p[i] = sqrt(sum);
			}
			else
			{
				a[j][i] = sum / p[i];
			}
		}
	}
}
void cholsl_0(double **a, int n, double *p, double *b, double *x)
{
	int i, k;
	double sum;

	for (i = 0; i < n; i++)
	{
		for (sum = b[i], k = i - 1; k >= 0; k--)
		{
			sum -= a[i][k] * x[k];
		}
		x[i] = sum / p[i];
	}
	for (i = n - 1; i >= 0; i--)
	{
		for (sum = x[i], k = i + 1; k < n; k++)
		{
			sum -= a[k][i] * x[k];
		}
		x[i] = sum / p[i];
	}
}

#define NRANSI
#define TINY 1.0e-20

void ludcmp_0(double **a, int n, int *indx, double *d)
{
	int i, imax, j, k;
	double big, dum, sum, temp;
	double *vv;

	imax = 0;

	vv = get_dvec_0(n);
	*d = 1.0;
	for (i = 0; i < n; i++) {
		big = 0.0;
		for (j = 0; j<n; j++)
			if ((temp = fabs(a[i][j])) > big) big = temp;
		if (big == 0.0)
		{
			//nrerror("Singular matrix in routine ludcmp");
			printf("Singular matrix in routine ludcmp\n");
		}
		vv[i] = 1.0 / big;
	}
	for (j = 0; j < n; j++) {
		for (i = 0; i < j; i++) {
			sum = a[i][j];
			for (k = 0; k < i; k++) sum -= a[i][k] * a[k][j];
			a[i][j] = sum;
		}
		big = 0.0;
		for (i = j; i < n; i++) {
			sum = a[i][j];
			for (k = 0; k < j; k++)
				sum -= a[i][k] * a[k][j];
			a[i][j] = sum;
			if ((dum = vv[i] * fabs(sum)) >= big) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) {
			for (k = 0; k < n; k++) {
				dum = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k] = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (a[j][j] == 0.0) a[j][j] = TINY;
		if (j != n - 1) {
			dum = 1.0 / (a[j][j]);
			for (i = j + 1; i < n; i++) a[i][j] *= dum;
		}
	}
	free_double_vec(vv);
}
#undef TINY
#undef NRANSI

void lubksb_0(double **a, int n, int *indx, double b[])
{
	int i, ii = -1, ip, j;
	double sum;

	for (i = 0; i < n; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii != -1)
			for (j = ii; j <= i - 1; j++) sum -= a[i][j] * b[j];
		else if (sum) ii = i;
		b[i] = sum;
	}
	for (i = n - 1; i >= 0; i--) {
		sum = b[i];
		for (j = i + 1; j < n; j++) sum -= a[i][j] * b[j];
		b[i] = sum / a[i][i];
	}
}
*/

void mbs_print_equil(MbsEquil* eq)
{
	int i;

	printf("\n MbsEquil \n");
	printf("Options \n");

	//printf(" nqui . . . .   = %d \n", eq->options->nqui);
	//print_intvector("qui ",eq->options->nqui,eq->options->qui);

	printf(" nquch . . . .  = %d \n", eq->options->nquch);
	print_intvector("quch", eq->options->nquch, eq->options->quch);


	printf("Variables \n");
	printf(" nx . . . .  = %d \n", eq->nx);

	print_vector("x   ", eq->nx, &(eq->x[0]) - 1);
	printf("   xch  = [");
	for (i = 1; i <= eq->options->nquch; i++) {
		printf(" %f ", *(eq->options->xch_ptr[i]));
	}
	printf("]\n");
	printf("   xe   = [");
	for (i = 1; i <= eq->options->nxe; i++) {
		printf(" %f ", *(eq->options->xe_ptr[i]));
	}
	printf("]\n");

	print_intvector("  xs  ", eq->nxs, &(eq->xs[0]) - 1);
	print_intvector("  xns ", eq->nxns, &(eq->xns[0]) - 1);

	/*
	printf(" nxe . . . .  = %d \n", eq->options->nxe);
	printf("xe_ptr . . . \n");
	for(i=1;i<=eq->options->nquch;i++)
	{
	printf("val xe_ptr[%d]  %f \n",i,*(eq->options->xe_ptr[i]));
	}*/

}
