//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef mbs_mxtool_h
#define mbs_mxtool_h
/*--------------------*/

#include "mbsysc_utilities_export.h"

void put_vector(double *v, int size, char *name);

void put_ivector(int *v, int size, char *name);

void put_matrix(double **m, int nL, int nC, char *name);

/*! \brief create (with memory allocation) a vector (length n) of doubles, starting at index 1
 *
 * \param[in] n size of the vector (0 element not taken into account)
 * \return requested vector (element 0 receives n: the size without this element)
 */
MBSYSC_UTILITIES_EXPORT double *mbs_vector(int n);
/*! \brief release memory for a vector of doubles, starting at index 1
 *
 * \param[out] v a vector of doubles
 */
MBSYSC_UTILITIES_EXPORT void free_mbs_vector(double *v);

/*! \brief create (with memory allocation) a vector (length n) of integers, starting at index 1
 *
 * \param[in] n size of the vector (0 element not taken into account)
 * \return requested vector (element 0 receives n: the size without this element)
 */
MBSYSC_UTILITIES_EXPORT int *mbs_ivector(int n);
/*! \brief release memory for a vector of integers, starting at index 1
 *
 * \param[out] v a vector of doubles
 */
MBSYSC_UTILITIES_EXPORT void free_mbs_ivector(int *v);

/*! \brief create (with memory allocation) a [nr times nc] matrix of doubles, starting at index 1
 *
 * \param[in] nr number of rows (0 element not taken into account)
 * \param[in] nc number of columns (0 element not taken into account)
 * \return requested matrix of doubles
 */
MBSYSC_UTILITIES_EXPORT double **mbs_matrix(int nr, int nc);
/*! \brief release memory for a matrix of doubles, starting at index 1
 *
 * \param[out] m matrix of doubles
 * \param[in] nr number of rows (0 element not taken into account)
 */
MBSYSC_UTILITIES_EXPORT void free_mbs_matrix(double **m, int nr);


/*--------------------*/
#endif
