//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef mbs_matrix_h
#define mbs_matrix_h
/*--------------------*/

void transpose(double M[4][4], double Mt[4][4]);

double norm(double v[4]);

void normalize(double v[4], double vn[4]);

double scalar_product(double v1[4], double v2[4]);

void vector_sum(double v1[4], double v2[4], double v[4]);

void vector_diff(double v1[4], double v2[4], double v[4]);

void cross_product(double v1[4], double v2[4], double v[4]);

void matrix_product(double M[4][4], double v[4], double Mv[4]);

 /**
  * Compute the product of 2 matrices
  */
void doublematrix_product(double M1[4][4], double M2[4][4], double M[4][4]);

void rotation_matrix(int type, double angle, double R[4][4]);

void number_product(double s, double v1[4], double v[4]);



/*--------------------*/
#endif
