//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef gsl_fct_h
#define gsl_fct_h
/*--------------------*/


/**
*
* in_matrix : matrix for the Eigenvalue problem
* n         : size of the matrix
* eval_a    : Result of the Eigenvalue problem: imaginary part of the 2*nx eigen values						[n X 1]
* eval_b	: Result of the Eigenvalue problem: imaginary part of the 2*nx eigen values						[n X 1]
* emat_r	: Result of the Eigenvalue problem: norm  of the eigen vectors (each column is a eigenvector)	[n X n]
* emat_phi	: Result of the Eigenvalue problem: phase of the eigen vectors (each column is a eigenvector)	[n X n]
*/
int eig_0(double **in_matrix, int n, double *eval_a, double *eval_b, double **emat_r, double **emat_phi);

/**
*
* in_matrix : matrix for the rank computation
* x : matrix first dimension
* y : matrix second dimension
*/
int rank_0(double** matrix_in, int x, int y);

#endif
