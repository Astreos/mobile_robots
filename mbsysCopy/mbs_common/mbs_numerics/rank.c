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

/**
 * Compute the rank of matrix_in
 *
 * using gsl libraries ... 
 *
 *
 *
 */

int rank_0(double** matrix_in, int x, int y)
{
	int i, j, n, m;
	int rank;

	gsl_matrix * A;
	gsl_matrix * V;
	gsl_vector * S;
	gsl_vector * work;

	if (x>y)
	{
		m = x;
		n = y;
	}
	else
	{
		m = y;
		n = x;
	}

	A = gsl_matrix_alloc(m, n);
	V = gsl_matrix_alloc(n, n);
	S = gsl_vector_alloc(n);
	work = gsl_vector_alloc(n);

	for (i = 0; i<x; i++)
	{
		for (j = 0; j<y; j++)
		{
			if (x>y)
			{
				gsl_matrix_set(A, i, j, matrix_in[i][j]);
			}
			else
			{
				gsl_matrix_set(A, j, i, matrix_in[i][j]);
			}
		}
	}

	gsl_linalg_SV_decomp(A, V, S, work);

	rank = 0;
	for (i = 0; i<n; i++)
	{
		if (fabs(gsl_vector_get(S, i))> 1e-30)
		{
			rank++;
		}
	}

	gsl_matrix_free(A);
	gsl_matrix_free(V);
	gsl_vector_free(S);
	gsl_vector_free(work);

	return rank;
}