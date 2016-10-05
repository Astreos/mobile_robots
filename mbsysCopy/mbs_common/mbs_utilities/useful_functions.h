/*
 * Main header of some useful functions
 *
 * author: Nicolas Van der Noot & Aubain Verle
 */
#ifndef _USEFUL_FUNCTIONS_H_
#define _USEFUL_FUNCTIONS_H_

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mbsysc_utilities_export.h"

#ifndef DBL_MAX
#define DBL_MAX 1.7976931348623158e+308
#endif

/**
 *  Function for logging messages in a file called "log" where
 *  the program is executed.
 *  It is useful for debugging when the printf messages do 
 *  not appear in the console (for instance in Simulink).
 * 
 */
void mbs_log(char* msg, ...);


// exponents related functions
MBSYSC_UTILITIES_EXPORT double pow_int(double a, int b);
MBSYSC_UTILITIES_EXPORT int expo_ten_inf(double nb);

// sign function
MBSYSC_UTILITIES_EXPORT double sign(double a);

// time function
MBSYSC_UTILITIES_EXPORT char* get_time_machine();

// char related functions
MBSYSC_UTILITIES_EXPORT char* get_char_vec(int x);
MBSYSC_UTILITIES_EXPORT char** get_char_tab(int x, int y);
MBSYSC_UTILITIES_EXPORT void free_char_vec(char *vec);
MBSYSC_UTILITIES_EXPORT void free_char_tab(char** mat);

// integer related functions
MBSYSC_UTILITIES_EXPORT int* get_ivec_0(int x);
MBSYSC_UTILITIES_EXPORT int* get_ivec_1(int x);
MBSYSC_UTILITIES_EXPORT int** get_imat_0(int x, int y);
MBSYSC_UTILITIES_EXPORT int** get_imat_1(int x, int y);

MBSYSC_UTILITIES_EXPORT void free_ivec_0(int *vec);
MBSYSC_UTILITIES_EXPORT void free_ivec_1(int *vec);
MBSYSC_UTILITIES_EXPORT void free_imat_0(int **mat);
MBSYSC_UTILITIES_EXPORT void free_imat_1(int **mat);

MBSYSC_UTILITIES_EXPORT void copy_ivec_0(int *vec1, int *vec2, int x);
MBSYSC_UTILITIES_EXPORT void copy_ivec_1(int *vec1, int *vec2, int x);
MBSYSC_UTILITIES_EXPORT void copy_imat_0(int **tab1, int **tab2, int x, int y);

MBSYSC_UTILITIES_EXPORT void print_ivec_0(int *vec, int x);
MBSYSC_UTILITIES_EXPORT void print_ivec_1(int *vec);
MBSYSC_UTILITIES_EXPORT void print_imat_0(int **mat, int x, int y);

MBSYSC_UTILITIES_EXPORT void sort_ivec_0(int *vec1, int *vec2, int x);
MBSYSC_UTILITIES_EXPORT void sort_ivec_1(int *vec1, int *vec2, int x);
/*! \brief sort and modify the (x sized) vector of integers, starting with index 1
 * The "Bubble sort" is used, complexity is O(n^2)
 *
 * \param[in,out] vec the vector to be sorted
 * \param[in] x the size of the vector (0 element not taken into account)
 */
MBSYSC_UTILITIES_EXPORT void sort_inplace_ivec_1(int *vec, int x);
MBSYSC_UTILITIES_EXPORT void f0123_ivec_0(int* vec, int x);
MBSYSC_UTILITIES_EXPORT void conc_ivec_0(int* vec1, int x1, int* vec2, int x2, int* vec3);
MBSYSC_UTILITIES_EXPORT void conc_ivec_1(int* vec1, int x1, int* vec2, int x2, int* vec3);
MBSYSC_UTILITIES_EXPORT void slct_ivec_0(int* vec1, int x1, int* vec2, int x2, int* vec3);
MBSYSC_UTILITIES_EXPORT int find_ivec_0(int* vec, int x, int f);
MBSYSC_UTILITIES_EXPORT int find_ivec_1(int* vec, int x, int f);

// double related functions
MBSYSC_UTILITIES_EXPORT double* get_dvec_0(int x);
MBSYSC_UTILITIES_EXPORT double* get_dvec_1(int x);
MBSYSC_UTILITIES_EXPORT double** get_dmat_0(int x, int y);
MBSYSC_UTILITIES_EXPORT double** get_dmat_1(int x, int y);

MBSYSC_UTILITIES_EXPORT void free_dvec_0(double *vec);
MBSYSC_UTILITIES_EXPORT void free_dvec_1(double *vec);
MBSYSC_UTILITIES_EXPORT void free_dmat_0(double **mat);
MBSYSC_UTILITIES_EXPORT void free_dmat_1(double **mat);

MBSYSC_UTILITIES_EXPORT void copy_dvec_0(double *vec1, double *vec2, int x);
MBSYSC_UTILITIES_EXPORT void copy_dvec_1(double *vec1, double *vec2);
MBSYSC_UTILITIES_EXPORT void copy_dmat_0(double **tab1, double **tab2, int x, int y);

MBSYSC_UTILITIES_EXPORT void print_dvec_0(double *vec, int x);
MBSYSC_UTILITIES_EXPORT void print_dvec_1(double *vec);
MBSYSC_UTILITIES_EXPORT void print_dmat_0(double **mat, int x, int y);

MBSYSC_UTILITIES_EXPORT void save_dvec_0(double *vec, int x,char *name);
MBSYSC_UTILITIES_EXPORT void save_dmat_0(double **mat, int x, int y, char *name);

MBSYSC_UTILITIES_EXPORT void slct_dvec_0(double* vec1, int x1, int* vec2, int x2, double* vec3);
MBSYSC_UTILITIES_EXPORT void slct_dmat_0(double **tab1, int x1, int y1, double **tab2, int y2, int* vec);
MBSYSC_UTILITIES_EXPORT void slctr_dmat_0(double **tab1, int x1, int y1, double **tab2, int x2, int* vec);

MBSYSC_UTILITIES_EXPORT int any_dvec_0(double *vec, int x);
MBSYSC_UTILITIES_EXPORT double norm_dvec_0(double *vec, int x);
MBSYSC_UTILITIES_EXPORT void zeros_dmat_0(double **mat, int x, int y);
MBSYSC_UTILITIES_EXPORT void zeros_dvec_0(double *vec, int x);

#endif
