//-------------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2008 by JF Collard
// Last update : 01/10/2008
//-------------------------------
//
// Gestion via Bugzilla :
// 01/10/2008 : JFC : Bug n°44
//

#include "nrutil.h"

void svbksb(double **u, double w[], double **v, int m, int n, double b[], double x[])
{
    int jj,j,i;
    double s,*tmp;

    tmp=dvector(1,n);
    for (j=1;j<=n;j++) {
        s=0.0;
        if (w[j]) {
            for (i=1;i<=m;i++) s += u[i][j]*b[i];
            s /= w[j];
        }
        tmp[j]=s;
    }
    for (j=1;j<=n;j++) {
        s=0.0;
        for (jj=1;jj<=n;jj++) s += v[j][jj]*tmp[jj];
        x[j]=s;
    }
    free_dvector(tmp,1,n);
}

void svbksb_0(double **u, double w[], double **v, int m, int n, double b[], double x[])
{
	int jj, j, i;
	double s, *tmp;

	tmp = dvector(1, n);
	for (j = 1; j <= n; j++) {
		s = 0.0;
		if (w[j-1]) {
			for (i = 1; i <= m; i++) s += u[i-1][j-1] * b[i-1];
			s /= w[j-1];
		}
		tmp[j] = s;
	}
	for (j = 1; j <= n; j++) {
		s = 0.0;
		for (jj = 1; jj <= n; jj++) s += v[j-1][jj-1] * tmp[jj];
		x[j-1] = s;
	}
	free_dvector(tmp, 1, n);
}
