/*! 
 * \author Nicolas Van der Noot
 * \file plot_sdl.h
 * \brief main SDL functions (plot curves and handles keyboard, joystick... inputs)
 */

#ifdef SDL

#ifndef _PLOT_SDL_H_INCLUDED_
#define _PLOT_SDL_H_INCLUDED_

#include "SDL.h"
#undef main
#include "SDL_ttf.h"
#include "SDL_opengl.h"

#include "realtime.h"
#include "auto_plot.h"
#include "color_sdl.h"

#include "mbs_data.h"
//#include "user_realtime.h"

// y axis: main parts
#define SCREEN_UP_HEIGHT_LOSS 40
#define SCREEN_BOTTOM_HEIGHT_LOSS 40
#define PLOT_Y_START SCREEN_UP_HEIGHT_LOSS

// x axis: main parts
#define SCREEN_WIDTH_LOSS 60
#define PLOT_X_START 0
#define X_ERASE_WIDTH SCREEN_WIDTH_LOSS

// up indications
#define UP_FIRST_Y_INDICATIONS (SCREEN_UP_HEIGHT_LOSS / 3)
#define UP_SECOND_Y_INDICATIONS (UP_FIRST_Y_INDICATIONS + ((3*SCREEN_UP_HEIGHT_LOSS) / 8))
#define UP_SEPARATION_INDICATION ((UP_FIRST_Y_INDICATIONS + UP_SECOND_Y_INDICATIONS) / 2)
#define UP_COLOR_WIDTH_LABEL 15
#define UP_COLOR_SPACE_LABEL 5
#define UP_COLOR_WIDTH_LOSS_BEGIN (UP_COLOR_SPACE_LABEL + UP_COLOR_SPACE_LABEL + 40)
#define UP_COLOR_WIDTH_LOSS_END 20

// bottom indications: x axis
#define LEGEND_SIGNAL_SCALING_X_POS 10
#define SIGNAL_SCALING_X_POS 22
#define LEGEND_PLOT_SCALING_X_POS 45
#define PLOT_SCALING_X_POS 57
#define LEGEND_PLOT_MOVE_X_POS_1 93
#define LEGEND_PLOT_MOVE_X_POS_1_BIS 110
#define LEGEND_PLOT_MOVE_X_POS_2 118
#define PLOT_MOVE_X_POS_1 105
#define PLOT_MOVE_X_POS_2 130
#define LEGEND_SWITCH_HOR_VERT_X_POS 138
#define SWITCH_HOR_VERT_X_POS 165
#define LEGEND_XY_LABEL_X_POS 210
#define XY_LABEL_X_POS 250
#define LEGEND_TIME_SPEED_X_POS 385
#define TIME_SPEED_X_POS 425
#define VISU_PAST_X_POS_1 535
#define VISU_PAST_X_POS_2 550
#define VISU_PAST_X_POS_3 557
#define LEGEND_SPEED_X_POS 580
#define SPEED_X_POS 592
#define LEGEND_QUIT_X_POS 620
#define QUIT_X_POS 632

// font sizes
#define FONT_AXIS_SIZE 12
#define FONT_LABEL_SIZE 12
#define FONT_SMALL_LABEL_SIZE 10
#define FONT_SCALING 25
#define FONT_ARROWS 15
#define FONT_PLAY_BREAK 14
#define FONT_HOR_MOUSE 25
#define FONT_MOUSE 45
#define FONT_SPEED 25

// x labels
#define X_LABEL_NB 10
#define X_LABEL_HEIGHT 10
#define X_LABEL_SHIFT_X 2
#define X_LABEL_SHIFT_Y 0

// y labels
#define Y_LABEL_NB 10
#define Y_LABEL_WIDTH 10
#define Y_LABEL_SHIFT_X 3
#define Y_LABEL_SHIFT_Y 2
#define Y_LABEL_SHIFT_Y_START (Y_LABEL_SHIFT_Y + 9)
#define Y_LABEL_SHIFT_Y_END (Y_LABEL_SHIFT_Y - 10)

#define STR_LABEL_MIN 1.0e-9

// scaling
#define PLOT_SCALE_MAX 4.0
#define SCALING_MULT_THRESHOLD 4.0
#define SCALING_PLOT_TOO_SMALL 0.0
#define SCALING_PLOT_TOO_HIGH 0.4
#define SCALING_PLOT_OPTIMAL_INCREASE 0.25
#define SCALING_PLOT_OPTIMAL_DECREASE 0.15
#define SCALING_NUL_DIFF 4.0
#define MANUAL_SCALING_PLOT 0.5
#define MANUAL_OFFSET_PLOT 0.25

