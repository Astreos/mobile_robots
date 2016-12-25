//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------
//
// Gestion via Bugzilla :
// 01/10/2008 : JFC : Bug n°37
//
#ifndef mbs_data_h
#define mbs_data_h
/*--------------------*/

#include "stdio.h"
#include "stdlib.h"

#include "mbs_user_interface.h"

#include "mbs_project_fct_ptr.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// utility structure for managing dynamic lib (for userfct and symbolic libraries)

#ifdef PRJ_FCT_PTR

#ifdef UNIX
    #include <dlfcn.h>
#else
	#include <windows.h>
#endif
/**
 * This structures contains a reference to a dynamic library handle.
 * It is intended to provide a generic interface, whatever the OS
 * (since dynamic library management is not identical between linux
 *  and windows).
 */
  
typedef struct MbsDataLibInfo{
	#ifdef UNIX
    void *lib_handle;
    #else
	HINSTANCE lib_handle;
	#endif
}MbsDataLibInfo;

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef PRJ_FCT_PTR

typedef struct{

	// Pointers to libraries from which the functions were loaded
    MbsDataLibInfo* symbolicLibHandle;
    MbsDataLibInfo* userfctLibHandle;
    
    // Pointers to project function

	// user function pointers
	struct{
        user_JointForces_ptr user_JointForces;
        user_dirdyn_init_ptr user_dirdyn_init;
        user_dirdyn_loop_ptr user_dirdyn_loop;
        user_dirdyn_finish_ptr user_dirdyn_finish;
        user_Derivative_ptr user_Derivative;
        user_DrivenJoints_ptr user_DrivenJoints;
        user_cons_hJ_ptr user_cons_hJ;
        user_cons_jdqd_ptr user_cons_jdqd;
        user_LinkForces_ptr user_LinkForces;
        user_ExtForces_ptr user_ExtForces;

        mbs_new_user_IO_ptr mbs_new_user_IO;
        mbs_new_user_model_ptr mbs_new_user_model;
        mbs_delete_user_model_ptr mbs_delete_user_model;
        mbs_delete_user_IO_ptr mbs_delete_user_IO;
        mbs_load_user_model_xml_ptr mbs_load_user_model_xml;

        #ifdef REAL_TIME
            user_realtime_options_ptr user_realtime_options;

            #ifdef SDL
                user_keyboard_ptr user_keyboard;
                user_realtime_plot_ptr user_realtime_plot;
                user_joystick_axes_ptr user_joystick_axes;
                user_joystick_buttons_ptr user_joystick_buttons;
            #endif

            #ifdef VISU_3D
                user_realtime_visu_ptr user_realtime_visu;
            #endif
        #endif
    } user;

    struct{
        // symbolic function pointers
        mbs_link_ptr	mbs_link;
        mbs_link3D_ptr	mbs_link3D;
        mbs_extforces_ptr mbs_extforces;
        mbs_accelred_ptr mbs_accelred;
        mbs_dirdyna_ptr mbs_dirdyna;
        mbs_invdyna_ptr mbs_invdyna;
        mbs_cons_hJ_ptr mbs_cons_hJ;
        mbs_cons_jdqd_ptr mbs_cons_jdqd;
        mbs_sensor_ptr mbs_sensor;
    } symb;

} Functions;

#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


struct MbsData
{
    // MATLAB FIELDS //

    // Données géométriques et dynamiques //
    int npt; ///< Number of anchor points.
    double *dpt[3+1]; ///< Array containing the coordinate of all anchor points.
    double *l[3+1]; ///< Array containing the center of mass coordinates (in the body fixed frame, one column per body).
    double *m; ///< Array containing the mass of each body.
    double *In[9+1]; ///< Array containing the inertia tensor component of each body (in the body fixed frame, relative to the center of mass).
                     ///< 1 column containing 9 rows for each body as follow: [I11;I12;I13;I21;I22;I23;I31;I32;I33]
    double g[3+1]; ///< The 3 gravity components.

    int nbody;  ///< Number of bodies in the system.
                ///< The 'fictitious' bodies are not taken into account.
    int njoint; ///< Number of joints in the system.

    // Infos partitionnement //
    int nqu;      ///< Number of independent articulations
    int nqc;      ///< Number of driven articulations, it includes qlocked and qdriven
    int nqlocked; ///< Number of locked articulations
    int nqdriven; ///< Number of driven articulations
    int nqa;      ///< Number of actuated articulations
    int nqv;      ///< Number of dependent articulations
    int nhu;      ///< Number of independent constraints
    
    int *qu;      ///< Array with the indices of the independent articulations (free)
    int *qc;      ///< Array with the indices of driven (locked and driven) articulations.
    int *qlocked; ///< Array with the indices of locked articulations.
                  ///< Those articulations have a constant position defined by the user \f$ q(nqc)=cte \f$.
    int *qdriven; ///< Array with the indices of driven articulations.
                  ///< Those articulations are controlled by a user function \f$ q(qc)=f(t) \f$.
    int *qa;      ///< Array with the indices of actuated articulations (only for inverse dynamic).
                  ///< Those articulations are controlled by an actuator.
    int *qv;      ///< Array with the indices of the dependent articulations \f$ q(qv)=q(qu) \f$.
    int *hu;      ///< Array with the indices of independent constraints.

    // Variables articulaires, valeurs initiales et limites //
    double *q;  ///< Array with the current values of the generalized coordinates.
    double *qd; ///< Array with the current values of the generalized velocities.
    double *qdd;///< Array with the current values of the generalized acceleration.
    
