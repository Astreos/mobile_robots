/*! 
 * \author Nicolas Van der Noot
 * \file open_gl_c_int.h
 * \brief interface to call C++ functions from C code
 */
#if (defined OPEN_GL) && (!defined SEPARATE_OPEN_GL) && (!defined PROCESS_OPEN_GL)

#ifndef _OPEN_GL_C_INT_H_
#define _OPEN_GL_C_INT_H_

#include "realtime_ext.h"

#if defined(__APPLE__) || defined(__MACH__)
    #define APPLE_VISU
#endif

#ifdef UNIX
    // comment this line to remove multi-processing on Unix (Linux-Mac OS) systems
    #define OPEN_GL_PROCESS_UNIX

    // comment this line to remove multi-thread on Unix (Linux-Mac OS) systems
    //#define OPEN_GL_THREAD_UNIX
#endif

// comment this line to remove multi-processing on Windows systems
#ifdef WIN32
    #define OPEN_GL_PROCESS_WIN
#endif

#ifdef __cplusplus
extern "C" {
#endif

// function prototypes
void init_open_gl(Realtime_visu *visu, MbsData *mbs_data, int nb_models, int *nb_q, double **q_vec, char **mbs_file, int start_viewpoint);
void free_open_gl(void *visu_class);
void update_open_gl(Simu_realtime *realtime);
void break_open_gl(Simu_realtime *realtime);

#ifdef __cplusplus
}
#endif

#endif
#endif
