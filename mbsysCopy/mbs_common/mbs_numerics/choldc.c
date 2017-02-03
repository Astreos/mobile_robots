//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include <stdio.h>
#include <math.h>

/**
 * Compute the Cholesky decomposition of a, i.e.
 * the lower triangular matrix L such that a=L*L'
 *
 * Elements off diagonal are stored directly in a.
 * Diagonal elements are stored in p.
 *
 * WARNING: a is modified !
 *
 */

void choldc(double **a, int n, double p[])
{
    void nrerror(char error_text[]);
    int i,j,k;
    double sum;

    for (i=1;i<=n;i++) {
        for (j=i;j<=n;j++) {
            for (sum=a[i][j],k=i-1;k>=1;k--) sum -= a[i][k]*a[j][k];
            if (i == j) {
                if (sum <= 0.0)
                    nrerror("choldc failed /n");
                p[i]=sqrt(sum);
            } else a[j][i]=sum/p[i];
        }
    }
}

void choldc_0(double **a, int n, double *p)
{
	//void nrerror(char error_text[]);
	int i, j, k;
	double sum;

	for (i = 0; i < n; i++)
	{
		for (j = i; j < n; j++)
		{
			for (sum = a[i][j], k = i - 1; k >= 0; k--)
			{
				sum -= a[i][k] * a[j][k];
			}
			if (i == j)
			{
				if (sum <= 0.0)
				{
					//nrerror("choldc failed");
					printf("choldc failed /n");
				}
				p[i] = sqrt(sum);
			}
			else
			{
				a[j][i] = sum / p[i];
			}
		}
	}
}
