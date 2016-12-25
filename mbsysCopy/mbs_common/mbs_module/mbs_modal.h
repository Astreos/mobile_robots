
#ifndef MODAL_h
#define MODAL_h

//#include "project_info.h" 
//#include "cmake_config.h"

#include "useful_functions.h"

#include "mbs_xml_reader.h"
#include "mbs_part.h"
#include "mbs_equil.h"
#include "mbs_data.h"
#include "MBSfun.h"
#include "mbs_modal_struct.h"

//#include "mstr.h"

/**
* Create a new modal structure.
* This function also initialize the options structures and
* the MbsAux for storing computational info during simulation.
*
* \p mbs_data the data structure of the model for which
*             the modal analysis will be computed
*/
MbsModal* mbs_new_modal(MbsData* mbs_data);

/**
* Create a new modal structure.
* This function also initialize the options structures.
* A pointer to the given MbsAux structure is kept by the
* returned structure.
*
* \p mbs_data the data structure of the model for which
*             the modal analysis will be computed
*/
MbsModal* mbs_new_modal_aux(MbsData* mbs_data,MbsAux* mbs_aux);

/**
*
* Run the Modal Analysis:
*		- mbs_modal_init
*		- mbs_modal_loop
*		- mbs_modal_finish
*
* The MbsData structure associated should not be modified ??QD
*
* @param[in,out] mo the MbsModal to be run
* @param[in] s the MbsData structure of the model for which the modal analysis is computed
*
*/
void mbs_run_modal(MbsModal *mo,MbsData *s);

/**
* Initialize the Modal structure variables by
*		allocating memory based on the number of independant (nqu) variables in the MBS system
*/
void mbs_modal_init(MbsModal *mo,MbsData *s);

/**
* Perform the Modal analysis:
*     - 1 - Linearization of the Fr term into the stiffness matrix Kr, and the damping matrix Gr (see Basics)
*     - 2 - Compution of matrix A (see basics)
*	  - 3 - Modal analysis (through gcl libraries)
*	  - 4 - Post processing
*			 -4.1. EigenModes  (Robotran Basics sec 5.4.2 ??))
*			 -4.2. EigenModes ordering  by types and frequencies
*
* Based on the solvepk algorithm (see Numerics)
*/
void mbs_modal_loop(MbsModal *mo,MbsData *s);

/**
* Set Modal flag to done in MbsData structure
* save the Modal analysis results in .txt file
*		- the A matrix
*		- the modal analysis (with UNSORTED MODES !!!)
*		- the mode animation (to read with the MBSysPad)
*/
void mbs_modal_finish(MbsModal *mo,MbsData *s);

/**
* Free the MbsModal structure (pointers table, ...)
*
* The options (MbsModalOptions), the Lpk (linearization) and MbsAux structures are also freed
*
*/
void mbs_delete_modal(MbsModal *mo,MbsData *s);



int mbs_modal_linearipk(double **GK, MbsModal *mo, MbsAux *aux, MbsData *s, int PosOrVel);


/**
* Additional function necessary to compute the modal analysis
* ??QD need to be documented properly...
*
*/
int mbs_modal_evecPos( double** mat_r, double** mat_phi, int nx, int ind, double *evec_r, double *evec_phi);
int mbs_modal_fct(double *x, int nx, double *Fx, MbsModal *mo,  MbsAux *aux, MbsData *s, int PosOrVel);
int MODAL_copy_x_mode( double** emat_r, double** emat_phi, int nx, int ind, double *evec_r, double *evec_phi);

/**
* Save the modal analysis results (eigenmode, eigenfrequencies, mode type, ....) into a ASCII text file  ??QD
*			UNSORTED MODES !!! ??add an option to choose whether it is saved sorted or unsorted
*/
int mbs_modal_save_result(MbsModal* mbs_modal,MbsData* s, char* filename);
/**
* Save the modes animations in the MbSysPad format 
*
*/
int mbs_modal_save_anim(MbsModal* mbs_modal,MbsData* s, char* filepath);

#endif
