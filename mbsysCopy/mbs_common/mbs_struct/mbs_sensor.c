//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2009
// Last update : 18/02/2009
//---------------------------
//

#include<stdlib.h>
#include "mbs_sensor.h"

void allocate_sensor(MbsSensor *psens, int njoint) {
    int j;

    for (j=1;j<=6;j++) {
        psens->J[j] = (double*) calloc(njoint+1,sizeof(double));
        psens->J[j][0] = njoint;
    }
}

void init_sensor(MbsSensor *psens, int njoint) {
    int i,j;

    for(i=1;i<=3;i++) {
        psens->P[i] = 0.0;
        psens->V[i] = 0.0;
        psens->A[i] = 0.0;
        psens->OM[i] = 0.0;
        psens->OMP[i] = 0.0;
        for (j=1;j<=3;j++) psens->R[i][j] = 0.0;
    }
    for(i=1;i<=6;i++)
        for (j=1;j<=njoint;j++) psens->J[i][j] = 0.0;
    
}

void free_sensor(MbsSensor *psens)
{
    int j;

    for (j=1;j<=6;j++) free(psens->J[j]);
}
