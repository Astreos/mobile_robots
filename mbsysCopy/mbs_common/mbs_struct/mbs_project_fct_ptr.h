
#ifndef _MBS_PROJECT_FCT_PTR_H_
#define _MBS_PROJECT_FCT_PTR_H_

#include <stdint.h>  // necessary for uint8_t for user_keyboard_ptr

/**
 * 
 * Declaration of functions that are project dependent 
 *
 */

typedef struct MbsData MbsData;
typedef struct MbsSensor MbsSensor;
typedef struct MbsDirdyn MbsDirdyn;

typedef double* (*user_JointForces_ptr)(MbsData*, double);
typedef void    (*user_dirdyn_init_ptr)(MbsData *MBSdata, MbsDirdyn *mbs_dd);
typedef void    (*user_dirdyn_loop_ptr)(MbsData *MBSdata, MbsDirdyn *mbs_dd);
typedef void    (*user_dirdyn_finish_ptr)(MbsData*, MbsDirdyn*);
typedef void    (*user_Derivative_ptr)(MbsData *s);
typedef void    (*user_DrivenJoints_ptr)(MbsData *s, double tsim);
typedef void    (*user_cons_hJ_ptr)(double *h, double **Jac, MbsData *s, double tsim);
typedef void    (*user_cons_jdqd_ptr)(double *jdqd, MbsData *s, double tsim);
typedef double  (*user_LinkForces_ptr)(double Z, double Zd, MbsData *mbs_data, double tsim, int ilnk);
typedef double* (*user_ExtForces_ptr)(double PxF[4], double RxF[4][4], double VxF[4], double OMxF[4], 
                                  double AxF[4], double OMPxF[4], MbsData *s, double tsim,int ixF);

typedef double  (*user_LinkForces_ptr)(double Z, double Zd, MbsData *mbs_data, double tsim, int ilnk);


typedef struct MDS_gen_strct  MDS_gen_strct;

typedef UserIO*     (*mbs_new_user_IO_ptr)(UserIoInfo* ioInfo);
typedef UserModel*  (*mbs_new_user_model_ptr)();
typedef void (*mbs_delete_user_model_ptr)(UserModel* ums);
typedef void (*mbs_delete_user_IO_ptr)(UserIO *uvs);
typedef void (*mbs_load_user_model_xml_ptr)(MDS_gen_strct* gen, UserModel* ums);


#ifdef REAL_TIME
    typedef struct Realtime_option Realtime_option;

    typedef void (*user_realtime_options_ptr)(MbsData* mbs_data, Realtime_option *options);
    typedef struct Simu_realtime Simu_realtime;

    #ifdef SDL
        typedef uint8_t Uint8;  // copied from sdl header

        typedef void (*user_keyboard_ptr)(MbsData* mbs_data, Simu_realtime *realtime, int cur_t_usec, const Uint8 *keystates);
        typedef void (*user_realtime_plot_ptr)(MbsData* mbs_data);
        typedef void (*user_joystick_axes_ptr)(MbsData* mbs_data, Simu_realtime *realtime, int nb_joysticks);
        typedef void (*user_joystick_buttons_ptr)(MbsData* mbs_data, int buttonID);
    #endif

    #ifdef JAVA
        typedef void    (*user_realtime_visu_ptr)(MbsData* mbs_data, int nb_models, int *nb_q, double **q_vec);
    #endif

#endif

typedef void    (*mbs_link_ptr)(double **frc,double **trq,double *Flnk,double *Z,double *Zd,MbsData *s, double tsim);
typedef void    (*mbs_link3D_ptr)(double **frc,double **trq, MbsData *s, double tsim);
typedef void    (*mbs_extforces_ptr)(double **frc, double **trq, MbsData *s, double tsim);
typedef int     (*mbs_accelred_ptr)(MbsData *s, double tsim);
typedef void    (*mbs_dirdyna_ptr)(double **M,double *c, MbsData *s, double tsim);
typedef void    (*mbs_invdyna_ptr)(double *Q, MbsData *s, double tsim);
typedef void    (*mbs_cons_hJ_ptr)(double *h,double **Jac,MbsData *s, double tsim);
typedef void    (*mbs_cons_jdqd_ptr)(double *Jdqd,MbsData *s, double tsim);
typedef void    (*mbs_sensor_ptr)(MbsSensor *sens, MbsData *s, int isens);
             
#endif
