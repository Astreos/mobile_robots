/*
 * Function related to the integrator (Runge Kutta order 4, from Numerical Recipes)
 */

#include "integrator.h"

/*
 * Given values for the variables y[1..n] and their derivatives dydx[1..n] known at x, use the
 * fourth-order Runge-Kutta method to advance the solution over an interval h and return the
 * incremented variables as yout[1..n], which need not be a distinct array from y. The user
 * supplies the routine derivs(x,y,dydx) , which returns derivatives dydx at x.
 */
void rk4(double y[], double dydx[], int n, double x, double h, double yout[],
         void (*derivs)(double, double [], double [],MbsData *, MbsDirdyn *),
         MbsData *s, MbsDirdyn *dd)
{
    int i;
    double xh,hh,h6,*dym,*dyt,*yt;

    dym = dd->rk4->dym;
    dyt = dd->rk4->dyt;
    yt  = dd->rk4->yt;

    hh=h*0.5;
    h6=h/6.0;

    xh=x+hh;

    // First step (derivs already called once in odeint)
    for (i=0;i<n;i++)
        yt[i]=y[i]+hh*dydx[i];

    // Second step
    (*derivs)(xh,yt,dyt,s,dd);
    for (i=0;i<n;i++)
        yt[i]=y[i]+hh*dyt[i];

    // Third step
    (*derivs)(xh,yt,dym,s,dd);
    for (i=0;i<n;i++)
    {
        yt[i]=y[i]+h*dym[i];
        dym[i] += dyt[i];
    }

    // Fourth step
    (*derivs)(x+h,yt,dyt,s,dd);

    // Accumulate increments with proper weights
    for (i=0;i<n;i++)
        yout[i]=y[i]+h6*(dydx[i]+dyt[i]+2.0*dym[i]);
}
