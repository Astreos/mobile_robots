
/**
 * @file mbs_dirdyn_struct.h
 *
 * This header defines structure of the
 * dirdyn module in C.
 *
 *
 * Creation date: 10/09/2015
 * @author Nicolas Docquier, Timothée Habra
 *
 *
 * (c) Universite catholique de Louvain
 */

#ifndef MBS_DIRDYN_STRUCT_h
#define MBS_DIRDYN_STRUCT_h

#include "mbs_aux.h"
#include "mbs_buffer.h"

 /**
  * Dopri5 structure for dirdyn options
  */
typedef struct MbsDirdynOptDopri5
{
    int flag_use; ///< 1 to use dopri5 (adaptive time step), 0 to use runge kutta 4 (fixed time step)
    int flag_waypoint; ///< 1 to use waypoints, 0 otherwise
    int flag_solout_wp; ///< 1 to call solout only at required waypoints (only if flag_waypoint activated)
    int flag_stop_stiff; ///< 1 to stop integration if it become stiff, 0 (default value) otherwise

    int nmax;    ///< maximal number of stpes [-]
    int verbose; ///< 1 to get print indications related to time adaptive integrator, 0 otherwise
     
    double rtoler; ///< relative error tolerances [-]
    double atoler; ///< absolute error tolerances [-]
    double dt_max; ///< maximal time step [s]
    double delta_t_wp; ///< time interval between two waypoints [s]

} MbsDirdynOptDopri5;

 /**
  * Structure defining the option of a direct dynamic
  */
typedef struct MbsDirdynOptions
{
    /// initial time of the simulation, defaut = 0.0
    double t0;
    /// final time of the simulation, defaut = 5.0
    double tf;
    /// initial value of the integration step size, default = 0.001
    double dt0;
    /** Determine whether results are written to files on disk (in resultsR folder):
     * 1: results are saved
     * 0: results are not saved 
     */
    int save2file;
    /** The keyword used for determining the name of result files */
    const char* resfilename;
    /** Path in which result file are saved.
     *  Default: the resultsR folder of the project
     */
    const char* respath;

    /** Path in which anim file is saved.
     *  Default: the animationR folder of the project
     */
    const char* animpath;

    int save_anim; ///< 1 to save the anim file, 0 otherwise (only valid if 'save2file' is set to 1)

    int save_visu; ///< 1 to save the visualizazion file (as it appears in 'user_realtime_visu.c'), 0 otherwise (only valid if 'save2file' is set to 1)

    int framerate; ///< number of frame per second for the .anim file

    /** The number of time steps between two buffer records
    * default: 1 (every time step are recorded)
    */
    int saveperiod;

    /** The maximal number of user variables saved
    * default: 12
    */
    int max_save_user;
    
    /** The number of time step that can be recorded in the buffer.
     * Results are written to disk when the buffer is full.
     * default: -1, compute the buffer size for saving results only once at the end according to dt0, t0 and tf.
     */
    int buffersize;

    int realtime; ///< 1 to activate to real-time features, 0 to deactivate them

    int accelred; ///< 1 to use accelred, 0 otherwise

    MbsDirdynOptDopri5 *dopri5; ///< Dopri5 structure for dirdyn options  

} MbsDirdynOptions;

 /**
  * Dopri5 structure for dirdyn
  */
typedef struct MbsDirdynDopri5
{
    int flag_save; ///< 1 if first dydt save done, 0 otherwise

    double solout_last_t; ///< last time solout was called

    double *dydt_save; ///< save dydt of the last time step

    double **alloc_tab; ///< allocated state vectors

} MbsDirdynDopri5;

 /**
  * Runge-Kutta 4 structure for dirdyn
  */
typedef struct MbsDirdynRK4
{
    double *yt; ///< allocated state vector

    double *dyt; ///< allocated derivative vector

    double *dym; ///< allocated saved derivative vector

} MbsDirdynRK4;

 /**
  * General structure of the direct dynamic module
  */
struct MbsDirdyn
{
    MbsDirdynOptions* options; ///< structure defining the option of a direct dynamic
    MbsAux* mbs_aux;  
    
    double tsim; ///< current simulation time
    double dt; ///< current integration step size
    
    int nState;      // size of the state vector
    
    /** pointers to the state vector */
    double *y;
    /** pointers to the output vector (???) */
    double *yout;
    /** pointers to the derivative vector */
    double *yd;  

    /** buffer to store generalized coordinates, velocities, acceleration, joint forces... */
    MbsBuffer **buffers;

    /** buffer used to save the visualization as it appears in 'user_realtime_visu.c' */
    MbsBuffer **buffer_visu;

    /** growing buffer to store user values */
    MbsGrowingBuffer *user_buffer;

    int bufferNb;
    /** pointer to the array that must be saved to buffers */
    double** savedArrays;
    
    /** counter for checking wheter results must be saved */
    int savePeriodCounter;

    MbsDirdynRK4 *rk4; ///< Runge-Kutta 4 structure for dirdyn
    MbsDirdynDopri5 *dopri5; ///< Dopri5 structure for dirdyn

};


#endif