    double *q0;  ///< Array with the initial values of the generalized coordinates.
    double *qd0; ///< Array with the initial values of the generalized velocities.
    double *qdd0;///< Array with the initial values of the generalized acceleration.

    double *qmin; ///< Deprecated? Array with the values of the minimal values of the generalized coordinates.
    double *qmax; ///< Deprecated? Array with the values of the maximal values of the generalized coordinates.

    // Frc, Trq, Qq, tsim
    double *frc[3+1]; ///< Matrix with the components of the resultant external forces (in the body fixed frame) applied to the center of mass of each body.
    double *trq[3+1]; ///< Matrix with the components of the resultant external torques (pure torque and couple produced by forces displacement in the body fixed frame) applied to each body.
    double *Qq;       ///< Array with the values of the joint forces.
    
    double tsim; ///< The time value.

    double t0;  ///< DO NOT USE: Initial time of the simulation [s] (for the user, not used by MBsysC).
    double tf;  ///< DO NOT USE: Final time of the simulation [s] (for the user, not used by MBsysC).
    double dt0; ///< DO NOT USE: Initial value of the integration step size [s] (for the user, not used by MBsysC).

    // Constraints
    double *lrod;   ///< Array with the length of each rod.
    int Nloopc;     ///< Number of loop constraints.
    int Ncons;      ///< Number of algebraic constraints.
    int Nuserc;     ///< Number of user constraints.
    double NRerr;   ///< Maximal error on constraint allowed in Newton-Raphson algorithm.
    double *lambda; ///< Array with the values of the Lagrange Multipliers related to the constraints.
	// Driven variables (and joint forces)
	double *Qc;     ///< Array with the value of joint force introduced in driven joint to respect the user function.
                    ///< The driven forces/torques are saved in the entries given by index vector, MbsData::qc 

    // Links
    int Nlink;  ///< Number of forces acting between two points of the system (force law implemented in user_LinkForces()).
    int Nlink3D;///< Number of 3D links.
    double *Z;  ///< Array with the current values of the distances between of the points of a link (see: user_LinkForces()).
    double *Zd; ///< Array with the current values of the speed (spreading) between of the points of a link (see: user_LinkForces()).
    double *Fl; ///< Array with the current values of the forces on each link (see: user_LinkForces()).
    
    double **l3DWr; // JFC : Attention: la convention des indices est inversée par rapport aux habitudes Robotran

    // Sensors
    int Nsensor; ///< Number of kinematic sensors.

    // Ext Forces
    int Nxfrc;   ///< Number of points where an external force is applied into a body.
    int *xfidpt; ///< Array of the points defined as force application points.
    double **SWr;///< Array of Swr vector for each external forces.
                 ///< Swr(9,1) = [Fx; Fy; Fz; Mx; My; Mz; dxF]:
                 ///< - Force components (expressed in the inertial frame) : Fx, Fy, Fz
                 ///< - Pure torque components (expressed in the inertial frame) : Mx, My, Mz
                 ///< - Application point local coordinates vector (expressed in the body-fixed frame): dxF(1:3,1)
                 ///<
                 ///< WARNING: Indexes convention is the opposite of what is usually done in MBsysC.

    // Wheel
    int Nwheel;   ///< DO NOT USE: Specific to some multibody system: Number of wheel.
    double *rnom; ///< DO NOT USE: Specific to some multibody system: Nominal radius of wheels.

#if !defined SENSORKIN
    // User Model
    int Nuser_model;       ///< Number of declared user models in MBsysPad.
    UserModel *user_model; ///< Structure containing all user model, this structure is specific for each project.

    int n_user_IO;
    UserIO *user_IO;
    UserIoInfo *__user_IO_info;

#endif

    // User State
    double *ux;  ///< Array with the values of the user variables.
    double *uxd; ///< Array with the values of the time derivatives of the user variables.
    double *ux0; ///< Array with the initial values of the user variables.
    int Nux;     ///< Number of user variable.

    // OTHER FIELDS //
    double *udd;   ///< For axelle red: array with the values of the acceleration of independent coordinate

    int DonePart;  ///< Flag that indicates if the coordinate partitioning module has been executed (default: 0=not done; 1=done).
    int DoneEquil; ///< Flag that indicates if the equilibrium module has been executed (default: 0=not done; 1=done).
    int DoneModal; ///< Flag that indicates if the modal module has been executed (default: 0=not done; 1=done).

    int process;   ///< Flag that indicate which module is currently running (1=partitioning, 2=equilibrium, 3=direct dynamic, 4=modal)
    int simu_end;  ///< DO NOT USE: Deprecated?
    int flag_stop; ///< DO NOT USE: stop the simulation (for the user, not used by MBsysC).
    
    char *mbs_filename; ///< Path to mbs file including the file with the extension (.mbs)
    char *build_name; ///< Path to the build folder

    #ifdef REAL_TIME
    void *realtime;  ///< Pointer to Simu_realtime structure.
    #endif

    #ifdef PRJ_FCT_PTR
    // reference to functions
    Functions fct;
    #endif

};


/*
 * Print values of mbs_data (used for debug)
 */
#ifdef __cplusplus
extern "C" {
#endif
    void mbs_print_data(MbsData *mbs_data);
	void print_vector(char* prefix, int n, double *vec);
	void print_intvector(char* prefix, int n, int *vec);
	void print_matrix(char* prefix, int r, int c, double **mat);
	void mbs_copy_data(MbsData* s, MbsData* s_copy);
#ifdef __cplusplus
}
#endif

/*--------------------*/
#endif
