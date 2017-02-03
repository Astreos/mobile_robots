 
#include <stdio.h>
//#include "mbs_xml_reader.h"
#include "mbs_part.h"
#include "mbs_dirdyn.h"
#include "mbs_buffer.h"
#include "mbs_data.h"
#include "mbs_aux.h"



/**
 * Structure defining the property of a node, i.e. a
 * body of the mbs that is able to bear one or several
 * LMGC90 contactor.
 * It is mainly defined by a sensor in the Robotran formalism.
 * 
 */
typedef struct MbsLmgcNode{
  
  /** id of the sensor in the mbs tree */
  int sensorId;
  /** sensor structure for this node */
  MbsSensor* sensor;
  
  /** reduced jacobian of the sensor. WARNING: indices start from 0 !*/
  double *JRed[6];
  
} MbsLmgcNode;


/**
 * Structure defining one multibody system and additional
 * stuff for coupling it to LMGC90
 * 
 * 
 */
typedef struct MbsLmgcData{
  
  /** Number of nodes in this system */
  int nb_nodes;
  
  /** Pointer to the MbsData structure of the multibody system */
  MbsData* mbs_data;
  
  /** Flag indicate if mbs_data was loaded or if a pointer to the memory was provided (-1: no data / 0: pointer to memory / 1: loaded)*/
  int mbs_data_loaded;
  
  /** Utility structure for keeping intermediate variable during computation */
  MbsAux* lds;
  
  /** Array of nodes associated to this mbs */
  MbsLmgcNode* nodes;
  
  /** Array of the generalized position in the contact configuration */
  double* qm;

  /** Array of the generalized free velocities (velocities without contact) */
  double* vFree;

  /** Array of the generalized velocities at the beginning of the time step */
  double* vBeg;

  /** Array of auxiliary generalized velocities (for computing W matrix) */
  double* vAux;
  
  /** Array of generalized reaction */
  double* reac;
  
  /** Array of auxiliary generalized reaction (for computing W matrix) */
  double* rAux;
  

  /** String defining the file from which to load the mbs data */
  char* mbsFile;

  /** String defining the file in which to save coordinate results */
  char* resqFile;

  /** String defining the file in which to save velocity results */
  char* resqdFile;

  /** String defining the file in which to save velocity results */
  char* resQqFile;
  
  /** String defining the file in which to save animation result */
  char* animFile;
  

  /** buffer to store generalized coordinates, velocities, acceleration, joint forces... */
  MbsBuffer **buffers;

  /** pointer to the array that must be saved to buffers */
  double** savedArrays;

  /** Number of buffer */
  int bufferNb;

  /** NOT IMPLEMENTED: The number of time steps between two buffer records; default: 1 (every time step are recorded). */
  int saveperiod;

  /** The number of time step that can be recorded in the buffer.
     * Results are written to disk when the buffer is full.
     * default: -1, compute the buffer size for saving results only once at the end according to dt0, t0 and tf.
     */
  int buffersize;

} MbsLmgcData;

/**
 * Global data structure for a Robotran-LMGC90 project
 * 
 * 
 */
typedef struct MbsLmgcSystem{
  
  /** number of mbs */
  int nb_mbs;
  

  /** array containing the "extended MbsData" structures of all mbs */
  MbsLmgcData* mbs_lmgc_s;
  
}MbsLmgcSystem;




/**
 * Global pointer to the Robotran-LMGC90 system
 */
extern MbsLmgcSystem* globalSystem;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                   PRE-INITIALIZATION FUNCTIONS                      */ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**
 * Set the number of mbs
 * 
 * This operation can be run only once in the process
 * \param[in] nb the number of multibody system in the simulation
 */
void mbs_lmgc_set_nb_mbs(int nb);

/**
 * Set mbs_data (already loaded in memory) for the specified mbs.
 * \param[in] iMbs the ID of the Multibody system associated to the mbs_data.\n
                   WARNING: ID's start from 0 !
 * \param[in] mbs_data the pointer to the multibody system data structure to use
 */ 
void mbs_lmgc_set_mbsdata(int iMbs, MbsData* mbs_data);

