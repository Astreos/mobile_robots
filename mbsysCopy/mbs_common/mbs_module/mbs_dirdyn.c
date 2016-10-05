/**
 * @file mbs_dirdyn.c
 *
 * This file implements the  functions of the
 * dirdyn module in C.
 *
 *
 * Creation date: 19/11/2014
 * @author Nicolas Docquier (based on the work of other from CEREM: nvdn, av, ...)
 *
 *
 * (c) Universite catholique de Louvain
 */

#include "mbs_dirdyn.h"
#include <stdlib.h>
#include <stdio.h>
#include "integrator.h"
#include "mbs_project_interface.h"
#include "user_realtime_visu.h"
#include "string.h"
#include "realtime.h"
#include "set_output.h"
#include "dopri5.h"
#include "MBSfun.h"

#define DOPRI5_NB_ALLOC_VEC 8 ///< number of vectors to allocate for dopri5

#define MAX_NB_BUFFER (4 + 2 + 3 + 1) // maximal number of buffer 4 for generic, 2 for user states, 3 for links (real number depends on the model)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MbsDirdyn* mbs_new_dirdyn(MbsData* mbs_data)
{
    MbsAux* mbs_aux;

    // Initialize the local data struct
    mbs_aux = initMbsAux(mbs_data);

    return mbs_new_dirdyn_aux(mbs_data, mbs_aux);
}

