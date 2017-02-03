
 /** \file mbs_buffer.c
  *
  * Implements function defined in mbs_buffer.h
  * 
  * @author Nicolas Docquier 
  *
  * (c) Universite catholique de Louvain
  */

#include "mbs_buffer.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "set_output.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 /** Macro to get the id in tx array corresponding to index (assuming b->nx exist)*/
#define TIME_ID(i)   ((i)*(b->nx+1))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MbsBuffer* mbs_new_buffer(char* filename, char* anim_name, int nx, int size, int id, int save_anim, int save_visu, double anim_period){
    
    MbsBuffer* b;
    FILE* fid;
    
    
    b = (MbsBuffer*) malloc(sizeof(MbsBuffer));
    
    
    // initialize name of associated text file
    b->filename = strdup(filename);
    
    // (re)-initialize the text file
    fid = fopen(b->filename, "w"); 
    if(fid == NULL)
    {
        printf("error in mbs_buffer_save: cannot open file '%s'\n", b->filename);
        return NULL;
    }else{
        fclose(fid);
    }    
    
    // initialize index to 0
    b->index = 0;
    
    // initialize sizes
    b->nx   = nx;
    b->size = size;
    
    // initialize time vector and value vector
    // those 2 vector are allocated together to ensure memory
    // contiguity ==> this is useful for the python interface
    // so as to initialize a numpy array with an empty column at index 0.   
    b->tx = (double*)malloc(b->size*(b->nx+1)*sizeof(double));
    

    // ID
    b->id = id;

    // first time flag
    b->flag_no_print = 1;

    // save last values
    b->last_x = (double*)malloc(b->nx*sizeof(double));
    b->last_t = 0.0;

    // anim info
    b->anim = (AnimInfo*)malloc(sizeof(AnimInfo));

    b->anim->save_anim   = save_anim;
    b->anim->save_visu   = save_visu;
    b->anim->anim_period = anim_period;
    b->anim->last_print_t = 0.0;

    // initialize name of associated text file
    b->anim->filename = strdup(anim_name);
    
    // (re)-initialize the text file
    if ((save_anim && (id == BUFFER_Q)) || (save_visu && (id == BUFFER_VISU)))
    {
        fid = fopen(b->anim->filename, "w"); 
        if(fid == NULL)
        {
            printf("error in mbs_buffer_save: cannot open file '%s'\n", b->anim->filename);
            return NULL;
        }else{
            fclose(fid);
        }
    }
    
    return b;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MbsGrowingBuffer* mbs_new_growing_buffer(int max_nx, int size, const char *respath)
{
    int i;

    MbsGrowingBuffer* b;

    b = (MbsGrowingBuffer*) malloc(sizeof(MbsGrowingBuffer));

    // initialize index to 0
    b->index = 0;

    // initialize sizes
    b->size   = size;
    b->max_nx = max_nx;
    b->nx     = 0;

    // initialize time vector    
    b->t = (double*) malloc(size*sizeof(double));

    for(i=0; i<size; i++)
    {
        b->t[i] = 0.0;
    }

    // initialize value vector
    b->X = (double**) malloc(max_nx*sizeof(double*));

    for(i=0; i<max_nx; i++)
    {
        b->X[i] = NULL; // initially: no user input to save
    }

    // initialize name of associated text file
    b->names = (char**)malloc(max_nx*sizeof(char*));

    // path for the results
    b->respath = respath;

    return b;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_delete_buffer(MbsBuffer* b){
    
    // free t and value vector together since thay were allocated together
    free(b->tx);  // free(b->x) is not necessary
    
    free(b->last_x);
    free(b->filename);

    free(b->anim->filename);
    free(b->anim);

    free(b);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_delete_growing_buffer(MbsGrowingBuffer *b)
{
    int i;

    free(b->t);

    for(i=0; i<b->nx; i++)
    {
        free(b->X[i]);
        free(b->names[i]);
    }
    free(b->X);
    free(b->names);

    free(b);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_buffer_save(MbsBuffer* b, double t, double *x){
    
    int i;
    
    // save time
    b->tx[TIME_ID(b->index)] = t;
    
    // copy values to the buffer
    for (i=0; i < b->nx ; i++){
        b->tx[b->index*(b->nx+1) + 1 + i] = x[i];
    }
    
    // increment the buffer index for next call
    b->index++;
    
    // check if buffer is full
    if(b->index == b->size){

        // write the buffer to file
        mbs_buffer_write(b);

        // save last values added
        b->last_t = t;

        for (i=0; i < b->nx ; i++){
            b->last_x[i] = x[i];
        }
      
        // reset the buffer index to start of the buffer (next save will overide old values)
        b->index = 0;
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_growing_buffer_save(MbsGrowingBuffer* b, double t)
{
    int i, j;
    int index, nb_new, cur_nx;
    char* f;
    FILE* fid;

    // add new user labels and allocate memory
    nb_new = get_output_nb_new();

    if (nb_new > 0)
    {
        cur_nx = b->nx;

        for(i=cur_nx; i < cur_nx + nb_new; i++)
        {
            if (i >= b->max_nx)
            {
                break;
            }

            b->names[i] = strdup(get_output_label(i));

            // (re)-initialize the text file
            f = (char*) malloc((strlen(b->respath)+strlen(b->names[i])+10) * sizeof(char));

            sprintf(f, "%s/%s.res", b->respath, b->names[i]);
            fid = fopen(f, "w");
            free(f);

            if(fid == NULL)
            {
                printf("error in mbs_buffer_save: cannot open file '%s'\n", b->names[i]);
                exit(EXIT_FAILURE);
            }
            else
            {
                fclose(fid);
            }

            b->X[i] = (double*) malloc(b->size*sizeof(double));

            for(j=0; j<b->size; j++)
            {
                b->X[i][j] = 0.0;
            }

            b->nx++;
        }

        set_output_nb_new(0);

        if (b->nx != get_output_nb())
        {
            printf("%d, %d\n", b->nx, get_output_nb());
            printf("Error: number of user inputs not the same as in the auto-output structure! \n");
            exit(EXIT_FAILURE);
        }
    }

    index = b->index;

    // save time
    b->t[index] = t;

    // copy values to the buffer
    for(i=0; i<b->nx; i++)
    {
        b->X[i][index] = get_output_value(i);
    }

    // increment the buffer index for next call
    b->index++;

    // check if buffer is full
    if(b->index >= b->size)
    {
        // write the buffer to file
        mbs_growing_buffer_write(b);

        // reset the buffer index to start of the buffer (next save will overide old values)
        b->index = 0;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_buffer_write(MbsBuffer* b){
    
    int i, j;
    FILE* fid;
    
    // Open the file associated to the buffer
    fid = fopen(b->filename, "a"); 
    if(fid == NULL)
    {
        printf("error in mbs_buffer_save: cannot open file '%s'\n", b->filename);
        return;
    }
    
    for (i=0; i<b->index; i++){
        // write time and array
        for (j=0; j<b->nx+1 ; j++){
            fprintf(fid, "  %g", b->tx[i*(b->nx+1) + j]);
        }
        
        // go to new line 
        fprintf(fid, "\n");
    }
    
    // Close file
    fclose(fid);

    // special case for .anim file
    if ( (b->anim->save_anim && (b->id == BUFFER_Q)) || (b->anim->save_visu && (b->id == BUFFER_VISU)) )
    {
        mbs_anim_write(b); 
    }

    // first print done
    b->flag_no_print = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_anim_write(MbsBuffer* b)
{
    int i;
    int next_index;
    double cur_t, prev_t, next_t, anim_period;
    double alpha, alpha_comp;
    double print_t, print_x;
    FILE* fid;

    // open the file associated to the buffer
    fid = fopen(b->anim->filename, "a"); 

    if(fid == NULL)
    {
        printf("error in mbs_buffer_save: cannot open file '%s'\n", b->anim->filename);
        return;
    }

    // safety
    if (b->index < 1)
    {
        fclose(fid);
        return;
    } 

    // period
    anim_period = b->anim->anim_period;

    // first time .anim is printed
    if (b->flag_no_print)
    {
        // first print
        for (i=0; i<b->nx+1 ; i++){
            fprintf(fid, "  %g", b->tx[i]);
        }
        fprintf(fid, "\n");

        b->anim->last_print_t = b->tx[TIME_ID(0)];

        // safety
        if (b->index < 2)
        {
            fclose(fid);
            return;
        }

        cur_t = b->tx[TIME_ID(0)] + anim_period;

        prev_t = b->tx[TIME_ID(0)];
        next_t = b->tx[TIME_ID(1)];

        next_index = 1;
    }
    else
    {
        cur_t = b->anim->last_print_t + anim_period;

        prev_t = b->anim->last_print_t;
        next_t = b->tx[TIME_ID(0)];

        next_index = 0;
    }

    print_t = b->anim->last_print_t;

    while (cur_t <= b->tx[TIME_ID(b->index-1)])
    {
        if ((prev_t <= cur_t) && (cur_t <= next_t))
        {
            // safety
            if (prev_t == next_t)
            {
                // write time
                print_t = b->tx[TIME_ID(next_index)];
                
                // write array
                for (i=0; i<b->nx+1 ; i++)
                {
                    fprintf(fid, "  %g", b->tx[TIME_ID(next_index) + i]);
                }
                
                // go to new line 
                fprintf(fid, "\n");
            }
            else
            {
                // factors
                alpha = (cur_t - prev_t) / (next_t - prev_t);
                alpha_comp = 1.0 - alpha;

                // write time
                print_t = alpha_comp * prev_t + alpha * next_t;

                fprintf(fid, "%g", print_t);

                // write array
                for (i=0; i<b->nx ; i++)
                {
                    if (next_index > 0)
                    {
                        print_x = alpha_comp * b->tx[TIME_ID(next_index)+1 + i] + alpha * b->tx[TIME_ID(next_index)+1 + i];
                    }
                    else
                    {
                        print_x = alpha_comp * b->last_x[i] + alpha * b->tx[TIME_ID(next_index)+1 + i];
                    }

                    fprintf(fid, "  %g", print_x);
                }

                // go to new line 
                fprintf(fid, "\n");
            }
            
            cur_t += anim_period;
        }
        else
        {
            next_index++;

            if (next_index >= b->index)
            {
                b->anim->last_print_t = print_t;
                fclose(fid);
                return;
            }

            prev_t = next_t;

            next_t = b->tx[TIME_ID(next_index)];
        }
    }

    b->anim->last_print_t = print_t;

    // close file
    fclose(fid);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void mbs_growing_buffer_write(MbsGrowingBuffer* b)
{
    int i, j;
    char* f;
    FILE* fid;

    for(i=0; i<b->nx; i++)
    {
        // open the file associated to the current arrow of the buffer
        f = (char*) malloc((strlen(b->respath)+strlen(b->names[i])+10) * sizeof(char));
        sprintf(f, "%s/%s.res", b->respath, b->names[i]);
        fid = fopen(f, "a");
        free(f);

        if(fid == NULL)
        {
            printf("error in mbs_buffer_save: cannot open file '%s'\n", b->names[i]);
            return;
        }

        for(j=0; j<b->index; j++)
        {
            // write line
            fprintf(fid, "%g  %g\n", b->t[j], b->X[i][j]);
        }

        // close file
        fclose(fid);
    }
}
