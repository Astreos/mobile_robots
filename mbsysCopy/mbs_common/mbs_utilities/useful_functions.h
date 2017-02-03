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
/*! \brief compute efficiently pow with an integer exponent
 *
 * \param[in] a basis (double)
 * \param[in] b integer exponant
 * \return a^b (same result as pow(a,b), except that b is a integer -> more efficient)
 */
MBSYSC_UTILITIES_EXPORT double pow_int(double a, int b);
/*! \brief get log with base 10
 *
 * \param[in] nb number to compute the log
 * \return the exponent (base 10) -> log_10 (this is the inferior exponent)
 */
MBSYSC_UTILITIES_EXPORT int expo_ten_inf(double nb);

// sign function
/*! \brief sign function
 *
 * \param[in] a input
 * \return 1 if positive (or 0), -1 if negative
 */
MBSYSC_UTILITIES_EXPORT double sign(double a);

// time function
/*! \brief returns the current time as a String
 *
 * source: http://en.wikipedia.org/wiki/C_date_and_time_functions
 */
MBSYSC_UTILITIES_EXPORT char* get_time_machine();

// char related functions
/*! \brief create (with memory allocation) a vector (length x) of char
 *
 * \param[in] x size of the vector
 * \return requested vector of char
 */
MBSYSC_UTILITIES_EXPORT char* get_char_vec(int x);
/*! \brief create (with memory allocation) a [x times y] matrix of chars
 *
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 * \return requested matrix of chars
 */
MBSYSC_UTILITIES_EXPORT char** get_char_tab(int x, int y);
/*! \brief release memory for a vector of char
 *
 * \param[out] vec vector of chars
 */
MBSYSC_UTILITIES_EXPORT void free_char_vec(char *vec);
/*! \brief release memory for a matrix of chars
 *
 * \param[out] mat matrix of chars
 */
MBSYSC_UTILITIES_EXPORT void free_char_tab(char** mat);

// integer related functions
/*! \brief create (with memory allocation) a vector (length x) of integers, starting at index 0
 *
 * \param[in] x size of the vector
 * \return requested vector
 */
MBSYSC_UTILITIES_EXPORT int* get_ivec_0(int x);
/*! \brief create (with memory allocation) a vector (length x) of integers, starting at index 1
 *
 * \param[in] x size of the vector (0 element not taken into account)
 * \return requested vector (element 0 receives x: the size without this element)
 */
MBSYSC_UTILITIES_EXPORT int* get_ivec_1(int x);
/*! \brief create (with memory allocation) a [x times y] matrix of integers, starting at index 0
 *
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 * \return requested matrix of integers
 */
MBSYSC_UTILITIES_EXPORT int** get_imat_0(int x, int y);
/*! \brief create (with memory allocation) a [x times y] matrix of integers, starting at index 1
 *
 * \param[in] x nb of rows (0 element not taken into account)
 * \param[in] y nb of columns (0 element not taken into account)
 * \return requested matrix of integers
 */
MBSYSC_UTILITIES_EXPORT int** get_imat_1(int x, int y);

/*! \brief release memory for a vector of integers, starting with index 0
 *
 * \param[out] vec a vector of integers
 */
MBSYSC_UTILITIES_EXPORT void free_ivec_0(int *vec);
/*! \brief release memory for a vector of integers, starting with index 1
 *
 * \param[out] vec a vector of integers
 */
MBSYSC_UTILITIES_EXPORT void free_ivec_1(int *vec);
/*! \brief release memory for a matrix of integers, starting with index 0
 *
 * \param[out] mat matrix of integers
 */
MBSYSC_UTILITIES_EXPORT void free_imat_0(int **mat);
/*! \brief release memory for a matrix of integers, starting with index 1
 *
 * \param[out] mat matrix of integers
 */
MBSYSC_UTILITIES_EXPORT void free_imat_1(int **mat);

/*! \brief Copy the content of a vector (of size x) of integers to a second vector, starting with index 0
 *
 * \param[in] vec1 the original vector of integer, starting with index 0
 * \param[out] vec2 the copy of vec1, starting with index 0
 * \param[in] x size of the vectors
 */
MBSYSC_UTILITIES_EXPORT void copy_ivec_0(int *vec1, int *vec2, int x);
/*! \brief Copy the content of a vector (of size x) of integers to a second vector, starting with index 1
 *
 * \param[in] vec1 the original vector of integer, starting with index 1
 * \param[out] vec2 the copy of vec1, starting with index 1
 * \param[in] x size of the vectors (0 element not taken into account)
 */
