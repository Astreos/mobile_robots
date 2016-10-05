
#ifndef EQUIL_h
#define EQUIL_h


#include "mbs_data.h"
#include "mbs_equil_struct.h"


/**
  * Allocate the memory for the Equilibrium Options (but not for all the Equilibrium structure)
  * Initialize the options structure with default options
  */
MbsEquil* mbs_new_equil(MbsData* mbs_data);
MbsEquil* mbs_new_equil_aux(MbsData* mbs_data, MbsAux* mbs_aux);

/**
  * Free the Equilibrium structure (pointers table, Equibrium options, ...)
  * 
  * The options (MbsEquilOptions) and MbsAux structures are also freed
  *
  */
void mbs_delete_equil(MbsEquil* eq, MbsData* mbs_data);

/**
  * Run the Equilibrium process: 
  *		- mbs_equil_init
  *		- mbs_equil_loop
  *		- mbs_equil_finish
  *
  * The MbsData structure associated is modified (equilibrium configuration, q,qd,Qq  and possibly other parameters m,I,dpt, ... )
  *
  * @param eq the MbsEquil to be run
  * @return the MbsEquil ...
  *
  */
void mbs_run_equil(MbsEquil* mbs_equil,MbsData* mbs_data);

 /**
  * Initialize the Equilibrium structure variables by 
  *		linking the pointers table, x_ptr with:
  *		- the independent variables			: qu
  *		- the substitution variables		: xch	(if defined by the user) 
  *		- the extra equilibrium variables	: xe	(if defined by the user)
  *		linking the extra equilibrium function pointer, fxe_ptr with the user_equil_fxe (if defined by the user)
  *		Allocate the memory for x_ptr and x based on Equilibrium Options 
  */
void mbs_equil_init(MbsEquil* mbs_equil,MbsData* mbs_data);
 
/**
  * Process the equibrium : find the equilibrium variables, x that satisfy f(x)= [Fruc ; fxe] = 0
  * Based on the solvepk algorithm (see Numerics)
  */
void mbs_equil_loop(MbsEquil* mbs_equil,MbsData* mbs_data);

 /**
  * Set equilibrum flag to done in MbsData structure
  * Put the MBSysPAD initial velocities and accelerations back in the MbsData structure 
  * offer the possibility for user to finish operation through the use of user_equil_finish
  * 
  */
void mbs_equil_finish(MbsEquil* mbs_equil,MbsData* mbs_data);


 /**
  * Allocate the memory for the changed equilibrium variables through a table of pointers
  * 
  * \p options		Equilibrium options structure (where the pointers table is stored)
  */
void mbs_equil_exchange(MbsEquilOptions *options);


 /**
  * Allocate the memory for the addition equilibrium variables through a table of pointers
  * 
  * \p options		Equilibrium options structure (where the pointers table is stored)
  */
void mbs_equil_addition(MbsEquilOptions *options);

 /**
  * Allocate the memory for the int table containing the index of the qu variables to ignore !
  * 
  * \p options		Equilibrium options structure (where the pointers table is stored)
  */
void mbs_equil_ignorance(MbsEquilOptions *options);


 /**
  * Q?? is that necessary
  */
void mbs_equil_save(MbsEquil* eq, MbsData *mbs_data, int n);

 /**
  * Update the equilibrium variables (qu, xsub and xe) through the pointers eq->x_ptr
  * Compute the equilibrium vector function f(x)    (i.e. the function to set to 0)
  *			-1- Extract Fru from Fruc 
  *			-2- Compute Fxe based on a the user-defined function 
  *
  * \p x		vector containing the equilibrium variables   (qu, xsub, xe)
  * \p nx		number of equilibrium variables
  * \p fx		vector containing the equilibrium functions evaluation, f(x)		
  * \p eq		?Q to change for a more general use of function of fsolvepk
  * \p aux		?Q to change for a more general use of function of fsolvepk
  * \p s		?Q to change for a more general use of function of fsolvepk
  */
int mbs_equil_fct(double x[], int nx, double fx[], MbsEquil *eq,  MbsAux *aux, MbsData *s);



//-----------------------------------------------------------------
//---> TO MOVE TO a new   user_equil.c   (where 3 additional functions have to be described)
// + need for additional test on "user_equil_fxe" i.e. the case when the user nxe>0 but without the definition fxe[] ! 
//-----------------------------------------------------------------

 /**
  * user function 
  * Necessary to express equilibrium f(x)=0
  * 
  * \p s		the MbsData of the system (from which the user_model is accessible)
  * \p f		vectors which contains the equibrium functions :  =f(xe, ... )			
  */
void user_equil_fxe(MbsData *mbs_data,double* f);

//-----------------------------------------------------------------
//---> TO MOVE TO UTILITIES
//-----------------------------------------------------------------

/**
  * Print the equilibrium structure and its options
  *
  * \p eq	the Equilibrium structure
  */
void mbs_print_equil(MbsEquil* eq);

//-----------------------------------------------------------------
//---> TO MOVE TO NUMERICS!
//-----------------------------------------------------------------






 /**
  * 4 functions to move to numerics	 and change the index !
  */
void cholsl_0(double **a, int n, double *p, double *b, double *x);
void choldc_0(double **a, int n, double *p);
void ludcmp_0(double **a, int n, int *indx, double *d);
void lubksb_0(double **a, int n, int *indx, double b[]);

/**
  * Solve equation f(x)=0 based on a given initial value for the variables, x0  
  * Algorithm developped by Paul Fisette and ?Q
  *
  * \p fun_ptr	vector containing the expression of f(x) as function of x
  * \p eq	?Q to change for a more general use of function of fsolvepk
  * \p aux	?Q to change for a more general use of function of fsolvepk
  * \p s	?Q to change for a more general use of function of fsolvepk
  */
int mbs_equil_fsolvepk(int (*fun_ptr)(double*, int, double*, MbsEquil*, MbsAux*, MbsData*), MbsEquil *eq, MbsAux *aux, MbsData *s );




#endif
