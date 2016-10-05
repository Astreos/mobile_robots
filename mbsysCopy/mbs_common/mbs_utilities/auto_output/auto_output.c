
#include "auto_output.h"
#include "useful_functions.h"

#include <stdlib.h>
#include <string.h>

/*! \brief initialize the AutoOutput structure
 * 
 * \param[in] max_nb maximum number of user outputs
 * \return AutoOutput structure initialized
 */
AutoOutput* init_auto_output(int max_nb)
{
    int i;
    
    AutoOutput *auto_output;

    // memory allocation
    auto_output = (AutoOutput*) malloc(sizeof(AutoOutput));

    auto_output->label_outputs = get_char_tab(max_nb, 50);

    auto_output->y_outputs = (double*) malloc(max_nb * sizeof(double));

    for (i=0; i<max_nb; i++)
    {
        auto_output->y_outputs[i] = 0.0;
    }

    auto_output->nb = 0;
    auto_output->max_nb = max_nb;

    auto_output->nb_new_outputs = 0;

    return auto_output;
}

/*! \brief release memory for AutoOutput
 * 
 * \param[out] auto_output AutoOutput structure to free
 */
void free_auto_output(AutoOutput *auto_output)
{
    free_char_tab(auto_output->label_outputs);
    free(auto_output->y_outputs);
    free(auto_output);
}

/*! \brief update auto_output when a 'set_output' is encountered
 * 
 * \param[out] auto_output AutoOutput structure
 * \param[in] value current output value
 * \param[in] label current output label
 */
void update_auto_output(AutoOutput *auto_output, double value, char* label)
{
    int nb;
    int index;

    index = index_output_string(auto_output, label);
    nb = auto_output->nb;

    // output already known
    if (index >= 0)
    {
        auto_output->y_outputs[index] = value;
    }
    else // new output
    {
        if (nb >= auto_output->max_nb) // maximal number of outputs already reached
        {
            return;
        }
        else // add a new output
        {
            auto_output->label_outputs[nb] = label;
            auto_output->y_outputs[nb] = value;

            auto_output->nb++;
            auto_output->nb_new_outputs++;
        }
    }
}

/*! \brief get index for a output with its label
 * 
 * \param[in] auto_output AutoOutput structure
 * \param[in] label label of the output
 * \return index of the output, -1 if not known
 */
int index_output_string(AutoOutput *auto_output, char* label)
{
    int i;

    for(i=0; i<auto_output->nb; i++)
    {
        if (strcmp(label, auto_output->label_outputs[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

/*! \brief get the output value corresponding to the requested index
 * 
 * \param[in] auto_output AutoOutput structure
 * \param[in] index requested index
 * \return requested value or -1 if there is no output for the requested index
 */
double get_output_value_auto(AutoOutput *auto_output, int index)
{
    if ((index < 0) || (index >= auto_output->nb))
    {
        return -1;
    }

    return auto_output->y_outputs[index];
}

/*! \brief get the output label corresponding to the requested index
 * 
 * \param[in] auto_output AutoOutput structure
 * \param[in] index requested index
 * \return requested label or "no_output" if there is no output for the requested index
 */
char* get_output_label_auto(AutoOutput *auto_output, int index)
{
    if ((index < 0) || (index >= auto_output->nb))
    {
        return "no_output";
    }

    return auto_output->label_outputs[index];
}
