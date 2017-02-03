#include "integrator.h"
#include "MBSfun.h"

void ThetaSC(double y[], double dydx[], int n, double x, double h, double yout[],
         void (*derivs)(double, double [], double [],MbsData *, MbsAux *),
         MbsData *s, MbsAux *mbs_aux)
{
    /*
     * WARNING : USER STATE NOT IMPLEMENTED
     */
    int i;

    double h2  = h/2.0;
    double xh2 = x + h2;

    double *yt2;
    yt2=dvector(1,n);

    // First step: going to middle position
    for (i=0;i<s->nqu;i++)
    {
        yout[i] = y[i]+h2*dydx[i];
        s->q[s->qu[i+1]] = yout[i];
    }

    // close loops and compute the dynamics
    dirdynared(mbs_aux, s);
    // Second step : computing "Vfree" and final position
    for (i=0;i<s->nqu;i++)
    {
        // Updating result vector
        yout[i+s->nqu] = s->qd[s->qu[i+1]] + h * s->qdd[s->qu[i+1]];
        yout[i]        = s->q[s->qu[i+1]]  + h2 * yout[i+s->nqu];
        // Updating variation vector
        dydx[i]        = yout[i+s->nqu];
        dydx[i+s->nqu] = s->qdd[s->qu[i+1]];
        // Updating mbs_data
        s->q[s->qu[i+1]]  = yout[i];
        s->qd[s->qu[i+1]] = yout[i+s->nqu];
    }
    // Last step : solving constraints
    if (s->nhu > 0)
    {
        int iter;
        // Closing loop in position (close geometry)
        iter = mbs_close_geo(s, mbs_aux);

        if (iter >= mbs_aux->MAX_NR_ITER)
        {
            printf("WARNING: loop constraints not solved after %d iterations.\n",iter);
        }
        // Closing loop in velocity (close kinematic)
        mbs_close_kin(s, mbs_aux);
    }
    s->tsim += h;
    free_dvector(yt2,1,n);
}
