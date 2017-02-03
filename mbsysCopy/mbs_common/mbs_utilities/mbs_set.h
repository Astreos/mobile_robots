/**
 * @file mbs_set.h
 *
 *
 * Creation date: 11/06/2015
 * @author Olivier Lantsoght (based on the work of other from CEREM)
 *
 *
 * (c) Universite catholique de Louvain
 */

#ifndef MBS_SET_h
#define MBS_SET_h

#include "mbs_data.h"

#ifdef __cplusplus
extern "C" {
#endif
     /**
      * Set the number of user constraints and update the related structures
      *
      * @param[in,out] mbs_data MbsData structure to modify
      * @param[in] Nuserc number of user constraints
      *
      * This function must be called between mbs_load() and mbs_new_part() in 'main.c'
      */
    void mbs_set_nb_userc(MbsData *mbs_data, int Nuserc);

    /*! \brief set a variable to independent partition
     * 
     * \param[in,out] mbs_data MbsData structure to modify
     * \param[in] qu variable to set to independent
     *
     * This function must be called between mbs_load() and mbs_new_part() in 'main.c'
     */
    void mbs_set_qu(MbsData *mbs_data, int qu);
    
    /*! \brief set a variable to dependent partition
     * 
     * \param[in,out] mbs_data MbsData structure to modify
     * \param[in] qv variable to set to dependent
     *
     * This function must be called between mbs_load() and mbs_new_part() in 'main.c'
     */
    void mbs_set_qv(MbsData *mbs_data, int qv);
    
    /*! \brief set a variable to driven partition
     * 
     * \param[in,out] mbs_data MbsData structure to modify
     * \param[in] qdriven variable to set to driven
     *
     * This function must be called between mbs_load() and mbs_new_part() in 'main.c'
     */
    void mbs_set_qdriven(MbsData *mbs_data, int qdriven);
    
    /*! \brief set a variable to locked partition
     * 
     * \param[in,out] mbs_data MbsData structure to modify
     * \param[in] qlocked variable to set to locked
     *
     * This function must be called between mbs_load() and mbs_new_part() in 'main.c'
     */
    void mbs_set_qlocked(MbsData *mbs_data, int qlocked);

    /*! \brief print the vectors qu, qv, qc, qdriven and qlocked
     *
     * \param[in] mbs_data MbsData structure to modify
     */
    void print_mbs_q_all(MbsData *mbs_data);
    
    /*! \brief print a vector of indexes with first elem in brackets
     *
     * \param[in] q_vec vector to print
     * \param[in] nq size of the vector - 1
     */
    void print_mbs_q_vec(int *q_vec, int nq);
#ifdef __cplusplus
}
#endif

/*! \brief add a new index in a q vector
 * It does not modify the vector and vector size if the index is already in the q vector.
 * 
 * \param[in,out] q_vec original q vector, have to be initialized at size njoint
 * \param[in] nq number of elements in the original vector
 * \param[in] new_q new index to add
 * 
 * \return the number of elements in the updated vector
 */
int add_mbs_q_elem(int *q_vec, int nq, int new_q);

/*! \brief remove an old index in a q vector
 * It does not modify the vector and vector size if the index is not in the q vector.
 *
 * \param[in,out] q_vec original q vector, have to be initialized at size njoint
 * \param[in] nq number of elements in the original vector
 * \param[in] old_q old index to remove
 * 
 * \return the number of elements in the updated vector
 */
int remove_mbs_q_elem(int *q_vec, int nq, int old_q);

/*! \brief DEPRECATED; do not use: update the nq variables, SHOULD not be used
 * The function still works, however the number of joint of a specific nature should have been updated at the same time as the nature of the joint was changed. 
 *
 * \param[in,out] mbs_data MbsData structure to modify
 * \param[in] new_nqu the new number of independent joints
 * \param[in] new_nqv the new number of dependent joints
 * \param[in] new_nqc the new number of driven and locked joints
 * \param[in] new_nqdriven the new number of driven joint
 * \param[in] new_nqlocked the new number of locked joint
 */
void udpate_nq(MbsData *mbs_data, int new_nqu, int new_nqv, int new_nqc, int new_nqdriven, int new_nqlocked);

#endif
