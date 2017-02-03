

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "mbs_lmgc.h"
#include "mbs_load_xml.h"
#include "mbs_project_interface.h"
#include "nrfct.h"


//#define DEBUG_PRINT


MbsLmgcSystem* globalSystem = NULL;

void nullifyArray(double* a, int size){
    int i;
    for(i=0; i<size; i++){
        a[i] = 0.0;
    }
}

void printArray(char* filepath, double time, double* a, int size){
    FILE* f;
    int i;
    f = fopen(filepath, "a");
    fprintf(f, "%f  ", time);
    for(i=0; i<size; i++){
        fprintf(f, "%f  ", a[i]);
    }
    fprintf(f, "  \n");
    fclose(f);
}


void mbs_lmgc_set_nb_mbs(int nb){

    int oldNb, i;

    printf("set_nb_mbs(%d)\n", nb);

    // initialize the global structure if needed
    if (globalSystem==NULL){
        globalSystem = (MbsLmgcSystem*)malloc(sizeof(MbsLmgcSystem));
        globalSystem->nb_mbs = 0;
    }

    oldNb=globalSystem->nb_mbs;

    if(oldNb>0){
        printf("WARNING: The number of mbs is already set to %d and can not be changed anymore.\n", oldNb);
        return;
    }
    else{
        globalSystem->mbs_lmgc_s = (MbsLmgcData*)malloc(nb*sizeof(MbsLmgcData));

        globalSystem->nb_mbs = nb;

        for(i=0; i<nb; i++){
            MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
            mld->mbsFile    = NULL;
            mld->mbs_data_loaded = -1;
            mld->resqFile   = NULL;
            mld->resqdFile  = NULL;
            mld->resQqFile  = NULL;
            mld->animFile   = NULL;
            mld->nb_nodes = 0;
            mld->nodes = NULL;
        }

    }
}

int mbs_lmgc_checkMbsIndex(int iMbs){
    if (iMbs>=globalSystem->nb_mbs){
        printf("WARNING: the specified mbs index %d +1 is greater than the number of mbs %d in the system. \n", iMbs, globalSystem->nb_mbs);
        return -1;
    }
    return 1;
}


void mbs_lmgc_set_filename(int iMbs, char** filenamePtr, char* filename){
    if(mbs_lmgc_checkMbsIndex(iMbs)<0) return;

    if( (*filenamePtr)!=NULL) free(*filenamePtr);

    (*filenamePtr) = strdup(filename);
}

void mbs_lmgc_set_mbsdata(int iMbs, MbsData* mbs_data){
    #ifdef DEBUG_PRINT
    printf("set_mbs_data(%d, %s)\n", iMbs, mbs_data->mbs_filename);
    #endif // DEBUG_PRINT
    
    MbsLmgcData* mld;
    
    mbs_lmgc_set_filename(iMbs, &(globalSystem->mbs_lmgc_s[iMbs].mbsFile) ,mbs_data->mbs_filename);
    
    mld = globalSystem->mbs_lmgc_s+iMbs;
    mld->mbs_data = mbs_data;
    mld->mbs_data_loaded = 0;
}

void mbs_lmgc_set_mbs_file(int iMbs, char* filename){
    #ifdef DEBUG_PRINT
    printf("set_mbs_file(%d, %s)\n", iMbs, filename);
    #endif // DEBUG_PRINT
    
    mbs_lmgc_set_filename(iMbs, &(globalSystem->mbs_lmgc_s[iMbs].mbsFile) ,filename);

    printf("Path to mbs filename for mbs number %d set to %s.\n", iMbs, globalSystem->mbs_lmgc_s[iMbs].mbsFile);
}

