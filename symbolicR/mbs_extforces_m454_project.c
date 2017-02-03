//
//-------------------------------------------------------------
//
//	ROBOTRAN - Version 6.6 (build : february 22, 2008)
//
//	Copyright 
//	Universite catholique de Louvain 
//	Departement de Mecanique 
//	Unite de Production Mecanique et Machines 
//	2, Place du Levant 
//	1348 Louvain-la-Neuve 
//	http://www.robotran.be// 
//
//	==> Generation Date : Sun Sep 11 16:30:54 2016
//
//	==> Project name : m454_project
//	==> using XML input file 
//
//	==> Number of joints : 24
//
//	==> Function : F19 : External Forces
//	==> Flops complexity : 76
//
//	==> Generation Time :  0.000 seconds
//	==> Post-Processing :  0.010 seconds
//
//-------------------------------------------------------------
//
 
#include <math.h> 

#include "mbs_data.h"
#include "mbs_project_interface.h"
 
void mbs_extforces(double **frc,double **trq,
MbsData *s, double tsim)

// double frc[3][24];
// double trq[3][24];
{ 
double PxF1[4]; 
double RxF1[4][4]; 
double VxF1[4]; 
double OMxF1[4]; 
double AxF1[4]; 
double OMPxF1[4]; 
double *SWr1; 
double PxF2[4]; 
double RxF2[4][4]; 
double VxF2[4]; 
double OMxF2[4]; 
double AxF2[4]; 
double OMPxF2[4]; 
double *SWr2; 
double PxF3[4]; 
double RxF3[4][4]; 
double VxF3[4]; 
double OMxF3[4]; 
double AxF3[4]; 
double OMPxF3[4]; 
double *SWr3; 
double PxF4[4]; 
double RxF4[4][4]; 
double VxF4[4]; 
double OMxF4[4]; 
double AxF4[4]; 
double OMPxF4[4]; 
double *SWr4; 
 
#include "mbs_extforces_m454_project.h" 
#define q s->q 
#define qd s->qd 
#define qdd s->qdd 
 
 

// === begin imp_aux === 

// === end imp_aux === 

// ===== BEGIN task 0 ===== 
 
// Sensor Kinematics 



// = = Block_0_0_0_0_0_1 = = 
 
// Trigonometric Variables  

  C3 = cos(q[3]);
  S3 = sin(q[3]);

// = = Block_0_0_0_0_0_5 = = 
 
// Trigonometric Variables  

  C9 = cos(q[9]);
  S9 = sin(q[9]);

// = = Block_0_0_0_0_0_9 = = 
 
// Trigonometric Variables  

  C15 = cos(q[15]);
  S15 = sin(q[15]);

// = = Block_0_0_0_0_0_13 = = 
 
// Trigonometric Variables  

  C21 = cos(q[21]);
  S21 = sin(q[21]);

// = = Block_0_0_1_1_0_1 = = 
 
// Sensor Kinematics 


  PxF1[1] = q[1];
  PxF1[2] = q[2];
  PxF1[3] = s->dpt[3][1]+s->dpt[3][5];
  RxF1[1][1] = C3;
  RxF1[1][2] = S3;
  RxF1[1][3] = 0;
  RxF1[2][1] = -S3;
  RxF1[2][2] = C3;
  RxF1[2][3] = 0;
  RxF1[3][1] = 0;
  RxF1[3][2] = 0;
  RxF1[3][3] = (1.0);
  VxF1[1] = qd[1];
  VxF1[2] = qd[2];
  VxF1[3] = 0;
  OMxF1[1] = 0;
  OMxF1[2] = 0;
  OMxF1[3] = qd[3];
  AxF1[1] = qdd[1];
  AxF1[2] = qdd[2];
  AxF1[3] = 0;
  OMPxF1[1] = 0;
  OMPxF1[2] = 0;
  OMPxF1[3] = qdd[3];
 
// Sensor Forces Computation 

  SWr1 = user_ExtForces(PxF1,RxF1,VxF1,OMxF1,AxF1,OMPxF1,s,tsim,1);
 
// Sensor Dynamics : Forces projection on body-fixed frames 

  xfrc11 = SWr1[1]*C3+SWr1[2]*S3;
  xfrc21 = -(SWr1[1]*S3-SWr1[2]*C3);
  xfrc31 = SWr1[3];
  frc[1][3] = s->frc[1][3]+xfrc11;
  frc[2][3] = s->frc[2][3]+xfrc21;
  frc[3][3] = s->frc[3][3]+SWr1[3];
  xtrq11 = SWr1[4]*C3+SWr1[5]*S3;
  xtrq21 = -(SWr1[4]*S3-SWr1[5]*C3);
  xtrq31 = SWr1[6];
  trq[1][3] = s->trq[1][3]+xtrq11-xfrc21*(SWr1[9]-s->l[3][3])+xfrc31*SWr1[8];
  trq[2][3] = s->trq[2][3]+xtrq21+xfrc11*(SWr1[9]-s->l[3][3])-xfrc31*(SWr1[7]-s->l[1][3]);
  trq[3][3] = s->trq[3][3]+xtrq31-xfrc11*SWr1[8]+xfrc21*(SWr1[7]-s->l[1][3]);

// = = Block_0_0_1_2_0_5 = = 
 
// Sensor Kinematics 


  PxF2[1] = q[7];
  PxF2[2] = q[8];
  PxF2[3] = s->dpt[3][1]+s->dpt[3][9];
  RxF2[1][1] = C9;
  RxF2[1][2] = S9;
  RxF2[1][3] = 0;
  RxF2[2][1] = -S9;
  RxF2[2][2] = C9;
  RxF2[2][3] = 0;
  RxF2[3][1] = 0;
  RxF2[3][2] = 0;
  RxF2[3][3] = (1.0);
  VxF2[1] = qd[7];
  VxF2[2] = qd[8];
  VxF2[3] = 0;
  OMxF2[1] = 0;
  OMxF2[2] = 0;
  OMxF2[3] = qd[9];
  AxF2[1] = qdd[7];
  AxF2[2] = qdd[8];
  AxF2[3] = 0;
  OMPxF2[1] = 0;
  OMPxF2[2] = 0;
  OMPxF2[3] = qdd[9];
 
// Sensor Forces Computation 

  SWr2 = user_ExtForces(PxF2,RxF2,VxF2,OMxF2,AxF2,OMPxF2,s,tsim,2);
 
// Sensor Dynamics : Forces projection on body-fixed frames 

  xfrc12 = SWr2[1]*C9+SWr2[2]*S9;
  xfrc22 = -(SWr2[1]*S9-SWr2[2]*C9);
  xfrc32 = SWr2[3];
  frc[1][9] = s->frc[1][9]+xfrc12;
  frc[2][9] = s->frc[2][9]+xfrc22;
  frc[3][9] = s->frc[3][9]+SWr2[3];
  xtrq12 = SWr2[4]*C9+SWr2[5]*S9;
  xtrq22 = -(SWr2[4]*S9-SWr2[5]*C9);
  xtrq32 = SWr2[6];
  trq[1][9] = s->trq[1][9]+xtrq12-xfrc22*(SWr2[9]-s->l[3][9])+xfrc32*SWr2[8];
  trq[2][9] = s->trq[2][9]+xtrq22+xfrc12*(SWr2[9]-s->l[3][9])-xfrc32*(SWr2[7]-s->l[1][9]);
  trq[3][9] = s->trq[3][9]+xtrq32-xfrc12*SWr2[8]+xfrc22*(SWr2[7]-s->l[1][9]);

// = = Block_0_0_1_3_0_9 = = 
 
// Sensor Kinematics 


  PxF3[1] = q[13];
  PxF3[2] = q[14];
  PxF3[3] = s->dpt[3][13]+s->dpt[3][1];
  RxF3[1][1] = C15;
  RxF3[1][2] = S15;
  RxF3[1][3] = 0;
  RxF3[2][1] = -S15;
  RxF3[2][2] = C15;
  RxF3[2][3] = 0;
  RxF3[3][1] = 0;
  RxF3[3][2] = 0;
  RxF3[3][3] = (1.0);
  VxF3[1] = qd[13];
  VxF3[2] = qd[14];
  VxF3[3] = 0;
  OMxF3[1] = 0;
  OMxF3[2] = 0;
  OMxF3[3] = qd[15];
  AxF3[1] = qdd[13];
  AxF3[2] = qdd[14];
  AxF3[3] = 0;
  OMPxF3[1] = 0;
  OMPxF3[2] = 0;
  OMPxF3[3] = qdd[15];
 
// Sensor Forces Computation 

  SWr3 = user_ExtForces(PxF3,RxF3,VxF3,OMxF3,AxF3,OMPxF3,s,tsim,3);
 
// Sensor Dynamics : Forces projection on body-fixed frames 

  xfrc13 = SWr3[1]*C15+SWr3[2]*S15;
  xfrc23 = -(SWr3[1]*S15-SWr3[2]*C15);
  xfrc33 = SWr3[3];
  frc[1][15] = s->frc[1][15]+xfrc13;
  frc[2][15] = s->frc[2][15]+xfrc23;
  frc[3][15] = s->frc[3][15]+SWr3[3];
  xtrq13 = SWr3[4]*C15+SWr3[5]*S15;
  xtrq23 = -(SWr3[4]*S15-SWr3[5]*C15);
  xtrq33 = SWr3[6];
  trq[1][15] = s->trq[1][15]+xtrq13-xfrc23*(SWr3[9]-s->l[3][15])+xfrc33*SWr3[8];
  trq[2][15] = s->trq[2][15]+xtrq23+xfrc13*(SWr3[9]-s->l[3][15])-xfrc33*(SWr3[7]-s->l[1][15]);
  trq[3][15] = s->trq[3][15]+xtrq33-xfrc13*SWr3[8]+xfrc23*(SWr3[7]-s->l[1][15]);

// = = Block_0_0_1_4_0_13 = = 
 
// Sensor Kinematics 


  PxF4[1] = q[19];
  PxF4[2] = q[20];
  PxF4[3] = s->dpt[3][17]+s->dpt[3][1];
  RxF4[1][1] = C21;
  RxF4[1][2] = S21;
  RxF4[1][3] = 0;
  RxF4[2][1] = -S21;
  RxF4[2][2] = C21;
  RxF4[2][3] = 0;
  RxF4[3][1] = 0;
  RxF4[3][2] = 0;
  RxF4[3][3] = (1.0);
  VxF4[1] = qd[19];
  VxF4[2] = qd[20];
  VxF4[3] = 0;
  OMxF4[1] = 0;
  OMxF4[2] = 0;
  OMxF4[3] = qd[21];
  AxF4[1] = qdd[19];
  AxF4[2] = qdd[20];
  AxF4[3] = 0;
  OMPxF4[1] = 0;
  OMPxF4[2] = 0;
  OMPxF4[3] = qdd[21];
 
// Sensor Forces Computation 

  SWr4 = user_ExtForces(PxF4,RxF4,VxF4,OMxF4,AxF4,OMPxF4,s,tsim,4);
 
// Sensor Dynamics : Forces projection on body-fixed frames 

  xfrc14 = SWr4[1]*C21+SWr4[2]*S21;
  xfrc24 = -(SWr4[1]*S21-SWr4[2]*C21);
  xfrc34 = SWr4[3];
  frc[1][21] = s->frc[1][21]+xfrc14;
  frc[2][21] = s->frc[2][21]+xfrc24;
  frc[3][21] = s->frc[3][21]+SWr4[3];
  xtrq14 = SWr4[4]*C21+SWr4[5]*S21;
  xtrq24 = -(SWr4[4]*S21-SWr4[5]*C21);
  xtrq34 = SWr4[6];
  trq[1][21] = s->trq[1][21]+xtrq14-xfrc24*(SWr4[9]-s->l[3][21])+xfrc34*SWr4[8];
  trq[2][21] = s->trq[2][21]+xtrq24+xfrc14*(SWr4[9]-s->l[3][21])-xfrc34*(SWr4[7]-s->l[1][21]);
  trq[3][21] = s->trq[3][21]+xtrq34-xfrc14*SWr4[8]+xfrc24*(SWr4[7]-s->l[1][21]);

// = = Block_0_0_1_4_1_0 = = 
 
// Symbolic Outputs  

  frc[1][4] = s->frc[1][4];
  frc[2][4] = s->frc[2][4];
  frc[3][4] = s->frc[3][4];
  frc[1][5] = s->frc[1][5];
  frc[2][5] = s->frc[2][5];
  frc[3][5] = s->frc[3][5];
  frc[1][6] = s->frc[1][6];
  frc[2][6] = s->frc[2][6];
  frc[3][6] = s->frc[3][6];
  frc[1][10] = s->frc[1][10];
  frc[2][10] = s->frc[2][10];
  frc[3][10] = s->frc[3][10];
  frc[1][11] = s->frc[1][11];
  frc[2][11] = s->frc[2][11];
  frc[3][11] = s->frc[3][11];
  frc[1][12] = s->frc[1][12];
  frc[2][12] = s->frc[2][12];
  frc[3][12] = s->frc[3][12];
  frc[1][16] = s->frc[1][16];
  frc[2][16] = s->frc[2][16];
  frc[3][16] = s->frc[3][16];
  frc[1][17] = s->frc[1][17];
  frc[2][17] = s->frc[2][17];
  frc[3][17] = s->frc[3][17];
  frc[1][18] = s->frc[1][18];
  frc[2][18] = s->frc[2][18];
  frc[3][18] = s->frc[3][18];
  frc[1][22] = s->frc[1][22];
  frc[2][22] = s->frc[2][22];
  frc[3][22] = s->frc[3][22];
  frc[1][23] = s->frc[1][23];
  frc[2][23] = s->frc[2][23];
  frc[3][23] = s->frc[3][23];
  frc[1][24] = s->frc[1][24];
  frc[2][24] = s->frc[2][24];
  frc[3][24] = s->frc[3][24];
  trq[1][4] = s->trq[1][4];
  trq[2][4] = s->trq[2][4];
  trq[3][4] = s->trq[3][4];
  trq[1][5] = s->trq[1][5];
  trq[2][5] = s->trq[2][5];
  trq[3][5] = s->trq[3][5];
  trq[1][6] = s->trq[1][6];
  trq[2][6] = s->trq[2][6];
  trq[3][6] = s->trq[3][6];
  trq[1][10] = s->trq[1][10];
  trq[2][10] = s->trq[2][10];
  trq[3][10] = s->trq[3][10];
  trq[1][11] = s->trq[1][11];
  trq[2][11] = s->trq[2][11];
  trq[3][11] = s->trq[3][11];
  trq[1][12] = s->trq[1][12];
  trq[2][12] = s->trq[2][12];
  trq[3][12] = s->trq[3][12];
  trq[1][16] = s->trq[1][16];
  trq[2][16] = s->trq[2][16];
  trq[3][16] = s->trq[3][16];
  trq[1][17] = s->trq[1][17];
  trq[2][17] = s->trq[2][17];
  trq[3][17] = s->trq[3][17];
  trq[1][18] = s->trq[1][18];
  trq[2][18] = s->trq[2][18];
  trq[3][18] = s->trq[3][18];
  trq[1][22] = s->trq[1][22];
  trq[2][22] = s->trq[2][22];
  trq[3][22] = s->trq[3][22];
  trq[1][23] = s->trq[1][23];
  trq[2][23] = s->trq[2][23];
  trq[3][23] = s->trq[3][23];
  trq[1][24] = s->trq[1][24];
  trq[2][24] = s->trq[2][24];
  trq[3][24] = s->trq[3][24];

// ====== END Task 0 ====== 


}
 

