/*! 
 * \author Nicolas Van der Noot
 * \file auto_output.h
 * \brief functions used to automatically update the user output values
 */

#ifndef _AUTO_OUTPUT_H_
#define _AUTO_OUTPUT_H_

/*! \brief User output auto-update
 */
typedef struct AutoOutput
{
    int nb; ///< current number of user outputs
    int max_nb; ///< maximum number of user outputs

    char **label_outputs; ///< tabular with the current user outputs labels

    double *y_outputs; ///< tabular with the current outputs values

    int nb_new_outputs; ///< number of new user outputs discovered during this time step
    
} AutoOutput;

// functions prototypes
AutoOutput* init_auto_output(int max_nb);
void free_auto_output(AutoOutput *auto_output);
void update_auto_output(AutoOutput *auto_output, double value, char* label);
int index_output_string(AutoOutput *auto_output, char* label);
double get_output_value_auto(AutoOutput *auto_output, int index);
char* get_output_label_auto(AutoOutput *auto_output, int index);

#endif
