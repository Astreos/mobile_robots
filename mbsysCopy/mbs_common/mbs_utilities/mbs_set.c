/**
 * @file mbs_set.c
 *
 *
 * Creation date: 11/06/2015
 * @author Olivier Lantsoght (based on the work of other from CEREM)
 *
 *
 * (c) Universite catholique de Louvain
 */

#include "mbs_set.h"
#include <stdio.h>
#include <stdlib.h>
#include "useful_functions.h"

void mbs_set_nb_userc(MbsData *mbs_data, int Nuserc)
{
    int i;

    mbs_data->Nuserc = Nuserc;
    mbs_data->Ncons  = mbs_data->Nloopc + mbs_data->Nuserc;
    mbs_data->nhu    = mbs_data->Ncons;

    // re-initialize hu vector
    if(mbs_data->hu!=NULL)
    {
        free(mbs_data->hu);
    }
    mbs_data->hu = (int*) malloc((mbs_data->nhu+1)*sizeof(int));
    mbs_data->hu[0] = mbs_data->nhu;
    for (i=1; i<=mbs_data->nhu; i++)
    {
        mbs_data->hu[i] = i;
    }
}

void mbs_set_qu(MbsData *mbs_data, int qu)
{
    if(qu > mbs_data->njoint){
        printf("mbs_set_qu: Warning, specified joint index is not in project dimension.\n");
        printf("            qu=%d > mbs_data->njoint=%d\n", qu, mbs_data->njoint);
        return;
    }

    // add element
    mbs_data->nqu = add_mbs_q_elem(mbs_data->qu, mbs_data->nqu, qu);

    // remove element
    mbs_data->nqv = remove_mbs_q_elem(mbs_data->qv, mbs_data->nqv, qu);
    mbs_data->nqc = remove_mbs_q_elem(mbs_data->qc, mbs_data->nqc, qu);
    mbs_data->nqdriven = remove_mbs_q_elem(mbs_data->qdriven, mbs_data->nqdriven, qu);
    mbs_data->nqlocked = remove_mbs_q_elem(mbs_data->qlocked, mbs_data->nqlocked, qu);
}

void mbs_set_qv(MbsData *mbs_data, int qv)
{
    if(qv > mbs_data->njoint){
        printf("mbs_set_qv: Warning, specified joint index is not in project dimension.\n");
        printf("            qv=%d > mbs_data->njoint=%d\n", qv, mbs_data->njoint);
        return;
    }

    // add element
    mbs_data->nqv = add_mbs_q_elem(mbs_data->qv, mbs_data->nqv, qv);

    // remove element
    mbs_data->nqu = remove_mbs_q_elem(mbs_data->qu, mbs_data->nqu, qv);
    mbs_data->nqc = remove_mbs_q_elem(mbs_data->qc, mbs_data->nqc, qv);
    mbs_data->nqdriven = remove_mbs_q_elem(mbs_data->qdriven, mbs_data->nqdriven, qv);
    mbs_data->nqlocked = remove_mbs_q_elem(mbs_data->qlocked, mbs_data->nqlocked, qv);
}

void mbs_set_qdriven(MbsData *mbs_data, int qdriven)
{
    if(qdriven > mbs_data->njoint){
        printf("mbs_set_qdriven: Warning, specified joint index is not in project dimension.\n");
        printf("                 qdriven=%d > mbs_data->njoint=%d\n", qdriven, mbs_data->njoint);
        return;
    }

    // add element
    mbs_data->nqc = add_mbs_q_elem(mbs_data->qc, mbs_data->nqc, qdriven);
    mbs_data->nqdriven = add_mbs_q_elem(mbs_data->qdriven, mbs_data->nqdriven, qdriven);

    // remove element
    mbs_data->nqu = remove_mbs_q_elem(mbs_data->qu, mbs_data->nqu, qdriven);
    mbs_data->nqv = remove_mbs_q_elem(mbs_data->qv, mbs_data->nqv, qdriven);
    mbs_data->nqlocked = remove_mbs_q_elem(mbs_data->qlocked, mbs_data->nqlocked, qdriven);
}

