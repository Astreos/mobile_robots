
#ifdef SDL

#include "auto_plot.h"
#include "useful_functions.h"

#include <stdlib.h>
#include <string.h>

/*! \brief initialize the AutoPlot structure
 * 
 * \param[in] max_nb maximum number of curves to plot
 * \return AutoPlot structure initialized
 */
AutoPlot* init_auto_plot(int max_nb)
{
    int i;
    
    AutoPlot *auto_plot;

    // memory allocation
    auto_plot = (AutoPlot*) malloc(sizeof(AutoPlot));

    auto_plot->label_curves = get_char_tab(max_nb, 50);

    auto_plot->y_curves = (double*) malloc(max_nb * sizeof(double));

    for (i=0; i<max_nb; i++)
    {
        auto_plot->y_curves[i] = 0.0;
    }

    auto_plot->nb = 0;
    auto_plot->max_nb = max_nb;

    auto_plot->nb_new_curves = 0;

    return auto_plot;
}

/*! \brief release memory for AutoPlot
 * 
 * \param[out] auto_plot AutoPlot structure to free
 */
void free_auto_plot(AutoPlot *auto_plot)
{
    free_char_tab(auto_plot->label_curves);
    free(auto_plot->y_curves);
    free(auto_plot);
}

/*! \brief update auto_plot when a 'set_plot' is encountered
 * 
 * \param[out] auto_plot AutoPlot structure
 * \param[in] value current curve value
 * \param[in] label current curve label
 */
void update_auto_plot(AutoPlot *auto_plot, double value, const char* label)
{
    int nb;
    int index;

    index = index_plot_string(auto_plot, label);
    nb = auto_plot->nb;

    // curve already known
    if (index >= 0)
    {
        auto_plot->y_curves[index] = value;
    }
    else // new curve
    {
        if (nb >= auto_plot->max_nb) // maximal number of curves already reached
        {
            return;
        }
        else // add a new curve
        {
            auto_plot->label_curves[nb] = (char*) label;
            auto_plot->y_curves[nb] = value;

            auto_plot->nb++;
            auto_plot->nb_new_curves++;
        }
    }
}

/*! \brief get index for a curve with its label
 * 
 * \param[in] auto_plot AutoPlot structure
 * \param[in] label label of the curve
 * \return index of the curve, -1 if not known
 */
int index_plot_string(AutoPlot *auto_plot, const char* label)
{
    int i;

    for(i=0; i<auto_plot->nb; i++)
    {
        if (strcmp(label, auto_plot->label_curves[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

#endif
