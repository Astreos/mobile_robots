//---------------------------
// UCL-CEREM-MBS
//
// Creation : 2006
// Last update : 24/04/2016
//---------------------------


#include<stdlib.h>
#include <stdio.h>
#include "math.h"
#include "lut.h"

MbsLut1D* mbs_lut_1D_alloc()
{
    MbsLut1D* newlut;
    newlut=(MbsLut1D*) malloc(sizeof(MbsLut1D));
    return newlut;
}

MbsLut2D* mbs_lut_2D_alloc()
{
    MbsLut2D* newlut;
    newlut=(MbsLut2D*) malloc(sizeof(MbsLut2D));
    return newlut;
}


void mbs_lut_1D_free(MbsLut1D *lut)
{
    free(lut->x);
    free(lut->y);

    free(lut);
}

void mbs_lut_2D_free(MbsLut2D *lut)
{
    int i;

    free(lut->x);
    free(lut->y);
    
    for(i=1;i<=lut->nx;i++)
    {
        free(lut->z[i]);
    }
    free(lut->z);

    free(lut);
}


double mbs_lut_1D_interp(MbsLut1D *lut, double x)
{
    int indx;
    int step,min,max;
    double alpha;
    double y;
    

    min=1;
    max=lut->nx;
    while(max-min>1)
    {
        step=(max-min)/2;
        if(x-lut->x[min+step] >=0)
        {
            min+=step;
        }
        else
        {
            max-=step;
        }
    }
    indx=min;


    if(indx>=lut->nx) 
        indx=lut->nx-1;

    if(indx != min)
    {
        min=(min+min)/2;
    }


    /**/

    alpha = (lut->y[indx+1]-lut->y[indx])/(lut->x[indx+1]-lut->x[indx]);
    
    y = alpha*(x-lut->x[indx])+lut->y[indx];
    
    return y;
}

double mbs_lut_2D_interp(MbsLut2D *lut, double x, double y)
{
    int indx,indy;
    int step,min,max;
    double alpha,beta,gamma,zeta;
    double z;
    

    min=1;
    max=lut->nx;
    while(max-min>1)
    {
        step=(max-min)/2;
        if(x-lut->x[min+step] >=0)
        {
            min+=step;
        }
        else
        {
            max-=step;
        }
    }
    indx=min;


    if(indx>=lut->nx) 
        indx=lut->nx-1;

    if(indx != min)
    {
        min=(min+min)/2;
    }

/**/    
    min=1;
    max=lut->ny;
    while(max-min>1)
    {
        step=(max-min)/2;
        if(y-lut->y[min+step] >=0)
        {
            min+=step;
        }
        else
        {
            max-=step;
        }
    }
    indy=min;

    if(indy >= lut->ny) 
        indy=lut->ny-1;

    if(indy != min)
    {
        min=(min+min)/2;
    }

    /**/

    alpha = lut->z[indx][indy]/((lut->x[indx+1]-lut->x[indx])*(lut->y[indy+1]-lut->y[indy]));
    beta = lut->z[indx+1][indy+1]/((lut->x[indx+1]-lut->x[indx])*(lut->y[indy+1]-lut->y[indy]));
    gamma = lut->z[indx+1][indy]/((lut->x[indx+1]-lut->x[indx])*(lut->y[indy+1]-lut->y[indy]));
    zeta = lut->z[indx][indy+1]/((lut->x[indx+1]-lut->x[indx])*(lut->y[indy+1]-lut->y[indy]));
    
    z = alpha*((lut->x[indx+1]-x)*(lut->y[indy+1]-y))+beta*((x-lut->x[indx])*(y-lut->y[indy]))+gamma*((x-lut->x[indx])*(lut->y[indy+1]-y))+zeta*((lut->x[indx+1]-x)*(y-lut->y[indy]));
    
    return z;
}


#ifdef MATLAB_MEX_FILE
MbsLut1D* mbs_lut_1D_load_mxarray(const mxArray *LUT_1D_ptr)
{
    mxArray *field_value_ptr;
    int i;
    
    MbsLut1D* lut;

    lut = mbs_lut_1D_alloc();
    // x //
    field_value_ptr = mxGetField(LUT_1D_ptr, 0,  "x");
    lut->nx = mxGetM(field_value_ptr);
    lut->x = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->x[i] = mxGetPr(field_value_ptr)[i-1];
    }

    // y //
    field_value_ptr = mxGetField(LUT_1D_ptr, 0,  "y");
    lut->y = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->y[i] = mxGetPr(field_value_ptr)[i-1];
    }

    return lut;
}

