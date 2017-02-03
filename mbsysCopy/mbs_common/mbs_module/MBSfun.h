//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef MBSfun_h
#define MBSfun_h
/*--------------------*/

 
#include "mbs_data.h"
#include "mbs_aux.h"
#include "mbs_sensor.h"



//void checkMBSdata(MbsData *MBSdata);

//void storeMBSdata(MbsData *s, char *s_name);
					  
/* dirdynared */

#ifdef DIRDYNARED
int dirdynared(MbsAux *mbs_aux,MbsData *s);
#endif

/* invdynared */

#ifdef INVDYNARED
int invdynared(MbsAux *mbs_aux, MbsData *s);
#endif

/**
* Compute the reduced Phi vector of size nquc=nqu+nqc for qu and qc variables (Robotran Basics Eq(7,20))
* necessary for 
*		- equilibrium (while acceleration are set to 0 beforehand)
*		- linearization of MBS system: get the tangent matrices (for control or modal analysis) 
*
* \p Fruc		the force vector in which the force must be returned
* \p aux		the local data structure
* \p s		the MbsData of the system
*/
int mbs_calc_Fruc(double Fruc[], MbsAux *aux, MbsData *s);

/*! \brief compute a position of the multibody system which solves the constraints
 *
 * The algorithm is a Newton/Raphson procedure which solves the equation: \f$ v^{k+1} = v^{k}-\left(J_{v}\right)^{-1} h\f$.\n
 * Robotran Basics Eq(17), chapter 3.2.1, pp. 12
 *
 * \param[in,out] s the MbsData structure.
 * \param[in,out] mbs_aux the MbsAux structure related to the MbsData structure.
 *
 * \return the number of iterations needed to close the system
 */
int mbs_close_geo(MbsData *s, MbsAux *mbs_aux);

/*! \brief compute the dependent velocities that solves the constraints.
 *
 * The function solves the linear equation: \f$ \dot{v} = \textbf{B}_{vu} \dot{u} \f$.\n
 * Robotran Basics Eq(18), chapter 3.2.1, pp. 12
 *
 * \param[in,out] s the MbsData structure.
 * \param[in,out] mbs_aux the MbsAux structure related to the MbsData structure.
 */
void mbs_close_kin(MbsData *s, MbsAux *mbs_aux);

/*! \brief compute the current value of the constraints (\f$h(q)\f$) and the constraint Jacobian matrix (\f$\textbf{J}=\frac{\partial h(q)}{\partial q^t}\f$).
 *
 * \param[in,out] s the MbsData structure.
 * \param[in,out] mbs_aux the MbsAux structure related to the MbsData structure.
 */
void mbs_calc_hJ(MbsData *s, MbsAux *mbs_aux);

/*! \brief compute the quadratic term of the constraints at acceleration level: \f$ \dot{\textbf{J}}\dot{q}(q,\dot{q}) \f$
 *
 * \param[in,out] s the MbsData structure.
 * \param[in,out] mbs_aux the MbsAux structure related to the MbsData structure.
 */
void mbs_calc_jdqd(MbsData *s, MbsAux *mbs_aux);

/*! \brief compute the force and torques applied on the multibody system.
 *
 * Set the matrices MbsData::frc and MbsData::trq at zero, then compute the contribution of:
 *  - Links forces
 *  - Links 3D forces
 *  - External forces
 *  - Joints forces
 *
 * \param[in,out] s the MbsData structure.
 */
void mbs_calc_force(MbsData *s);
					  
/*--------------------*/
#endif
