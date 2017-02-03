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
//	==> Function : F 1 : Direct Dynamics (Semi-Explicit formulation) : RNEA
//	==> Flops complexity : 792
//
//	==> Generation Time :  0.020 seconds
//	==> Post-Processing :  0.010 seconds
//
//-------------------------------------------------------------
//
 
#include <math.h> 

#include "mbs_data.h"
#include "mbs_project_interface.h"
 
void mbs_dirdyna(double **M,double *c,
MbsData *s, double tsim)

// double M[24][24];
// double c[24];
{ 
 
#include "mbs_dirdyna_m454_project.h" 
#define q s->q 
#define qd s->qd 
#define qdd s->qdd 
 
 

// === begin imp_aux === 

// === end imp_aux === 

// ===== BEGIN task 0 ===== 

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

// = = Block_0_1_0_0_0_1 = = 
 
// Forward Kinematics 

  BS13 = -qd[3]*qd[3];
  BS53 = -qd[3]*qd[3];

// = = Block_0_1_0_0_0_5 = = 
 
// Forward Kinematics 

  BS19 = -qd[9]*qd[9];
  BS59 = -qd[9]*qd[9];

// = = Block_0_1_0_0_0_9 = = 
 
// Forward Kinematics 

  BS115 = -qd[15]*qd[15];
  BS515 = -qd[15]*qd[15];

// = = Block_0_1_0_0_0_13 = = 
 
// Forward Kinematics 

  BS121 = -qd[21]*qd[21];
  BS521 = -qd[21]*qd[21];

// = = Block_0_1_0_1_0_2 = = 
 
// Forward Kinematics 

  OM14 = -qd[3]*S4;
  OM34 = qd[3]*C4;

// = = Block_0_1_0_1_0_3 = = 
 
// Forward Kinematics 

  OM15 = -qd[3]*S5;
  OM35 = qd[3]*C5;

// = = Block_0_1_0_1_0_6 = = 
 
// Forward Kinematics 

  OM110 = -qd[9]*S10;
  OM310 = qd[9]*C10;

// = = Block_0_1_0_1_0_7 = = 
 
// Forward Kinematics 

  OM111 = -qd[9]*S11;
  OM311 = qd[9]*C11;

// = = Block_0_1_0_1_0_10 = = 
 
// Forward Kinematics 

  OM116 = -qd[15]*S16;
  OM316 = qd[15]*C16;

// = = Block_0_1_0_1_0_11 = = 
 
// Forward Kinematics 

  OM117 = -qd[15]*S17;
  OM317 = qd[15]*C17;

// = = Block_0_1_0_1_0_14 = = 
 
// Forward Kinematics 

  OM122 = -qd[21]*S22;
  OM322 = qd[21]*C22;

// = = Block_0_1_0_1_0_15 = = 
 
// Forward Kinematics 

  OM123 = -qd[21]*S23;
  OM323 = qd[21]*C23;

// = = Block_0_2_0_1_0_2 = = 
 
// Backward Dynamics 

  FA14 = -(s->frc[1][4]-s->m[4]*s->g[3]*S4);
  FA34 = -(s->frc[3][4]+s->m[4]*s->g[3]*C4);
  CF24 = -(s->trq[2][4]-OM14*OM34*(s->In[1][4]-s->In[9][4]));

// = = Block_0_2_0_1_0_3 = = 
 
// Backward Dynamics 

  FA15 = -(s->frc[1][5]-s->m[5]*s->g[3]*S5);
  FA35 = -(s->frc[3][5]+s->m[5]*s->g[3]*C5);
  CF25 = -(s->trq[2][5]-OM15*OM35*(s->In[1][5]-s->In[9][5]));

// = = Block_0_2_0_1_0_4 = = 
 
// Backward Dynamics 

  FA16 = -(s->frc[1][6]-s->m[6]*BS13*s->dpt[1][4]*C6);
  FA26 = -(s->frc[2][6]+s->m[6]*BS13*s->dpt[1][4]*S6);

// = = Block_0_2_0_1_0_6 = = 
 
// Backward Dynamics 

  FA110 = -(s->frc[1][10]-s->m[10]*s->g[3]*S10);
  FA310 = -(s->frc[3][10]+s->m[10]*s->g[3]*C10);
  CF210 = -(s->trq[2][10]-OM110*OM310*(s->In[1][10]-s->In[9][10]));

// = = Block_0_2_0_1_0_7 = = 
 
// Backward Dynamics 

  FA111 = -(s->frc[1][11]-s->m[11]*s->g[3]*S11);
  FA311 = -(s->frc[3][11]+s->m[11]*s->g[3]*C11);
  CF211 = -(s->trq[2][11]-OM111*OM311*(s->In[1][11]-s->In[9][11]));

// = = Block_0_2_0_1_0_8 = = 
 
// Backward Dynamics 

  FA112 = -(s->frc[1][12]-s->m[12]*BS19*s->dpt[1][8]*C12);
  FA212 = -(s->frc[2][12]+s->m[12]*BS19*s->dpt[1][8]*S12);

// = = Block_0_2_0_1_0_10 = = 
 
// Backward Dynamics 

  FA116 = -(s->frc[1][16]-s->m[16]*s->g[3]*S16);
  FA316 = -(s->frc[3][16]+s->m[16]*s->g[3]*C16);
  CF216 = -(s->trq[2][16]-OM116*OM316*(s->In[1][16]-s->In[9][16]));

// = = Block_0_2_0_1_0_11 = = 
 
// Backward Dynamics 

  FA117 = -(s->frc[1][17]-s->m[17]*s->g[3]*S17);
  FA317 = -(s->frc[3][17]+s->m[17]*s->g[3]*C17);
  CF217 = -(s->trq[2][17]-OM117*OM317*(s->In[1][17]-s->In[9][17]));

// = = Block_0_2_0_1_0_12 = = 
 
// Backward Dynamics 

  FA118 = -(s->frc[1][18]-s->m[18]*BS115*s->dpt[1][12]*C18);
  FA218 = -(s->frc[2][18]+s->m[18]*BS115*s->dpt[1][12]*S18);

// = = Block_0_2_0_1_0_14 = = 
 
// Backward Dynamics 

  FA122 = -(s->frc[1][22]-s->m[22]*s->g[3]*S22);
  FA322 = -(s->frc[3][22]+s->m[22]*s->g[3]*C22);
  CF222 = -(s->trq[2][22]-OM122*OM322*(s->In[1][22]-s->In[9][22]));

// = = Block_0_2_0_1_0_15 = = 
 
// Backward Dynamics 

  FA123 = -(s->frc[1][23]-s->m[23]*s->g[3]*S23);
  FA323 = -(s->frc[3][23]+s->m[23]*s->g[3]*C23);
  CF223 = -(s->trq[2][23]-OM123*OM323*(s->In[1][23]-s->In[9][23]));

// = = Block_0_2_0_1_0_16 = = 
 
// Backward Dynamics 

  FA124 = -(s->frc[1][24]-s->m[24]*BS121*s->dpt[1][16]*C24);
  FA224 = -(s->frc[2][24]+s->m[24]*BS121*s->dpt[1][16]*S24);

// = = Block_0_2_0_2_0_1 = = 
 
// Backward Dynamics 

  FF13 = -(s->frc[1][3]-s->m[3]*BS13*s->l[1][3]-FA14*C4-FA15*C5-FA16*C6+FA26*S6-FA34*S4-FA35*S5);
  FF23 = -(s->frc[2][3]+s->frc[2][4]+s->frc[2][5]-s->m[4]*BS53*s->dpt[2][2]-s->m[5]*BS53*s->dpt[2][3]-FA16*S6-FA26*C6);
  CF33 = -(s->trq[3][3]+s->trq[3][6]+s->frc[2][3]*s->l[1][3]-s->dpt[1][4]*(FA16*S6+FA26*C6)+s->dpt[2][2]*(FA14*C4+FA34*
 S4)+s->dpt[2][3]*(FA15*C5+FA35*S5)+C4*(s->trq[3][4]+qd[3]*qd[4]*s->In[9][4]*S4+qd[4]*OM14*(s->In[1][4]-s->In[5][4]))-S4*(
 s->trq[1][4]+qd[3]*qd[4]*s->In[1][4]*C4+qd[4]*OM34*(s->In[5][4]-s->In[9][4]))+C5*(s->trq[3][5]+qd[3]*qd[5]*s->In[9][5]*S5+
 qd[5]*OM15*(s->In[1][5]-s->In[5][5]))-S5*(s->trq[1][5]+qd[3]*qd[5]*s->In[1][5]*C5+qd[5]*OM35*(s->In[5][5]-s->In[9][5])));
  CM33_1 = -(C3*(s->m[4]*s->dpt[2][2]+s->m[5]*s->dpt[2][3])+S3*(s->m[3]*s->l[1][3]+s->m[6]*s->dpt[1][4]));
  CM33_2 = C3*(s->m[3]*s->l[1][3]+s->m[6]*s->dpt[1][4])-S3*(s->m[4]*s->dpt[2][2]+s->m[5]*s->dpt[2][3]);
  CM33_3 = s->In[9][3]+s->In[9][6]+s->In[1][4]*S4*S4+s->In[1][5]*S5*S5+s->In[9][4]*C4*C4+s->In[9][5]*C5*C5+s->m[3]*
 s->l[1][3]*s->l[1][3]+s->m[4]*s->dpt[2][2]*s->dpt[2][2]+s->m[5]*s->dpt[2][3]*s->dpt[2][3]+s->m[6]*s->dpt[1][4]*s->dpt[1][4];
  FF2_13 = FF13*C3-FF23*S3;
  FF2_23 = FF13*S3+FF23*C3;
  FM21_13 = s->m[3]+s->m[4]+s->m[5]+s->m[6];
  FM22_23 = s->m[3]+s->m[4]+s->m[5]+s->m[6];

// = = Block_0_2_0_2_0_5 = = 
 
// Backward Dynamics 

  FF19 = -(s->frc[1][9]-s->m[9]*BS19*s->l[1][9]-FA110*C10-FA111*C11-FA112*C12+FA212*S12-FA310*S10-FA311*S11);
  FF29 = -(s->frc[2][10]+s->frc[2][11]+s->frc[2][9]-s->m[10]*BS59*s->dpt[2][6]-s->m[11]*BS59*s->dpt[2][7]-FA112*S12-
 FA212*C12);
  CF39 = -(s->trq[3][12]+s->trq[3][9]+s->frc[2][9]*s->l[1][9]-s->dpt[1][8]*(FA112*S12+FA212*C12)+s->dpt[2][6]*(FA110*C10
 +FA310*S10)+s->dpt[2][7]*(FA111*C11+FA311*S11)+C10*(s->trq[3][10]+qd[10]*qd[9]*s->In[9][10]*S10+qd[10]*OM110*(s->In[1][10]-
 s->In[5][10]))-S10*(s->trq[1][10]+qd[10]*qd[9]*s->In[1][10]*C10+qd[10]*OM310*(s->In[5][10]-s->In[9][10]))+C11*(s->trq[3][11]
 +qd[11]*qd[9]*s->In[9][11]*S11+qd[11]*OM111*(s->In[1][11]-s->In[5][11]))-S11*(s->trq[1][11]+qd[11]*qd[9]*s->In[1][11]*C11+
 qd[11]*OM311*(s->In[5][11]-s->In[9][11])));
  CM39_7 = -(C9*(s->m[10]*s->dpt[2][6]+s->m[11]*s->dpt[2][7])+S9*(s->m[12]*s->dpt[1][8]+s->m[9]*s->l[1][9]));
  CM39_8 = C9*(s->m[12]*s->dpt[1][8]+s->m[9]*s->l[1][9])-S9*(s->m[10]*s->dpt[2][6]+s->m[11]*s->dpt[2][7]);
  CM39_9 = s->In[9][12]+s->In[9][9]+s->In[1][10]*S10*S10+s->In[1][11]*S11*S11+s->In[9][10]*C10*C10+s->In[9][11]*C11*C11+
 s->m[10]*s->dpt[2][6]*s->dpt[2][6]+s->m[11]*s->dpt[2][7]*s->dpt[2][7]+s->m[12]*s->dpt[1][8]*s->dpt[1][8]+s->m[9]*s->l[1][9]*
 s->l[1][9];
  FF8_19 = FF19*C9-FF29*S9;
  FF8_29 = FF19*S9+FF29*C9;
  FM87_19 = s->m[10]+s->m[11]+s->m[12]+s->m[9];
  FM88_29 = s->m[10]+s->m[11]+s->m[12]+s->m[9];

// = = Block_0_2_0_2_0_9 = = 
 
// Backward Dynamics 

  FF115 = -(s->frc[1][15]-s->m[15]*BS115*s->l[1][15]-FA116*C16-FA117*C17-FA118*C18+FA218*S18-FA316*S16-FA317*S17);
  FF215 = -(s->frc[2][15]+s->frc[2][16]+s->frc[2][17]-s->m[16]*BS515*s->dpt[2][10]-s->m[17]*BS515*s->dpt[2][11]-FA118*
 S18-FA218*C18);
  CF315 = -(s->trq[3][15]+s->trq[3][18]+s->frc[2][15]*s->l[1][15]-s->dpt[1][12]*(FA118*S18+FA218*C18)+s->dpt[2][10]*(
 FA116*C16+FA316*S16)+s->dpt[2][11]*(FA117*C17+FA317*S17)+C16*(s->trq[3][16]+qd[15]*qd[16]*s->In[9][16]*S16+qd[16]*OM116*(
 s->In[1][16]-s->In[5][16]))-S16*(s->trq[1][16]+qd[15]*qd[16]*s->In[1][16]*C16+qd[16]*OM316*(s->In[5][16]-s->In[9][16]))+C17*
 (s->trq[3][17]+qd[15]*qd[17]*s->In[9][17]*S17+qd[17]*OM117*(s->In[1][17]-s->In[5][17]))-S17*(s->trq[1][17]+qd[15]*qd[17]*
 s->In[1][17]*C17+qd[17]*OM317*(s->In[5][17]-s->In[9][17])));
  CM315_13 = -(C15*(s->m[16]*s->dpt[2][10]+s->m[17]*s->dpt[2][11])+S15*(s->m[15]*s->l[1][15]+s->m[18]*s->dpt[1][12]));
  CM315_14 = C15*(s->m[15]*s->l[1][15]+s->m[18]*s->dpt[1][12])-S15*(s->m[16]*s->dpt[2][10]+s->m[17]*s->dpt[2][11]);
  CM315_15 = s->In[9][15]+s->In[9][18]+s->In[1][16]*S16*S16+s->In[1][17]*S17*S17+s->In[9][16]*C16*C16+s->In[9][17]*C17*
 C17+s->m[15]*s->l[1][15]*s->l[1][15]+s->m[16]*s->dpt[2][10]*s->dpt[2][10]+s->m[17]*s->dpt[2][11]*s->dpt[2][11]+s->m[18]*
 s->dpt[1][12]*s->dpt[1][12];
  FF14_115 = FF115*C15-FF215*S15;
  FF14_215 = FF115*S15+FF215*C15;
  FM1413_115 = s->m[15]+s->m[16]+s->m[17]+s->m[18];
  FM1414_215 = s->m[15]+s->m[16]+s->m[17]+s->m[18];

// = = Block_0_2_0_2_0_13 = = 
 
// Backward Dynamics 

  FF121 = -(s->frc[1][21]-s->m[21]*BS121*s->l[1][21]-FA122*C22-FA123*C23-FA124*C24+FA224*S24-FA322*S22-FA323*S23);
  FF221 = -(s->frc[2][21]+s->frc[2][22]+s->frc[2][23]-s->m[22]*BS521*s->dpt[2][14]-s->m[23]*BS521*s->dpt[2][15]-FA124*
 S24-FA224*C24);
  CF321 = -(s->trq[3][21]+s->trq[3][24]+s->frc[2][21]*s->l[1][21]-s->dpt[1][16]*(FA124*S24+FA224*C24)+s->dpt[2][14]*(
 FA122*C22+FA322*S22)+s->dpt[2][15]*(FA123*C23+FA323*S23)+C22*(s->trq[3][22]+qd[21]*qd[22]*s->In[9][22]*S22+qd[22]*OM122*(
 s->In[1][22]-s->In[5][22]))-S22*(s->trq[1][22]+qd[21]*qd[22]*s->In[1][22]*C22+qd[22]*OM322*(s->In[5][22]-s->In[9][22]))+C23*
 (s->trq[3][23]+qd[21]*qd[23]*s->In[9][23]*S23+qd[23]*OM123*(s->In[1][23]-s->In[5][23]))-S23*(s->trq[1][23]+qd[21]*qd[23]*
 s->In[1][23]*C23+qd[23]*OM323*(s->In[5][23]-s->In[9][23])));
  CM321_19 = -(C21*(s->m[22]*s->dpt[2][14]+s->m[23]*s->dpt[2][15])+S21*(s->m[21]*s->l[1][21]+s->m[24]*s->dpt[1][16]));
  CM321_20 = C21*(s->m[21]*s->l[1][21]+s->m[24]*s->dpt[1][16])-S21*(s->m[22]*s->dpt[2][14]+s->m[23]*s->dpt[2][15]);
  CM321_21 = s->In[9][21]+s->In[9][24]+s->In[1][22]*S22*S22+s->In[1][23]*S23*S23+s->In[9][22]*C22*C22+s->In[9][23]*C23*
 C23+s->m[21]*s->l[1][21]*s->l[1][21]+s->m[22]*s->dpt[2][14]*s->dpt[2][14]+s->m[23]*s->dpt[2][15]*s->dpt[2][15]+s->m[24]*
 s->dpt[1][16]*s->dpt[1][16];
  FF20_121 = FF121*C21-FF221*S21;
  FF20_221 = FF121*S21+FF221*C21;
  FM2019_121 = s->m[21]+s->m[22]+s->m[23]+s->m[24];
  FM2020_221 = s->m[21]+s->m[22]+s->m[23]+s->m[24];

// = = Block_0_3_0_0_0_0 = = 
 
// Symbolic Outputs  

  M[1][1] = FM21_13;
  M[1][3] = CM33_1;
  M[2][2] = FM22_23;
  M[2][3] = CM33_2;
  M[3][1] = CM33_1;
  M[3][2] = CM33_2;
  M[3][3] = CM33_3;
  M[3][6] = s->In[9][6];
  M[4][4] = s->In[5][4];
  M[5][5] = s->In[5][5];
  M[6][3] = s->In[9][6];
  M[6][6] = s->In[9][6];
  M[7][7] = FM87_19;
  M[7][9] = CM39_7;
  M[8][8] = FM88_29;
  M[8][9] = CM39_8;
  M[9][7] = CM39_7;
  M[9][8] = CM39_8;
  M[9][9] = CM39_9;
  M[9][12] = s->In[9][12];
  M[10][10] = s->In[5][10];
  M[11][11] = s->In[5][11];
  M[12][9] = s->In[9][12];
  M[12][12] = s->In[9][12];
  M[13][13] = FM1413_115;
  M[13][15] = CM315_13;
  M[14][14] = FM1414_215;
  M[14][15] = CM315_14;
  M[15][13] = CM315_13;
  M[15][14] = CM315_14;
  M[15][15] = CM315_15;
  M[15][18] = s->In[9][18];
  M[16][16] = s->In[5][16];
  M[17][17] = s->In[5][17];
  M[18][15] = s->In[9][18];
  M[18][18] = s->In[9][18];
  M[19][19] = FM2019_121;
  M[19][21] = CM321_19;
  M[20][20] = FM2020_221;
  M[20][21] = CM321_20;
  M[21][19] = CM321_19;
  M[21][20] = CM321_20;
  M[21][21] = CM321_21;
  M[21][24] = s->In[9][24];
  M[22][22] = s->In[5][22];
  M[23][23] = s->In[5][23];
  M[24][21] = s->In[9][24];
  M[24][24] = s->In[9][24];
  c[1] = FF2_13;
  c[2] = FF2_23;
  c[3] = CF33;
  c[4] = CF24;
  c[5] = CF25;
  c[6] = -s->trq[3][6];
  c[7] = FF8_19;
  c[8] = FF8_29;
  c[9] = CF39;
  c[10] = CF210;
  c[11] = CF211;
  c[12] = -s->trq[3][12];
  c[13] = FF14_115;
  c[14] = FF14_215;
  c[15] = CF315;
  c[16] = CF216;
  c[17] = CF217;
  c[18] = -s->trq[3][18];
  c[19] = FF20_121;
  c[20] = FF20_221;
  c[21] = CF321;
  c[22] = CF222;
  c[23] = CF223;
  c[24] = -s->trq[3][24];

// ====== END Task 0 ====== 


}
 

