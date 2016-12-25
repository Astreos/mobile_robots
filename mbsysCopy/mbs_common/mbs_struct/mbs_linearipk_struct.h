

#ifndef MBS_LINEARIPK_STRUCT_h
#define MBS_LINEARIPK_STRUCT_h



typedef struct MbsLpk   // computation structure necessary for MBS equation linearization through lpk method 
{
	int *diverge_ind; // size n_x
	int n_diverge;
	double maxcomp;

	double *q_saved;
	double *qd_saved;
	double *Qq_saved;

	int nx;
	double *x_equil;
	double *x_mid;
	double *x_ext;

	double *F_equil;
	double *F_mid;
	double *F_ext;

	double **GK_prem;
	double **GK_comp;

	int verbose;

	int itermax;
	double relincr;
	double absincr;
	double equitol;
	double lintol;


} MbsLpk;



#endif