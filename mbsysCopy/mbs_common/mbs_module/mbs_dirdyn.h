/**
 * @file mbs_dirdyn.h
 *
 * This header defines functions of the
 * dirdyn module in C.
 *
 *
 * Creation date: 19/11/2014
 * @author Nicolas Docquier (based on the work of other from CEREM: nvdn, av, ...)
 *
 *
 * (c) Universite catholique de Louvain
 */

#ifndef MBS_DIRDYN_h
#define MBS_DIRDYN_h

#include "mbs_dirdyn_struct.h"
#include "mbs_data.h"

#include "mbsysc_module_export.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
  * Create a new direct dynamic structure.
  * This function also initialize the options structures and
  * the MbsAux for storing computational info during simulation.
  *
  * \param mbs_data the data structure of the model for which
  *             the time integration will be computed
  */
 
MBSYSC_MODULE_EXPORT MbsDirdyn* mbs_new_dirdyn(MbsData* mbs_data);

 /**
  * Create a new direct dynamic structure.
  * This function also initialize the options structures.
  * A pointer to the given MbsAux structure is kept by the
  * returned structure.
  *
  * \p mbs_data the data structure of the model for which
  *             the time integration will be computed
  */
MbsDirdyn* mbs_new_dirdyn_aux(MbsData* mbs_data, MbsAux* mbs_aux);

 /**
  * Run the direct dynamic simulation
  *
  * The MbsData structure associated to dirdyn is modified 
  *
  * @param[in,out] dirdyn the MbsDirdyn to be run
  * @param[in,out] mbs_data the MbsData structure of the model for which the direct dynamic is computed
  */

MBSYSC_MODULE_EXPORT void mbs_run_dirdyn(MbsDirdyn* dirdyn, MbsData* mbs_data);


 /**
  * Run the startup operations of the run_dirdyn function
  * 
  */
void mbs_dirdyn_init(MbsDirdyn* dirdyn, MbsData* mbs_data);


 /**
  * Time loop of the run_dirdyn function:
  *  - call the time integrator
  *  - call the discrete state functions
  *  - call the storing function
  */ 
void mbs_dirdyn_loop(MbsDirdyn* dirdyn, MbsData* mbs_data);


 /**
  * Run the finalization operations of the run_dirdyn function
  */ 
void mbs_dirdyn_finish(MbsDirdyn* dirdyn, MbsData* mbs_data);


 /**
  * Free memory of the direct dynamic structure
  * The options (MbsDirdynOptions) and MbsAux structures are also freed
  */
 
MBSYSC_MODULE_EXPORT void mbs_delete_dirdyn(MbsDirdyn* dirdyn, MbsData* mbs_data);

 /**
  * Compute the derivatives of the system
  * (1st derivatives to be given to the ODE integrator)
  *
  * \p t    the integration variable (time in the case of MBS time integration)
  * \p y    the state vector for which the derivative must be computed
  * \p dydt the state derivative vector in which the derivative must be returned
  * \p s    the MbsData of the system
  * \p mbs_aux  the local data structure
  */
#ifdef __cplusplus
}
#endif

 /**
  * Write the content of buffer associated to the given MbsDirdyn to disk.
  * This is usefull for writing simulation results at the end of the simulation.
  * This sepcific function is written to be able to ask for writing buffer from
  * python.
  * 
  *  \p dd the MbsDirdyn struct for which the buffers must be written
  */

void mbs_dirdyn_write_buffers(MbsDirdyn* dd);

void mbs_fct_dirdyn(double t, double y[], double dydt[], MbsData *s, MbsDirdyn *dd);
void save_realtime_update(MbsDirdyn* dd, MbsData* mbs_data);
double loop_dopri5(double cur_t0, double cur_tf, double dt0, double **dopri5_alloc_tab, MbsData *s, MbsDirdyn *dd, FILE *fileout_dopri5);
void fcn_dopri5(unsigned n, long nr, double tsim, double y[], double dydt[], MbsData *s, MbsDirdyn *dd);
void solout_dopri5(long nr, double tsim_old, double tsim, double y[], unsigned n, int* irtrn, int init_flag, MbsData *s, MbsDirdyn *dd);

#endif
