
#include "set_plot.h"

#ifdef SDL

#include "auto_plot.h"

static AutoPlot *auto_plot; // static AutoPlot structure with all the current user plots information
int flag_plot = 0; ///< 1 if init_set_plot was called, 0 otherwise

/*! \brief initialize the static auto_plot structure
 * 
 * \param[in,out] screen_sdl SDL gestion main structure
 */
void init_set_plot(Screen_sdl *screen_sdl)
{
    flag_plot = 1;

    auto_plot = init_auto_plot(screen_sdl->max_nb_curves);

    screen_sdl->auto_plot = auto_plot;
}

/*! \brief release memory of auto_plot
 */
void free_set_plot()
{
    if (flag_plot)
    {
        free_auto_plot(auto_plot);
    }   
}

/*! \brief function called by the user to plot a curve
 * 
 * \param[in] value current value of the curve
 * \param[in] label label of the curve
 */
void set_plot(double value, char* label)
{
    if (flag_plot)
    {
        update_auto_plot(auto_plot, value, label);
    }
}

#endif