/**
 * Set the path to the mbs file for the specified mbs.
 * \param[in] iMbs the ID of the Multibody system.\n
                   WARNING: ID's start from 0 !
 * \param[in] filename the path to the mbs file including the mbs file name and extension (.mbs)
 */ 
void mbs_lmgc_set_mbs_file(int iMbs, char* filename);

/**
 * Set the path to the result files for the specified mbs.
 * It specify the path + the name without extension.
 * A suffix _q.res will be added for the position result file 
 * A suffix _qd.res will be added for the velocity result file 
 * \param[in] iMbs the ID of the Multibody system.\n
                   WARNING: ID's start from 0 !
 * \param[in] filename the path to the folder where results have to be saved including the prefix of the results files
 */  
void mbs_lmgc_set_res_file(int iMbs, char* filename);

/**
 * Set the path to the anim file for the specified mbs.
 * \param[in] iMbs the ID of the Multibody system.\n
                   WARNING: ID's start from 0 !
 * \param[in] filename the path to the folder where results have to be saved including the prefix of the results files
 */ 
void mbs_lmgc_set_anim_file(int iMbs, char* filename);

/**
 * Set the number of nodes for the specified mbs and
 * set the index of sensors on which the nodes are attached.
 * the size of sensorIds must be equal to nbNodes.
 * \param[in] iMbs the ID of the Multibody system.\n
                   WARNING: ID's start from 0 !
 * \param[in] nbNodes the number of nodes attached to the multibody system.
 * \param[in] sensorIds pointer to the lists containing the sensors id's.
 */ 
void mbs_lmgc_set_node_sensor_ids(int iMbs, int nbNodes, int* sensorIds);

/**
 * Get the number of nodes attached to a mbs.
 * \param[in] iMbs the ID of the Multibody system associated to the mbs_data.\n
                   WARNING: ID's start from 0 !
 * \return the number of nodes attached on the specified mbs.
 */ 
int mbs_lmgc_get_node_nb(int iMbs);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                         BINDING FUNCTIONS                           */ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**
 * Initialize the system
 */
void mbs_lmgc_initialize();

/**
 * \brief Save final configuration, free the memory and finalize the process
 * \details If the MbsData structure was provided by function mbs_lmgc_set_mbsdata() , the structure is not freed.
 */
void mbs_lmgc_finalize();


/**
 * \brief Increment time step.
 * \details Set the time at the beginning of the next iteration in the time simulation loop and save the system configuration.
 * 
 * @param tsim the new value of time at the begining of the time step
 */
void mbs_lmgc_increment(double tsim);

/**
 * \brief Compute the free velocity of all mbs.
 * \details After this function:
 *   - qm is set to the new contact configuration
 *   - acceleration (MbsData->qdd) are computed with tsim (begin of time step), qm (contact configuration) and qd (value of begin of time step)
 *   - vFree is computed for the current step
 * 
 * \param[in] h the size of the time step
 * \param[in] theta the value of the theta parameter in the theta scheme
 */
void mbs_lmgc_compute_free_vlocy(double h, double theta);

/**
 * \brief Update the kinematics of 3D nodes belonging to the specified mbs.
 * \details For each sensor attached to a nodes of the mbs it:
 *  - updates the kinematics (q, qd, qdd, rotation matrix, jacobian matrix); 
 *  - computes the reduced jacobian of the sensor:\n
 *     \f$ J_{(sens,red)} = J{uc} + J{v}*Bv_{uc} \f$
 * 
 * Current value of q, Jv and \f$ Bv_{uc} \f$ are used (i.e. value stored
 * in MbsData and MbsAux)
 * 
 * @param i_mbs the index of the mbs for which the nodes must be updated
 * @param coorTT the array in which the position of nodes must be copied:\n
          \f$ x_1, y_1, z_1, x_2, y_2, z_2, ... x_i, y_i, z_i \f$ being the coordinate of all nodes of the mbs
 * @param localFrameTT the array in which the rotation matrix of nodes
 *        must be copied
 */
void mbs_lmgc_update_nodes_3D(int i_mbs, double* coorTT, double* localFrameTT);