MbsLut2D* mbs_lut_2D_load_mxarray(const mxArray *LUT_2D_ptr)
{
    mxArray *field_value_ptr;
    int i,j;

    MbsLut2D* lut;

    lut = mbs_lut_2D_alloc();
    // x //
    field_value_ptr = mxGetField(LUT_2D_ptr, 0,  "x");
    lut->nx = mxGetM(field_value_ptr);
    lut->x = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->x[i] = mxGetPr(field_value_ptr)[i-1];
    }

    // y //
    field_value_ptr = mxGetField(LUT_2D_ptr, 0,  "y");
    lut->ny = mxGetM(field_value_ptr);
    lut->y = (double*) calloc(lut->ny +1,sizeof(double));
    for(i=1;i<=lut->ny ;i++) 
    {
        lut->y[i] = mxGetPr(field_value_ptr)[i-1];
    }

    // z //
    field_value_ptr = mxGetField(LUT_2D_ptr, 0,  "z");
    lut->z = (double**) calloc(lut->nx +1,sizeof(double*));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->z[i] = (double*) calloc(lut->ny+1,sizeof(double)); 
        for(j=1;j<=lut->ny;j++)
        {
            lut->z[i][j] = mxGetPr(field_value_ptr)[(i-1)+lut->nx*(j-1)];
        }
    }

    return lut;

}
#endif


MbsLut1D* mbs_lut_1D_load_1vector(const double *LUT_1D_ptr,const int nx)
{
    int i;
    
    MbsLut1D* lut;

    lut = mbs_lut_1D_alloc();
    // x //
    lut->nx = nx;
    lut->x = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->x[i] = LUT_1D_ptr[i-1];
    }

    // y //
    lut->y = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->y[i] = LUT_1D_ptr[nx+i-1];
    }

    return lut;
}

MbsLut1D* mbs_lut_1D_load_2vectors(const double *x_ptr,const double *y_ptr,const int nx)
{
    int i;
    
    MbsLut1D* lut;

    lut = mbs_lut_1D_alloc();
    // x //
    lut->nx = nx;
    lut->x = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->x[i] = x_ptr[i-1];
    }

    // y //
    lut->y = (double*) calloc(lut->nx +1,sizeof(double));
    for(i=1;i<=lut->nx ;i++) 
    {
        lut->y[i] = y_ptr[i-1];
    }

    return lut;
}


MbsLut1D* mbs_lut_1D_load_file(char* fileName)
{
    FILE *flut;
	char line[256];
    char* lineChk;
	int i, j, chk;
	MbsLut1D* lut;
	
	lut = mbs_lut_1D_alloc();

	// read the file
	flut = fopen(fileName,"r");
	if(flut==0){
        printf("Impossible to find the file %s.", fileName );
	}

	lineChk = fgets(line,256,flut);	  // read the information line
	lineChk = fgets(line,256, flut);  // read the number of elements
	sscanf(line,"%d",&lut->nx);       // save the number of elements

	lut->x=calloc(lut->nx+1,sizeof(double));  // table memory allocation
	lut->y=calloc(lut->nx+1,sizeof(double));  // table memory allocation

	lut->x[0] = 0;
	lut->y[0] = 0;

	// read the x vector
	for(j=1;j<=lut->nx;j++){
		chk = fscanf(flut,"%lf", &lut->x[j] );
	}
	lineChk = fgets(line,256,flut);
	
	// read the y vector
	for(j=1;j<=lut->nx;j++){
		chk = fscanf(flut,"%lf", &lut->y[j] );
	}
	
    fclose(flut);
	return lut;
}

MbsLut2D* mbs_lut_2D_load_file(char* fileName)
{
	FILE *flut;
	char line[256];
    char* lineChk;
	int i, j, chk;
	MbsLut2D* lut;
	
	lut = mbs_lut_2D_alloc();

	// read the file
	flut = fopen(fileName,"r");
	if(flut==0){
        printf("Impossible to find the file %s.", fileName );
	}

	lineChk = fgets(line,256,flut);	  // read the information line
	lineChk = fgets(line,256, flut);  // read the number of elements
	sscanf(line,"%d %d",&lut->nx, &lut->ny);  // save the number of elements

	lut->x=calloc(lut->nx+1,sizeof(double));  // table memory allocation
	lut->y=calloc(lut->ny+1,sizeof(double));  // table memory allocation
	lut->z=calloc(lut->nx+1,sizeof(double*)); // table memory allocation

	lut->x[0] = 0;
	lut->y[0] = 0;

	// read the y vector
	for(j=1;j<=lut->ny;j++){
		chk = fscanf(flut,"%lf", &lut->y[j] );
	}
	lineChk = fgets(line,256,flut);
	
	// read the x vector and the z matrix
	for(i=1;i<=lut->nx;i++){
		chk = fscanf(flut,"%lf", &lut->x[i] ); // 1st element = x[i]
		lut->z[i] = (double*) calloc(lut->ny+1,sizeof(double)); 
		for(j=1;j<=lut->ny;j++){
            chk = fscanf(flut,"%lf", &lut->z[i][j] );
		}
		lineChk = fgets(line,256,flut);
	}
    
    // Close and return
	fclose(flut);
	return lut;
}