MBSYSC_UTILITIES_EXPORT void copy_ivec_1(int *vec1, int *vec2, int x);
 /*! \brief Copy the content of a [x times y] matrix of integers to a second matrix, starting with index 0
 *
 * \param[in] tab1 the original matrix of integer, starting with index 0
 * \param[out] tab2 the copy of tab1, starting with index 0
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 */
MBSYSC_UTILITIES_EXPORT void copy_imat_0(int **tab1, int **tab2, int x, int y);

/*! \brief print a (x sized) vector of integers in console, starting with index 0
 * 
 * \param[in] vec the vector to be printed
 * \param[in] x the size of the vector
 */
MBSYSC_UTILITIES_EXPORT void print_ivec_0(int *vec, int x);
/*! \brief print a vector (of size specified in vec[0]) of integers in console, starting with index 1
 * 
 * \param[in] vec the vector to be printed
 */
MBSYSC_UTILITIES_EXPORT void print_ivec_1(int *vec);
 /*! \brief print a [x times y] matrix of integers in console, starting with index 0
 * 
 * \param[in] mat the matrix to be printed
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 */
MBSYSC_UTILITIES_EXPORT void print_imat_0(int **mat, int x, int y);

/*! \brief sort the first (x sized) vector of integers into the second vector, starting with index 0
 * 
 * \param[in] vec1 the vector to be sorted
 * \param[out] vec2 the sorted vector
 * \param[in] x the size of the vectors
 */
MBSYSC_UTILITIES_EXPORT void sort_ivec_0(int *vec1, int *vec2, int x);
/*! \brief sort the first (x sized) vector of integers into the second vector, starting with index 1
 * 
 * \param[in] vec1 the vector to be sorted, starting with index 1
 * \param[out] vec2 the sorted vector, starting with index 1
 * \param[in] x the size of the vectors (0 element not taken into account)
 */
MBSYSC_UTILITIES_EXPORT void sort_ivec_1(int *vec1, int *vec2, int x);
/*! \brief sort and modify the (x sized) vector of integers, starting with index 1
 * The "Bubble sort" is used, complexity is O(n^2)
 *
 * \param[in,out] vec the vector to be sorted
 * \param[in] x the size of the vector (0 element not taken into account)
 */
MBSYSC_UTILITIES_EXPORT void sort_inplace_ivec_1(int *vec, int x);
/*! \brief Fill the (x sized) vector of integers as [0 1 2 3 ... x-1], starting with index 0
 * 
 * \param[out] vec the vector to be filled
 * \param[in] x the size of the vector
 */
MBSYSC_UTILITIES_EXPORT void f0123_ivec_0(int* vec, int x);
/*! \brief concatenate the first (x1 sized) vector and the second (x2 sized) vectors of integers into the third (size >=x1+x2) vector (without memory allocation), starting with index 0
 * 
 * \param[in] vec1  the first vector to be concatenated
 * \param[in] x1    the size of the first vector
 * \param[in] vec2  the second vector to be concatenated
 * \param[in] x2    the size of the second vector
 * \param[out] vec3 the concatenation of both vector such as vec3 = [vec1 vec2]. The vector have to be already allocated to a sufficient size.
 */
MBSYSC_UTILITIES_EXPORT void conc_ivec_0(int* vec1, int x1, int* vec2, int x2, int* vec3);
/*! \brief concatenate the first (x1 sized) vector and the second (x2 sized) vectors of integers into the third (size >=x1+x2) vector (without memory allocation), starting with index 1
 * 
 * \param[in] vec1  the first vector to be concatenated, starting with index 1
 * \param[in] x1    the size of the first vector (0 element not taken into account)
 * \param[in] vec2  the second vector to be concatenated, starting with index 1
 * \param[in] x2    the size of the second vector (0 element not taken into account)
 * \param[out] vec3 the concatenation of both vector such as vec3 = [vec1 vec2], starting with index 1. The vector have to be already allocated to a sufficient size.
 */
MBSYSC_UTILITIES_EXPORT void conc_ivec_1(int* vec1, int x1, int* vec2, int x2, int* vec3);
/*! \brief fill the third (x2 sized) vector by slicing the first (x1 sized) vector of integers at the indexes specified in the second (x2 sized) vector, starting with index 0 
 *
 * \param[in] vec1  the vector containing the original values to be sliced
 * \param[in] x1    the size of the vector vec1
 * \param[in] vec2  the vector containing the indexes of the slices
 * \param[in] x2    the size of the vector vec2,
 * \param[out] vec3 the sliced vector such as vec3[i] = vec1[vec2[i]], starting with index 0. The vector have to be already allocated to a sufficient (x2) size.
 */
