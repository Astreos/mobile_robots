//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#include "math.h"
#include "mbs_matrix.h"

void transpose(double M[4][4], double Mt[4][4])
{
    int i,j;
    for(i=1;i<=3;i++)
    {
        for(j=1;j<=3;j++)
        {
            Mt[i][j] = M[j][i];
        }
    }
}


/*----------*/
double norm(double v[4])
{
    return pow(pow(v[1],2)+pow(v[2],2)+pow(v[3],2),0.5);
}


/*----------*/
void normalize(double v[4], double vn[4])
{
    double n;

    n = norm(v);
    
    vn[1]=v[1]/n;
    vn[2]=v[2]/n;
    vn[3]=v[3]/n;
}


/*----------*/
double scalar_product(double v1[4], double v2[4])
{
    return v1[1]*v2[1]+v1[2]*v2[2]+v1[3]*v2[3];
}


/*----------*/
void vector_sum(double v1[4], double v2[4], double v[4])
{
    v[1] = v1[1]+v2[1];
    v[2] = v1[2]+v2[2];
    v[3] = v1[3]+v2[3];
}

/*----------*/
void vector_diff(double v1[4], double v2[4], double v[4])
{
    v[1] = v1[1]-v2[1];
    v[2] = v1[2]-v2[2];
    v[3] = v1[3]-v2[3];
}

/*----------*/
void cross_product(double v1[4], double v2[4], double v[4])
{
    v[1] = v1[2]*v2[3]-v1[3]*v2[2];
    v[2] = v1[3]*v2[1]-v1[1]*v2[3];
    v[3] = v1[1]*v2[2]-v1[2]*v2[1];
}


/*----------*/
void matrix_product(double M[4][4], double v[4], double Mv[4])
{
    Mv[1] = M[1][1]*v[1] + M[1][2]*v[2] + M[1][3]*v[3];
    Mv[2] = M[2][1]*v[1] + M[2][2]*v[2] + M[2][3]*v[3];
    Mv[3] = M[3][1]*v[1] + M[3][2]*v[2] + M[3][3]*v[3];
}


/*----------*/
void doublematrix_product(double M1[4][4], double M2[4][4], double M[4][4])
{
    int i,j;

    for(i=1;i<=3;i++)
    {
        for(j=1;j<=3;j++)
        {
            M[i][j] = M1[i][1]*M2[1][j] + M1[i][2]*M2[2][j] + M1[i][3]*M2[3][j];
        }
    }

}

/*----------*/
void rotation_matrix(int type, double angle, double R[4][4])
{
    switch(type) {
        case 1:
            R[1][1] = 1;
            R[2][1] = 0;
            R[3][1] = 0;
            R[1][2] = 0;
            R[2][2] = cos(angle);
            R[3][2] = -sin(angle);
            R[1][3] = 0;
            R[2][3] = sin(angle);
            R[3][3] = cos(angle);
            break;
        case 2:
            R[1][1] = cos(angle);
            R[2][1] = 0;
            R[3][1] = sin(angle);
            R[1][2] = 0;
            R[2][2] = 1;
            R[3][2] = 0;
            R[1][3] = -sin(angle);
            R[2][3] = 0;
            R[3][3] = cos(angle);
            break;
        case 3:
            R[1][1] = cos(angle);
            R[2][1] = -sin(angle);
            R[3][1] = 0;
            R[1][2] = sin(angle);
            R[2][2] = cos(angle);
            R[3][2] = 0;
            R[1][3] = 0;
            R[2][3] = 0;
            R[3][3] = 1;
            break;
    }


}


void number_product(double s, double v1[4], double v[4]){
	int i;
	for(i = 1; i < 4; i++) v[i] = s*v1[i];
}