// other
#define NUL_AXIS_DASH_WIDTH 8
#define X_AXIS_NB_Y_PIXELS_WIDTH 2
#define START_VECTOR 12345

// mouse
#define WHEEL_MOUSE_FACTOR 0.9 // [-]
#define FQC_MOUSE 30.0  // frequence of the SDL window refresh when using the mouse for translations [Hz]
#define TIME_RELEASE_MOUSE_PLOT (1e6 / FQC_MOUSE)  // [us]

#define TIME_MOUSE_DELAY 5e4


// -- Structures -- //

/*! \brief SDL main data structure
 */
typedef struct Screen_sdl
{
    // SDL pointers
    SDL_Window   *win; ///< SDL window
    SDL_Renderer *ren; ///< SDL renderer

    // fonts pointers
    TTF_Font     *font_label;
    TTF_Font     *font_small_label;
    TTF_Font     *font_axis;
    TTF_Font     *font_scaling;
    TTF_Font     *font_arrows;
    TTF_Font     *font_play;
    TTF_Font     *font_hor_mouse;
    TTF_Font     *font_mouse;
    TTF_Font     *font_break;
    TTF_Font     *font_speed;

    // colors
    SDL_Color text_color; ///< color for text

    int color_tab[NB_COLORS_SDL]; ///< colors for the curves

    // other variables

    int break_plot_flag;          ///< break flag
    int plot_auto_scaling;        ///< auto scaling flag
    int signal_auto_scaling;      ///< signal auto scaling flag
    int last_plot_auto_scaling;   ///< last auto scaling flag
    int last_signal_auto_scaling; ///< last signal auto scaling flag
    int hor_plot_scaling;  ///< horizontal plot scaling flag
    int index_vec;         ///< index vector
    int index_sdl;         ///< index for SDL
    int last_x_label_flag; ///< last x label flag
    int expo_y; ///< y exponent
    int expo_x; ///< x exponent
    int increase_plot_y_diff_flag; ///< flag increase y diff
    int decrease_plot_y_diff_flag; ///< flag decrease y diff
    int up_plot_y_flag;            ///< flag up legend plot
    int bottom_plot_y_flag;        ///< flag bottom legend plot
    int increase_plot_x_diff_flag; ///< flag increase x diff
    int decrease_plot_x_diff_flag; ///< flag decrease x diff
    int left_plot_y_flag;  ///< flag move plot to left
    int right_plot_y_flag; ///< flag move plot to right
    int bottom_flag;       ///< flag move plot to bottom
    int scaling_flag;      ///< scaling flag
    int last_scaling_flag; ///< last scaling flag
    int last_color;        ///< last color
    int x_label_print_count;     ///< x label print count
    int change_mouse_wheel_flag; ///< flag when mouse wheel changes
    int buffer_size;       ///< buffer sive for past save
    int min_tsim_index;    ///< index for minimal tsim_save value 
    int max_tsim_index;    ///< index for maximal tsim_save value 
    int flag_buffer_round; ///< 1 if buffer already filled, 0 otherwise
    int flag_started;      ///< flag started
    int max_nb_curves;     ///< maximal number of curves
    int cur_nb_curves;     ///< current number of curves
    int max_nb_legends;    ///< maximal number of legends
    int nb_legends;        ///< current number of legends

    double tsim_pixel;   ///< simulation time for pixel [s]
    double y_min, y_max; ///< min and max values for y
    double x_min, x_max; ///< min and max values for x
    double last_y_min, last_y_max; ///< last min and max values for y
    double last_legend_y_min, last_legend_y_max; ///< last min and max values for y legends
    double last_legend_x_min, last_legend_x_max; ///< last min and max values for x legends
    double min_tsim; ///< minimal simulation time in buffer [s]
    double max_tsim; ///< maximal simulation time in buffer [s]

    // pixel size related
    int curve_width;   ///< curve width in number of pixels
    int screen_width;  ///< screen width in number of pixels
    int screen_height; ///< screen height in number of pixels

    // screen width related
    int plot_x_end;        ///< x end
    int screen_plot_width; ///< width in x
    int x_label_width;     ///< width of the x labels

    // screen height related
    int plot_y_end;         ///< end of y axis
    int plot_y_mid;         ///< middle of y axis
    int screen_plot_height; ///< screen plot height
    int y_label_height;     ///< y label height

    // bottom indications: y axis
    int bottom_mid_y_indications; ///< middle bottom y indications
    int scaling_indications;      ///< scaling indications
    int x_axis_indication;        ///< x scaling indications
    int y_axis_indication;        ///< y scaling indications
    int speed_indication_1;       ///< speed indication 1
    int speed_indication_2;       ///< speed indication 2
    int legend_speed_indication_1;  ///< legend speed indication 1
    int legend_speed_indication_2;  ///< legend speed indication 2
    int scaling_bottom_indications; ///< scaling bottom indications
    int scaling_bottom_arrow_indications_1; ///< scaling bottom arrow indications 1
    int scaling_bottom_arrow_indications_2; ///< scaling bottom arrow indications 2
    int scaling_bottom_arrow_indications_3; ///< scaling bottom arrow indications 3
    int scaling_up_indications;   ///< scaling up indications
    int scaling_up_indications_2; ///< scaling up indications 2

    int *scaling_expo_mult;      ///< vector of scaling exponents
    int *last_scaling_expo_mult; ///< vector of last scaling exponents 
    int *pixel_pos_label;        ///< vector of pixel pose indications for up legend

    double *t_vec;     ///< current time vector to plot during breaks
    double *y_tab_min; ///< vector of minimal reached values for each curve
    double *y_tab_max; ///< vector of maximal reached values for each curve
    double *previous_y_vec;      ///< vector if previous y curves values
    double *scaling_offset;      ///< vector of current scalings
    double *last_scaling_offset; ///< vector of last scalings

    double **y_tab; ///< current values of the SDL window, to plot during a break

    double *tsim_save; ///< save time for past investigations
    double **y_save;   ///< save curves for past investigations

    AutoPlot *auto_plot; ///< automatic user plot update structure

    // joysticks
    int nb_joysticks;         ///< number of joysticks
    SDL_Joystick **joysticks; ///< joysticks structure

} Screen_sdl;


