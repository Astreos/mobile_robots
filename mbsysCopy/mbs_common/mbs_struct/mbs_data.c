
#include "mbs_data.h"
#include "useful_functions.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Those utility functions should be move elsewhere
// We should use function of useful_functions. h but signature is not the same ... we need to merge that all!

void print_vector(char* prefix, int n, double *vec){
    int i;
    printf(" %s = [", prefix);
    for(i=1; i <= n ; i++){
       printf(" %lf ", vec[i]);
    }
    printf("]\n");
}

void print_intvector(char* prefix, int n, int *vec){
    int i;
    printf(" %s = [", prefix);
    for(i=1; i <= n ; i++){
       printf(" %d ", vec[i]);
    }
    printf("]\n");
}

void print_matrix(char* prefix, int r, int c, double **mat){
    int i, j;
    printf(" %s = [", prefix);
    for(i=1; i <= r ; i++){
        for(j=1; j <= c ; j++){
            printf(" %lf ", mat[i][j]);
        }
        printf(" \n        ");
    }
    printf("]\n");
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_print_data(MbsData* s){
    printf("\n MbsData \n");

    printf(" nbody  . .  = %d \n", s->nbody);
    printf(" njoint . .  = %d \n", s->njoint);
    printf(" npt  . . .  = %d \n", s->npt);
    printf(" nqu  . . .  = %d \n", s->nqu);
    printf(" nqc  . . .  = %d \n", s->nqc);
    printf(" nqa  . . .  = %d \n", s->nqa);
    printf(" nqv  . . .  = %d \n", s->nqv);
    printf(" nhu  . . .  = %d \n", s->nhu);
    printf(" nqlocked .  = %d \n", s->nqlocked);
    printf(" nqdriven .  = %d \n", s->nqdriven);
    //printf(" Nloopc . .  = %d \n", s->Nloopc);
    printf(" Nuserc . .  = %d \n", s->Nuserc);
    printf(" Ncons  . .  = %d \n", s->Ncons);
    printf(" Nlink  . .  = %d \n", s->Nlink);
    printf(" Nlink3D  .  = %d \n", s->Nlink3D);
    printf(" Nsensor  .  = %d \n", s->Nsensor);
    printf(" Nxfrc  . .  = %d \n", s->Nxfrc);
    //printf(" Nwheel . .  = %d \n", s->Nwheel);
    printf(" Nuser_model = %d \n", s->Nuser_model);
    printf(" Nux . . . . = %d \n", s->Nux);

    print_vector("g",  3, s->g);

    print_vector("q  ", s->njoint, s->q);
    print_vector("qd ", s->njoint, s->qd);
    print_vector("qdd", s->njoint, s->qdd);
    print_vector("Qq",  s->njoint, s->Qq);

    printf("\n");
    print_intvector("qc  ", s->nqc, s->qc);
    print_intvector("qu  ", s->nqu, s->qu);
    print_intvector("qa  ", s->nqa, s->qa);
    print_intvector("qv  ", s->nqv, s->qv);
    print_intvector("hu  ", s->nhu, s->hu);

    print_matrix("dpt ", 3, s->npt, s->dpt);
    print_vector("m",  s->njoint, s->m);
    print_matrix("In ", 9, s->njoint, s->In);
    print_matrix("l ", 3, s->njoint, s->l);

    /*printUserModel(s->usrMod);

    printUserIO(s->userIO);*/
}



void mbs_copy_data(MbsData* s, MbsData* s_copy)
{
	s_copy->npt=s->npt;

	copy_dmat_0(s->dpt,s_copy->dpt, 3+1, s->npt + 1);
	copy_dmat_0(s->l, s_copy->l, 3 + 1, s->njoint + 1);
	copy_dmat_0(s->l, s_copy->l, 3 + 1, s->npt + 1);
	copy_dvec_0(s->m, s_copy->m, s->npt + 1);

	s_copy->nbody = s->nbody;
	s_copy->njoint = s->njoint;

	// Infos partitionnement //
	s_copy->nqu = s->nqu;
	s_copy->nqc = s->nqc;
	s_copy->nqlocked = s->nqlocked;
	s_copy->nqdriven = s->nqdriven;
	s_copy->nqa = s->nqa;
	s_copy->nqv = s->nqv;
	s_copy->nhu = s->nhu;
	copy_ivec_0(s->qu, s_copy->qu, s->njoint+1);
	copy_ivec_0(s->qc, s_copy->qc, s->njoint+1);
	copy_ivec_0(s->qlocked, s_copy->qlocked, s->njoint+1);
	copy_ivec_0(s->qdriven, s_copy->qdriven, s->njoint+1);
	copy_ivec_0(s->qa, s_copy->qa, s->njoint+1);
	copy_ivec_0(s->qv, s_copy->qv, s->njoint+1);
	copy_ivec_0(s->hu, s_copy->hu, s->njoint+1);

    // Variables articulaires, valeurs initiales et limites //
	copy_dvec_0(s->q, s_copy->q, s->njoint + 1);
	copy_dvec_0(s->qd, s_copy->qd, s->njoint + 1);
	copy_dvec_0(s->qdd, s_copy->qdd, s->njoint + 1);
	copy_dvec_0(s->q0, s_copy->q0, s->njoint + 1);
	copy_dvec_0(s->qd0, s_copy->qd0, s->njoint + 1);
	copy_dvec_0(s->qdd0, s_copy->qdd0, s->njoint + 1);
	copy_dvec_0(s->qmin, s_copy->qmin, s->njoint + 1);
	copy_dvec_0(s->qmax, s_copy->qmax, s->njoint + 1);

    // Frc, Trq, Qq, tsim
	copy_dmat_0(s->frc, s_copy->frc, 3 + 1, s->njoint + 1);
	copy_dmat_0(s->frc, s_copy->frc, 3 + 1, s->njoint + 1);
	copy_dvec_0(s->Qq, s_copy->Qq, s->njoint + 1);

	// time
	s_copy->tsim = s->tsim;
	s_copy->t0 = s->t0;
	s_copy->dt0 = s->dt0;
	s_copy->tf = s->tf;

    // set to 1 to stop the simulation
    s_copy->flag_stop=s->flag_stop;

    // Constraints
	// ?? copy_dvec_0(s->lrod, s_copy->lrod, s->njoint + 1);
	s_copy->Nloopc = s->Nloopc;
	s_copy->Ncons = s->Ncons;
	s_copy->Nuserc = s->Nuserc;
	s_copy->NRerr = s->NRerr;
	// ?? copy_dvec_0(s->lambda, s_copy->lambda, s->Nqv + 1);

    // Links and Link3Ds
	s_copy->Nlink = s->Nlink;
	s_copy->Nlink3D = s->Nlink3D;
	copy_dvec_0(s->Z, s_copy->Z, s->Nlink + 1);
	copy_dvec_0(s->Zd, s_copy->Zd, s->Nlink + 1);
	copy_dvec_0(s->Fl, s_copy->Fl, s->Nlink + 1);
	copy_dmat_0(s->l3DWr, s_copy->l3DWr, s->Nlink3D + 1, 6 + 1); // attention indice

    // Sensors
    s_copy->Nsensor=s->Nsensor;

    // Ext Forces
	s_copy->Nxfrc = s->Nxfrc;
	copy_ivec_0(s->xfidpt, s_copy->xfidpt, s->Nxfrc+1);
	copy_dmat_0(s->SWr, s_copy->SWr, s->Nxfrc + 1, 9 + 1);// attention indice inversée

#if !defined SENSORKIN
    // User Model
    //int Nuser_model;
    //UserModel *user_model;


#endif

    // User State
	s_copy->Nux = s->Nux;
	copy_dvec_0(s->ux,s_copy->ux,s->Nux+1);
	copy_dvec_0(s->ux0,s_copy->ux0, s->Nux + 1);
	copy_dvec_0(s->uxd,s_copy->uxd, s->Nux + 1);

    // OTHER FIELDS //
	copy_dvec_0(s->udd, s_copy->udd, s->njoint+ 1);

	// Processing
	s_copy->DonePart = s->DonePart;
	s_copy->DoneEquil = s->DoneEquil;
	s_copy->DoneModal = s->DoneModal;
	s_copy->process = s->process;
	s_copy->simu_end= s->simu_end;

    // ?? char *mbs_filename;
    // ?? char *build_name;

	// ??
    //#ifdef REAL_TIME
    //void *realtime;
    //#endif
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */







/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