void mbs_set_qlocked(MbsData *mbs_data, int qlocked)
{
    if(qlocked > mbs_data->njoint){
        printf("mbs_set_qlocked: Warning, specified joint index is not in project dimension.\n");
        printf("                 qlocked=%d > mbs_data->njoint=%d\n", qlocked, mbs_data->njoint);
        return;
    }

    // add element
    mbs_data->nqc = add_mbs_q_elem(mbs_data->qc, mbs_data->nqc, qlocked);
    mbs_data->nqlocked = add_mbs_q_elem(mbs_data->qlocked, mbs_data->nqlocked, qlocked);

    // remove element
    mbs_data->nqu = remove_mbs_q_elem(mbs_data->qu, mbs_data->nqu, qlocked);
    mbs_data->nqv = remove_mbs_q_elem(mbs_data->qv, mbs_data->nqv, qlocked);
    mbs_data->nqdriven = remove_mbs_q_elem(mbs_data->qdriven, mbs_data->nqdriven, qlocked);
}

void udpate_nq(MbsData *mbs_data, int new_nqu, int new_nqv, int new_nqc, int new_nqdriven, int new_nqlocked)
{
    mbs_data->nqu = new_nqu;
    mbs_data->nqv = new_nqv;
    mbs_data->nqc = new_nqc;
    mbs_data->nqdriven = new_nqdriven;
    mbs_data->nqlocked = new_nqlocked;

    // check
    if (mbs_data->nqdriven + mbs_data->nqlocked != mbs_data->nqc)
    {
        printf("Error in mbs_set::udpate_nq(): nqdriven (%d) + nqlocked (%d) != nqc (%d) !\n", mbs_data->nqdriven, mbs_data->nqlocked, mbs_data->nqc);
        exit(EXIT_FAILURE);
    }

    if (mbs_data->nqu + mbs_data->nqv + mbs_data->nqc != mbs_data->njoint)
    {
        printf("Error in mbs_set::udpate_nq(): nqu (%d) + nqv (%d) + nqc (%d) != njoint (%d) !\n", mbs_data->nqu, mbs_data->nqv, mbs_data->nqc, mbs_data->njoint);
        exit(EXIT_FAILURE);
    }
}

int add_mbs_q_elem(int *q_vec, int nq, int new_q)
{
    // safety vector should already been allocated 
    if (q_vec == NULL)
    {
        printf("Error in mbs_set::add_mbs_q_elem(): Vector not initialized, should be already initialized.\n");
        exit(EXIT_FAILURE);
    }

    // nothing yet in this vector
    if (!nq)
    {
        q_vec[1] = new_q;
        return 1;
    }
    
    // Look if new_q belongs to q_vec
    if (find_ivec_1(q_vec, nq, new_q) == -1){
        // Case it was not found, we add it
        q_vec[nq+1] = new_q;
        nq =nq+1;
    }
    // We sort vector
    sort_inplace_ivec_1(q_vec, nq);
    
    return nq;
}

int remove_mbs_q_elem(int *q_vec, int nq, int old_q)
{
    int index;
    // safety vector should already been allocated 
    if (q_vec == NULL)
    {
        printf("Error in mbs_set::remove_mbs_q_elem(): Vector not initialized, should be already initialized.\n");
        exit(EXIT_FAILURE);
    }
    
    // nothing in the vector
    if (!nq)
    {
        return nq;
    }
    
    // Look if old_q belongs to q_vec
    index = find_ivec_1(q_vec, nq, old_q);
    if (index != -1){
        // Case it was found, we remove it
        q_vec[index] = q_vec[nq];
        q_vec[nq] = 0;
        nq =nq-1;
    }
    // We sort vector
    sort_inplace_ivec_1(q_vec, nq);
    
    return nq;
}

void print_mbs_q_all(MbsData *mbs_data)
{
    printf("qu    ->   ");
    print_mbs_q_vec(mbs_data->qu, mbs_data->nqu);
    printf("qv    ->   ");
    print_mbs_q_vec(mbs_data->qv, mbs_data->nqv);
    printf("qc    ->   ");
    print_mbs_q_vec(mbs_data->qc, mbs_data->nqc);
    printf("qdriven -> ");
    print_mbs_q_vec(mbs_data->qdriven, mbs_data->nqdriven);
    printf("qlocked -> ");
    print_mbs_q_vec(mbs_data->qlocked, mbs_data->nqlocked);
}

void print_mbs_q_vec(int *q_vec, int nq)
{
    int i;

    if (nq)
    {
        printf("nb: %d - q: [%d]", nq, q_vec[0]);

        for(i=1; i<=nq; i++)
        {
            printf(" ; %d", q_vec[i]);
        }

        printf("\n");
    }
    else
    {
        printf("NULL\n");
    }
}
