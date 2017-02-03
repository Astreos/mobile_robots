//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

void lubksb(double **a, int n, int *indx, double b[])
{
    int i,ii=0,ip,j;
    double sum;

    for (i=1;i<=n;i++) {
        ip=indx[i];
        sum=b[ip];
        b[ip]=b[i];
        if (ii)
            for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
        else if (sum) ii=i;
        b[i]=sum;
    }
    for (i=n;i>=1;i--) {
        sum=b[i];
        for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
        b[i]=sum/a[i][i];
    }
}

void lubksb_0(double **a, int n, int *indx, double b[])
{
	int i, ii = -1, ip, j;
	double sum;

	for (i = 0; i < n; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii != -1)
			for (j = ii; j <= i - 1; j++) sum -= a[i][j] * b[j];
		else if (sum) ii = i;
		b[i] = sum;
	}
	for (i = n - 1; i >= 0; i--) {
		sum = b[i];
		for (j = i + 1; j < n; j++) sum -= a[i][j] * b[j];
		b[i] = sum / a[i][i];
	}
}