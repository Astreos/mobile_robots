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
//	==> Generation Date : Sun Sep 11 16:30:55 2016
//
//	==> Project name : m454_project
//	==> using XML input file 
//
//	==> Number of joints : 24
//
//	==> Function : F 6 : Sensors Kinematical Informations (sens) 
//	==> Flops complexity : 416
//
//	==> Generation Time :  0.010 seconds
//	==> Post-Processing :  0.010 seconds
//
//-------------------------------------------------------------
//
 
#include <math.h> 

#include "mbs_data.h"
#include "mbs_project_interface.h"
#include "mbs_sensor.h"
 
void  mbs_gensensor(MbsSensor *sens, 
              MbsData *s,
              int isens)
{ 
 
#include "mbs_gensensor_m454_project.h" 
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

// = = Block_0_0_0_0_0_2 = = 
 
// Trigonometric Variables  

  C4 = cos(q[4]);
  S4 = sin(q[4]);

// = = Block_0_0_0_0_0_3 = = 
 
// Trigonometric Variables  

  C5 = cos(q[5]);
  S5 = sin(q[5]);

// = = Block_0_0_0_0_0_4 = = 
 
// Trigonometric Variables  

  C6 = cos(q[6]);
  S6 = sin(q[6]);

// = = Block_0_0_0_0_0_5 = = 
 
// Trigonometric Variables  

  C9 = cos(q[9]);
  S9 = sin(q[9]);

// = = Block_0_0_0_0_0_6 = = 
 
// Trigonometric Variables  

  C10 = cos(q[10]);
  S10 = sin(q[10]);

// = = Block_0_0_0_0_0_7 = = 
 
// Trigonometric Variables  

  C11 = cos(q[11]);
  S11 = sin(q[11]);

// = = Block_0_0_0_0_0_8 = = 
 
// Trigonometric Variables  

  C12 = cos(q[12]);
  S12 = sin(q[12]);

// = = Block_0_0_0_0_0_9 = = 
 
// Trigonometric Variables  

  C15 = cos(q[15]);
  S15 = sin(q[15]);

// = = Block_0_0_0_0_0_10 = = 
 
// Trigonometric Variables  

  C16 = cos(q[16]);
  S16 = sin(q[16]);

// = = Block_0_0_0_0_0_11 = = 
 
// Trigonometric Variables  

  C17 = cos(q[17]);
  S17 = sin(q[17]);

// = = Block_0_0_0_0_0_12 = = 
 
// Trigonometric Variables  

  C18 = cos(q[18]);
  S18 = sin(q[18]);

// = = Block_0_0_0_0_0_13 = = 
 
// Trigonometric Variables  

  C21 = cos(q[21]);
  S21 = sin(q[21]);

// = = Block_0_0_0_0_0_14 = = 
 
// Trigonometric Variables  

  C22 = cos(q[22]);
  S22 = sin(q[22]);

// = = Block_0_0_0_0_0_15 = = 
 
// Trigonometric Variables  

  C23 = cos(q[23]);
  S23 = sin(q[23]);

// = = Block_0_0_0_0_0_16 = = 
 
// Trigonometric Variables  

  C24 = cos(q[24]);
  S24 = sin(q[24]);

// = = Block_0_0_0_6_0_4 = = 
 
// Trigonometric Variables  

  C3p6 = C3*C6-S3*S6;
  S3p6 = C3*S6+S3*C6;

// = = Block_0_0_0_12_0_8 = = 
 
// Trigonometric Variables  

  C12p9 = C12*C9-S12*S9;
  S12p9 = C12*S9+S12*C9;

// = = Block_0_0_0_18_0_12 = = 
 
// Trigonometric Variables  

  C15p18 = C15*C18-S15*S18;
  S15p18 = C15*S18+S15*C18;

// = = Block_0_0_0_24_0_16 = = 
 
// Trigonometric Variables  

  C21p24 = C21*C24-S21*S24;
  S21p24 = C21*S24+S21*C24;

// ====== END Task 0 ====== 

// ===== BEGIN task 1 ===== 
 
switch(isens)
{
 
// 
break;
case 1:
 


// = = Block_1_0_0_1_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[1];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[1];
    sens->A[1] = qdd[1];
 
// 
break;
case 2:
 


// = = Block_1_0_0_2_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[1];
    sens->P[2] = q[2];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[1];
    sens->V[2] = qd[2];
    sens->A[1] = qdd[1];
    sens->A[2] = qdd[2];
 
// 
break;
case 3:
 


// = = Block_1_0_0_3_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[1];
    sens->P[2] = q[2];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = C3;
    sens->R[1][2] = S3;
    sens->R[2][1] = -S3;
    sens->R[2][2] = C3;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[1];
    sens->V[2] = qd[2];
    sens->OM[3] = qd[3];
    sens->A[1] = qdd[1];
    sens->A[2] = qdd[2];
    sens->OMP[3] = qdd[3];
 
// 
break;
case 4:
 


// = = Block_1_0_0_4_0_2 = = 
 
// Sensor Kinematics 


    ROcp3_14 = C3*C4;
    ROcp3_24 = S3*C4;
    ROcp3_74 = C3*S4;
    ROcp3_84 = S3*S4;
    RLcp3_14 = -s->dpt[2][2]*S3;
    RLcp3_24 = s->dpt[2][2]*C3;
    POcp3_14 = RLcp3_14+q[1];
    POcp3_24 = RLcp3_24+q[2];
    POcp3_34 = s->dpt[3][1]+s->dpt[3][2];
    OMcp3_14 = -qd[4]*S3;
    OMcp3_24 = qd[4]*C3;
    ORcp3_14 = -RLcp3_24*qd[3];
    ORcp3_24 = RLcp3_14*qd[3];
    VIcp3_14 = ORcp3_14+qd[1];
    VIcp3_24 = ORcp3_24+qd[2];
    OPcp3_14 = -(qdd[4]*S3+qd[3]*qd[4]*C3);
    OPcp3_24 = qdd[4]*C3-qd[3]*qd[4]*S3;
    ACcp3_14 = qdd[1]-ORcp3_24*qd[3]-RLcp3_24*qdd[3];
    ACcp3_24 = qdd[2]+ORcp3_14*qd[3]+RLcp3_14*qdd[3];

// = = Block_1_0_0_4_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp3_14;
    sens->P[2] = POcp3_24;
    sens->P[3] = POcp3_34;
    sens->R[1][1] = ROcp3_14;
    sens->R[1][2] = ROcp3_24;
    sens->R[1][3] = -S4;
    sens->R[2][1] = -S3;
    sens->R[2][2] = C3;
    sens->R[3][1] = ROcp3_74;
    sens->R[3][2] = ROcp3_84;
    sens->R[3][3] = C4;
    sens->V[1] = VIcp3_14;
    sens->V[2] = VIcp3_24;
    sens->OM[1] = OMcp3_14;
    sens->OM[2] = OMcp3_24;
    sens->OM[3] = qd[3];
    sens->A[1] = ACcp3_14;
    sens->A[2] = ACcp3_24;
    sens->OMP[1] = OPcp3_14;
    sens->OMP[2] = OPcp3_24;
    sens->OMP[3] = qdd[3];
 
// 
break;
case 5:
 


// = = Block_1_0_0_5_0_3 = = 
 
// Sensor Kinematics 


    ROcp4_15 = C3*C5;
    ROcp4_25 = S3*C5;
    ROcp4_75 = C3*S5;
    ROcp4_85 = S3*S5;
    RLcp4_15 = -s->dpt[2][3]*S3;
    RLcp4_25 = s->dpt[2][3]*C3;
    POcp4_15 = RLcp4_15+q[1];
    POcp4_25 = RLcp4_25+q[2];
    POcp4_35 = s->dpt[3][1]+s->dpt[3][3];
    OMcp4_15 = -qd[5]*S3;
    OMcp4_25 = qd[5]*C3;
    ORcp4_15 = -RLcp4_25*qd[3];
    ORcp4_25 = RLcp4_15*qd[3];
    VIcp4_15 = ORcp4_15+qd[1];
    VIcp4_25 = ORcp4_25+qd[2];
    OPcp4_15 = -(qdd[5]*S3+qd[3]*qd[5]*C3);
    OPcp4_25 = qdd[5]*C3-qd[3]*qd[5]*S3;
    ACcp4_15 = qdd[1]-ORcp4_25*qd[3]-RLcp4_25*qdd[3];
    ACcp4_25 = qdd[2]+ORcp4_15*qd[3]+RLcp4_15*qdd[3];

// = = Block_1_0_0_5_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp4_15;
    sens->P[2] = POcp4_25;
    sens->P[3] = POcp4_35;
    sens->R[1][1] = ROcp4_15;
    sens->R[1][2] = ROcp4_25;
    sens->R[1][3] = -S5;
    sens->R[2][1] = -S3;
    sens->R[2][2] = C3;
    sens->R[3][1] = ROcp4_75;
    sens->R[3][2] = ROcp4_85;
    sens->R[3][3] = C5;
    sens->V[1] = VIcp4_15;
    sens->V[2] = VIcp4_25;
    sens->OM[1] = OMcp4_15;
    sens->OM[2] = OMcp4_25;
    sens->OM[3] = qd[3];
    sens->A[1] = ACcp4_15;
    sens->A[2] = ACcp4_25;
    sens->OMP[1] = OPcp4_15;
    sens->OMP[2] = OPcp4_25;
    sens->OMP[3] = qdd[3];
 
// 
break;
case 6:
 


// = = Block_1_0_0_6_0_4 = = 
 
// Sensor Kinematics 


    RLcp5_16 = s->dpt[1][4]*C3;
    RLcp5_26 = s->dpt[1][4]*S3;
    POcp5_16 = RLcp5_16+q[1];
    POcp5_26 = RLcp5_26+q[2];
    POcp5_36 = s->dpt[3][1]+s->dpt[3][4];
    OMcp5_36 = qd[3]+qd[6];
    ORcp5_16 = -RLcp5_26*qd[3];
    ORcp5_26 = RLcp5_16*qd[3];
    VIcp5_16 = ORcp5_16+qd[1];
    VIcp5_26 = ORcp5_26+qd[2];
    OPcp5_36 = qdd[3]+qdd[6];
    ACcp5_16 = qdd[1]-ORcp5_26*qd[3]-RLcp5_26*qdd[3];
    ACcp5_26 = qdd[2]+ORcp5_16*qd[3]+RLcp5_16*qdd[3];

// = = Block_1_0_0_6_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp5_16;
    sens->P[2] = POcp5_26;
    sens->P[3] = POcp5_36;
    sens->R[1][1] = C3p6;
    sens->R[1][2] = S3p6;
    sens->R[2][1] = -S3p6;
    sens->R[2][2] = C3p6;
    sens->R[3][3] = (1.0);
    sens->V[1] = VIcp5_16;
    sens->V[2] = VIcp5_26;
    sens->OM[3] = OMcp5_36;
    sens->A[1] = ACcp5_16;
    sens->A[2] = ACcp5_26;
    sens->OMP[3] = OPcp5_36;
 
// 
break;
case 7:
 


// = = Block_1_0_0_7_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[7];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[7];
    sens->A[1] = qdd[7];
 
// 
break;
case 8:
 


// = = Block_1_0_0_8_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[7];
    sens->P[2] = q[8];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[7];
    sens->V[2] = qd[8];
    sens->A[1] = qdd[7];
    sens->A[2] = qdd[8];
 
// 
break;
case 9:
 


// = = Block_1_0_0_9_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[7];
    sens->P[2] = q[8];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = C9;
    sens->R[1][2] = S9;
    sens->R[2][1] = -S9;
    sens->R[2][2] = C9;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[7];
    sens->V[2] = qd[8];
    sens->OM[3] = qd[9];
    sens->A[1] = qdd[7];
    sens->A[2] = qdd[8];
    sens->OMP[3] = qdd[9];
 
// 
break;
case 10:
 


// = = Block_1_0_0_10_0_6 = = 
 
// Sensor Kinematics 


    ROcp9_110 = C10*C9;
    ROcp9_210 = C10*S9;
    ROcp9_710 = S10*C9;
    ROcp9_810 = S10*S9;
    RLcp9_110 = -s->dpt[2][6]*S9;
    RLcp9_210 = s->dpt[2][6]*C9;
    POcp9_110 = RLcp9_110+q[7];
    POcp9_210 = RLcp9_210+q[8];
    POcp9_310 = s->dpt[3][1]+s->dpt[3][6];
    OMcp9_110 = -qd[10]*S9;
    OMcp9_210 = qd[10]*C9;
    ORcp9_110 = -RLcp9_210*qd[9];
    ORcp9_210 = RLcp9_110*qd[9];
    VIcp9_110 = ORcp9_110+qd[7];
    VIcp9_210 = ORcp9_210+qd[8];
    OPcp9_110 = -(qdd[10]*S9+qd[10]*qd[9]*C9);
    OPcp9_210 = qdd[10]*C9-qd[10]*qd[9]*S9;
    ACcp9_110 = qdd[7]-ORcp9_210*qd[9]-RLcp9_210*qdd[9];
    ACcp9_210 = qdd[8]+ORcp9_110*qd[9]+RLcp9_110*qdd[9];

// = = Block_1_0_0_10_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp9_110;
    sens->P[2] = POcp9_210;
    sens->P[3] = POcp9_310;
    sens->R[1][1] = ROcp9_110;
    sens->R[1][2] = ROcp9_210;
    sens->R[1][3] = -S10;
    sens->R[2][1] = -S9;
    sens->R[2][2] = C9;
    sens->R[3][1] = ROcp9_710;
    sens->R[3][2] = ROcp9_810;
    sens->R[3][3] = C10;
    sens->V[1] = VIcp9_110;
    sens->V[2] = VIcp9_210;
    sens->OM[1] = OMcp9_110;
    sens->OM[2] = OMcp9_210;
    sens->OM[3] = qd[9];
    sens->A[1] = ACcp9_110;
    sens->A[2] = ACcp9_210;
    sens->OMP[1] = OPcp9_110;
    sens->OMP[2] = OPcp9_210;
    sens->OMP[3] = qdd[9];
 
// 
break;
case 11:
 


// = = Block_1_0_0_11_0_7 = = 
 
// Sensor Kinematics 


    ROcp10_111 = C11*C9;
    ROcp10_211 = C11*S9;
    ROcp10_711 = S11*C9;
    ROcp10_811 = S11*S9;
    RLcp10_111 = -s->dpt[2][7]*S9;
    RLcp10_211 = s->dpt[2][7]*C9;
    POcp10_111 = RLcp10_111+q[7];
    POcp10_211 = RLcp10_211+q[8];
    POcp10_311 = s->dpt[3][1]+s->dpt[3][7];
    OMcp10_111 = -qd[11]*S9;
    OMcp10_211 = qd[11]*C9;
    ORcp10_111 = -RLcp10_211*qd[9];
    ORcp10_211 = RLcp10_111*qd[9];
    VIcp10_111 = ORcp10_111+qd[7];
    VIcp10_211 = ORcp10_211+qd[8];
    OPcp10_111 = -(qdd[11]*S9+qd[11]*qd[9]*C9);
    OPcp10_211 = qdd[11]*C9-qd[11]*qd[9]*S9;
    ACcp10_111 = qdd[7]-ORcp10_211*qd[9]-RLcp10_211*qdd[9];
    ACcp10_211 = qdd[8]+ORcp10_111*qd[9]+RLcp10_111*qdd[9];

// = = Block_1_0_0_11_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp10_111;
    sens->P[2] = POcp10_211;
    sens->P[3] = POcp10_311;
    sens->R[1][1] = ROcp10_111;
    sens->R[1][2] = ROcp10_211;
    sens->R[1][3] = -S11;
    sens->R[2][1] = -S9;
    sens->R[2][2] = C9;
    sens->R[3][1] = ROcp10_711;
    sens->R[3][2] = ROcp10_811;
    sens->R[3][3] = C11;
    sens->V[1] = VIcp10_111;
    sens->V[2] = VIcp10_211;
    sens->OM[1] = OMcp10_111;
    sens->OM[2] = OMcp10_211;
    sens->OM[3] = qd[9];
    sens->A[1] = ACcp10_111;
    sens->A[2] = ACcp10_211;
    sens->OMP[1] = OPcp10_111;
    sens->OMP[2] = OPcp10_211;
    sens->OMP[3] = qdd[9];
 
// 
break;
case 12:
 


// = = Block_1_0_0_12_0_8 = = 
 
// Sensor Kinematics 


    RLcp11_112 = s->dpt[1][8]*C9;
    RLcp11_212 = s->dpt[1][8]*S9;
    POcp11_112 = RLcp11_112+q[7];
    POcp11_212 = RLcp11_212+q[8];
    POcp11_312 = s->dpt[3][1]+s->dpt[3][8];
    OMcp11_312 = qd[12]+qd[9];
    ORcp11_112 = -RLcp11_212*qd[9];
    ORcp11_212 = RLcp11_112*qd[9];
    VIcp11_112 = ORcp11_112+qd[7];
    VIcp11_212 = ORcp11_212+qd[8];
    OPcp11_312 = qdd[12]+qdd[9];
    ACcp11_112 = qdd[7]-ORcp11_212*qd[9]-RLcp11_212*qdd[9];
    ACcp11_212 = qdd[8]+ORcp11_112*qd[9]+RLcp11_112*qdd[9];

// = = Block_1_0_0_12_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp11_112;
    sens->P[2] = POcp11_212;
    sens->P[3] = POcp11_312;
    sens->R[1][1] = C12p9;
    sens->R[1][2] = S12p9;
    sens->R[2][1] = -S12p9;
    sens->R[2][2] = C12p9;
    sens->R[3][3] = (1.0);
    sens->V[1] = VIcp11_112;
    sens->V[2] = VIcp11_212;
    sens->OM[3] = OMcp11_312;
    sens->A[1] = ACcp11_112;
    sens->A[2] = ACcp11_212;
    sens->OMP[3] = OPcp11_312;
 
// 
break;
case 13:
 


// = = Block_1_0_0_13_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[13];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[13];
    sens->A[1] = qdd[13];
 
// 
break;
case 14:
 


// = = Block_1_0_0_14_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[13];
    sens->P[2] = q[14];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[13];
    sens->V[2] = qd[14];
    sens->A[1] = qdd[13];
    sens->A[2] = qdd[14];
 
// 
break;
case 15:
 


// = = Block_1_0_0_15_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[13];
    sens->P[2] = q[14];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = C15;
    sens->R[1][2] = S15;
    sens->R[2][1] = -S15;
    sens->R[2][2] = C15;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[13];
    sens->V[2] = qd[14];
    sens->OM[3] = qd[15];
    sens->A[1] = qdd[13];
    sens->A[2] = qdd[14];
    sens->OMP[3] = qdd[15];
 
// 
break;
case 16:
 


// = = Block_1_0_0_16_0_10 = = 
 
// Sensor Kinematics 


    ROcp15_116 = C15*C16;
    ROcp15_216 = S15*C16;
    ROcp15_716 = C15*S16;
    ROcp15_816 = S15*S16;
    RLcp15_116 = -s->dpt[2][10]*S15;
    RLcp15_216 = s->dpt[2][10]*C15;
    POcp15_116 = RLcp15_116+q[13];
    POcp15_216 = RLcp15_216+q[14];
    POcp15_316 = s->dpt[3][10]+s->dpt[3][1];
    OMcp15_116 = -qd[16]*S15;
    OMcp15_216 = qd[16]*C15;
    ORcp15_116 = -RLcp15_216*qd[15];
    ORcp15_216 = RLcp15_116*qd[15];
    VIcp15_116 = ORcp15_116+qd[13];
    VIcp15_216 = ORcp15_216+qd[14];
    OPcp15_116 = -(qdd[16]*S15+qd[15]*qd[16]*C15);
    OPcp15_216 = qdd[16]*C15-qd[15]*qd[16]*S15;
    ACcp15_116 = qdd[13]-ORcp15_216*qd[15]-RLcp15_216*qdd[15];
    ACcp15_216 = qdd[14]+ORcp15_116*qd[15]+RLcp15_116*qdd[15];

// = = Block_1_0_0_16_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp15_116;
    sens->P[2] = POcp15_216;
    sens->P[3] = POcp15_316;
    sens->R[1][1] = ROcp15_116;
    sens->R[1][2] = ROcp15_216;
    sens->R[1][3] = -S16;
    sens->R[2][1] = -S15;
    sens->R[2][2] = C15;
    sens->R[3][1] = ROcp15_716;
    sens->R[3][2] = ROcp15_816;
    sens->R[3][3] = C16;
    sens->V[1] = VIcp15_116;
    sens->V[2] = VIcp15_216;
    sens->OM[1] = OMcp15_116;
    sens->OM[2] = OMcp15_216;
    sens->OM[3] = qd[15];
    sens->A[1] = ACcp15_116;
    sens->A[2] = ACcp15_216;
    sens->OMP[1] = OPcp15_116;
    sens->OMP[2] = OPcp15_216;
    sens->OMP[3] = qdd[15];
 
// 
break;
case 17:
 


// = = Block_1_0_0_17_0_11 = = 
 
// Sensor Kinematics 


    ROcp16_117 = C15*C17;
    ROcp16_217 = S15*C17;
    ROcp16_717 = C15*S17;
    ROcp16_817 = S15*S17;
    RLcp16_117 = -s->dpt[2][11]*S15;
    RLcp16_217 = s->dpt[2][11]*C15;
    POcp16_117 = RLcp16_117+q[13];
    POcp16_217 = RLcp16_217+q[14];
    POcp16_317 = s->dpt[3][11]+s->dpt[3][1];
    OMcp16_117 = -qd[17]*S15;
    OMcp16_217 = qd[17]*C15;
    ORcp16_117 = -RLcp16_217*qd[15];
    ORcp16_217 = RLcp16_117*qd[15];
    VIcp16_117 = ORcp16_117+qd[13];
    VIcp16_217 = ORcp16_217+qd[14];
    OPcp16_117 = -(qdd[17]*S15+qd[15]*qd[17]*C15);
    OPcp16_217 = qdd[17]*C15-qd[15]*qd[17]*S15;
    ACcp16_117 = qdd[13]-ORcp16_217*qd[15]-RLcp16_217*qdd[15];
    ACcp16_217 = qdd[14]+ORcp16_117*qd[15]+RLcp16_117*qdd[15];

// = = Block_1_0_0_17_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp16_117;
    sens->P[2] = POcp16_217;
    sens->P[3] = POcp16_317;
    sens->R[1][1] = ROcp16_117;
    sens->R[1][2] = ROcp16_217;
    sens->R[1][3] = -S17;
    sens->R[2][1] = -S15;
    sens->R[2][2] = C15;
    sens->R[3][1] = ROcp16_717;
    sens->R[3][2] = ROcp16_817;
    sens->R[3][3] = C17;
    sens->V[1] = VIcp16_117;
    sens->V[2] = VIcp16_217;
    sens->OM[1] = OMcp16_117;
    sens->OM[2] = OMcp16_217;
    sens->OM[3] = qd[15];
    sens->A[1] = ACcp16_117;
    sens->A[2] = ACcp16_217;
    sens->OMP[1] = OPcp16_117;
    sens->OMP[2] = OPcp16_217;
    sens->OMP[3] = qdd[15];
 
// 
break;
case 18:
 


// = = Block_1_0_0_18_0_12 = = 
 
// Sensor Kinematics 


    RLcp17_118 = s->dpt[1][12]*C15;
    RLcp17_218 = s->dpt[1][12]*S15;
    POcp17_118 = RLcp17_118+q[13];
    POcp17_218 = RLcp17_218+q[14];
    POcp17_318 = s->dpt[3][12]+s->dpt[3][1];
    OMcp17_318 = qd[15]+qd[18];
    ORcp17_118 = -RLcp17_218*qd[15];
    ORcp17_218 = RLcp17_118*qd[15];
    VIcp17_118 = ORcp17_118+qd[13];
    VIcp17_218 = ORcp17_218+qd[14];
    OPcp17_318 = qdd[15]+qdd[18];
    ACcp17_118 = qdd[13]-ORcp17_218*qd[15]-RLcp17_218*qdd[15];
    ACcp17_218 = qdd[14]+ORcp17_118*qd[15]+RLcp17_118*qdd[15];

// = = Block_1_0_0_18_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp17_118;
    sens->P[2] = POcp17_218;
    sens->P[3] = POcp17_318;
    sens->R[1][1] = C15p18;
    sens->R[1][2] = S15p18;
    sens->R[2][1] = -S15p18;
    sens->R[2][2] = C15p18;
    sens->R[3][3] = (1.0);
    sens->V[1] = VIcp17_118;
    sens->V[2] = VIcp17_218;
    sens->OM[3] = OMcp17_318;
    sens->A[1] = ACcp17_118;
    sens->A[2] = ACcp17_218;
    sens->OMP[3] = OPcp17_318;
 
// 
break;
case 19:
 


// = = Block_1_0_0_19_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[19];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[19];
    sens->A[1] = qdd[19];
 
// 
break;
case 20:
 


// = = Block_1_0_0_20_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[19];
    sens->P[2] = q[20];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = (1.0);
    sens->R[2][2] = (1.0);
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[19];
    sens->V[2] = qd[20];
    sens->A[1] = qdd[19];
    sens->A[2] = qdd[20];
 
// 
break;
case 21:
 


// = = Block_1_0_0_21_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = q[19];
    sens->P[2] = q[20];
    sens->P[3] = s->dpt[3][1];
    sens->R[1][1] = C21;
    sens->R[1][2] = S21;
    sens->R[2][1] = -S21;
    sens->R[2][2] = C21;
    sens->R[3][3] = (1.0);
    sens->V[1] = qd[19];
    sens->V[2] = qd[20];
    sens->OM[3] = qd[21];
    sens->A[1] = qdd[19];
    sens->A[2] = qdd[20];
    sens->OMP[3] = qdd[21];
 
// 
break;
case 22:
 


// = = Block_1_0_0_22_0_14 = = 
 
// Sensor Kinematics 


    ROcp21_122 = C21*C22;
    ROcp21_222 = S21*C22;
    ROcp21_722 = C21*S22;
    ROcp21_822 = S21*S22;
    RLcp21_122 = -s->dpt[2][14]*S21;
    RLcp21_222 = s->dpt[2][14]*C21;
    POcp21_122 = RLcp21_122+q[19];
    POcp21_222 = RLcp21_222+q[20];
    POcp21_322 = s->dpt[3][14]+s->dpt[3][1];
    OMcp21_122 = -qd[22]*S21;
    OMcp21_222 = qd[22]*C21;
    ORcp21_122 = -RLcp21_222*qd[21];
    ORcp21_222 = RLcp21_122*qd[21];
    VIcp21_122 = ORcp21_122+qd[19];
    VIcp21_222 = ORcp21_222+qd[20];
    OPcp21_122 = -(qdd[22]*S21+qd[21]*qd[22]*C21);
    OPcp21_222 = qdd[22]*C21-qd[21]*qd[22]*S21;
    ACcp21_122 = qdd[19]-ORcp21_222*qd[21]-RLcp21_222*qdd[21];
    ACcp21_222 = qdd[20]+ORcp21_122*qd[21]+RLcp21_122*qdd[21];

// = = Block_1_0_0_22_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp21_122;
    sens->P[2] = POcp21_222;
    sens->P[3] = POcp21_322;
    sens->R[1][1] = ROcp21_122;
    sens->R[1][2] = ROcp21_222;
    sens->R[1][3] = -S22;
    sens->R[2][1] = -S21;
    sens->R[2][2] = C21;
    sens->R[3][1] = ROcp21_722;
    sens->R[3][2] = ROcp21_822;
    sens->R[3][3] = C22;
    sens->V[1] = VIcp21_122;
    sens->V[2] = VIcp21_222;
    sens->OM[1] = OMcp21_122;
    sens->OM[2] = OMcp21_222;
    sens->OM[3] = qd[21];
    sens->A[1] = ACcp21_122;
    sens->A[2] = ACcp21_222;
    sens->OMP[1] = OPcp21_122;
    sens->OMP[2] = OPcp21_222;
    sens->OMP[3] = qdd[21];
 
// 
break;
case 23:
 


// = = Block_1_0_0_23_0_15 = = 
 
// Sensor Kinematics 


    ROcp22_123 = C21*C23;
    ROcp22_223 = S21*C23;
    ROcp22_723 = C21*S23;
    ROcp22_823 = S21*S23;
    RLcp22_123 = -s->dpt[2][15]*S21;
    RLcp22_223 = s->dpt[2][15]*C21;
    POcp22_123 = RLcp22_123+q[19];
    POcp22_223 = RLcp22_223+q[20];
    POcp22_323 = s->dpt[3][15]+s->dpt[3][1];
    OMcp22_123 = -qd[23]*S21;
    OMcp22_223 = qd[23]*C21;
    ORcp22_123 = -RLcp22_223*qd[21];
    ORcp22_223 = RLcp22_123*qd[21];
    VIcp22_123 = ORcp22_123+qd[19];
    VIcp22_223 = ORcp22_223+qd[20];
    OPcp22_123 = -(qdd[23]*S21+qd[21]*qd[23]*C21);
    OPcp22_223 = qdd[23]*C21-qd[21]*qd[23]*S21;
    ACcp22_123 = qdd[19]-ORcp22_223*qd[21]-RLcp22_223*qdd[21];
    ACcp22_223 = qdd[20]+ORcp22_123*qd[21]+RLcp22_123*qdd[21];

// = = Block_1_0_0_23_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp22_123;
    sens->P[2] = POcp22_223;
    sens->P[3] = POcp22_323;
    sens->R[1][1] = ROcp22_123;
    sens->R[1][2] = ROcp22_223;
    sens->R[1][3] = -S23;
    sens->R[2][1] = -S21;
    sens->R[2][2] = C21;
    sens->R[3][1] = ROcp22_723;
    sens->R[3][2] = ROcp22_823;
    sens->R[3][3] = C23;
    sens->V[1] = VIcp22_123;
    sens->V[2] = VIcp22_223;
    sens->OM[1] = OMcp22_123;
    sens->OM[2] = OMcp22_223;
    sens->OM[3] = qd[21];
    sens->A[1] = ACcp22_123;
    sens->A[2] = ACcp22_223;
    sens->OMP[1] = OPcp22_123;
    sens->OMP[2] = OPcp22_223;
    sens->OMP[3] = qdd[21];
 
// 
break;
case 24:
 


// = = Block_1_0_0_24_0_16 = = 
 
// Sensor Kinematics 


    RLcp23_124 = s->dpt[1][16]*C21;
    RLcp23_224 = s->dpt[1][16]*S21;
    POcp23_124 = RLcp23_124+q[19];
    POcp23_224 = RLcp23_224+q[20];
    POcp23_324 = s->dpt[3][16]+s->dpt[3][1];
    OMcp23_324 = qd[21]+qd[24];
    ORcp23_124 = -RLcp23_224*qd[21];
    ORcp23_224 = RLcp23_124*qd[21];
    VIcp23_124 = ORcp23_124+qd[19];
    VIcp23_224 = ORcp23_224+qd[20];
    OPcp23_324 = qdd[21]+qdd[24];
    ACcp23_124 = qdd[19]-ORcp23_224*qd[21]-RLcp23_224*qdd[21];
    ACcp23_224 = qdd[20]+ORcp23_124*qd[21]+RLcp23_124*qdd[21];

// = = Block_1_0_0_24_1_0 = = 
 
// Symbolic Outputs  

    sens->P[1] = POcp23_124;
    sens->P[2] = POcp23_224;
    sens->P[3] = POcp23_324;
    sens->R[1][1] = C21p24;
    sens->R[1][2] = S21p24;
    sens->R[2][1] = -S21p24;
    sens->R[2][2] = C21p24;
    sens->R[3][3] = (1.0);
    sens->V[1] = VIcp23_124;
    sens->V[2] = VIcp23_224;
    sens->OM[3] = OMcp23_324;
    sens->A[1] = ACcp23_124;
    sens->A[2] = ACcp23_224;
    sens->OMP[3] = OPcp23_324;

break;
default:
break;
}


// ====== END Task 1 ====== 


}
 