MbsDirdyn* mbs_new_dirdyn_aux(MbsData* mbs_data, MbsAux* mbs_aux)
{
    MbsDirdyn *dirdyn;
    MbsDirdynOptions* opts;
    MbsDirdynDopri5 *dopr;
    MbsDirdynOptDopri5 *dopr_opt;

    // Initialize the direct dynamic structure
    dirdyn = (MbsDirdyn*) malloc(sizeof(MbsDirdyn));

    // keep the pointer to the auxiliary data structure
    dirdyn->mbs_aux = mbs_aux;

    // Initialize the options struct with default options
    opts  = (MbsDirdynOptions*) malloc(sizeof(MbsDirdynOptions));
    opts->t0 = 0.;
    opts->tf = 5.;
    opts->dt0 = 1e-3;
    opts->save2file = 1;
    opts->save_anim = 1;
    opts->save_visu = 0;
    opts->framerate = 1000;
    opts->saveperiod = 1;
    opts->max_save_user = 12;
    opts->buffersize = -1;
    opts->resfilename = NULL;
    opts->respath = NULL;
    opts->animpath = NULL;
    opts->realtime = 0;
    opts->accelred = 0;

    // adaptive time step integrator
    dopr_opt = (MbsDirdynOptDopri5*) malloc(sizeof(MbsDirdynOptDopri5));
    dopr_opt->flag_use = 0;
    dopr_opt->flag_waypoint = 0;
    dopr_opt->flag_solout_wp = 0;
    dopr_opt->flag_stop_stiff = 0;
    dopr_opt->verbose = 1;
    dopr_opt->rtoler = 1.0e-3;
    dopr_opt->atoler = 1.0e-6;
    dopr_opt->dt_max = 1.0e-3;
    dopr_opt->nmax = 1.0e9;
    dopr_opt->delta_t_wp = 1.0e-3;

    opts->dopri5 = dopr_opt;
    dirdyn->options = opts;

    // initial the saving counter
    dirdyn->savePeriodCounter = 0;

    // Runge Kutta 4
    dirdyn->rk4 = (MbsDirdynRK4*) malloc(sizeof(MbsDirdynRK4));

    // adaptive time step
    dopr = (MbsDirdynDopri5*) malloc(sizeof(MbsDirdynDopri5));
    dopr->flag_save = 0;

    dirdyn->dopri5 = dopr;

    return dirdyn;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_delete_dirdyn(MbsDirdyn* dirdyn, MbsData* mbs_data)
{
    free(dirdyn->dopri5);
    free(dirdyn->rk4);
    free(dirdyn->options->dopri5);
    free(dirdyn->options);
    freeMbsAux(dirdyn->mbs_aux, mbs_data);
    free(dirdyn);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_run_dirdyn(MbsDirdyn* dd, MbsData* mbs_data)
{
    int i;
    FILE* animFile;

    // 1. Initialize the simulation
    // - - - - - - - - - - - - - -
    mbs_dirdyn_init(dd, mbs_data);


    // 2. Run the simulation
    // - - - - - - - - - - -
    mbs_dirdyn_loop(dd, mbs_data);


    // 3. Finish the simulation
    // - - - - - - - - - - - -
    mbs_dirdyn_finish(dd, mbs_data);
}

void mbs_dirdyn_save(MbsDirdyn* dd, MbsData *mbs_data, double t){

    int i;
    #ifdef JAVA
    Simu_realtime *realtime;
    Realtime_java *java;
    #endif

    for (i=0 ; i < dd->bufferNb ; i++){
        mbs_buffer_save(dd->buffers[i], t, dd->savedArrays[i]);
    }

    mbs_growing_buffer_save(dd->user_buffer, t);

    if (dd->options->save_visu)
    {
        #ifdef JAVA
        realtime = (Simu_realtime*) mbs_data->realtime;
        java = realtime->ext->java;

        user_realtime_visu(mbs_data, java->nb_models, java->nb_q, java->anim_q);

        for(i=0; i<realtime->options->nb_models; i++)
        {
            mbs_buffer_save(dd->buffer_visu[i], t, java->anim_q[i]);
        }
        #endif
    }
}

void mbs_dirdyn_init(MbsDirdyn* dd, MbsData* mbs_data)
{
    int i;

    #ifdef JAVA
    Simu_realtime *realtime;
    Realtime_java *java;
    #endif

    // INITIALIZATION
    // - - - - - - - -

    // options (indications for the user)
    mbs_data->t0   = dd->options->t0;
    mbs_data->tsim = mbs_data->t0;
    mbs_data->tf   = dd->options->tf;
    mbs_data->dt0  = dd->options->dt0;

    // initialise vectors
    dd->nState = 2*mbs_data->nqu + mbs_data->Nux;
    dd->y    = (double*) malloc(dd->nState*sizeof(double));
    dd->yout = (double*) malloc(dd->nState*sizeof(double));
    dd->yd   = (double*) malloc(dd->nState*sizeof(double));

    // allocated vectors for dopri5
    if (dd->options->dopri5->flag_use)
    {
        dd->dopri5->alloc_tab = (double**) malloc(DOPRI5_NB_ALLOC_VEC*sizeof(double*));

        for(i=0; i<DOPRI5_NB_ALLOC_VEC; i++)
        {
            dd->dopri5->alloc_tab[i] = (double*) malloc (dd->nState*sizeof(double));
        }

        // adaptive time step
        dd->dopri5->dydt_save = (double*) malloc(dd->nState*sizeof(double));
        dd->dopri5->solout_last_t = mbs_data->t0 - mbs_data->dt0;
    }
    else
    {
        dd->rk4->yt  = (double*) malloc(dd->nState*sizeof(double));
        dd->rk4->dyt = (double*) malloc(dd->nState*sizeof(double));
        dd->rk4->dym = (double*) malloc(dd->nState*sizeof(double));
    }

    // real-time modules activated
    #ifdef REAL_TIME
    if (dd->options->realtime)
    {
        mbs_realtime_init(mbs_data, dd->options->t0, dd->options->tf, dd->options->dt0);
    }
    #else
    if (dd->options->realtime)
    {
        printf("\nTo use the real-time features (mbs_dirdyn->options->realtime activated), set the CMake flag 'FLAG_REAL_TIME' to ON !\n");
        exit(EXIT_FAILURE);
    }
    #endif

    // init time
    dd->tsim = dd->options->t0;
    dd->dt   = dd->options->dt0;

    // user intialization
    user_dirdyn_init(mbs_data, dd);

    // Simulation state initialization
    for(i=1; i<=mbs_data->nqu; i++)
    {
        dd->y[i-1]               = mbs_data->q[mbs_data->qu[i]];
        dd->y[i-1+mbs_data->nqu] = mbs_data->qd[mbs_data->qu[i]];
    }
    for(i=1; i<=mbs_data->Nux; i++)
    {
        dd->y[i-1+2*mbs_data->nqu] = mbs_data->ux[i];
    }

    if(dd->options->save2file)
    {
        const char *resfilename, *respath, *animpath;
        int bufId, njoint, Nux, Nlink, nqc;
        char* f;
        char* f_anim;

        char * fnameSuffix[MAX_NB_BUFFER];
        int bufferIDs[MAX_NB_BUFFER]; 
        int bufElemNb[MAX_NB_BUFFER];


        njoint = mbs_data->njoint;
        Nux = mbs_data->Nux;
        Nlink = mbs_data->Nlink;
        nqc = mbs_data->nqc;
        
        // generic buffer properties
        fnameSuffix[0] = "q";    bufferIDs[0] = BUFFER_Q;    bufElemNb[0]=njoint;
        fnameSuffix[1] = "qd";   bufferIDs[1] = BUFFER_QD;   bufElemNb[1]=njoint;
        fnameSuffix[2] = "qdd";  bufferIDs[2] = BUFFER_QDD;  bufElemNb[2]=njoint;
        fnameSuffix[3] = "Qq";   bufferIDs[3] = BUFFER_QQ;   bufElemNb[3]=njoint;
        dd->bufferNb=4;
        
        // buffer properties for user states, if any
        if(Nux){
            int id = dd->bufferNb;
            fnameSuffix[id+0] = "ux";    bufferIDs[id+0] = BUFFER_UX;    bufElemNb[id+0]=Nux;
            fnameSuffix[id+1] = "uxd";   bufferIDs[id+1] = BUFFER_UXD;   bufElemNb[id+1]=Nux;
            dd->bufferNb += 2;
        }
       
        // buffer properties for links (link 1D only)
        if(mbs_data->Nlink){
            int id = dd->bufferNb;
            fnameSuffix[id+0] = "linkZ";    bufferIDs[id+0] = BUFFER_LINK_Z;    bufElemNb[id+0]=Nlink;
            fnameSuffix[id+1] = "linkZD";   bufferIDs[id+1] = BUFFER_LINK_ZD;   bufElemNb[id+1]=Nlink;
            fnameSuffix[id+2] = "linkF";    bufferIDs[id+2] = BUFFER_LINK_F;    bufElemNb[id+2]=Nlink;
            dd->bufferNb += 3;            
        }

        // buffer properties for joint/force associated to driven joints
        if(nqc){
            int id = dd->bufferNb;
            // A column is saved for each joint, even if there are not driven !
            fnameSuffix[id+0] = "Qc";    bufferIDs[id+0] = BUFFER_QC;    bufElemNb[id+0]=njoint;
            dd->bufferNb += 1;            
        }

        // set the filename if not specified
        if(dd->options->resfilename == NULL){
            resfilename = "dirdyn";
        }
        else{
            resfilename = dd->options->resfilename;
        }

        // set the path to result files if not specified
        if(dd->options->respath == NULL)
        {
            #ifdef UNIX
                respath = "../../resultsR";
            #else
                printf("Error: respath undefined ! You can set it in the main file, for instance with ");
                printf("'mbs_dirdyn->options->respath = PROJECT_SOURCE_DIR\"/../resultsR\";' ");
                exit(EXIT_FAILURE);
            #endif
        }
        else{
            respath = dd->options->respath;
        }

        // set the path to anim file if not specified
        if(dd->options->animpath == NULL)
        {
            #ifdef UNIX
                animpath = "../../animationR";
            #else
                printf("Error: animpath undefined ! You can set it in the main file, for instance with ");
                printf("'mbs_dirdyn->options->animpath = PROJECT_SOURCE_DIR\"/../animationR\";' ");
                exit(EXIT_FAILURE);
            #endif
        }
        else{
            animpath = dd->options->animpath;
        }

        // set the buffer size if not specified
        if (dd->options->buffersize<1){
            // compute size assuming a constant time step size
            dd->options->buffersize = (int)(dd->options->tf - dd->options->t0)/
                                      (dd->dt * dd->options->saveperiod)
                                      + 1;
            // add some extra space
            dd->options->buffersize *= 1.1;
            dd->options->buffersize += 10;
        }

        
        // initialize the buffers for saving results
        
        dd->buffers = (MbsBuffer**)malloc(dd->bufferNb*sizeof(MbsBuffer*));

        f      = (char*)malloc(sizeof(char)*(strlen(respath) +strlen(resfilename)+30));
        f_anim = (char*)malloc(sizeof(char)*(strlen(animpath)+strlen(resfilename)+30));

        // animation path

        

        for(bufId=0 ; bufId < dd->bufferNb; bufId++){
            sprintf(f,      "%s/%s_%s.res",  respath,  resfilename, fnameSuffix[bufId]);
            sprintf(f_anim, "%s/%s_%s.anim", animpath, resfilename, fnameSuffix[bufId]);

            dd->buffers[bufId] = mbs_new_buffer(f, f_anim, bufElemNb[bufId], dd->options->buffersize, bufferIDs[bufId], dd->options->save_anim, dd->options->save_visu, 1./(double)dd->options->framerate);
        }

        if (dd->options->save_visu)
        {
            #ifdef JAVA
            if (!dd->options->realtime)
            {
                printf("Error: real-time features must be activated to set 'save_visu' to 1 ! \n");
                exit(EXIT_FAILURE);
            }

            realtime = (Simu_realtime*) mbs_data->realtime;

            if (!realtime->options->flag_visu)
            {
                printf("Error: flag_visu must be set to 1 to set 'save_value' to 1 ! \n");
            }

            java = realtime->ext->java;
            dd->buffer_visu = (MbsBuffer**) malloc(realtime->options->nb_models*sizeof(MbsBuffer*));

            for(i=0; i<realtime->options->nb_models; i++)
            {
                sprintf(f,      "%s/visu_%d_q.res",  respath, i);
                sprintf(f_anim, "%s/visu_%d_q.anim", animpath, i);

                dd->buffer_visu[i] = mbs_new_buffer(f, f_anim, java->nb_q[i], dd->options->buffersize, BUFFER_VISU, dd->options->save_anim, dd->options->save_visu, 1./(double)dd->options->framerate);
            }

            #else
            printf("Error: Java libraries must be activated to set 'save_visu' to 1 ! \n");
            exit(EXIT_FAILURE);
            #endif
        }

        free(f_anim);
        free(f);

        // 'auto_output' structure initialized
        init_set_output(dd->options->max_save_user);

        // growing buffer for user variables
        dd->user_buffer = mbs_new_growing_buffer(dd->options->max_save_user, dd->options->buffersize, respath);

        // binding buffers to data that must be saved
        dd->savedArrays = (double**) malloc(dd->bufferNb*sizeof(double*));
        bufId = 0;  dd->savedArrays[bufId] = mbs_data->q+1;
        bufId++  ;  dd->savedArrays[bufId] = mbs_data->qd+1;
        bufId++  ;  dd->savedArrays[bufId] = mbs_data->qdd+1;
        bufId++  ;  dd->savedArrays[bufId] = mbs_data->Qq+1;
        if(Nux){
            bufId++  ;  dd->savedArrays[bufId] = mbs_data->ux+1;
            bufId++  ;  dd->savedArrays[bufId] = mbs_data->uxd+1;
        }
        if(Nlink){
            bufId++  ;  dd->savedArrays[bufId] = mbs_data->Z +1;
            bufId++  ;  dd->savedArrays[bufId] = mbs_data->Zd+1;            
            bufId++  ;  dd->savedArrays[bufId] = mbs_data->Fl+1;            
        }
        if(nqc){
            bufId++  ;  dd->savedArrays[bufId] = mbs_data->Qc +1;           
        }

        // save the initial config
        mbs_dirdyn_save(dd, mbs_data, mbs_data->tsim);
    }
}

void mbs_dirdyn_loop(MbsDirdyn* dd, MbsData* mbs_data)
{
    FILE *animFile;
    FILE *fileout_dopri5;
    MbsDirdynOptDopri5 *dopr_opt;
    int i;
    double cur_t0, cur_tf;

    // NUMERICAL INTEGRATION
    // - - - - - - - - - - -

    dopr_opt = dd->options->dopri5;

    if (dopr_opt->flag_use)
    {
        if (dopr_opt->verbose)
        {
            fileout_dopri5 = stdout;
        }
        else
        {
            fileout_dopri5 = NULL;
        }

        cur_t0 = dd->options->t0;

        if (dopr_opt->flag_waypoint)
        {
            cur_tf = cur_t0 + dopr_opt->delta_t_wp;
        }
        else
        {
            cur_tf = dd->options->tf;
        }

        while (cur_tf <= dd->options->tf)
        {
            loop_dopri5(cur_t0, cur_tf, dd->options->dt0, dd->dopri5->alloc_tab, mbs_data, dd, fileout_dopri5);

            cur_t0 = cur_tf;
            cur_tf += dopr_opt->delta_t_wp;

            // stop the simulation if 'flag_stop' set to 1
            if (mbs_data->flag_stop)
            {
                break;
            }
        }
    }
    else
    {
        while(dd->tsim < dd->options->tf)
        {

            // user loop
            user_dirdyn_loop(mbs_data, dd);

            mbs_fct_dirdyn(dd->tsim, dd->y, dd->yd, mbs_data, dd);
            rk4(dd->y, dd->yd, dd->nState, dd->tsim, dd->dt, dd->yout, mbs_fct_dirdyn, mbs_data, dd);

            dd->tsim += dd->dt;

            for (i=0 ; i<dd->nState ; i++)
            {
                dd->y[i] = dd->yout[i];
            }

            // save data + real-time features
            save_realtime_update(dd, mbs_data);

            // stop the simulation if 'flag_stop' set to 1
            if (mbs_data->flag_stop)
            {
                break;
            }

            // printf("DIRDYN: current time: %f \n", dd->tsim);
        }
    }
}

void mbs_dirdyn_finish(MbsDirdyn* dd, MbsData* mbs_data){
    int i;

    #ifdef REAL_TIME
    Simu_realtime *realtime;
    #endif

    // ENDING SIMULATION
    // - - - - - - - -

    // real-time structure release
    #ifdef REAL_TIME
    realtime = (Simu_realtime*) mbs_data->realtime;

    if (dd->options->realtime)
    {
        mbs_realtime_finish((Simu_realtime*) mbs_data->realtime);
    }
    #endif

    // user finalization
    user_dirdyn_finish(mbs_data, dd);

    // release memory allocated for dopri5
    if (dd->options->dopri5->flag_use)
    {
        free(dd->dopri5->dydt_save);

        for(i=0; i<DOPRI5_NB_ALLOC_VEC; i++)
        {
            free(dd->dopri5->alloc_tab[i]);
        }
        free(dd->dopri5->alloc_tab);
    }
    else
    {
        free(dd->rk4->yt);
        free(dd->rk4->dyt);
        free(dd->rk4->dym);
    }

    free(dd->y);
    free(dd->yd);
    free(dd->yout);

    if(dd->options->save2file)
    {
        // write the buffer (in case they were not saved automatically)
        mbs_dirdyn_write_buffers(dd);

        // free memory used by buffers
        for(i=0 ; i < dd->bufferNb; i++){
            mbs_delete_buffer(dd->buffers[i]);
        }
        free(dd->buffers);

        #ifdef REAL_TIME
        if (dd->options->save_visu)
        {
            for(i=0; i<realtime->options->nb_models; i++)
            {
                mbs_buffer_write(dd->buffer_visu[i]);
                mbs_delete_buffer(dd->buffer_visu[i]);
            }
            free(dd->buffer_visu);
        }
        #endif

        // user inputs final gestion
        mbs_growing_buffer_write(dd->user_buffer);

        mbs_delete_growing_buffer(dd->user_buffer);

        // release memory for the 'auto_output' structure
        free_set_output();

        free(dd->savedArrays);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void mbs_dirdyn_write_buffers(MbsDirdyn* dd){
    int i;
    for (i=0 ; i < dd->bufferNb ; i++){
        mbs_buffer_write(dd->buffers[i]);
    }    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*! \brief direct dynamics derivative computation
 *
 * \param[in] tsim current simulation time [s]
 * \param[in] y state vector of size n
 * \param[out] dydt derivative
 * \param[in,out] s Robotran main structure
 * \param[in,out] dd direct dynamic main module structure
 */
void mbs_fct_dirdyn(double tsim, double y[], double dydt[], MbsData *s, MbsDirdyn *dd)
{
    int i;

    s->tsim = tsim;

    // Update state variables
    for(i=1;i<=s->nqu;i++)
    {
        s->q[s->qu[i]] = y[i-1];
        s->qd[s->qu[i]] = y[i+s->nqu-1];
    }
    for(i=1;i<=s->Nux;i++)
    {
        s->ux[i] = y[i+2*s->nqu-1];
    }

    if (dd->options->accelred) // Full symbolic computation of qdd (including solving constraint, inv(M), ...)
    {
        i =  mbs_accelred(s, tsim);
    }
    else // Numerical solving of constraints and inv(M)
    {
        i = dirdynared(dd->mbs_aux,s);
    }

    if(i<0){
        printf("Loop closing Error : NR iteration overrun !\n");
        exit(-1);
    }
    // User Derivatives
    if(s->Nux>0) user_Derivative(s);

    // Update state vector
    for(i=1;i<=s->nqu;i++)
    {
        dydt[i-1]        = s->qd [s->qu[i]];
        dydt[i+s->nqu-1] = s->qdd[s->qu[i]];
    }
    for(i=1;i<=s->Nux;i++)
    {
        dydt[i+2*s->nqu-1] = s->uxd[i];
    }
}

/*! \brief update the real-time and saving modules if requested
 *
 * \param[in] dd direct dynamics module
 * \param[in] mbs_data Robotran main structure
 */
void save_realtime_update(MbsDirdyn* dd, MbsData* mbs_data)
{
    #ifdef REAL_TIME
    Simu_realtime *realtime;
    #endif

    // save results to the buffer
    if(dd->options->save2file)
    {
        dd->savePeriodCounter++;

        if(dd->savePeriodCounter%dd->options->saveperiod == 0)
        {
            mbs_dirdyn_save(dd, mbs_data, mbs_data->tsim);
        }
    }

    // real-time modules
    #ifdef REAL_TIME
    if (dd->options->realtime)
    {
        realtime = (Simu_realtime*) mbs_data->realtime;

        // update real-time buffers
        mbs_realtime_update(realtime, mbs_data->tsim);

        // real-time loop iteration
        mbs_realtime_loop(realtime, mbs_data->tsim);

        // quit the simulation
        if (realtime->simu_quit)
        {
            mbs_data->flag_stop = 1;
        }
    }
    #endif
}

/*! \brief dopri5 loop (one call to dopri 5)
 *
 * \param[in] cur_t0 initial time [s]
 * \param[in] cur_tf final time [s]
 * \param[in] dt0 first time step [s]
 * \param[in,out] dopri5_alloc_tab allocated vectors for dopri5
 * \param[in,out] s Robotran main structure
 * \param[in,out] dd direct dynamic main module structure
 * \param[out] fileout_dopri5 fileout for dopri5 fprintf
 * \return last time step used [s]
 */
double loop_dopri5(double cur_t0, double cur_tf, double dt0, double **dopri5_alloc_tab, MbsData *s, MbsDirdyn *dd, FILE *fileout_dopri5)
{
    double hout;

    dopri5(dd->nState, fcn_dopri5, cur_t0, dd->y, cur_tf, &(dd->options->dopri5->rtoler), &(dd->options->dopri5->atoler),
            0, solout_dopri5, 1, fileout_dopri5, 0.0, 0.0, 0.0, 0.0, 0.0, dd->options->dopri5->dt_max, dt0, dd->options->dopri5->nmax,
            1, 0, 0, NULL, 0, dopri5_alloc_tab, s, dd, &hout);

    return hout;
}

/*! \brief dopri5 derivative function
 *
 * \param[in] n dimension of the system [-]
 * \param[in] nr solour iteration [-]
 * \param[in] tsim current simulation time [s]
 * \param[in] y state vector of size n
 * \param[out] dydt derivative of state vector y
 * \param[in,out] s Robotran main structure
 * \param[in,out] dd direct dynamic main module structure
 */
void fcn_dopri5(unsigned n, long nr, double tsim, double y[], double dydt[], MbsData *s, MbsDirdyn *dd)
{
    int i;
    double *dydt_save;

    dydt_save = dd->dopri5->dydt_save;

    if ( (!nr) && dd->dopri5->flag_save ) // use previously computed solution dor first time step
    {
        for(i=0; i<n; i++)
        {
            dydt[i] = dydt_save[i];
        }
    }
    else // compute new solution
    {
        mbs_fct_dirdyn(tsim, y, dydt, s, dd);

        if (nr == 6)
        {
            for(i=0; i<n; i++)
            {
                dydt_save[i] = dydt[i];
            }

            dd->dopri5->flag_save = 1;
        }
    }
}

/*! \brief dopri5 simulation own gestion at each succesful step
 *
 * \param[in] nr solour iteration [-]
 * \param[in] tsim_old last time solout was called [s]
 * \param[in] tsim current simulation time [s]
 * \param[in] y state vector of size n
 * \param[in] n dimension of the system [-]
 * \param[out] irtrn set a negative value to stop the simulation
 * \param[in] init_flag 1 if initial call to solout, 0 otherwise
 * \param[in,out] s Robotran main structure
 * \param[in,out] dd direct dynamic main module structure
 */
void solout_dopri5(long nr, double tsim_old, double tsim, double y[], unsigned n, int* irtrn, int init_flag, MbsData *s, MbsDirdyn *dd)
{
    // when using waypoints, only waypoints are used if asked
    if ((!init_flag) && dd->options->dopri5->flag_waypoint && dd->options->dopri5->flag_solout_wp)
    {
        return;
    }

    // only call new values (used for waypoints method)
    if (tsim > dd->dopri5->solout_last_t)
    {
        dd->dopri5->solout_last_t = tsim;

         // user loop
        user_dirdyn_loop(s, dd);

        // save data + real-time features
        save_realtime_update(dd, s);

        // quit the simulation if requested
        *irtrn = (s->flag_stop) ? -1 : 1;
    }
}
