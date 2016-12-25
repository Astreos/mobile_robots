//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include <math.h>

void mbs_calspan(double *Fwhl,double *Mwhl,
             double anglis, double angcamb)
{
    double Flong,Flat,Frad,Mz;
    double Fslip,
           A0,A1,A2,A3,A4,Aomegat,
           B1,B2,B3,B4,
           vt,SN,AF1,AF2,AF3,
           uY,angcampr,alphabar,galphabar;
//-----------------------------------------------------------------------------
// Parametres
// ----------

// module de young
//  Epn = 195000;//460000;

// Coefficients de Calspan
// -----------------------

// longitudinal slip
  Fslip = 1;
// force coefficients
  A0 = 2625;
  A1 = 14.47;
  A2 = 12930;
  A3 = 2.29; 
  A4 = 18175;
  Aomegat = 1;
// friction coefficients
  B1 = -0.464e-4;
  B2 = 0;
  B3 = 1.216;
  B4 = 0.218e-10;
  vt = 0;
// skid number ratio
  SN = 1.0274;
// torque coefficients
  AF1 = -0.171e-4;
  AF2 = 0.171e-4;
  AF3 = 0;

// Force normale
// -------------

//  Frad = Epn * (rnom - rpercee);
  Frad= Fwhl[3];

// Force longitudinale
// -------------------

  Flong = 0.0;

// Force laterale
// --------------

// tire sideforce friction coefficient
  uY = (B1*Frad+B2*vt+B3+B4*Frad*Frad) * SN;

  if( Frad <= Aomegat * A2)
  {
     angcampr = (-(A2*A3*(A4-Frad)*Frad) / (A4*(A1*Frad*(Frad-A2)-A0*A2))) * angcamb;
     if( (anglis + angcampr) == 0)
        alphabar = 0;
     else
        alphabar = (A1*Frad*(Frad-A2)-A0*A2) / (A2*uY*Frad) * (anglis + angcampr);
     //end
  }
  else
  {
     angcampr = -(A2*A3*Aomegat*(A4-Aomegat*A2)) / (A4*(A1*A2*Aomegat*(Aomegat-1)-A0)) * angcamb;
     if( (anglis + angcampr) == 0)
        alphabar = 0;
     else
        alphabar = (A1*A2*Aomegat*(Aomegat-1)-A0) / (uY*Frad) * (anglis + angcampr);
     //end
  }//end
  if( fabs(alphabar) < 3)
     galphabar = alphabar - alphabar*fabs(alphabar)/3 + (alphabar*alphabar*alphabar)/27;
  else
     galphabar = alphabar/fabs(alphabar);
  //end

  Flat = uY * Frad * galphabar * Fslip;

// Couple d'allignement
// --------------------
  Mz = (AF1*Frad + AF2*fabs(Flat))*Flat - AF3*Frad*angcamb;
//--
    Fwhl[1]=Flong;
    Fwhl[2]=Flat;
    Fwhl[3]=Frad;
    Mwhl[1]=0;
    Mwhl[2]=0;
    Mwhl[3]=Mz;
}
