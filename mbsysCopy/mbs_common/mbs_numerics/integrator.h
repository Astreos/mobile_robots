/*
 * Integrator headers
 */
#ifndef __INTEGRATOR_H_INCLUDED__  // guard against multiple/recursive includes
#define __INTEGRATOR_H_INCLUDED__

#include "mbs_data.h"
#include "mbs_aux.h"
#include "nrutil.h"
#include "mbs_dirdyn_struct.h"

void rk4(double y[], double dydx[], int n, double x, double h, double yout[],
         void (*derivs)(double, double [], double [],MbsData *, MbsDirdyn *),
         MbsData *s, MbsDirdyn *dd);

void eulerExplicit(double y[], double dydx[], int n, double x, double h, double yout[],
         void (*derivs)(double, double [], double [],MbsData *, MbsAux *),
         MbsData *s, MbsAux *mbs_aux);

void ThetaSC(double y[], double dydx[], int n, double x, double h, double yout[],
         void (*derivs)(double, double [], double [],MbsData *, MbsAux *),
         MbsData *s, MbsAux *mbs_aux);

void derivs(double x, double y[], double dydx[],MbsData *s, MbsAux *mbs_aux);

#endif
