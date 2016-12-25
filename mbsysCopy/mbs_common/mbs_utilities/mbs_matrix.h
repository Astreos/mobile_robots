//---------------------------
// UCL-CEREM-MBS
//
// @version MBsysLab_s 1.7.a
//
// Creation : 2006
// Last update : 01/10/2008
//---------------------------

#ifndef mbs_matrix_h
#define mbs_matrix_h
/*--------------------*/
/*! \brief Compute the transpose of a matrix with first index is 1.
 * 
 * The input and output matrices have unused index 0. See get_dmat_1() to such matrices creation.
 *
 * \param[in] M[4][4] matrix with first line and row unused.
 * \param[in,out] Mt[4][4] the transpose of M, index starting at 1.
 */
void transpose(double M[4][4], double Mt[4][4]);

/*! \brief Compute the Euclidean norm of a double vector with first index is 1.
 * 
 * The vector has unused index 0. See get_dvec_1() to such vector creation.
 *
 * \param[in] v[4] vector of size 4 with first element unused.
 *
 * \return The norm of the vector v.
 */
double norm(double v[4]);

/*! \brief Compute the normalize vector with first index is 1.
 * 
 * The vector has unused index 0. See get_dvec_1() to such vector creation.\n
 * WARNING: if norm(v) is 0, a division by 0 will occurs.
 *
 * \param[in] v[4] vector of size 4 with first element unused.
 * \param[in,out] vn[4] the normalized vector v, index starting at 1.
 */
void normalize(double v[4], double vn[4]);

/*! \brief Compute and return the scalar product of 2 vectors with first index is 1.
 * 
 * The vectors have unused index 0. See get_dvec_1() to such vector creation.
 *
 * \param[in] v1[4] vector of size 4 with first element unused.
 * \param[in] v2[4] vector of size 4 with first element unused.
 *
 * \return The scalar product between v1 and v2
 */
double scalar_product(double v1[4], double v2[4]);

/*! \brief Sum each component of 2 vectors in a third vector. The first index is 1.
 * 
 * The vectors have unused index 0. See get_dvec_1() to such vector creation.
 *
 * \param[in] v1[4] vector of size 4 with first element unused.
 * \param[in] v2[4] vector of size 4 with first element unused.
 * \param[in,out] v[4] vector such as \f$ v_{i} = v1_{i} + v2_{i} \f$
 */
void vector_sum(double v1[4], double v2[4], double v[4]);

/*! \brief Subtract each component of 2 vectors in a third vector. The first index is 1.
 * 
 * The vectors have unused index 0. See get_dvec_1() to such vector creation.
 *
 * \param[in] v1[4] vector of size 4 with first element unused.
 * \param[in] v2[4] vector of size 4 with first element unused.
 * \param[in,out] v[4] vector such as \f$ v_{i} = v1_{i} - v2_{i} \f$ with first element unused.
 */
void vector_diff(double v1[4], double v2[4], double v[4]);

/*! \brief Compute the cross product of 2 vectors in a third vector. The first index is 1.
 * 
 * The vectors have unused index 0. See get_dvec_1() to such vector creation.
 *
 * \param[in] v1[4] vector of size 4 with first element unused.
 * \param[in] v2[4] vector of size 4 with first element unused.
 * \param[in,out] v[4] vector such as \f$ \overrightarrow{v} = \overrightarrow{v1} \times \overrightarrow{v2} \f$ with first element unused.
 */
void cross_product(double v1[4], double v2[4], double v[4]);

/*! \brief Compute the product of a matrix by a vector. The first index is 1.
 * 
 * The matrices and vector have unused index 0. See get_dmat_1() and get_dvec_1() to such matrix and vector creation.
 *
 * \param[in] M[4][4] matrix of size 4x4 with first line and row unused, will be multiplied by v.
 * \param[in] v[4] vector of size 4 with first element unused.
 * \param[in,out] Mv[4][4] matrix such as \f$ \textbf{Mv} = \textbf{M}  \overrightarrow{v} \f$ with first line and row unused.
 */
void matrix_product(double M[4][4], double v[4], double Mv[4]);

/*! \brief Compute the product of two matrices. The first index is 1.
 * 
 * The matrices have unused index 0. See get_dmat_1() to such matrix and vector creation.
 *
 * \param[in] M1[4][4] matrix of size 4x4 with first line and row unused.
 * \param[in] M2[4][4] matrix of size 4x4 with first line and row unused.
 * \param[in,out] M[4][4] matrix such as \f$ \textbf{M} = \textbf{M1} \ \textbf{M2} \f$ with first line and row unused.
 */
void doublematrix_product(double M1[4][4], double M2[4][4], double M[4][4]);

/*! \brief Compute the rotation matrix for a specified angle around a specific axis. The first index is 1.
 *
 * \param[in] type integer for axis selection: \f$1=\hat{X}\f$ ; \f$2=\hat{Y}\f$ ; \f$3=\hat{Z}\f$.
 * \param[in] angle value of the rotation angle expressed in radian.
 * \param[in,out] R[4][4] corresponding rotation matrix with first line and row unused.
 */
void rotation_matrix(int type, double angle, double R[4][4]);

/*! \brief Compute the product of a vector by a real. The first index is 1.
 * 
 * The vector has unused index 0. See get_dvec_1() to such vector creation.
 *
 * \param[in] s real that will multiply the vector.
 * \param[in] v1[4] vector to be multiplied. The first index is 1.
 * \param[in,out] v[4] vector such as \f$ v_{i}=v1_{i}s \f$. The first index is 1.
 */
void number_product(double s, double v1[4], double v[4]);



/*--------------------*/
#endif