void mbs_lmgc_set_res_file(int iMbs, char* filename){

    char* f;
    #ifdef DEBUG_PRINT
    printf("set_res_file(%d, %s)\n", iMbs, filename);
    #endif // DEBUG_PRINT
    
    // allocate a char* with more space for appending the suffix and extension
    f = (char*)malloc(sizeof(char)*(strlen(filename)+10));

    // set the res file for coordinates
    strcpy(f, filename);
    strcat(f, "_q.res");
    mbs_lmgc_set_filename(iMbs, &(globalSystem->mbs_lmgc_s[iMbs].resqFile) ,f);

    // set the res file for velocities
    strcpy(f, filename);
    strcat(f, "_qd.res");
    mbs_lmgc_set_filename(iMbs, &(globalSystem->mbs_lmgc_s[iMbs].resqdFile) ,f);

    // set the res file for joint forces
    strcpy(f, filename);
    strcat(f, "_Qq.res");
    mbs_lmgc_set_filename(iMbs, &(globalSystem->mbs_lmgc_s[iMbs].resQqFile) ,f);

    free(f);

    printf("Path to results filename for mbs number %d set to %s.\n", iMbs, globalSystem->mbs_lmgc_s[iMbs].resqFile);
}



void mbs_lmgc_set_anim_file(int iMbs, char* filename){
    #ifdef DEBUG_PRINT
    printf("set_res_file(%d, %s)\n", iMbs, filename);
    #endif // DEBUG_PRINT
    
    mbs_lmgc_set_filename(iMbs, &(globalSystem->mbs_lmgc_s[iMbs].animFile) ,filename);

    printf("Path to anim filename for mbs number %d set to %s.\n", iMbs, globalSystem->mbs_lmgc_s[iMbs].animFile);
}

void mbs_lmgc_free_node_list(int nbNodes, MbsLmgcNode* nodeList){
    int j,k;

    for (j=0 ; j<nbNodes ; j++){
        if (nodeList[j].sensor!=NULL){
            free_sensor(nodeList[j].sensor);
        }
        for (k=0;k<6;k++){
            if(nodeList[j].JRed[k]!=NULL){
                free(nodeList[j].JRed[k]);
            }
        }
    }
    free(nodeList);
}

void mbs_lmgc_set_node_sensor_ids(int iMbs, int nbNodes, int* sensorIds){

    int i, k;
    MbsLmgcData* mld;

    printf("set_node_sensor_id(%d, %d, [", iMbs, nbNodes);
    for(i=0;i<nbNodes;i++) printf("%d  ", sensorIds[i]);
    printf("])\n");

    if(mbs_lmgc_checkMbsIndex(iMbs)<0) return;

    mld = globalSystem->mbs_lmgc_s+iMbs;

    if(mld->nodes !=NULL) mbs_lmgc_free_node_list(mld->nb_nodes, mld->nodes);

    mld->nb_nodes = nbNodes;

    mld->nodes = (MbsLmgcNode*)malloc(nbNodes*sizeof(MbsLmgcNode));

    for(i=0; i<nbNodes; i++){
        mld->nodes[i].sensorId = sensorIds[i];
        mld->nodes[i].sensor = NULL;
        for (k=0;k<6;k++) mld->nodes[i].JRed[k]=NULL;
    }
}

int mbs_lmgc_get_node_nb(int iMbs){

    if(mbs_lmgc_checkMbsIndex(iMbs)<0) return -1;

    return globalSystem->mbs_lmgc_s[iMbs].nb_nodes;
}


