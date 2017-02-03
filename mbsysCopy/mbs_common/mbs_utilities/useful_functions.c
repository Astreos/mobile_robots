/*
 * Useful functions for the simulation
 *
 * author: Nicolas Van der Noot & Aubain Verle
 */

#include "useful_functions.h"
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>



#include <stdarg.h>

void mbs_log(char* msg, ...){
    va_list argptr;
    FILE *f = fopen("log", "a");   
    
    va_start(argptr, msg);
    vfprintf(f, msg, argptr);
    va_end(argptr);   
    
    fclose(f);
}
#ifdef UNIX
#include <execinfo.h>
void mbs_print_trace (void){
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
     printf ("%s\n", strings[i]);

  free (strings);
}
#endif // UNIX

// -- Exponents related functions -- //

double pow_int(double a, int b)
{
    int i;
    double cur_val;

    // special case
    if (b == 0)
    {
        return 1.0;
    }

    cur_val = a;

    // loop
    for(i=1; i<abs(b); i++)
    {
        cur_val *= a;
    }

    if (b < 0)
    {
        return 1.0 / cur_val;
    }
    else
    {
        return cur_val;
    }
}

int expo_ten_inf(double nb)
{
    int iter;
    double cur_nb;

    nb = fabs(nb);

    iter = 0;

    if (nb < 1.0)
    {
        cur_nb = 1.0;

        // loop
        while(cur_nb - nb > 1e-9)
        {
            iter--;
            cur_nb *= 0.1;
        }
    }
    else if (nb > 1.0)
    {
        cur_nb = 10.0;

        // loop
        while(nb - cur_nb >= 0.0)
        {
            iter++;
            cur_nb *= 10.0;
        }
    }
    return iter;
}


// -- Sign function -- //
double sign(double a)
{
    return (a<0 ? -1 : 1);
}


// -- Time function -- //
char* get_time_machine()
{
    time_t cur_t;
    char* t_string;

    // obtain current time as seconds elapsed since the Epoch
    cur_t = time(NULL);

    if (cur_t == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to compute the current time.");
        return NULL;
    }

    // convert to local time format
    t_string = ctime(&cur_t);

    if (t_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.");
        return NULL;
    }

    return t_string;
}


// -- Char related functions -- //
char* get_char_vec(int x)
{
    char *vec;

    vec = (char*) malloc(x*sizeof(char));

    vec = " ";

    return vec;
}

char** get_char_tab(int x, int y)
{
    int i;

    char **mat;

    mat = (char**) malloc(x*sizeof(char*));

    for(i=0; i<x; i++)
    {
        mat[i] = " ";
    }

    return mat;
}

void free_char_vec(char *vec)
{
    free(vec);
}

void free_char_tab(char** mat)
{
    int i;

    for(i=0; i<0; i++)
    {
        free(mat[i]);
    }

    free(mat);
}


// -- Integer related functions -- //
int* get_ivec_0(int x)
{
    return (int*) calloc(x,sizeof(int));
}

int* get_ivec_1(int x)
{
    int *vec;

    vec = (int*) calloc(x+1,sizeof(int));

    vec[0] = x;

    return vec;
}

int** get_imat_0(int x, int y)
{
    int i;
    int **mat;

    mat = (int**) calloc(x,sizeof(int*));

    mat[0] = (int*) calloc(x*y,sizeof(int));

    for(i=1; i<x; i++)
    {
        mat[i] = &(mat[0][i*y]);
    }

    return mat;
}

int** get_imat_1(int x, int y)
{
    int i;

    int **mat;

    mat = (int**) calloc(x+1,sizeof(int*));

    mat[0] = (int*) calloc(x*(y+1)+1,sizeof(int));

    mat[0][0] = x;

    for(i=1; i<=x; i++)
    {
        mat[i] = &(mat[0][(i-1)*(y+1)+1]);
        mat[i][0] = y;
    }

    return mat;
}

void free_ivec_0(int *vec)
{
    free(vec);
}


void free_ivec_1(int *vec)
{
    free(vec);
}

void free_imat_0(int **mat)
{
    free(mat[0]);
    free(mat);
}

void free_imat_1(int **mat)
{
    free(mat[0]);
    free(mat);
}

void copy_ivec_0(int *vec1, int *vec2, int x)
{
    int i;

    for(i=0; i<x; i++)
    {
        vec2[i] = vec1[i];
    }
}

void copy_ivec_1(int *vec1, int *vec2, int x)
{
    int i;

    for(i=0; i<=x; i++)
    {
        vec2[i] = vec1[i];
    }
}

void copy_imat_0(int **tab1, int **tab2, int x , int y)
{
    int i,j;

    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            tab2[i][j] = tab1[i][j];
        }
    }
}