/**
 * \brief Update the kinematics of 2D nodes belonging to the specified mbs.
 * \details For each sensor attached to a nodes of the mbs it:
 *  - updates the kinematics (q, qd, qdd, rotation matrix, jacobian matrix); 
 *  - computes the reduced jacobian of the sensor:\n
 *     \f$ J_{(sens,red)} = J{uc} + J{v}*Bv_{uc} \f$
 *
 * The only axis considered in translation are X and Y; the only rotation is around Z axis.\n
 * Current value of q, Jv and \f$ Bv_{uc} \f$ are used (i.e. value stored
 * in MbsData and MbsAux)
 * 
 * @param i_mbs the index of the mbs for which the nodes must be updated
 * @param coorTT the array in which the position of nodes must be copied:\n
          \f$ x_1, y_1, x_2, y_2, ... x_i, y_i \f$ being the coordinate of all nodes of the mbs
 */
void mbs_lmgc_update_nodes_2D(int i_mbs, double* coorTT);

/**
 * \brief Reset the global reaction to zero for the specified mbs.
 * \details The vector to be reset corresponds to flag iReac_ of LMGC90.\n
 *         The corresponding array is reac stored in MbsLmgcData. 
 * 
 * @param i_mbs the index of the mbs for which the reaction must be reset
 */
void mbs_lmgc_nullify_reac(int i_mbs);

/**
 * \brief Reset the auxiliary reaction to zero for the specified mbs.
 * \details The vector to be reset corresponds to flag iRaux_ of LMGC90.\n
 *         The corresponding array is rAux stored in MbsLmgcData.
 * 
 * @param i_mbs the index of the mbs for which the reaction must be reset
 */
void mbs_lmgc_nullify_rAux(int i_mbs);


/**
 * \brief Add the given node reaction contribution to the reaction of the global mbs.
 * \details The given reaction is thus projected on the generalized coordinates space:
 *    - \f$ reac += J_{(sens,red)}^{T} * reacNode \f$
 *    - reac is stored in MbsLmgcData.
 *    - current value of \f$ J_{(sens,red)} \f$ is used (i.e. value stored MbsLmgcData -> MbsLmgcNode)
 * 
 * The array on which it is added corresponds to LMGC90 flag iReac_ .
 * 
 * @param i_mbs the index of the mbs for which the reaction must be applied
 * @param i_node the index of the node on which the reaction is applied
 * @param reacNode a 6 element (fx, fy, fz, tx, ty, tz) array
 *                 containing the components of the force and 
 *                 torque to apply on the node.\n
                   Components are given in the global frame.
 */
void mbs_lmgc_add_reac(int i_mbs, int i_node, double* reacNode);

/**
 * \brief Add the given node reaction to the reaction of the global mbs.
 * \details The given reaction is thus projected on the generalized coordinates space:
 *    - \f$ raux += J_{(sens,red)}^{T} * reacNode \f$
 *    - rAux is stored in MbsLmgcData.
 *    - current value of \f$ J_{(sens,red)} \f$ is used (i.e. value stored MbsLmgcData -> MbsLmgcNode)
 * 
 * The array on which it is added corresponds to LMGC90 flag iRaux_ .
 * 
 * @param i_mbs the index of the mbs for which the reaction must be applied
 * @param i_node the index of the node on which the reaction is applied
 * @param reacNode a 6 element (fx, fy, fz, tx, ty, tz) array
 *        containing the components of the force and torque
 *        to apply on the node.\n
 *        Components are given in the global frame.
 */
void mbs_lmgc_add_rAux(int i_mbs, int i_node, double* reacNode);


/**
 * \brief Reset the generalized velocity to zero for the specified mbs.
 * \details The array to be reset corresponds to the LMGC90 flag iVfree. 
 *          The corresponding array is vFree stored in MbsLmgcData.
 * 
 * @param i_mbs the index of the mbs for which the velocity must be reset
 */
void mbs_lmgc_nullify_vFree(int i_mbs);

/**
 * \brief Reset the generalized velocity to zero for the specified mbs.
 * \details The array to be reset corresponds to the LMGC90 flag iVaux_.
 *          The corresponding array is vAux stored in MbsLmgcData.
 * 
 * @param i_mbs the index of the mbs for which the velocity must be reset
 */
void mbs_lmgc_nullify_vAux(int i_mbs);

