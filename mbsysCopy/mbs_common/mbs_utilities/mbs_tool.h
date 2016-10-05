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

MBSYSC_UTILITIES_EXPORT double *mbs_vector(int n);
MBSYSC_UTILITIES_EXPORT void free_mbs_vector(double *v);

MBSYSC_UTILITIES_EXPORT int *mbs_ivector(int n);
MBSYSC_UTILITIES_EXPORT void free_mbs_ivector(int *v);

MBSYSC_UTILITIES_EXPORT double **mbs_matrix(int nr, int nc);
MBSYSC_UTILITIES_EXPORT void free_mbs_matrix(double **m, int nr);


/*--------------------*/
#endif
