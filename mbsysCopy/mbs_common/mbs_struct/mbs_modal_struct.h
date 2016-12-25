

#ifndef MBS_MODAL_STRUCT_h
#define MBS_MODAL_STRUCT_h

#include "useful_functions.h"

#include "mbs_data.h"
#include "mbs_linearipk.h"

typedef struct MbsModalOptions
{
    double time;		///< [defaut = 0.0]
    double lintol;		///< [defaut = 1e-3]
    double relincr;		///< [defaut = 1e-2]
    double absincr;		///< [defaut = 1e-3]
 
    double equitol;		///<				  [defaut = 1e-6]
    int save_mat;		///< no = 0, yes = 1, [defaut = 0]
    int save_result;	///< no = 0, yes = 1, [defaut = 0]
    int save_anim;		///< no = 0, yes = 1, [defaut = 0]
    int renamefile;		///< no = 0, yes = 1, [defaut = 0]
    int verbose;		///< no = 0, yes = 1, [defaut = 1]
    int clearmbsglobal; ///< inout = 1, out = 2, none = 3, all = 4, [defaut = 1]

	double mode_ampl;	///< [defaut = 0.2]

	int nqui; ///< number of ignored independent variables for quasistatic equilibrium
    int *qui; ///< index  of ignored independant variables
    /* The keyword used for determining the name of result files */
    const char* resfilename;

    /** Path in which result file are saved.
     *  Default: the resultsR folder of the project
     */
    const char* respath;
	char* animpath;

} MbsModalOptions;

typedef struct MbsModalLpk
{
    int *diverge_ind; // size n_x
    int n_diverge;
    double maxcomp;

    double *x_equil;
    double *x_mid;
    double *x_ext;

    double *F_equil;
    double *F_mid;
    double *F_ext; 

    double **GK_prem;
    double **GK_comp;

	int itermax;

} MbsModalLpk;

typedef struct MbsModalMode
{
    int type; ///< 1=unoscillating stable; 2=rigid; 3=unoscillating unstable; 4=oscillating damped; 5=oscillating undamped; 6=oscillating unstable
    
    double eval_a; 
    double eval_b;
    
    double ksi; 
    double nu_0; 
    double nu;

    double *evec_r;
    double *evec_phi;

} MbsModalMode;

typedef struct MbsModal
{
     MbsModalOptions *options; 
	 MbsAux *aux; 

	// MbsModalLpk *lpk_old; ///< structure associated to the linearipk method for linearization of Gr and Kr
	 MbsLpk *lpk;


    double *q_saved;
    double *qd_saved;

	double **Mr; ///<              mass		 matrix (Robotran Basics Eq(42))	
	double **Kr; ///<	Linearized stiffness matrix (Robotran Basics Eq(42))	
    double **Gr; ///<	Linearized damping	 matrix (Robotran Basics Eq(42)) 
    double **Bvuc;

	int nx;
    int nz;

    double **Mrzz;

    double **A;

    double *vec_a;		///< Result of the Eigenvalue problem: real		 part of the 2*nx eigen values						[2*nx]
    double *vec_b;		///< Result of the Eigenvalue problem: imaginary part of the 2*nx eigen values						[2*nx]	
    double **mat_r;		///< Result of the Eigenvalue problem: norm  of the eigen vectors (each column is a eigenvector)	[2*nx X 2*nx] 	
    double **mat_phi;	///< Result of the Eigenvalue problem: phase of the eigen vectors (each column is a eigenvector)	[2*nx X 2*nx]

    int nMode;
    MbsModalMode **modeList; 
    int *iModeSorted;

	 



} MbsModal;





#endif