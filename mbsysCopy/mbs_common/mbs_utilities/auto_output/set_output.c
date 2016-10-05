
#include "set_output.h"
#include "auto_output.h"

static AutoOutput *auto_output; // static AutoOutput structure with all the current user values to output
int flag_output = 0; ///< 1 if init_set_output was called, 0 otherwise

/*! \brief initialize the static auto_output structure
 */
void init_set_output(int max_nb_curves)
{
    flag_output = 1;

    auto_output = init_auto_output(max_nb_curves);
}

/*! \brief release memory of auto_output
 */
void free_set_output()
{
    if (flag_output)
    {
        free_auto_output(auto_output);
    }
}

/*! \brief function called by the user to output the time evolution of a value
 * 
 * \param[in] value current value to output
 * \param[in] label label of the value
 */
void set_output(double value, char* label)
{
    if (flag_output)
    {
        update_auto_output(auto_output, value, label);
    }
}

/*! \brief get the output value corresponding to the requested index
 * 
 * \param[in] index requested index
 * \return requested value or -1 if there is no output for the requested index
 */
double get_output_value(int index)
{
    return get_output_value_auto(auto_output, index);
}

/*! \brief get the output label corresponding to the requested index
 * 
 * \param[in] index requested index
 * \return requested label or "no_output" if there is no output for the requested index
 */
char* get_output_label(int index)
{
    return get_output_label_auto(auto_output, index);
}

/*! \brief get the number of user inputs
 * 
 * \return number of user inputs
 */
int get_output_nb()
{
    return auto_output->nb;
}

/*! \brief get the number of new user inputs
 * 
 * \return number of new user inputs
 */
int get_output_nb_new()
{
    return auto_output->nb_new_outputs;
}

/*! \brief set the number of new user inputs
 * 
 * \param[in] nb number of new outputs to set
 */
void set_output_nb_new(int nb)
{
    auto_output->nb_new_outputs = nb;
}