void print_ivec_0(int *vec, int x)
{
    int i;

    printf("[");

    for(i=0; i<x; i++)
    {
        printf("%d ",vec[i]);
    }

    printf("]\n");
}

void print_ivec_1(int *vec)
{
    int i;

    printf("[");

    for(i=1; i<=vec[0]; i++)
    {
        printf("%d ",vec[i]);
    }

    printf("]\n");
}

void print_imat_0(int **mat, int x , int y)
{
    int i,j;

    for(i=0; i<x; i++)
    {
        printf("|");

        for(j=0; j<y; j++)
        {
            printf("%d ", mat[i][j]);
        }

        printf("|\n");
    }
}

void sort_ivec_0(int *vec1, int *vec2, int x)
{
    int i, j;
    int threshold = -INT_MAX;
    int val = INT_MAX;
    int n = 0;

    for(i=0; i<x; )
    {
        n = 0;
        val = INT_MAX;
        for(j=0; j<x; j++)
        {
            if(vec1[j]>threshold)
            {
                if (val > vec1[j])
                {
                    n=1;
                    val = vec1[j];
                }
                else if (val == vec1[j])
                {
                    n++;
                }
            }
        }
        while(n != 0)
        {
            vec2[i] = val;
            i++;
            n--;
        }
        threshold = val;
    }
}

void sort_ivec_1(int *vec1, int *vec2, int x)
{
    int i, j;
    int threshold = -INT_MAX;
    int val = INT_MAX;
    int n = 0;

    for(i=1; i<=x; )
    {
        n = 0;
        val = INT_MAX;
        for(j=1; j<=x; j++)
        {
            if(vec1[j]>threshold)
            {
                if (val > vec1[j])
                {
                    n=1;
                    val = vec1[j];
                }
                else if (val == vec1[j])
                {
                    n++;
                }
            }
        }
        while(n != 0)
        {
            vec2[i] = val;
            i++;
            n--;
        }
        threshold = val;
    }
}

void sort_inplace_ivec_1(int *vec, int x)
{
    int i, val;
    int sorted = 0;
    
    while (sorted != 1){
        sorted = 1;
        for (i=1; i<x; i++){
            if (vec[i]>vec[i+1]){
                sorted = 0;
                val = vec[i+1];
                vec[i+1] = vec[i];
                vec[i] = val;
            }
        }
    }
}

void f0123_ivec_0(int* vec, int x)
{
    int i;

    for(i=0; i<x; i++)
    {
        vec[i] = i;
    }
}

void conc_ivec_0(int* vec1, int x1, int* vec2, int x2, int* vec3)
{
    int i;

    for(i=0; i<x1; i++)
    {
        vec3[i] = vec1[i];
    }

    for(i=0; i<x2; i++)
    {
        vec3[x1+i] = vec2[i];
    }
}

void conc_ivec_1(int* vec1, int x1, int* vec2, int x2, int* vec3)
{
    int i;

    for(i=1; i<=x1; i++)
    {
        vec3[i] = vec1[i];
    }

    for(i=1; i<=x2; i++)
    {
        vec3[x1+i] = vec2[i];
    }
    vec3[0]=x1+x2;
}

void slct_ivec_0(int* vec1, int x1, int* vec2, int x2, int* vec3)
{
    int i;

    for(i=0; i<x2; i++)
    {
        vec3[i] = vec1[vec2[i]];
    }
}

int find_ivec_0(int* vec, int x, int f)
{
    int i;

    for(i=0; i<x; i++)
    {
        if (vec[i] == f)
        {
            return i;
        }
    }

    return -1;
}

int find_ivec_1(int* vec, int x, int f)
{
    int i;

    for(i=1; i<=x; i++)
    {
        if (vec[i] == f)
        {
            return i;
        }
    }

    return -1;
}


// -- Double related functions -- //
double* get_dvec_0(int x)
{
    return (double*) calloc(x,sizeof(double));
}

double* get_dvec_1(int x)
{
    double *vec;

    vec = (double*) calloc(x+1,sizeof(double));

    vec[0] = x;

    return vec;
}

double** get_dmat_0(int x, int y)
{
    int i;
    double **mat;

    mat = (double**) calloc(x,sizeof(double*));

    mat[0] = (double*) calloc(x*y,sizeof(double));

    for(i=1; i<x; i++)
    {
        mat[i] = &(mat[0][i*y]);
    }

    return mat;
}

double** get_dmat_1(int x, int y)
{
    int i;
    double **mat;

    mat = (double**) calloc(x+1,sizeof(double*));

    mat[0] = (double*) calloc((x+1)*(y+1),sizeof(double));

    mat[0][0] = x;

    for(i=1; i<=x; i++)
    {
        mat[i] = &(mat[0][(i)*(y+1)]);
        mat[i][0] = y;
    }

    return mat;
}


