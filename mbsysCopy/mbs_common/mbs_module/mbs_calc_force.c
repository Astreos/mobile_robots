

/**
 *
 * author Nicolas Docquier
 *
 */

#include "mbs_data.h"
#include "mbs_project_interface.h"

void mbs_calc_force(MbsData* s){

    int i,j;

    // calcul des forces appliquées sur les corps
    for(i=1;i<=s->njoint;i++)
    {
        for(j=1;j<=3;j++)
        {
            s->frc[j][i]=0.0;
            s->trq[j][i]=0.0;
        }
    }

    if(s->Nlink > 0) mbs_link(s->frc,s->trq,s->Fl,s->Z,s->Zd,s,s->tsim);

    if(s->Nlink3D > 0) mbs_link3D(s->frc,s->trq,s,s->tsim);

    if(s->Nxfrc > 0) mbs_extforces(s->frc,s->trq,s,s->tsim);

    s->Qq = user_JointForces(s,s->tsim);

}