MBSYSC_UTILITIES_EXPORT void slct_ivec_0(int* vec1, int x1, int* vec2, int x2, int* vec3);
/*! \brief return the index of the value f in the (x sized) vector vec of integers, starting with index 0. Return -1 if not found.
 *
 * \param[in] vec a vector of integers, starting with index 0
 * \param[in] x   the size of the vector
 * \param[in] f   the integer to look after in the vector
 *
 * \return -1 if f does not belong to vec, otherwise it returns the index of f in vec.
 */
MBSYSC_UTILITIES_EXPORT int find_ivec_0(int* vec, int x, int f);
/*! \brief return the index of the value f in the (x sized) vector vec of integers, starting with index 1. Return -1 if not found.
 *
 * \param[in] vec a vector of integers, starting with index 1
 * \param[in] x   the size of the vector (0 element not taken into account)
 * \param[in] f   the integer to look after in the vector
 *
 * \return -1 if f does not belong to vec, otherwise it returns the index of f in vec.
 */
MBSYSC_UTILITIES_EXPORT int find_ivec_1(int* vec, int x, int f);

// double related functions
/*! \brief create (with memory allocation) a vector (length x) of doubles, starting at index 0
 *
 * \param[in] x size of the vector
 * \return requested vector
 */
MBSYSC_UTILITIES_EXPORT double* get_dvec_0(int x);
/*! \brief create (with memory allocation) a vector (length x) of doubles, starting at index 1
 *
 * \param[in] x size of the vector (0 element not taken into account)
 * \return requested vector (element 0 receives x: the size without this element)
 */
MBSYSC_UTILITIES_EXPORT double* get_dvec_1(int x);
/*! \brief create (with memory allocation) a [x times y] matrix of doubles, starting at index 0
 *
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 * \return requested matrix of doubles
 */
MBSYSC_UTILITIES_EXPORT double** get_dmat_0(int x, int y);
/*! \brief create (with memory allocation) a [x times y] matrix of doubles, starting at index 1
 *
 * \param[in] x nb of rows (0 element not taken into account)
 * \param[in] y nb of columns (0 element not taken into account)
 * \return requested matrix of doubles
 */
MBSYSC_UTILITIES_EXPORT double** get_dmat_1(int x, int y);

/*! \brief release memory for a vector of doubles, starting at index 0
 *
 * \param[out] vec a vector of doubles
 */
MBSYSC_UTILITIES_EXPORT void free_dvec_0(double *vec);
/*! \brief release memory for a vector of doubles, starting at index 1
 *
 * \param[out] vec a vector of doubles
 */
MBSYSC_UTILITIES_EXPORT void free_dvec_1(double *vec);
/*! \brief release memory for a matrix of doubles, starting at index 0
 *
 * \param[out] mat matrix of doubles
 */
MBSYSC_UTILITIES_EXPORT void free_dmat_0(double **mat);
/*! \brief release memory for a matrix of doubles, starting at index 1
 *
 * \param[out] mat matrix of doubles
 */
MBSYSC_UTILITIES_EXPORT void free_dmat_1(double **mat);

/*! \brief Copy the content of a vector (of size x) of doubles to a second vector, starting with index 0
 *
 * \param[in] vec1 the original vector of doubles, starting with index 0
 * \param[out] vec2 the copy of vec1, starting with index 0
 * \param[in] x size of the vectors
 */
MBSYSC_UTILITIES_EXPORT void copy_dvec_0(double *vec1, double *vec2, int x);
/*! \brief Copy the content of a vector (of size specified in vec1[0]) of doubles to a second vector, starting with index 1
 *
 * \param[in] vec1 the original vector of doubles(of size specified in vec1[0]), starting with index 1
 * \param[out] vec2 (already allocated at size specified in vec1[0]) the copy of vec1, starting with index 1
 */
MBSYSC_UTILITIES_EXPORT void copy_dvec_1(double *vec1, double *vec2);
 /*! \brief Copy the content of a [x times y] matrix of doubles to a second matrix, starting with index 0
 *
 * \param[in] tab1 the original matrix of doubles, starting with index 0
 * \param[out] tab2 the copy of tab1, starting with index 0
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 */
MBSYSC_UTILITIES_EXPORT void copy_dmat_0(double **tab1, double **tab2, int x, int y);

/*! \brief print a (x sized) vector of doubles in console, starting with index 0
 * 
 * \param[in] vec the vector to be printed
 * \param[in] x the size of the vector
 */
MBSYSC_UTILITIES_EXPORT void print_dvec_0(double *vec, int x);
/*! \brief print a vector (of size specified in vec[0]) of doubles in console, starting with index 1
 * 
 * \param[in] vec the vector to be printed
 */
MBSYSC_UTILITIES_EXPORT void print_dvec_1(double *vec);
 /*! \brief print a [x times y] matrix of doubles in console, starting with index 0
 * 
 * \param[in] mat the matrix to be printed
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 */
MBSYSC_UTILITIES_EXPORT void print_dmat_0(double **mat, int x, int y);

