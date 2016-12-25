
#ifndef MBS_EQUIL_STRUCT_h
#define MBS_EQUIL_STRUCT_h

#include "useful_functions.h"

#include "mbs_data.h"
#include "mbs_aux.h"


typedef struct MbsEquilOptions
{
    int		method;			///< fsolvepk = 1, optim = 2,							[defaut = 1]
    double	equitol;		///< equilibrium acceptation tolerance					[defaut = 1e-6]
	int		verbose;		///< no = 0, yes = 1, defaut = 1
    
	// solvepk related...
	int		quasistatic;	///< no = 0, yes = 1,									[defaut = 0] 
	double	relax;			///< relaxation in pk algorithm							[defaut = 1.0]
    int		smooth;			///< smoothing in pk algorithm	no = 0, yes = 1			[defaut = 0]
    double	senstol;		///< tolerance for testing a variable non-sensitivity	[defaut = 1e-6]
    double	devjac;			///< deviate state for Gradient calculation				[default = 1e-6]
    int		itermax;		///< maximum number of iteration for solvepk			[defaut = 30]
    
    int		visualize;		///< Q?		no = 0, yes = 1, defaut = 0
    int		clearmbsglobal;	///< Q?		inout = 1, out = 2, none = 3, all = 4, defaut = 1  ???

    //int nqui; ///< number of ignored independent variables for quasistatic equilibrium  ??Q should disappear 
    //int *qui; ///< index  of ignored independant variables   
    
    int nquch;			///< number of changed independant variables
    int *quch;			///< index  of changed independant variables   
    double **xch_ptr;	///< vector of pointers pointing to the substitution variables (the ones that replaces the changed qu)

    int nxe;								///< number of extra equilibrium variables
    double **xe_ptr;						///< vector of pointers pointing to the extra variables
    void (* fxe_ptr)(MbsData*, double*);	///< pointer pointing on the .c function calculating the added equilibrum functions

} MbsEquilOptions;

typedef struct MbsEquil
{
    MbsEquilOptions* options; ///< structure defining the option of a equilibrium
	MbsAux* aux;  

	double *qd_saved;	///< initial generalized velocities as defined in the mbsysPad  
	double *qdd_saved;	///< initial generalized accelerations as defined in the mbsysPad

	int nx;		///< number of variables for the equilibrium
	double *x;	///<           variables for the equilibrium
	double **x_ptr; ///< vector of pointers pointing to the equilibrium variables 

	//----- SolvePk related variables -----//
	int nxs;	///< number of sensitive variables for the equilibrium
	int *xs;	///< index  of sensitive variables for the equilibrium
	int nxns;	///< number of non sensitive variables for the equilibrium
	int *xns;	///< index  of non sensitive variables for the equilibrium
	double norm_pk; ///< norm of the equlibrium variables vector x  (parameter to minimize in the solvepk algorithm)

} MbsEquil;

#endif
