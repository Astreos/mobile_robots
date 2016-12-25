//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include <math.h>

#include "mbsysc_utilities_export.h"

double norm_vector(double *v, int n)
{
    double norm = 0.0;
    int i;

    for(i=1;i<=n;i++) norm += v[i]*v[i];

    return sqrt(norm);
}

MBSYSC_UTILITIES_EXPORT double norminf_vector(double *v, int n);  // missing header with prototype

double norminf_vector(double *v, int n)
{
    double norm = 0.0;
    int i;

    for(i=1;i<=n;i++)
        if (fabs(v[i]) > norm) norm = fabs(v[i]);

    return norm;
}
