/*! 
 * \author Nicolas Van der Noot
 * \file color_sdl.h
 * \brief colors description, used for the SDL plots
 */

#ifdef REAL_TIME

#ifndef _COLORS_SDL_H_
#define _COLORS_SDL_H_

#define NB_COLORS_SDL 12     ///< number of colors available to the user
#define NB_COLORS_TOT_SDL 15 ///< total number of colors
#define NB_COLORS_INIT_SDL 3 ///< number of initial colors (not available to the user)

// -- Colors enumeration -- //

enum{WHITE_SDL, BLACK_SDL, LIGHT_GRAY_SDL, BLUE_SDL, RED_SDL, DARK_GREEN_SDL, PURPLE_SDL, TURQUOISE_SDL,
    ORANGE_SDL, LIGHT_BLUE_SDL, PINK_SDL, LIGHT_GREEN_SDL, DARK_YELLOW_SDL, YELLOW_SDL, GREEN_SDL};

#endif
#endif