/**
 * \brief Compute the generalized velocity (vAux), 
 * \details It solve \f$ M_{red} * vAux = rAux \f$ \n
            The array for vAux and rAux corresponds to the LMGC90 flag iVaux_e_invM_t_Raux_
 * 
 * WARNING: this function assumes that the reduced mass matrix \f$ M_{red} \f$
 * stored in the MbsAux structure (lds field of the MbsLmgcData of the specified mbs) has already 
 * been computed.\n
   vAux will be computed simply by calling cholsl.
 * 
 * @param i_mbs the index of the mbs for which the velocity must be reset
 * 
 */
void mbs_lmgc_comp_vAux(int i_mbs);

/**
 * \brief Compute and set the velocity of a node at the beginning of the current time step.
 * \details Compute the velocity of a node (linear and angular velocity) from
 * a given value of the generalized velocities.\n
   This is computed by \f$ vlocyNode = J_{(sens,red)} * vBeg_{uc} \f$ 
 * 
 * The array from which the velocity corresponds to the LMGC90 flag iVbeg_ \n
   The corresponding array is vBeg stored in MbsLmgcData.
 * 
 * @param i_mbs the index of the mbs from which the velocity is taken
 * @param i_node the index of the node for which the velocity is computed
 * @param vlocyNode a 6 element (vx, vy, vz, omx, omy, omz) array
 *        in which the components of the velocity will be stored.\n
          Components are computed and expressed in the global frame.
 */
void mbs_lmgc_get_vBeg(int i_mbs, int i_node, double* vlocyNode);

/**
 * \brief Compute and set the free velocity of a node for the current time step.
 * \details Compute the velocity of a node (linear and angular velocity) from
 * a given value of the generalized velocities.\n
   This is computed by \f$ vlocyNode = J_{(sens,red)} * vBeg_{uc} \f$
 * 
 * The array from which the velocity corresponds to the LMGC90 flag iVfree.\n
   The corresponding array is vFree stored in MbsLmgcData.
 * 
 * @param i_mbs the index of the mbs from which the velocity is taken
 * @param i_node the index of the node for which the velocity is computed
 * @param vlocyNode a 6 element (vx, vy, vz, omx, omy, omz) array
 *        in which the components of the velocity will be stored.\n
          Components are computed and expressed in the global frame.
 */
void mbs_lmgc_get_vFree(int i_mbs, int i_node, double* vlocyNode);

/**
 * \brief Compute and set the auxiliary velocity of a node for the current time step.
 * \details Compute the velocity of a node (linear and angular velocity) from
 * a given value of the generalized velocities.\n
   This is computed by \f$ vlocyNode = J_{(sens,red)} * vAux_{uc} \f$
 * 
 * The array from which the velocity corresponds to the LMGC90 flag iVaux_. \n
    The corresponding array is vAux stored in MbsLmgcData.
 * 
 * @param i_mbs the index of the mbs from which the velocity is taken
 * @param i_node the index of the node for which the velocity is computed
 * @param vlocyNode a 6 element (vx, vy, vz, omx, omy, omz) array
 *        in which the components of the velocity will be stored.\n
          Components are computed and expressed in the global frame.
 */
void mbs_lmgc_get_vAux(int i_mbs, int i_node, double* vlocyNode);

/**
 * \brief Compute the new value of the generalized velocity and position for all mbs
 * \details The following operation are done:
 *   - Compute vAux by solving : \f$ M_{red} * vAux = rAux \f$
 *   - Update the independent velocity with : \f$ qd_{u} = vFree + vAux \f$
 *   - Update the independent velocity with : \f$ q_{u} = qm_{u} + h * \theta * qd_{u} \f$
 *   - Update driven joint position, velocity and acceleration at the value at the end of the time step
 *   - Solve constraints (dependent joints) in position and velocity.
 *   - Update tsim at the end of step (coherence with coordinate value in memory)
 * 
 * Precondition:
 *   - reac contains the new value of reactions
 * 
 * WARNING:
 *   - This function modify the vAux array of each system
 * 
 * @param h the size of the time step
 * @param theta the value of the theta parameter in the theta scheme
 */
void mbs_lmgc_compute_dof(double h, double theta);


/**
 * \brief Finalize time step.
 * \details Update the vBeg value for next time step.
 */
void mbs_lmgc_update_dof();