/*! \brief save a (x sized) vector of doubles in the specified file, starting with index 0
 * The file is open with the option "wt"
 *
 * \param[in] vec the vector to save, starting with index 0
 * \param[in] x the size of the vector
 * \param[in] name the path an name of the file in which saving the vector
 */
MBSYSC_UTILITIES_EXPORT void save_dvec_0(double *vec, int x,char *name);
/*! \brief save a [x times y] matrix of doubles to a second matrix, starting with index 0
 * The file is open with the option "wt"
 *
 * \param[in] mat the matrix to save, starting with index 0
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 * \param[in] name the path an name of the file in which saving the vector
 */
MBSYSC_UTILITIES_EXPORT void save_dmat_0(double **mat, int x, int y, char *name);

/*! \brief fill the third (x2 sized) vector by slicing the first (x1 sized) vector of doubles at the indexes specified in the second (x2 sized) vector, starting with index 0 
 *
 * \param[in] vec1  the vector containing the original values of doubles to be sliced
 * \param[in] x1    the size of the vector vec1
 * \param[in] vec2  the vector containing the indexes of the slices
 * \param[in] x2    the size of the vector vec2,
 * \param[out] vec3 the sliced vector such as vec3[i] = vec1[vec2[i]], starting with index 0. The vector have to be already allocated to a sufficient (x2) size.
 */
MBSYSC_UTILITIES_EXPORT void slct_dvec_0(double* vec1, int x1, int* vec2, int x2, double* vec3);
/*! \brief fill the second [x1 times y2] matrix by selecting the columns of the first [x1 times y1] matrix of doubles. The indexes of the columns are specified in the (y2 sized) vector, starting with index 0 
 *
 * \param[in] tab1  the matrix containing the original values of doubles to be sliced
 * \param[in] x1    nb of rows of the matrices
 * \param[in] y1    nb of columns of the first matrix (unused)
 * \param[out] tab2 the sliced matrix such as tab2[i][j] = tab1[i][vec[j]], starting with index 0. The matrix have to be already allocated to a sufficient size.
 * \param[in] y2    the size of the vector vec
 * \param[in] vec  the vector containing the indexes of the slices
 */
MBSYSC_UTILITIES_EXPORT void slct_dmat_0(double **tab1, int x1, int y1, double **tab2, int y2, int* vec);
/*! \brief fill the second [x2 times y1] matrix by selecting the rows of the first [x1 times y1] matrix of doubles. The indexes of the rows are specified in the (x2 sized) vector, starting with index 0 
 *
 * \param[in] tab1  the matrix containing the original values of doubles to be sliced
 * \param[in] x1    nb of rows of the first matrix (unused)
 * \param[in] y1    nb of columns of the matrices
 * \param[out] tab2 the sliced matrix such as tab2[i][j] = tab1[vec[i]][j], starting with index 0. The matrix have to be already allocated to a sufficient size.
 * \param[in] x2    the size of the vector vec
 * \param[in] vec  the vector containing the indexes of the slices
 */
MBSYSC_UTILITIES_EXPORT void slctr_dmat_0(double **tab1, int x1, int y1, double **tab2, int x2, int* vec);

/*! \brief check if all values of a [x sized] vector of doubles are equal to 0, return 0 in that case, starting with index 0 
 *
 * \param[in] vec the vector to be checked
 * \param[in] x the size of the vector
 *
 * \return 0 if all element are null; 1 otherwise
 */
MBSYSC_UTILITIES_EXPORT int any_dvec_0(double *vec, int x);
/*! \brief compute the norm of a [x sized] vector of doubles, starting with index 0 
 *
 * \param[in] vec the vector of doubles
 * \param[in] x the size of the vector
 *
 * \return the norm of the vector: \f$ \sqrt { \sum {(vec_{i}^{2})}} \f$
 */
MBSYSC_UTILITIES_EXPORT double norm_dvec_0(double *vec, int x);
/*! \brief set all the element of a [x times y] matrix of doubles to zero, starting with index 0
 *
 * \param[in] mat the matrix to be nullified, starting with index 0
 * \param[in] x nb of rows
 * \param[in] y nb of columns
 */
MBSYSC_UTILITIES_EXPORT void zeros_dmat_0(double **mat, int x, int y);
/*! \brief set all the element of a [x sized] vector of doubles to zero, starting with index 0
 *
 * \param[in] vec the vector to be nullified, starting with index 0
 * \param[in] x the size of the vector
 */
MBSYSC_UTILITIES_EXPORT void zeros_dvec_0(double *vec, int x);

#endif