void free_dvec_0(double *vec)
{
    free(vec);
}

void free_dvec_1(double *vec)
{
    free(vec);
}

void free_dmat_0(double **mat)
{
    free(mat[0]);
    free(mat);
}

void free_dmat_1(double **mat)
{
    free(mat[0]);
    free(mat);
}


void copy_dvec_0(double *vec1, double *vec2, int x)
{
    int i;

    for(i=0; i<x; i++)
    {
        vec2[i] = vec1[i];
    }
}

void copy_dvec_1(double *vec1, double *vec2)
{
    int i, vec_size;

    vec_size = (int) vec1[0];

    if (vec_size != (int) vec2[0])
    {
        printf("copy_dvec_1 warning: vec1[0] (%d) != vec2[0] (%d) !\n", vec_size, (int) vec2[0]);
        return;
    }

    for(i=1; i<=vec_size; i++)
    {
        vec2[i] = vec1[i];
    }
}

void copy_dmat_0(double **tab1, double **tab2, int x , int y)
{
    int i,j;

    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            tab2[i][j] = tab1[i][j];
        }
    }
}


void print_dvec_0(double *vec, int x)
{
    int i;

    printf("[");

    for(i=0; i<x; i++)
    {
        printf("%f ",vec[i]);
    }

    printf("]\n");
}

void print_dvec_1(double *vec)
{
    int i;
    int vec_size;

    vec_size = (int) vec[0];

    if (vec_size <= 0)
    {
        printf("print_dvec_1 warning: vec[0] is %d !\n", vec_size);
        return;
    }

    printf("[");

    for(i=1; i<=vec_size; i++)
    {
        printf("%f ",vec[i]);
    }

    printf("]\n");
}

void print_dmat_0(double **mat, int x , int y)
{
    int i,j;

    for(i=0; i<x; i++)
    {
        printf("|");
        for(j=0; j<y; j++)
        {
            printf("%f ", mat[i][j]);
        }
        printf("|\n");
    }
}

void save_dvec_0(double *vec, int x, char *name)
{
    FILE *file_out;
    int i;
    // File declaration
    file_out = NULL; // internal filename

    // Opening file
    file_out = fopen(name,"wt");

    // Fill the file
    if(file_out == NULL)
    {
        printf("error: cannot open file '%s'\n", name);
        exit(1);
    }

    for(i=0; i<x; i++)
    {
            fprintf(file_out,"\t %f ",vec[i]);
    }

    fclose(file_out);
}

void save_dmat_0(double **mat, int x, int y,char *name)
{
    FILE *file_out;
    int i,j;
    // File declaration
    file_out = NULL; // internal filename

    // Opening file
    file_out = fopen(name,"wt");

    // Fill the file
    if(file_out == NULL)
    {
        printf("error: cannot open file '%s'\n", name);
        exit(1);
    }

    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            fprintf(file_out,"\t %12.5e ",mat[i][j]);
        }
        fprintf(file_out,"\n");
    }

    fclose(file_out);
}

void slct_dvec_0(double* vec1, int x1, int* vec2, int x2, double* vec3)
{
    int i;

    for(i=0; i<x2; i++)
    {
        vec3[i] = vec1[vec2[i]];
    }
}

void slct_dmat_0(double **tab1, int x1, int y1, double **tab2, int y2, int* vec)
{
    int i,j;

    for(i=0; i<y2; i++)
    {
        for(j=0; j<x1; j++)
        {
            tab2[j][i] = tab1[j][vec[i]];
        }
    }
}

void slctr_dmat_0(double **tab1, int x1, int y1, double **tab2, int x2, int* vec)
{
    int i,j;

    for(i=0; i<x2; i++)
    {
        for(j=0; j<y1; j++)
        {
            tab2[i][j] = tab1[vec[i]][j];
        }
    }
}


int any_dvec_0(double *vec, int x)
{
    int i;
    int any=0;

    for(i=0; i<x; i++)
    {
        if(vec[i] != 0.0)
        {
            any =1;
            return any;
        }
    }
    return any;
}

double norm_dvec_0(double *vec, int x)
{
    int i;
    double norm_2 = 0.0;

    for(i=0; i<x; i++)
    {
        norm_2 =norm_2 + (vec[i]*vec[i]);
    }
    return sqrt(norm_2);
}

void zeros_dvec_0(double *vec, int x)
{
    int i;

    for(i=0; i<x; i++)
    {
        vec[i] = 0.0;
    }
}

void zeros_dmat_0(double **mat, int x, int y)
{
    int i,j;

    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            mat[i][j] = 0.0;
        }
    }
}