// -- Functions prototypes -- //

void wait_key_generic(Simu_realtime *realtime, int cur_t_usec, double tsim);

Screen_sdl* init_screen_sdl(Realtime_option *options);
void new_curves_save(Screen_sdl *screen_sdl, int nb_new_curves);
TTF_Font* init_font(char *folder, int font_size);
void free_screen_sdl(Screen_sdl *screen_sdl);

void log_SDL_error(char *SDL_function);
void SDL_set_color(Screen_sdl *screen_sdl, int color_num);
void refresh_screen(Screen_sdl *screen_sdl);
void draw_pixel(SDL_Renderer *ren, int i, int j);
void draw_rectangle(SDL_Renderer *ren, int x1, int x2, int y1, int y2);
void draw_horizontal_line(SDL_Renderer *ren, int x1, int x2, int y);
void draw_vertical_line(SDL_Renderer *ren, int x, int y1, int y2);
void print_text_sdl(char *str, TTF_Font *font, SDL_Color txt_col, SDL_Renderer *ren, int x, int y, int pos_flag);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int pos_flag);
SDL_Texture* renderText(TTF_Font *font, char *message, SDL_Color text_color, SDL_Renderer *ren);

void update_plot_vectors(Simu_realtime *realtime, double t);
void update_scale_signals(Screen_sdl *screen_sdl, Simu_realtime *realtime, int screen_flag);
void update_full_vectors(Simu_realtime *realtime, double tsim);
void update_x_min_max(Screen_sdl *screen_sdl, Simu_realtime *realtime);
void update_y_min_max(Simu_realtime *realtime);
void update_y_tab(Screen_sdl *screen_sdl);
void update_sdl_legend(Simu_realtime *realtime, int new_nb_legends);
double bound_diff_min_max(double min_thres, double max_thres, double cur_min, double cur_max, double cur_move, int sign_flag);
void mouse_sdl(Screen_sdl *screen_sdl, Simu_realtime *realtime, double tsim, int mouse_flag);
void set_bottom_flag(Simu_realtime *realtime);

void str_scaling(char *str, int scaling_expo_mult, double scaling_offset);
int str_label(char *str, double low_nb, double high_nb);
void str_axis(char *str, double nb, int expo);

void limit_x_limits(Screen_sdl *screen_sdl);
double scale_signal(double no_scale_nb, int scaling_expo_mult, double scaling_offset);
double unscale_signal(double scale_nb, int scaling_expo_mult, double scaling_offset);

void plot_screen_sdl(Simu_realtime *realtime, double tsim, int screen_flag);

void break_gestion(Simu_realtime *realtime, double tsim);
void events_simu(Simu_realtime *realtime, double tsim);
void last_break_gestion(Simu_realtime *realtime, double tsim);

#endif
#endif
