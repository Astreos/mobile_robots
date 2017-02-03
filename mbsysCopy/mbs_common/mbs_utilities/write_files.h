/*
 * Write the output vectors and tabs (.anim,...)
 *
 * author: Nicolas Van der Noot and Allan Barrea
 */ 
#ifndef __WRITE_FILES_H_INCLUDED__  // guard against multiple/recursive includes
#define __WRITE_FILES_H_INCLUDED__


#include "mbs_data.h"

#define NB_OUTPUT_VEC 5

// -- Structures -- //

// .anim structure
typedef struct Write_files
{
    double *t;
    double **qq;
    int kount;
    double **out_vec;

} Write_files;

// -- Functions prototypes -- //

Write_files* init_write_files(int nstep, int njoint);
void free_write_files(Write_files *write_files, int njoint);
int write_anim_file(Write_files* write_files, int njoint, const char *fileout);
int write_out_files(Write_files* write_files, const char generic_fileout[50]);

// configure
void update_write_files(Write_files *write_files, MbsData *mbs_data);


#endif
