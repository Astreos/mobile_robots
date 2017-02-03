//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef nr_fct_h
#define nr_fct_h
/*--------------------*/

void ludcmp(double **a, int n, int *indx, double *d);
void ludcmp_0(double **a, int n, int *indx, double *d);

void lubksb(double **a, int n, int *indx, double b[]);
void lubksb_0(double **a, int n, int *indx, double b[]);

void choldc(double **a, int n, double p[]);
void choldc_0(double **a, int n, double *p);

void cholsl(double **a, int n, double p[], double b[], double x[]);
void cholsl_0(double **a, int n, double *p, double *b, double *x);

void svdcmp(double **a, int m, int n, double w[], double **v);
void svdcmp_0(double **a, int m, int n, double w[], double **v);

void svbksb(double **u, double w[], double **v, int m, int n, double b[], double x[]);
void svbksb_0(double **u, double w[], double **v, int m, int n, double b[], double x[]);

void gaussj(double **a, int n, double **b, int m);


#endif
