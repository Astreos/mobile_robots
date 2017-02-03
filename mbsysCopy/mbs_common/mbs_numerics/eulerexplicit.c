#include "integrator.h"

void eulerExplicit(double y[], double dydx[], int n, double x, double h, double yout[],
         void (*derivs)(double, double [], double [],MbsData *, MbsAux *),
         MbsData *s, MbsAux *mbs_aux)
{
    int i;
    // First and only step (derivs already called once in odeint)
    for (i=0;i<n;i++)
    {
        yout[i]=y[i]+h*dydx[i];
    }

    s->tsim += h;
    // this is done here because the Euler explicit method do not call deriv function (see mbs_fct_dirdyn in mbs_dirdyn.c file) wherein the mbs_data->tsim is updated.
}
