//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.9.3
//
// Creation : 2006
// Last update : 01/08/2016
//---------------------------

#include <stdio.h>
#include <math.h>
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_eigen.h"

/**
 * Compute the Eigen analysis 
 *
 * using gsl libraries ... 
 *
 *
 *
 */
int eig_0(double **in_matrix, int n, double *eval_a, double *eval_b, double **emat_r, double **emat_phi)
{
	int i, j;
	double max_r, phi_0;
	int ind_max_r;

	gsl_matrix * matrix = gsl_matrix_alloc(n, n);

	gsl_vector_complex * r_vec = gsl_vector_complex_alloc(n);
	gsl_matrix_complex * r_mat = gsl_matrix_complex_alloc(n, n);

	gsl_complex cplx;

	gsl_eigen_nonsymmv_workspace * w = gsl_eigen_nonsymmv_alloc(n);


	ind_max_r = 0.0; // Q??   line added needed for some project but not forl all... non sense ?
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			gsl_matrix_set(matrix, i, j, in_matrix[i][j]);
		}
	}

	gsl_eigen_nonsymmv(matrix, r_vec, r_mat, w);

	//printf("\n\n\n  ind_max_r : %d\n\n\n", ind_max_r);

	for (i = 0; i < n; i++)
	{
		cplx = gsl_vector_complex_get(r_vec, i);
		eval_a[i] = GSL_REAL(cplx);
		eval_b[i] = GSL_IMAG(cplx);

		max_r = 0.0;
		for (j = 0; j<n; j++)
		{
			cplx = gsl_matrix_complex_get(r_mat, j, i);
			emat_r[j][i] = sqrt(GSL_REAL(cplx) * GSL_REAL(cplx) + GSL_IMAG(cplx) * GSL_IMAG(cplx));
			emat_phi[j][i] = atan2(GSL_IMAG(cplx), GSL_REAL(cplx));
			if (emat_r[j][i]>max_r)
			{
				max_r = emat_r[j][i];
				ind_max_r = j;
			}
		}
		phi_0 = emat_phi[ind_max_r][i];
		for (j = 0; j < n; j++)
		{
			emat_r[j][i] = emat_r[j][i] / max_r;
			emat_phi[j][i] = emat_phi[j][i] - phi_0;
		}
	}

	gsl_matrix_free(matrix);
	gsl_vector_complex_free(r_vec);
	gsl_matrix_complex_free(r_mat);

	gsl_eigen_nonsymmv_free(w);

	return EXIT_SUCCESS;
}