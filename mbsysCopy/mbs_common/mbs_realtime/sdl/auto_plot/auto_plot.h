/*! 
 * \author Nicolas Van der Noot
 * \file auto_plot.h
 * \brief functions used to automatically update the user curves plotted
 */

#ifdef SDL

#ifndef _AUTO_PLOT_H_
#define _AUTO_PLOT_H_

/*! \brief User curves to plot auto-update
 */
typedef struct AutoPlot
{
    int nb; ///< current number of curves to plot
    int max_nb; ///< maximum number of curves to plot

    char **label_curves; ///< tabular with the current curves labels

    double *y_curves; ///< tabular with the current values of the plots to curve

    int nb_new_curves; ///< number of new curves discovered during this time step
    
} AutoPlot;

// functions prototypes
AutoPlot* init_auto_plot(int max_nb);
void free_auto_plot(AutoPlot *auto_plot);
void update_auto_plot(AutoPlot *auto_plot, double value, const char* label);
int index_plot_string(AutoPlot *auto_plot, const char* label);

#endif
#endif
