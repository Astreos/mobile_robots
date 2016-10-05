//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include <math.h> 


void mbs_bakker(double *Fwhl,double *Mwhl,
			 double anglis, double angcamb, double gliss)
{
double Flong,Flat,Frad,FradkN,Mz;
double anglisdeg, angcambdeg,
	   k,
       phi,deltaSh,deltaSv,
       sxs,sys,
	   A0,A1,A2,A3,
	   B0,B1,B2,B3,
	   C0,C1,C2,C3;
	const double pi=3.14159265358979;
//-----------------------------------------------------------------------------

	// Parametres
	// ----------
	
	anglisdeg = anglis*180/pi;
	angcambdeg = angcamb*180/pi;
	
// module de young

//	Epn = 195000;
//	Damp = 00.0;
	
// Force normale
// -------------
	
//	Frad = Epn * (rnom - rpercee) - Damp * Vctz;

	Frad=	Fwhl[3];

	FradkN = Frad/1000.0;
	
// Force longitudinale
// -------------------
	

	k	= gliss*100.0;
	
	A0	= 1.65;
	A1	= -21.3*pow(FradkN,2)+1144.0*FradkN;
	A2	= (49.6*pow(FradkN,2)+226.0*FradkN)/(A0*A1*exp(0.069*FradkN));
	A3	= -0.006*pow(FradkN,2)+0.056*FradkN+0.486;
	phi = (1.0-A3)*k+(A3/A2)*atan(A2*k) ;
	Flong = A1*sin(A0*atan(A2*phi));
	
// Force laterale
// --------------
	
	B0	= 1.3;
	B1	= -22.1*pow(FradkN,2)+1011*FradkN;
	B2	= (1078.0*sin(1.82*atan(0.208*FradkN))*(1.0-0.022*fabs(angcambdeg)))/(B0*B1);
	B3	= -0.354*FradkN+0.707;
	deltaSh = 0.028*angcambdeg;
	deltaSv = (14.8*FradkN)*angcambdeg;
	phi = (1.0-B3)*(anglisdeg+deltaSh)+(B3/B2)*atan(B2*(anglisdeg+deltaSh));
	Flat = -(B1*sin(B0*atan(B2*phi))+deltaSv);

// Flong et Flat combinees
// -----------------------
	
	k = k/100.0;
	
	if (k==0.0 && anglis==0.0)
	{sxs = 1.0;
	sys = 1.0;
	}
	else
	{sxs = fabs(k/(sqrt(pow(k,2)+pow(tan(anglis),2))));
	sys = fabs(tan(anglis)/(sqrt(pow(k,2)+pow(tan(anglis),2))));
	}
	
	Flong = sxs*Flong;
	Flat = sys*Flat;
	
// Couple d'alignement
// -------------------
	
	C0	= 2.4;
	C1	= -2.72*pow(FradkN,2)-2.28*FradkN;
	C2	= (-1.86*pow(FradkN,2)-2.73*FradkN)/(C0*C1*exp(0.11*FradkN))*(1.0-0.03*fabs(angcambdeg));
	C3	= (-0.07*pow(FradkN,2)+0.643*FradkN-4.04)*(1.0-0.07*fabs(angcambdeg));
	deltaSh = 0.015*angcambdeg;
	deltaSv = (-0.066*pow(FradkN,2)+0.945*FradkN)*angcambdeg;
	phi = (1.0-C3)*(anglisdeg+deltaSh)+(C3/C2)*atan(C2*(anglisdeg+deltaSh));
	
	Mz = C1*sin(C0*atan(C2*phi))+deltaSv;
	
/////////////////////////////////////
    Fwhl[1]=Flong;
	Fwhl[2]=Flat;
	Fwhl[3]=Frad;
	Mwhl[1]=0.0;
	Mwhl[2]=0.0;
	Mwhl[3]=Mz;
}
void mbs_bakker_lin_0(double *Fwhl,double *Mwhl,
			 double anglis, double angcamb, double gliss)
{
double Flong,Flat,Frad,FradkN,Mz;
double anglisdeg, angcambdeg,
	   k,
       phi,deltaSh,deltaSv,
       sxs,sys,
	   A0,A1,A2,A3,
	   B0,B1,B2,B3,
	   C0,C1,C2,C3;
	const double pi=3.14159265358979;
//-----------------------------------------------------------------------------

	// Parametres
	// ----------
	
	anglisdeg = anglis*180.0/pi;
	angcambdeg = angcamb*180.0/pi;
	
// module de young

//	Epn = 195000;
//	Damp = 00.0;
	
// Force normale
// -------------
	
//	Frad = Epn * (rnom - rpercee) - Damp * Vctz;

	Frad=	Fwhl[3];

	FradkN = Frad/1000.0;
	
// Force longitudinale
// -------------------
	

	k	= gliss*100.0;
	
	A0	= 1.65;
	A1	= -21.3*pow(FradkN,2)+1144.0*FradkN;
	A2	= (49.6*pow(FradkN,2)+226.0*FradkN)/(A0*A1*exp(0.069*FradkN));
	A3	= -0.006*pow(FradkN,2)+0.056*FradkN+0.486;
	phi = (1-A3)*k+(A3/A2)*atan(A2*k) ;
	Flong = 1.1e5 * gliss; //A1*A3*A2*k;
	
// Force laterale
// --------------
	
	B0	= 1.3;
	B1	= -22.1*pow(FradkN,2)+1011*FradkN;
	B2	= (1078*sin(1.82*atan(0.208*FradkN))*(1.0-0.022*fabs(angcambdeg)))/(B0*B1);
	B3	= -0.354*FradkN+0.707;
	deltaSh = 0.028*angcambdeg;
	deltaSv = (14.8*FradkN)*angcambdeg;
	phi = (1.0-B3)*(anglisdeg+deltaSh)+(B3/B2)*atan(B2*(anglisdeg+deltaSh));
	Flat = - 1e5 * anglis ; // -(B1*sin(B0*atan(B2*phi))+deltaSv);

// Flong et Flat combinees
// -----------------------
	
	k = k/100.0;
	
	if (k==0.0 && anglis==0.0)
	{sxs = 1.0;
	sys = 1.0;
	}
	else
	{sxs = fabs(k/(sqrt(pow(k,2)+pow(tan(anglis),2))));
	sys = fabs(tan(anglis)/(sqrt(pow(k,2)+pow(tan(anglis),2))));
	}
	
	Flong = sxs*Flong;
	Flat = sys*Flat;
	
// Couple d'alignement
// -------------------
	
	C0	= 2.4;
	C1	= -2.72*pow(FradkN,2)-2.28*FradkN;
	C2	= (-1.86*pow(FradkN,2)-2.73*FradkN)/(C0*C1*exp(0.11*FradkN))*(1.0-0.03*fabs(angcambdeg));
	C3	= (-0.07*pow(FradkN,2)+0.643*FradkN-4.04)*(1-0.07*fabs(angcambdeg));
	deltaSh = 0.015*angcambdeg;
	deltaSv = (-0.066*pow(FradkN,2)+0.945*FradkN)*angcambdeg;
	phi = (1.0-C3)*(anglisdeg+deltaSh)+(C3/C2)*atan(C2*(anglisdeg+deltaSh));
	
	Mz = 0.0;//C1*sin(C0*atan(C2*phi))+deltaSv;
	
/////////////////////////////////////
    Fwhl[1]=Flong;
	Fwhl[2]=Flat;
	Fwhl[3]=Frad;
	Mwhl[1]=0.0;
	Mwhl[2]=0.0;
	Mwhl[3]=Mz;
}