
#include "mbs_load_xml.h"
#include "mbs_load_libraries.h"
#include "useful_functions.h"

#include <string.h>


// if the flage fo accessing user/symbolic functions via pointers is active
// define pre-processor functions to access functions for user_model and user_IO
// This macro works only in a function where a MbsData is stored in a variable called s
#ifdef PRJ_FCT_PTR
	#define mbs_new_user_model()              s->fct.user.mbs_new_user_model()
	#define mbs_load_user_model_xml(mgs, um)  s->fct.user.mbs_load_user_model_xml(mgs, um)
	#define mbs_delete_user_model(um)         s->fct.user.mbs_delete_user_model(um)
	#define mbs_new_user_IO(a)                s->fct.user.mbs_new_user_IO(a)
	#define mbs_delete_user_IO(uio)           s->fct.user.mbs_delete_user_IO(uio)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


MbsData* mbs_load(const char* mbs_filename, const char* build_name){
    MDS_gen_strct *mds;
    MbsData *mbs_data;

    mds = MDS_mbs_reader(mbs_filename);

    mbs_data = (MbsData*) malloc(sizeof(MbsData));

    // if we call project and user function pointer, init them
    #ifdef PRJ_FCT_PTR
    mbs_load_symbolic_functions(mbs_data, mbs_filename, build_name);
    mbs_load_user_functions(mbs_data, mbs_filename, build_name);
    #endif

    mbs_data = MDS_create_MBSdataStruct(mds, mbs_data);

    free_MDS_gen_strct(mds);

    mbs_data->mbs_filename = (char*) malloc (1+strlen(mbs_filename));
    mbs_data->build_name   = (char*) malloc (1+strlen(build_name));

    strcpy(mbs_data->mbs_filename, mbs_filename);
    strcpy(mbs_data->build_name  , build_name);

    return mbs_data;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Implementation of utility function for user_IO ==> should be moved
 * (see def in mbs_user_interface.h)
 */ 
void mbs_set_user_IO(UserIoInfo* ioInfo, double val){
    int i,j;
    
    // set input initial value to 0.0
    for(i=0; i<ioInfo->n_in; i++){
        for(j=0; j<ioInfo->size_in[i] ;j++){
            ioInfo->ptr_in[i][j] = val;
        }
    }
    
    // set output initial value to 0.0
    for(i=0; i<ioInfo->n_out; i++){
        for(j=0; j<ioInfo->size_out[i] ;j++){
            ioInfo->ptr_out[i][j] = val;
        }        
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UserIoInfo* mbs_xml_create_user_IO_info(MDS_gen_strct* mds_gen_strct, MbsData* s){
    
    int i, i_in, i_out;
    UserIoInfo* ioInfo = (UserIoInfo*) malloc(sizeof(UserIoInfo));
    MDS_user_IOs_strct *mds_IOs = mds_gen_strct->user_IOs;
    
    // number of input/output
    ioInfo->n_in  = mds_IOs->n_in;
    
    ioInfo->n_out = mds_IOs->n_out;

    // sizes of input/output
    ioInfo->size_in  = (int*) malloc(sizeof(int)*ioInfo->n_in);
    ioInfo->size_out = (int*) malloc(sizeof(int)*ioInfo->n_out);

    i_in = 0; i_out = 0;
    for(i=0 ; i<mds_IOs->n_user_IO ; i++){
        int io_type = mds_IOs->user_IO_list[i]->type;

        if(io_type==1){
            ioInfo->size_in[i_in] = mds_IOs->user_IO_list[i]->size;
            i_in++;
        }
        else if(io_type==2){
            ioInfo->size_out[i_out] = mds_IOs->user_IO_list[i]->size;
            i_out++;
        }
    }

    // data pointer
    ioInfo->ptr_in  = (double**) malloc(sizeof(double*)*ioInfo->n_in);
    ioInfo->ptr_out = (double**) malloc(sizeof(double*)*ioInfo->n_out);
    
    // initialization of data point is project dependent. UserIO must be initialized.


    return ioInfo;
}

void mbs_delete_user_IO_info(UserIoInfo* ioInfo){
    
    free(ioInfo->ptr_in);
    free(ioInfo->ptr_out);
    
    
    free(ioInfo->size_in);
    free(ioInfo->size_out);
    
    free(ioInfo);
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*! \brief return a (nj sized) vector (starting at index 1) of integer which is a copy of the provided (nq sized) vector (starting at index 0) with all value incremented by 1.
 *  If nj is greater than nq, the unused value are set to 0.
 *
 * \param[in] q_mds the original (nq sized) vector of integers, starting at index 0
 * \param[in] nq    the size of the q_mds vector
 * \param[in] nj    the size of the returned (starting at index 1) vector (0 element not taken into account)
 *
 * \return requested vector (element 0 receives nj: the size without this element)
 */
int* MDS_translate_q(int* q_mds ,int nq, int nj)
{
    int* q_MBSdataStruct;
    q_MBSdataStruct = get_ivec_1(nj);
    
    if(nq)
    {
        int i;
        for(i=0; i<nq; i++)
        {
            q_MBSdataStruct[i+1] =q_mds[i]+1;
        }
    }
    return q_MBSdataStruct;
}

MbsData* MDS_create_MBSdataStruct(MDS_gen_strct* mds_gen_strct, MbsData* s)
{
    int i,j;
    int ind_joint = 0;
    int ind_state_value = 0;



    s->nbody    = mds_gen_strct->bodytree->n_body;
    s->njoint   = mds_gen_strct->bodytree->n_joint;

    s->npt      = mds_gen_strct->n_point;

    s->nqu      = mds_gen_strct->bodytree->n_qu;
    s->nqv      = mds_gen_strct->bodytree->n_qv;
    s->nqc      = mds_gen_strct->bodytree->n_qc;
    s->nqlocked = mds_gen_strct->bodytree->n_qlocked;
    s->nqdriven = mds_gen_strct->bodytree->n_qdriven;
    s->nqa      = mds_gen_strct->bodytree->n_qa;

    if(mds_gen_strct->cuts==NULL){
        s->Nloopc   = 0;
    }else{
        s->Nloopc   = mds_gen_strct->cuts->n_rod + 3 * mds_gen_strct->cuts->n_ball + 6 * mds_gen_strct->cuts->n_solid;
    }

    s->Nuserc   = 0;
    s->Ncons    = s->Nloopc + s->Nuserc;

    s->nhu = s->Ncons;

    if(mds_gen_strct->links!=NULL){
        s->Nlink    = mds_gen_strct->links->n_link;
        s->Nlink3D  = mds_gen_strct->links->n_link3D;
    }
    else{
        s->Nlink    = 0;
        s->Nlink3D  = 0;
    }

    s->Nsensor  = mds_gen_strct->n_sensor;
    s->Nxfrc    = mds_gen_strct->n_extforce;

    s->Nwheel   = 0; // dead function (at this time)

#if !defined SENSORKIN
    s->Nuser_model = mds_gen_strct->user_models->n_user_model;
    s->n_user_IO   = mds_gen_strct->user_IOs->n_user_IO;
#endif
    s->Nux = mds_gen_strct->n_state_value;


//    geometric ans dynamic data

    //pt

    s->dpt[0] = NULL;
    if (s->npt)
    {
        s->dpt[0]=(double*) calloc((4)*(s->npt+1),sizeof(double));
        s->dpt[0][0] = 3;
        for(i=1;i<=3;i++)
        {
            s->dpt[i] = &(s->dpt[0][(i)*(s->npt+1)]);
            s->dpt[i][0] = (double) s->npt;
        }
        for(i=0; i<mds_gen_strct->n_point; i++)
        {
            for(j=0; j<3; j++)
            {
                s->dpt[j+1][i+1] = mds_gen_strct->point_list[i]->pt[j];
                mds_gen_strct->point_list[i]->d_pt[j]->MBSdata_d_ptr = &(s->dpt[j+1][i+1]);
            }
        }
    }
    else
        for(i=1;i<=3;i++)
            s->dpt[i] = NULL;

    // l, m, In

    ind_joint = 0;

    s->l[0] = (double*) calloc((4)*(s->njoint+1),sizeof(double));
    s->l[0][0] = 3;
    for(i=1;i<=3;i++)
    {
        s->l[i] = &(s->l[0][(i)*(s->njoint+1)]);
        s->l[i][0] = (double) s->njoint;
    }

    s->m = (double*) calloc(s->njoint+1,sizeof(double));
    s->m[0] = (double) s->njoint;

    s->In[0] = (double*) calloc((10)*(s->njoint+1),sizeof(double));
    s->In[0][0] = 9;
    for(i=1;i<=9;i++)
    {
        s->In[i] = &(s->In[0][(i)*(s->njoint+1)]);
        s->In[i][0] = (double) s->njoint;
    }

    for(i=0; i<mds_gen_strct->bodytree->n_body; i++)
    {
        ind_joint = ind_joint + mds_gen_strct->bodytree->body_list[i]->n_joint;
        for(j=0; j<3; j++)
        {
            s->l[j+1][ind_joint] = mds_gen_strct->bodytree->body_list[i]->com[j];
        }

        s->m[ind_joint] = mds_gen_strct->bodytree->body_list[i]->mass;

        s->In[1][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[0];
        s->In[2][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[1];
        s->In[3][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[2];
        s->In[4][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[1];
        s->In[5][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[3];
        s->In[6][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[4];
        s->In[7][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[2];
        s->In[8][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[4];
        s->In[9][ind_joint] = mds_gen_strct->bodytree->body_list[i]->inertia[5];
    }

    // g
    s->g[0] = 0.0;
    copy_dvec_0(mds_gen_strct->base->gravity, &(s->g[1]), 3);

    // partitioning infos, the allocated vector are sized to have the possibility to handle all joint index.

    s->qu = MDS_translate_q(mds_gen_strct->bodytree->qu, mds_gen_strct->bodytree->n_qu, s->njoint);
    s->qv = MDS_translate_q(mds_gen_strct->bodytree->qv, mds_gen_strct->bodytree->n_qv, s->njoint);
    s->qc = MDS_translate_q(mds_gen_strct->bodytree->qc, mds_gen_strct->bodytree->n_qc, s->njoint);
    s->qlocked = MDS_translate_q(mds_gen_strct->bodytree->qlocked, mds_gen_strct->bodytree->n_qlocked, s->njoint);
    s->qdriven = MDS_translate_q(mds_gen_strct->bodytree->qdriven, mds_gen_strct->bodytree->n_qdriven, s->njoint);
    s->qa = MDS_translate_q(mds_gen_strct->bodytree->qa, mds_gen_strct->bodytree->n_qa, s->njoint);



    if (s->nhu) // to change
    {
        s->hu = (int*) calloc(s->nhu+1,sizeof(int));
        s->hu[0] = s->nhu;
        for(i=0; i<s->nhu; i++)
        {
            s->hu[i+1] = i+1;
        }
    }
    else
        s->hu = NULL;

    // joints data
    s->q0   = get_dvec_1(s->njoint);
    s->qd0  = get_dvec_1(s->njoint);
    s->qdd0 = get_dvec_1(s->njoint);

    s->q   = get_dvec_1(s->njoint);
    s->qd  = get_dvec_1(s->njoint);
    s->qdd = get_dvec_1(s->njoint);


    for(i=0; i<mds_gen_strct->bodytree->n_joint; i++)
    {
        s->q0[i+1] = mds_gen_strct->bodytree->joint_list[i]->q0;
        s->qd0[i+1] = mds_gen_strct->bodytree->joint_list[i]->qd0;
        s->qdd0[i+1] = mds_gen_strct->bodytree->joint_list[i]->qdd0;

        mds_gen_strct->bodytree->joint_list[i]->d_qf->MBSdata_d_ptr = &(s->q0[i+1]);
    }

    copy_dvec_1(s->q0, s->q);
    copy_dvec_1(s->qd0, s->qd);
    copy_dvec_1(s->qdd0, s->qdd);

    s->qmin = NULL; // dead function (at this time)
    s->qmax = NULL;

// Frc, Trq, Qq, tsim

    // frc
    s->frc[0] = (double*) calloc((4)*(s->njoint+1),sizeof(double));
    s->frc[0][0] = 3;
    for(i=1;i<=3;i++)
    {
        s->frc[i] = &(s->frc[0][(i)*(s->njoint+1)]);
        s->frc[i][0] = (double) s->njoint;
    }

    // trq
    s->trq[0] = (double*) calloc((4)*(s->njoint+1),sizeof(double));
    s->trq[0][0] = 3;
    for(i=1;i<=3;i++)
    {
        s->trq[i] = &(s->trq[0][(i)*(s->njoint+1)]);
        s->trq[i][0] = (double) s->njoint;
    }

    // Qq  (and Qc)
    s->Qq = (double*) calloc(s->njoint+1,sizeof(double));
    s->Qq[0] = (double) s->njoint;
	s->Qc = (double*)calloc(s->njoint + 1, sizeof(double));
	s->Qc[0] = (double)s->njoint;
    for(i=1;i<=s->njoint;i++)
	{
        s->Qq[i] = 0.0;
		s->Qc[i] = 0.0;
	}

    // tsim
    s->tsim = 0.0;

    // correctly updated later
    s->t0  = 0.0;
    s->tf  = 0.0;
    s->dt0 = 0.0;

    // 1 to stop the simulation, 0 otherwise
    s->flag_stop = 0;

// Constraint data

    // lrod
    if (mds_gen_strct->cuts!=NULL && mds_gen_strct->cuts->n_rod)
    {
        s->lrod = (double*) calloc(mds_gen_strct->cuts->n_rod+1,sizeof(double));
        s->lrod[0] = mds_gen_strct->cuts->n_rod;
        for(i=0; i<mds_gen_strct->cuts->n_rod; i++)
        {
            s->lrod[i+1] = mds_gen_strct->cuts->rod_list[i]->length;
            mds_gen_strct->cuts->rod_list[i]->d_length->MBSdata_d_ptr = &(s->lrod[i+1]);
        }
    }
    else
        s->lrod = NULL;

    // NRerr
        s->NRerr = 1e-9; //default value

    if (s->nqv){
        s->lambda = (double*) calloc(s->nqv+1,sizeof(double));
        s->lambda[0] = s->nqv;
        for (i=1; i<=s->nqv; i++){
            s->lambda[i] = 0.0;
        }
    }


// Link Data

    // Z, Zd, Fl
    if (s->Nlink)
    {
        s->Z  = (double*) calloc(s->Nlink+1,sizeof(double));
        s->Zd = (double*) calloc(s->Nlink+1,sizeof(double));
        s->Fl = (double*) calloc(s->Nlink+1,sizeof(double));
        s->Z[0]  = (double) s->Nlink;
        s->Zd[0] = (double) s->Nlink;
        s->Fl[0] = (double) s->Nlink;
        for(i=1;i<=s->Nlink;i++)
        {
            s->Z[i]  = 0.0;
            s->Zd[i] = 0.0;
            s->Fl[i] = 0.0;
        }
    }
    else
    {
        s->Z  = NULL;
        s->Zd = NULL;
        s->Fl = NULL;
    }

    // l3DWr
    if (s->Nlink3D)
    {
        s->l3DWr = (double**) calloc(s->Nlink3D+1,sizeof(double*));
        s->l3DWr[0] = NULL;
        for(i=1;i<=s->Nlink3D;i++)
        {
            s->l3DWr[i] = (double*) calloc(6+1,sizeof(double));
            for(j=0;j<=6;j++)
                s->l3DWr[i][j] = 0.0;
        }
    }
    else
        s->l3DWr = NULL;

// Ext. Forces Data

    // xfidpt
    if (s->Nxfrc)
    {
        s->xfidpt = (int*) calloc(s->Nxfrc+1,sizeof(int));
        s->xfidpt[0] = s->Nxfrc;
        for(i=0;i<mds_gen_strct->n_extforce;i++)
        {
            s->xfidpt[i+1] = mds_gen_strct->extforce_point_list[i]+1;
        }
    }
    else
        s->xfidpt = NULL;

    // SWr
    if (s->Nxfrc)
    {
        s->SWr = (double**) calloc(s->Nxfrc+1,sizeof(double*));
        s->SWr[0] = NULL;
        for(i=1;i<=s->Nxfrc;i++)
        {
            s->SWr[i] = (double*) calloc(9+1,sizeof(double));
            for(j=0;j<=9;j++)
                s->SWr[i][j] = 0.0;
        }
    }
    else
        s->SWr = NULL;

// Wheel Data

    // rnom
    if (s->Nwheel)
    {
        s->rnom = (double*) calloc(s->Nwheel+1,sizeof(double));
        s->rnom[0] = (double) s->Nwheel;
        // to complet
    }
    else
        s->rnom = NULL;

#if !defined SENSORKIN
// User Model

    // user_model
    if (s->Nuser_model)
    {
        s->user_model = mbs_new_user_model();
        mbs_load_user_model_xml(mds_gen_strct, s->user_model);
    }
    else
    {
        s->user_model = NULL;
    }

    // ux, uxd, ux0
    if (s->Nux)
    {
        s->ux  = (double*) calloc(s->Nux+1,sizeof(double));
        s->uxd = (double*) calloc(s->Nux+1,sizeof(double));
        s->ux0 = (double*) calloc(s->Nux+1,sizeof(double));
        s->ux[0]  = (double) s->Nux;
        s->uxd[0] = (double) s->Nux;
        s->ux0[0] = (double) s->Nux;

        for(i=0; i<mds_gen_strct->n_state; i++)
        {
            for(j=0; j<mds_gen_strct->state_list[i]->n_value; j++)
            {
                s->ux0[ind_state_value+1] = mds_gen_strct->state_list[i]->value_list[j];
                ind_state_value++;
            }
        }
        copy_dvec_0(s->ux0, s->ux, s->Nux+1);
    }
    else
    {
        s->ux  = NULL;
        s->uxd = NULL;
        s->ux0 = NULL;
    }

    s->__user_IO_info = mbs_xml_create_user_IO_info(mds_gen_strct, s);
    s->user_IO = mbs_new_user_IO(s->__user_IO_info);

    if(s->n_user_IO){
        mbs_set_user_IO(s->__user_IO_info, 0.);
    }
    
#endif

//    Other

    // udd is sized to have the possibility to handle all joints acceleration.
    s->udd = get_dvec_1(s->njoint);

    s->DonePart = 0;
    s->DoneEquil = 0;
    s->DoneModal = 0;

    //s->process = 0;
    s->simu_end = 0;

    return s;
}


////////////////////////////////////////////////////////////////////////////        CAUTION  CAUTION


void mbs_delete_data(MbsData *s)
{
    int i;

    // Donnees geometriques et dynamiques
    if (s->npt)
        free(s->dpt[0]);
    
    free(s->l[0]);

    free(s->m);

    free(s->In[0]);

    // partitioning informations, always allocated
    free_ivec_1(s->qu);
    free_ivec_1(s->qv);
    free_ivec_1(s->qc);
    free_ivec_1(s->qlocked);
    free_ivec_1(s->qdriven);
    free_ivec_1(s->qa);

    // Variables articulaires, valeures initiales, limites
    free_dvec_1(s->q);
    free_dvec_1(s->qd);
    free_dvec_1(s->qdd);
    free_dvec_1(s->q0);
    free_dvec_1(s->qd0);
    free_dvec_1(s->qdd0);


    if (s->qmin != NULL)
        free(s->qmin);
    if (s->qmax != NULL)
        free(s->qmax);

    // frc, trq, Qq
    free(s->frc[0]);
    free(s->trq[0]);
    free(s->Qq);
	free(s->Qc);

    // Constraints
    if (s->lrod != NULL)
        free(s->lrod);

    if (s->nqv)
        free(s->lambda);

    // Links
    if (s->Nlink)
    {
        free(s->Z);
        free(s->Zd);
        free(s->Fl);
    }
    if (s->Nlink3D)
        for(i=1;i<=s->Nlink3D;i++)
            free(s->l3DWr[i]);

    // Ext. forces
    if (s->Nxfrc)
    {
        free(s->xfidpt);
        for(i=1;i<=s->Nxfrc;i++)
            free(s->SWr[i]);
        free(s->SWr);
    }

    // Wheel
    if (s->Nwheel)
    {
        free(s->rnom);
    }

    // User state
    if (s->Nux)
    {
        free(s->ux);
        free(s->uxd);
        free(s->ux0);
    }

#ifndef SENSORKIN
    // User models
    if (s->Nuser_model)
    {
        mbs_delete_user_model(s->user_model);
    }
    
    mbs_delete_user_IO(s->user_IO);
    mbs_delete_user_IO_info(s->__user_IO_info);
#endif

    // Other
    free_dvec_1(s->udd);

    if (s->nhu) // to change
    {
        free(s->hu);
    }

    if (s->mbs_filename)
    {
        free(s->mbs_filename);
    }

    if (s->build_name)
    {
        free(s->build_name);
    }

    #ifdef PRJ_FCT_PTR
    // close the dynamics libraries associated to s (symbolic and userfct)
    mbs_close_libraries(s);
    #endif

    free(s);
}