MbsData* mbs_lmgc_initialize_data(char* mbsFilename){

    MbsData *mbs_data;
    MbsPart *mbs_part;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    /*                       LOADING                             *
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


    printf("Loading the *.mbs file: %s !\n", mbsFilename);
    mbs_data = mbs_load(mbsFilename, "build");
    

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    /*                     COORD PART                            *
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


    //if(mbs_data->Ncons>0){
        mbs_part = mbs_new_part(mbs_data);
        mbs_part->options->rowperm=1;
        mbs_run_part(mbs_part, mbs_data);

        mbs_delete_part(mbs_part);
    //}

    return mbs_data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_initialize(){
	
    int i, j, k, nbNodes, nbJoint, nqu, nquc;
    MbsLmgcData* mld;
    MbsData *md;  
    MbsAux  *ma;
    
    
    printf("mbs_lmgc_initialize() \n");

    // number of multibody system involved in the problem
    int nbMbs = globalSystem->nb_mbs;


    for (i=0 ; i<nbMbs ; i++){
        mld = globalSystem->mbs_lmgc_s+i;

        if (mld->mbs_data_loaded == -1){
            // initialize MbsData structures and load mbs file
            mld->mbs_data = mbs_lmgc_initialize_data(mld->mbsFile);
            mld->mbs_data_loaded = 1;
        }

        // initialize mbs local data structure
        mld->lds = initMbsAux(mld->mbs_data);

        md = mld->mbs_data;
        ma = mld->lds;
        
        nbJoint = md->njoint; 
        nqu = md->nqu;
        nquc = ma->iquc[0];

        // initialize nodes bearing contactors
        nbNodes = mld->nb_nodes;

        for (j=0 ; j<nbNodes ; j++){
            mld->nodes[j].sensor = (MbsSensor*)malloc(sizeof(MbsSensor));
            allocate_sensor(mld->nodes[j].sensor, nbJoint);
            init_sensor(mld->nodes[j].sensor, nbJoint);

            for (k=0 ; k<6 ; k++) {
                mld->nodes[j].JRed[k] = (double*) calloc(nquc,sizeof(double));
            }
        }

        // initialize vectors (size = nb of independent coordinates)
        mld->qm    = (double*) malloc(nqu*sizeof(double));
        mld->vBeg  = (double*) malloc(nquc*sizeof(double));
        mld->vFree = (double*) malloc(nquc*sizeof(double));
        mld->vAux  = (double*) malloc(nquc*sizeof(double));
        mld->reac  = (double*) malloc(nqu*sizeof(double));
        mld->rAux  = (double*) malloc(nqu*sizeof(double));

        nullifyArray(mld->qm,    nqu);
        nullifyArray(mld->vFree, nquc);
        nullifyArray(mld->vAux,  nquc);
        nullifyArray(mld->reac,  nqu);
        nullifyArray(mld->rAux,  nqu);

        // init vBeg to the initial value of qd
        for (j=0; j<nquc; j++){
            mld->vBeg[j] = md->qd[ma->iquc[j+1]];
        }
        
        /* Preparing the buffers for the results. Saved quantities are:
         *  - Generalized coordinate (q);
         *  - Generalized velocities (qd);
         *  - Generalized joints forces/torques (Qq);
         */
        mld->bufferNb = 3;
        mld->buffersize = 10000; // Arbitrary dimension as number of steps is unknown
        
        // Some value for readability of function call that are always true or false in our simulation
        int save_anim = 1;    // We always write animation file
        int save_visu = 0;    // We are not compatible with real-time needed for this option
        int framerate = 1000; // Frame rate is always set to 1000 for compatibility with MBsysPad visualization
        
        const int bufferIDs[6] = {BUFFER_Q, BUFFER_QD, BUFFER_QDD, BUFFER_QQ, BUFFER_UX, BUFFER_UXD}; // Automatic buffer ID, only [0],[1] and [2] are used.
        
        // Allocation of the buffers
        mld->buffers = (MbsBuffer**)malloc(mld->bufferNb*sizeof(MbsBuffer*));
        mld->buffers[0] = mbs_new_buffer(mld->resqFile, mld->animFile, md->njoint, mld->buffersize, bufferIDs[0], save_anim, save_visu, 1./(double)framerate); // Coordinate
        mld->buffers[1] = mbs_new_buffer(mld->resqdFile, mld->animFile, md->njoint, mld->buffersize, bufferIDs[1], save_anim, save_visu, 1./(double)framerate);// Velocities
        mld->buffers[2] = mbs_new_buffer(mld->resQqFile, mld->animFile, md->njoint, mld->buffersize, bufferIDs[3], save_anim, save_visu, 1./(double)framerate);// Forces-torques
        
        // Array with pointer to fields to be saved
        mld->savedArrays = (double**) malloc(mld->bufferNb*sizeof(double*));
        mld->savedArrays[0] = md->q+1;
        mld->savedArrays[1] = md->qd+1;
        mld->savedArrays[2] = md->Qq+1;
    }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_finalize(){
    int i,j, k;


    printf("mbs_lmgc_finalize() \n");


    for (i=0 ; i<globalSystem->nb_mbs ; i++){
        MbsLmgcData* mld;
        mld = globalSystem->mbs_lmgc_s+i;
        
        // Saving last configuration
        for (j=0 ; j < mld->bufferNb ; j++){
            mbs_buffer_save(mld->buffers[j], mld->mbs_data->tsim, mld->savedArrays[j]);
        }
        // write the buffer (in case they were not saved automatically)
        for (j=0 ; j < mld->bufferNb ; j++){
            mbs_buffer_write(mld->buffers[j]);
        }
        // free memory used by buffers
        for(j=0 ; j < mld->bufferNb; j++){
            mbs_delete_buffer(mld->buffers[j]);
        }
        free(mld->buffers);
        free(mld->savedArrays);

        free(mld->qm);
        free(mld->vBeg);
        free(mld->vFree);
        free(mld->vAux);
        free(mld->reac);
        free(mld->rAux);

        mbs_lmgc_free_node_list(mld->nb_nodes, mld->nodes);


        freeMbsAux(mld->lds, mld->mbs_data);
        if (mld->mbs_data_loaded == 1){
            // If mbs_lmgc create the mbs_data, it deallocate it.
            mbs_delete_data(mld->mbs_data);
        }
    }

    free(globalSystem->mbs_lmgc_s);
    free(globalSystem);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_increment(double tsim){
    int i, j;

    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_increment() \n");
    #endif

    for (i=0 ; i<globalSystem->nb_mbs ; i++){
        MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
        MbsData *md = mld->mbs_data;
        MbsAux  *ma = mld->lds;
        
        // Preparing next time step
        md->tsim = tsim;

        // Saving the configuration at the beginning of the timestep in the buffer
        for (j=0 ; j < mld->bufferNb ; j++){
            mbs_buffer_save(mld->buffers[j], md->tsim, mld->savedArrays[j]);
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_compute_free_vlocy(double h, double theta){
    int i,j;
    double *qc, *qdc, *qddc;
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_compute_free_vlocy(h=%f, theta=%f) \n", h, theta);    
    #endif
    
    for (i=0 ; i<globalSystem->nb_mbs ; i++){
        MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
        MbsData *md = mld->mbs_data;

        // compute the contact configuration and copy it to mbs_data.qu
        for(j=0; j<md->nqu ; j++){
            mld->qm[j] = md->q[md->qu[j+1]]+h*(1-theta)*md->qd[md->qu[j+1]];
            md->q[md->qu[j+1]] = mld->qm[j];
        }
        
        // close loops and compute the dynamics
        md->tsim += h*(1-theta); // The time modification is needed for correct computation of driven joints
        dirdynared(mld->lds, md);
        md->tsim -= h*(1-theta);
        // Driven joint position does not need to be stored in mld->qm vector because it is only used to compute independent joint final position.
        
        /* QUESTION 
         * 
         * Driven joint velocity and acceleration have been updated.
         * The independent joints have keep the velocity of time step begin (the acceleration has been updated).
         * The loops have been closed in velocity with the qd(u)of the beginning and qd(c) of the intermediate point. Same for torque/force computation
         * 
         * Do we want to keep this behavior?
         * 
         */
        
        // compute Vfree for independent joints
        for(j=0; j<md->nqu ; j++){
            mld->vFree[j] = md->qd[md->qu[j+1]] + h * md->qdd[md->qu[j+1]];
        }
        
        // compute Vfree for driven joints
        if(md->nqc>0)
        {
            // First step: saving values in qm configuration
            qc   = (double*) malloc(md->nqc*sizeof(double));
            qdc  = (double*) malloc(md->nqc*sizeof(double));
            qddc = (double*) malloc(md->nqc*sizeof(double));
            for(j=0 ; j<md->nqc ; j++)
            {
                qc[j]   = md->q[md->qc[j+1]];
                qdc[j]  = md->qd[md->qc[j+1]];
                qddc[j] = md->qdd[md->qc[j+1]];
            }
            // Second step: computing driven joints free velocity
            user_DrivenJoints(md,md->tsim+h);
            
            // Third step: store the result in vFree and restore qm configuration
            for(j=0 ; j<md->nqc ; j++)
            {
                mld->vFree[j+md->nqu] = md->qd[md->qc[j+1]];
                md->q[md->qc[j+1]]    = qc[j];
                md->qd[md->qc[j+1]]   = qdc[j];
                md->qdd[md->qc[j+1]]  = qddc[j];
            }
            free(qc);
            free(qdc);
            free(qddc);
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_update_nodes_3D(int i, double* coorTT, double* localFrameTT){
    int j,k,l,m;
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_update_nodes_3D(i=%d, coorTT[0]=%f, localFrameTT[0]=%f) \n", i, coorTT[0], localFrameTT[0]);    
    #endif
        
    MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
    MbsData *md = mld->mbs_data;
    MbsAux  *ma = mld->lds;

    // update nodes of the current mbs
    for(j=0; j<mld->nb_nodes ; j++){
        MbsLmgcNode* node = mld->nodes+j;
        
        // compute the sensor -> call symbolic file
        mbs_sensor(node->sensor, md, node->sensorId);
        
        // compute the reduced jacobian: Jred = Ju + Jv*Bvu
        for(k=0; k<6 ; k++){
            for(l=0; l < ma->iquc[0] ; l++){
                node->JRed[k][l] = node->sensor->J[k+1][ma->iquc[l+1]];
                for(m=0; m<md->nqv ; m++){
                    node->JRed[k][l] += node->sensor->J[k+1][md->qv[m+1]] * ma->Bvuc[m+1][l+1];
                }
            }
        }
        
        // update coorTT
        for(k=0; k<3; k++){
            coorTT[3*j+k] = node->sensor->P[k+1];
        }
        
        // update localFrameTT
        for(k=0; k<3; k++){
            for(l=0; l<3; l++){
                localFrameTT[9*j+3*k+l] = node->sensor->R[k+1][l+1];
                //localFrameTT[9*j+3*k+l] = node->sensor->R[l+1][k+1];
            }
        }
        
    }
}

void mbs_lmgc_update_nodes_2D(int i, double* coorTT){
    int j,k,l,m;
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_update_nodes_2D(i=%d, coorTT[0]=%f) \n", i, coorTT[0]); 
    #endif
        
    MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
    MbsData *md = mld->mbs_data;
    MbsAux  *ma = mld->lds;

    // update nodes of the current mbs
    for(j=0; j<mld->nb_nodes ; j++){
        MbsLmgcNode* node = mld->nodes+j;
        #ifdef DEBUG_PRINT
        printf("i_node=%d, Jred = ", j);
        #endif
        // compute the sensor -> call symbolic file
        mbs_sensor(node->sensor, md, node->sensorId);
        // compute the reduced jacobian: Jred = Ju + Jv*Bvu
        for(k=0; k<6 ; k++){
            for(l=0; l < ma->iquc[0] ; l++){
                node->JRed[k][l] = node->sensor->J[k+1][ma->iquc[l+1]];
                for(m=0; m<md->nqv ; m++){
                    node->JRed[k][l] += node->sensor->J[k+1][md->qv[m+1]] * ma->Bvuc[m+1][l+1];
                }
                #ifdef DEBUG_PRINT
                printf("%.3e ", node->JRed[k][l]);
                #endif // DEBUG_PRINT
            }
            #ifdef DEBUG_PRINT
            printf("\n                 ");
            #endif // DEBUG_PRINT
        }
        #ifdef DEBUG_PRINT
            printf("\n");
        #endif // DEBUG_PRINT
        
        // update coorTT
        coorTT[3*j  ] = node->sensor->P[1];
        coorTT[3*j+1] = node->sensor->P[2];
        coorTT[3*j+2] = atan2(node->sensor->R[1][2], node->sensor->R[1][1]);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_nullify_reac(int i_mbs){
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_nullify_reac(i_mbs=%d) \n", i_mbs);
    #endif 
    
    nullifyArray(globalSystem->mbs_lmgc_s[i_mbs].reac, globalSystem->mbs_lmgc_s[i_mbs].mbs_data->nqu);
}

void mbs_lmgc_nullify_rAux(int i_mbs){
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_nullify_rAux(i_mbs=%d) \n", i_mbs);
    #endif 
    
    nullifyArray(globalSystem->mbs_lmgc_s[i_mbs].rAux, globalSystem->mbs_lmgc_s[i_mbs].mbs_data->nqu);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Add the given node reaction to the reaction of the global mbs.
 * The given reac is thus projected on the generalized coordinates space:
 * reac_generalized = Jsens^T * reac_node
 * 
 * The array on which it is added is specified by the argument reacGlob
 * 
 * @param i_mbs the index of the mbs for which the reaction must be applied
 * @param i_node the index of the node on which the reaction is applied
 * @param reacNode a 6 element (fx, fy, fz, tx, ty, tz) array
 *        containing the components of the force and torque
 *        to apply on the node reference point. Components are
 *        given in the global frame.
 * @param reacGlob the pointer to the array on which the reac is added
 */
void mbs_lmgc_add_reaction(int i_mbs, int i_node, double* reacNode, double* reacGlob){

    int i, j;
    MbsLmgcNode* node = NULL;
    
    // get node
    node = globalSystem->mbs_lmgc_s[i_mbs].nodes+i_node;
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_add_reaction(i_mbs=%d, i_node=%d, Generalized independent reaction =[ ", i_mbs, i_node);
    #endif

    // compute Jsens^T * reac_node
	for(i=0 ; i < globalSystem->mbs_lmgc_s[i_mbs].mbs_data->nqu ; i++)
	{
		for(j=0; j<6 ; j++)
		{
			reacGlob[i] += node->JRed[j][i] * reacNode[j];
		}
		#ifdef DEBUG_PRINT
		printf("  %.3e  ", reacGlob[i]);
		#endif
	}
	#ifdef DEBUG_PRINT
	printf("]\n");
	#endif
}

void mbs_lmgc_add_reac(int i_mbs, int i_node, double* reacNode){

    
    
    #ifdef DEBUG_PRINT
    int i;
    printf("mbs_lmgc_add_reac(i_mbs=%d, i_node=%d", i_mbs, i_node);
    for (i=0 ; i<6 ; i++)
        printf(", reacNode[%d]=%+f",i, reacNode[i]);
    printf("\n");
    #endif
    
    mbs_lmgc_add_reaction(i_mbs, i_node, reacNode, globalSystem->mbs_lmgc_s[i_mbs].reac);
    
}

void mbs_lmgc_add_rAux(int i_mbs, int i_node, double* reacNode){
    
    #ifdef DEBUG_PRINT
    int i;
    printf("mbs_lmgc_add_rAux(i_mbs=%d, i_node=%d", i_mbs, i_node);
    for (i=0 ; i<6 ; i++)
        printf(", reacNode[%d]=%+f",i, reacNode[i]);
    printf("\n");
    #endif
    
    mbs_lmgc_add_reaction(i_mbs, i_node, reacNode, globalSystem->mbs_lmgc_s[i_mbs].rAux);
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_nullify_vFree(int i_mbs){

    nullifyArray(globalSystem->mbs_lmgc_s[i_mbs].vFree, globalSystem->mbs_lmgc_s[i_mbs].lds->iquc[0]);
        
}

void mbs_lmgc_nullify_vAux(int i_mbs){

    nullifyArray(globalSystem->mbs_lmgc_s[i_mbs].vAux, globalSystem->mbs_lmgc_s[i_mbs].lds->iquc[0]);
        
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_comp_vAux(int i_mbs){

    double *vlocy, *reac;
    MbsLmgcData* mld;
    
    vlocy = globalSystem->mbs_lmgc_s[i_mbs].vAux;
    reac  = globalSystem->mbs_lmgc_s[i_mbs].rAux;

    mld = globalSystem->mbs_lmgc_s + i_mbs;
    // solve M*vlocy = reac
    // WARING: cholsl uses array with index starting at 1
    cholsl(mld->lds->Mr, mld->mbs_data->nqu, mld->lds->p_Mr,
           reac-1, vlocy-1);


    #ifdef DEBUG_PRINT
    {
        int i;
        printf("vlocy = ");
        for (i=0; i<mld->mbs_data->nqu; i++){
            printf("%f  ", vlocy[i]);
        }
        printf(" \n");
        printf("reac = ");
        for (i=0; i<mld->mbs_data->nqu; i++){
            printf("%f  ", reac[i]);
        }
        printf(" \n");
    }
    #endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**
 * 
 * Compute the velocity of a node (linear and angular velocity) from
 * a given value of the generalized velocities. This is computed as
 *     vlocy_node = Jsens * vlocy_generalized
 * 
 * The array from which the velocity is computed by the argument vlocyGen.
 * 
 * 
 * @param i_mbs the index of the mbs from which the velocity is taken
 * @param i_node the index of the node for which the velocity is computed
 * @param vlocyNode a 6 element (vx, vy, vz, omx, omy, omz) array
 *        in which the components of the velocity will be stored.
 *        Components are computed in the global frame.
 * @param vlocyGen the array from which the velocity is computed
 * 
 *  
 */
void mbs_lmgc_get_velocity(int i_mbs, int i_node, double* vlocyNode, double* vlocyGen){

    int i, j;
    MbsLmgcNode* node;

    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_get_vlocy(i_mbs=%d, i_node=%d, vlocyGen=?) \n", i_mbs, i_node);
    #endif


    // get node
    node = globalSystem->mbs_lmgc_s[i_mbs].nodes+i_node;

    // compute Jsens * vlocyGen
	for(i=0 ; i < 6 ; i++)
	{
        vlocyNode[i] = 0. ;  
		for(j=0; j < globalSystem->mbs_lmgc_s[i_mbs].lds->iquc[0] ; j++)
		{
			vlocyNode[i] += node->JRed[i][j] * vlocyGen[j];
		}
	}


    #ifdef DEBUG_PRINT
    printf("vlocyNode = [");
    for (i=0 ; i<6 ; i++)
        printf("%+f, ", vlocyNode[i]);
    printf("] \n");
    #endif    
}


void mbs_lmgc_get_vBeg(int i_mbs, int i_node, double* vlocyNode){
    mbs_lmgc_get_velocity(i_mbs, i_node, vlocyNode, globalSystem->mbs_lmgc_s[i_mbs].vBeg);
}

void mbs_lmgc_get_vFree(int i_mbs, int i_node, double* vlocyNode){
    mbs_lmgc_get_velocity(i_mbs, i_node, vlocyNode, globalSystem->mbs_lmgc_s[i_mbs].vFree);
}

void mbs_lmgc_get_vAux(int i_mbs, int i_node, double* vlocyNode){
    mbs_lmgc_get_velocity(i_mbs, i_node, vlocyNode, globalSystem->mbs_lmgc_s[i_mbs].vAux);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_compute_dof(double h, double theta){
    int i,j;
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_compute_dof(h=%f, theta=%f) \n", h, theta);
    #endif
    
    for (i=0 ; i<globalSystem->nb_mbs ; i++){

        MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
        MbsData *md = mld->mbs_data;

        // solve M*vlocy = reac and store in vAux
        // WARNING: the cholsl function uses array with index starting at 1
        cholsl(mld->lds->Mr, md->nqu, mld->lds->p_Mr,
               mld->reac-1, mld->vAux-1);

        // add vFree and store in mbs_data->qd_u
        for (j=0 ; j < md->nqu ; j++){
            md->qd[md->qu[j+1]] = mld->vFree[j] + mld->vAux[j];
        }

        // update independent positions
        for (j=0 ; j < md->nqu ; j++){
            md->q[md->qu[j+1]] = mld->qm[j] + h * theta * md->qd[md->qu[j+1]];
        }
        
        // update driven joints positions
        user_DrivenJoints(md,md->tsim+h);
        // REMARK: Accelerations for driven joints are also updated.
        //         This has no impact because the acceleration is computed again (in qm configuration) before being used.
        
        // solve constraints
        if (md->nhu > 0)
        {
            int iter;

            // Geometric resolution (position)
            iter = mbs_close_geo(md, mld->lds);

            if (iter>=mld->lds->MAX_NR_ITER){
                printf("WARNING: loop constraints not solved after %d iterations.\n",iter);
            }

            // Kinematic resolution (velocities)
            mbs_close_kin(md, mld->lds);
        }
        // Update time to the end of step, to be coherent with the current values of joints
        md->tsim+=h;
    }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void mbs_lmgc_update_dof(){

    int i, j;
    
    #ifdef DEBUG_PRINT
    printf("mbs_lmgc_update_dof() \n");
    #endif
    
        
    for (i=0 ; i<globalSystem->nb_mbs ; i++){

        MbsLmgcData* mld = globalSystem->mbs_lmgc_s+i;
        MbsData *md = mld->mbs_data;
        MbsAux  *ma = mld->lds;

        // update vBeg array
        for (j=0; j<ma->iquc[0]; j++){
            mld->vBeg[j] = md->qd[ma->iquc[j+1]];
        }
    }

}
