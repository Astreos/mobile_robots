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
// 01/10/2008 : JFC : Bug n°38
//

#ifndef MBSsensorStruct_h
#define MBSsensorStruct_h
/*--------------------*/

typedef struct MbsSensor
{
        double P[4];    ///< Position vector of the sensor expressed in the inertial frame: \f$P(1:3)=[P_x; P_y; P_z]\f$.
        double R[4][4]; ///< Rotation matrix from the inertial frame to the sensor frame: \f$[\hat{\mathbf{X}}^S]=R(1:3,1:3).[\hat{\mathbf{X}}^0]\f$
        double V[4];    ///< Velocity vector of the sensor expressed in the inertial frame: \f$V(1:3)=[V_x; V_y; V_z]\f$.
        double OM[4];   ///< Angular velocity vector of the sensor expressed in the inertial frame: \f$OM(1:3)=[\omega_x; \omega_y; \omega_z]\f$.
        double A[4];    ///< Acceleration vector of the sensor expressed in the inertial frame: \f$A(1:3)=[A_x; A_y; A_z]\f$.
        double OMP[4];  ///< Angular acceleration vector of the sensor expressed in the inertial frame: \f$OMP(1:3)=[\dot\omega_x; \dot\omega_y; \dot\omega_z]\f$.
        double *J[7];   ///< Jacobian matrix of the sensor: \f$J(1:njoint,1:6)=\begin{bmatrix}\frac{\partial V}{\partial \dot q^T} \\ \frac{\partial OM}{\partial \dot q^T}\end{bmatrix}\f$
        
/**/
} MbsSensor;

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * \brief Allocate the Jacobian matrix of the MbsSensor according to the number of joints in the multibody system.
 * \param[in,out] psens Pointer to the MbsSensor structure to modify.
 * \param[in] njoint Number of joint in the MBS, see MbsData::njoint.
 */
void allocate_sensor(MbsSensor *psens, int njoint);

/*!
 * \brief Initialize all fields of MbsSensor structure to 0.
 * \param[in,out] psens Pointer to the MbsSensor structure to initialize.
 * \param[in] njoint Number of joint in the MBS, see MbsData::njoint.
 */
void init_sensor(MbsSensor *psens, int njoint);

/*!
 * \brief Free the memory of MbsSensor structure.
 * \param[in,out] psens Pointer to the MbsSensor structure to be freed.
 */
void free_sensor(MbsSensor *psens);

#ifdef __cplusplus
}
#endif

#endif
