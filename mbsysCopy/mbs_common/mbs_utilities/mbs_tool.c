//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------


#include "stdio.h"
#include "stdlib.h"
#include "mbs_tool.h"

#ifdef MATLAB_MEX_FILE
void put_vector(double *v, int size, char *name)
{
    int i;
    mxArray *mx_v;
    mx_v = mxCreateDoubleMatrix(size,1,mxREAL);
    for(i=0;i<size;i++)
    {
        mxGetPr(mx_v)[i] = v[i+1];
    }
#ifdef OLDVER
    mexPutArray(mx_v,"base");//v6
#else
    mexPutVariable("base", name,mx_v);
#endif
    mxDestroyArray(mx_v);
}

void put_ivector(int *v, int size, char *name)
{
    int i;
    mxArray *mx_v;
    mx_v = mxCreateDoubleMatrix(size,1,mxREAL);
    for(i=0;i<size;i++)
    {
        mxGetPr(mx_v)[i] = (double)v[i+1];
    }
#ifdef OLDVER
    mexPutArray(mx_v,"base");//v6
#else
    mexPutVariable("base", name,mx_v);
#endif
    mxDestroyArray(mx_v);
}

void put_matrix(double **m, int nL, int nC, char *name)
{
    int i,j;
    mxArray *mx_m;
    mx_m = mxCreateDoubleMatrix(nL,nC,mxREAL);
    for(i=0;i<nL;i++)
    {
        for(j=0;j<nC;j++)
        {
            mxGetPr(mx_m)[i+j*nL] = m[i+1][j+1];
        }
    }
#ifdef OLDVER
    mexPutArray(mx_m,"base");//v6
#else
    mexPutVariable("base", name,mx_m);
#endif
    mxDestroyArray(mx_m);
}
#endif

double *mbs_vector(int n)
{
    int i;
    double *v;
    v=(double*) calloc(n+1,sizeof(double));
    v[0] = (double) n;
    for (i=1;i<n;i++) v[i] = 0.0;
    return v;
}

void free_mbs_vector(double *v)
{
    if (v != NULL)
        free(v);
}

int *mbs_ivector(int n)
{
    int i,*v;
    v=(int*) calloc(n+1,sizeof(int));
    v[0] = n;
    for (i=1;i<n;i++) v[i] = 0;
    return v;
}

void free_mbs_ivector(int *v)
{
    if (v != NULL)
        free(v);
}

double **mbs_matrix(int nr, int nc)
{
    double **m;
    int i,j;
    m=(double**) calloc(nr+1,sizeof(double*));
    m[0] = NULL;
    for (i=1;i<=nr;i++)
    {
        m[i]=(double*) calloc(nc+1,sizeof(double));
        m[i][0] = (double) nc;
        for (j=1;j<nc;j++) m[i][j] = 0.0;
    }
    return m;
}

void free_mbs_matrix(double **m, int nr)
{
    int i;
    if (m != NULL)
    {
        for (i=1;i<=nr;i++)
        {
            free(m[i]);
        }
        free(m);
    }
}
