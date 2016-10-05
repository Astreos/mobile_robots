/*
 * SDL functions (plot curves and handles keyboard, joystick... inputs) functions
 *
 * author: Nicolas Van der Noot
 */

#ifdef SDL

#include "useful_functions.h"
#include "plot_sdl.h"
#include "cmake_sdl_config.h"
#include "time_functions.h"
#include "events_sdl.h"
#include "set_plot.h"
//#include "java_functions.h"
#include "realtime_functions.h"

#define TIME_NO_INTERACTION_BREAK 3e6 ///< time eith no interaction to go on break mode [us]
#define TIME_SDL_DELAY 25             ///< time delay for SDL

//#define SDL_FILES_PATH ROBOTRAN_SOURCE_DIR"/mbs_common/mbs_realtime/sdl/SDL_files"

/*! \brief initialize the Screen_sdl structure
 * 
 * \param[in] options real-time options
 * \return the initialized SDL main data structure
 */
Screen_sdl* init_screen_sdl(Realtime_option *options)
{
    // -- Variables declaration -- //

    int i, j;
    int screen_width, screen_height;
    int max_nb_curves, buffer_size;

    #ifdef UNIX
    int x_SDL_position;
    int min_x_SDL_position;
    #endif

    double t0;

    // genral structure
    Screen_sdl *screen_sdl;

    // SDL window
    SDL_Window *win;

    // SDL Renderer
    SDL_Renderer *ren;

    // SDL colors
    SDL_Color text_color;

    // display information
    SDL_DisplayMode current;
    

    // -- Structure initialization -- //

    screen_sdl = (Screen_sdl*) malloc(sizeof(Screen_sdl));

    screen_width  = options->screen_width;
    screen_height = options->screen_height;

    max_nb_curves = options->max_nb_curves;
    buffer_size   = options->buffer_size;

    // initialize SDL
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) != 0)
    {
        log_SDL_error("SDL_Init");
    }

    screen_sdl->nb_joysticks = SDL_NumJoysticks();

    if (screen_sdl->nb_joysticks >= 1)
    {
        #ifdef PRINT_REPORT
        printf("Number of joysticks detected: %d\n\n", screen_sdl->nb_joysticks);
        #endif

        screen_sdl->joysticks = (SDL_Joystick**) malloc(screen_sdl->nb_joysticks*sizeof(SDL_Joystick*));

        for(i=0; i<screen_sdl->nb_joysticks; i++)
        {
            screen_sdl->joysticks[i] = SDL_JoystickOpen(i);

            if ( screen_sdl->joysticks[i] == NULL )
            {
                fprintf(stderr,"Error to open the joystick !\n");
            }
        }

        SDL_JoystickEventState(SDL_ENABLE);
    }

    #ifdef UNIX

    // display information
    if(SDL_GetCurrentDisplayMode(0, &current))
    {
        log_SDL_error("SDL_GetCurrentDisplayMode");
    }

    // SDL window X position
    x_SDL_position     = (int) (0.75 * current.w - screen_width/2.0);
    min_x_SDL_position = current.w / 2;

    if (x_SDL_position < min_x_SDL_position)
    {
        x_SDL_position = min_x_SDL_position;
    }

    // window
    win = SDL_CreateWindow(
        "Real-time graph",                   // window title
        x_SDL_position,                      // initial x position
        SDL_WINDOWPOS_CENTERED,              // initial y position
        screen_width,                        // width, in pixels
        screen_height,                       // height, in pixels
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags
    );

    #else

    // window
    win = SDL_CreateWindow(
        "Real-time graph",                   // window title
        SDL_WINDOWPOS_CENTERED,              // initial x position
        SDL_WINDOWPOS_CENTERED,              // initial y position
        screen_width,                        // width, in pixels
        screen_height,                       // height, in pixels
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags
    );

    #endif

    // pixel size related
    screen_sdl->curve_width   = options->curve_width;
    screen_sdl->screen_width  = options->screen_width;
    screen_sdl->screen_height = options->screen_height;

    // screen width related
    screen_sdl->plot_x_end = (screen_width - SCREEN_WIDTH_LOSS);
    screen_sdl->screen_plot_width = (screen_sdl->plot_x_end - PLOT_X_START);
    screen_sdl->x_label_width = (screen_sdl->screen_plot_width / X_LABEL_NB);

    // screen height related
    screen_sdl->plot_y_end = (screen_height - SCREEN_BOTTOM_HEIGHT_LOSS);
    screen_sdl->plot_y_mid = ((PLOT_Y_START + screen_sdl->plot_y_end) / 2);
    screen_sdl->screen_plot_height = (screen_sdl->plot_y_end - PLOT_Y_START);
    screen_sdl->y_label_height = (screen_sdl->screen_plot_height / Y_LABEL_NB);

    // bottom indications: y axis
    screen_sdl->bottom_mid_y_indications = (screen_height - (SCREEN_BOTTOM_HEIGHT_LOSS / 2));
    screen_sdl->scaling_indications = (screen_sdl->bottom_mid_y_indications + 3);
    screen_sdl->x_axis_indication = (screen_sdl->bottom_mid_y_indications - 8);
    screen_sdl->y_axis_indication = (screen_sdl->bottom_mid_y_indications + 10);
    screen_sdl->speed_indication_1 = (screen_sdl->bottom_mid_y_indications - 4);
    screen_sdl->speed_indication_2 = (screen_sdl->bottom_mid_y_indications + 14);
    screen_sdl->legend_speed_indication_1 = (screen_sdl->speed_indication_1 - 5);
    screen_sdl->legend_speed_indication_2 = (screen_sdl->speed_indication_2 - 5);
    screen_sdl->scaling_bottom_indications = (screen_height - (SCREEN_BOTTOM_HEIGHT_LOSS / 3)+2);
    screen_sdl->scaling_bottom_arrow_indications_1 = (screen_sdl->scaling_bottom_indications + 2);
    screen_sdl->scaling_bottom_arrow_indications_2 = (screen_sdl->scaling_bottom_indications);
    screen_sdl->scaling_bottom_arrow_indications_3 = (screen_sdl->scaling_bottom_indications + 5);
    screen_sdl->scaling_up_indications = (screen_height - ((2*SCREEN_BOTTOM_HEIGHT_LOSS) / 3));
    screen_sdl->scaling_up_indications_2 = (screen_sdl->scaling_up_indications + 2);

    if (win == NULL) 
    {
        log_SDL_error("SDL_CreateWindow");
    }
    screen_sdl->win = win;

    // renderer
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);

    // the next line is faster, but it causes problems on some computers
    //ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE); 
    if (ren == NULL)
    {
        log_SDL_error("SDL_CreateRenderer");
    }
    SDL_RenderClear(ren);
    screen_sdl->ren = ren;

    // TTF (SDL text)
    if (TTF_Init() != 0)
    {
        log_SDL_error("TTF_Init");
    }

    // font for thes axes
    screen_sdl->font_axis = init_font(SDL_FILES_PATH"/HunDIN1451.ttf", FONT_AXIS_SIZE);

    // font for the labels
    screen_sdl->font_label = init_font(SDL_FILES_PATH"/MonospaceTypewriter.ttf", FONT_LABEL_SIZE);

    // font for the small labels
    screen_sdl->font_small_label = init_font(SDL_FILES_PATH"/MonospaceTypewriter.ttf", FONT_SMALL_LABEL_SIZE);

    // font for the scaling
    screen_sdl->font_scaling = init_font(SDL_FILES_PATH"/arrow_7.ttf", FONT_SCALING);

    // font for the arrows
    screen_sdl->font_arrows = init_font(SDL_FILES_PATH"/Arrows.ttf", FONT_ARROWS);

    // font for the play icon
    screen_sdl->font_play = init_font(SDL_FILES_PATH"/Arrows_tbf.ttf", FONT_PLAY_BREAK);

    // font for the horizontal mouse
    screen_sdl->font_hor_mouse = init_font(SDL_FILES_PATH"/Arrows_tbf.ttf", FONT_HOR_MOUSE);

    // font for the mouse (horizontal and vertical)
    screen_sdl->font_mouse = init_font(SDL_FILES_PATH"/arrow_7.ttf", FONT_MOUSE);

    // font for the break icon
    screen_sdl->font_break = init_font(SDL_FILES_PATH"/PIZZADUDEBULLETS.ttf", FONT_PLAY_BREAK);

    // font for the speed of the simulation
    screen_sdl->font_speed = init_font(SDL_FILES_PATH"/Arrows.ttf", FONT_SPEED);


    // text color: black
    text_color.r = 0;
    text_color.g = 0;
    text_color.b = 0;
    text_color.a = 0;
    screen_sdl->text_color = text_color;

    // index of the sdl window and vectors
    screen_sdl->index_vec = 0;
    screen_sdl->index_sdl = 0;

    // time for a pixel
    screen_sdl->tsim_pixel = 1.0 / options->fqc_screen;

    // x axis bounds
    screen_sdl->x_min = 0.0;
    screen_sdl->x_max = 0.0;

    // y axis bounds
    screen_sdl->y_min = options->y_min_init;
    screen_sdl->y_max = options->y_max_init;

    screen_sdl->last_y_min = screen_sdl->y_min;
    screen_sdl->last_y_max = screen_sdl->y_max;

    screen_sdl->last_legend_x_min = screen_sdl->x_min;
    screen_sdl->last_legend_x_max = screen_sdl->x_max;

    screen_sdl->last_legend_y_min = screen_sdl->y_min;
    screen_sdl->last_legend_y_max = screen_sdl->y_max;

    // time limits
    t0 = options->t0;

    screen_sdl->min_tsim = t0;
    screen_sdl->max_tsim = t0 + SAFETY_TIME;

    screen_sdl->min_tsim_index = 0;
    screen_sdl->max_tsim_index = buffer_size-1;
    
    screen_sdl->flag_started = 0;
    screen_sdl->flag_buffer_round = 0;

    // x label
    screen_sdl->x_label_print_count = 0;
    screen_sdl->last_x_label_flag   = 0;

    // scaling
    screen_sdl->scaling_flag      = 0;
    screen_sdl->last_scaling_flag = 0;

    // break
    screen_sdl->break_plot_flag = 0;

    // string exponents
    screen_sdl->expo_y = 0;
    screen_sdl->expo_x = 0;

    // flag: horizontal plot
    screen_sdl->hor_plot_scaling = 0;

    // flag: bottom indications
    screen_sdl->bottom_flag = 1;

    // flags: modify y axis
    screen_sdl->increase_plot_y_diff_flag = 0;
    screen_sdl->decrease_plot_y_diff_flag = 0;
    screen_sdl->up_plot_y_flag = 0;
    screen_sdl->bottom_plot_y_flag = 0;

    // flags: modify x axis
    screen_sdl->increase_plot_x_diff_flag = 0;
    screen_sdl->decrease_plot_x_diff_flag = 0;
    screen_sdl->left_plot_y_flag  = 0;
    screen_sdl->right_plot_y_flag = 0;

    screen_sdl->cur_nb_curves = 0;

    // full vectors save 
    screen_sdl->y_save = (double**) malloc(max_nb_curves * sizeof(double*));

    screen_sdl->previous_y_vec = (double*) malloc(max_nb_curves * sizeof(double));

    for(i=0; i<max_nb_curves; i++)
    {
        screen_sdl->previous_y_vec[i] = 0.0;
    }

    // y values tab for SDL
    screen_sdl->y_tab = (double**) malloc(max_nb_curves * sizeof(double*));
    for (i=0; i<max_nb_curves; i++)
    {
        screen_sdl->y_tab[i] = (double*) malloc(screen_sdl->screen_plot_width * sizeof(double));

        for(j=0; j<screen_sdl->screen_plot_width; j++)
        {
            screen_sdl->y_tab[i][j] = 0.0;
        }
    }

    // minimal and maximal values (for auto-scaling)
    screen_sdl->y_tab_min = (double*) malloc(max_nb_curves * sizeof(double));
    screen_sdl->y_tab_max = (double*) malloc(max_nb_curves * sizeof(double));

    screen_sdl->scaling_expo_mult = (int*) malloc(max_nb_curves*sizeof(int));
    screen_sdl->last_scaling_expo_mult = (int*) malloc(max_nb_curves*sizeof(int));

    screen_sdl->scaling_offset  = (double*) malloc(max_nb_curves*sizeof(double));
    screen_sdl->last_scaling_offset  = (double*) malloc(max_nb_curves*sizeof(double));
    
    for(i=0; i<max_nb_curves; i++)
    {
        screen_sdl->y_tab_min[i] = START_VECTOR;
        screen_sdl->y_tab_max[i] = START_VECTOR;

        screen_sdl->scaling_expo_mult[i]      = 0;
        screen_sdl->last_scaling_expo_mult[i] = 0;

        screen_sdl->scaling_offset[i]      = 0.0;
        screen_sdl->last_scaling_offset[i] = 0.0;
    }

    // full time save
    screen_sdl->tsim_save = (double*) malloc(buffer_size * sizeof(double));

    for(i=0; i<buffer_size; i++)
    {
        screen_sdl->tsim_save[i] = t0;
    }

    // buffer size
    screen_sdl->buffer_size = buffer_size;

    // time vector for SDL
    screen_sdl->t_vec = (double*) malloc(screen_sdl->screen_plot_width * sizeof(double));
    for (i=0; i<screen_sdl->screen_plot_width; i++)
    {
        screen_sdl->t_vec[i] = t0;
    }

    // colors
    for (i=0; i<NB_COLORS_SDL; i++)
    {
        screen_sdl->color_tab[i] = options->color_tab[i];
    }   

    screen_sdl->last_color = -1;

    // auto-scaling
    screen_sdl->plot_auto_scaling        = 0;
    screen_sdl->signal_auto_scaling      = 0;
    screen_sdl->last_plot_auto_scaling   = 0;
    screen_sdl->last_signal_auto_scaling = 0;
    screen_sdl->change_mouse_wheel_flag  = 0;

    screen_sdl->nb_legends = 0;
    screen_sdl->max_nb_legends = options->max_nb_legends;

    screen_sdl->max_nb_curves = max_nb_curves;

    // auto plot
    init_set_plot(screen_sdl);

    // clean the screen for the first time
    refresh_screen(screen_sdl);

    // return structure
    return screen_sdl;
}

/*! \brief update for new user curves
 * 
 * \param[in,out] screen_sdl SDL main data structure
 * \param[in] nb_new_curves number of new curves
 */
void new_curves_save(Screen_sdl *screen_sdl, int nb_new_curves)
{
    int i, j;
    int cur_nb_curves, buffer_size;

    cur_nb_curves   = screen_sdl->cur_nb_curves;
    buffer_size = screen_sdl->buffer_size;

    for(i=cur_nb_curves; i<cur_nb_curves+nb_new_curves; i++)
    {
        screen_sdl->y_save[i] = (double*) malloc(buffer_size * sizeof(double));

        for(j=0; j<buffer_size; j++)
        {
            screen_sdl->y_save[i][j] = 0.0; 
        }
    }
        
    screen_sdl->cur_nb_curves += nb_new_curves;
}

/*! \brief initialize a new Font variable (pointer)
 * 
 * \param[in] folder folder with Fonts
 * \param[in] font_size font size
 * \return requested Font variable (pointer)
 */
TTF_Font* init_font(char *folder, int font_size)
{
    TTF_Font* font;

    font = TTF_OpenFont(folder, font_size);

    if (font == NULL)
    {
        log_SDL_error("TTF_OpenFont");
    }

    return font;
}

/*! \brief free the main SDL structure
 * 
 * \param[out] screen_sdl SDL main data structure
 */
void free_screen_sdl(Screen_sdl *screen_sdl)
{
    // variable declaration
    int i;

    // fonts
    TTF_CloseFont(screen_sdl->font_label);
    TTF_CloseFont(screen_sdl->font_small_label);
    TTF_CloseFont(screen_sdl->font_axis);
    TTF_CloseFont(screen_sdl->font_scaling);
    TTF_CloseFont(screen_sdl->font_arrows);
    TTF_CloseFont(screen_sdl->font_play);
    TTF_CloseFont(screen_sdl->font_hor_mouse);
    TTF_CloseFont(screen_sdl->font_mouse);
    TTF_CloseFont(screen_sdl->font_break);
    TTF_CloseFont(screen_sdl->font_speed);

    // joystick
    if (screen_sdl->nb_joysticks >= 1)
    {
        for(i=0; i<screen_sdl->nb_joysticks; i++)
        {
            SDL_JoystickClose(screen_sdl->joysticks[i]);
        }
        free(screen_sdl->joysticks);
    }

    // SDL main variables
    SDL_DestroyRenderer(screen_sdl->ren);
    SDL_DestroyWindow(screen_sdl->win);
    SDL_Quit();

    free(screen_sdl->tsim_save);    

    free(screen_sdl->t_vec);

    
    for (i=0; i<screen_sdl->max_nb_curves; i++)
    {
        free(screen_sdl->y_tab[i]);
    }

    for (i=0; i<screen_sdl->cur_nb_curves; i++)
    {
        free(screen_sdl->y_save[i]);
    }

    free(screen_sdl->y_tab);
    free(screen_sdl->y_save);
    free(screen_sdl->previous_y_vec);
    free(screen_sdl->y_tab_min);
    free(screen_sdl->y_tab_max);
    free(screen_sdl->scaling_expo_mult);
    free(screen_sdl->last_scaling_expo_mult);
    free(screen_sdl->scaling_offset);
    free(screen_sdl->last_scaling_offset);

    free_set_plot();

    if (screen_sdl->nb_legends)
    {
        free(screen_sdl->pixel_pos_label);
    }

    free(screen_sdl);
}

/*! \brief show an error message and exit the simulation
 * 
 * \param[in] SDL_function SDL function name
 */
void log_SDL_error(char *SDL_function)
{
    printf("%s error: %s\n",SDL_function, SDL_GetError());
    exit(EXIT_FAILURE);
}

/*! \brief change the color in SDL
 * 
 * \param[in,out] screen_sdl SDL main data structure
 * \param[in] color_num color number
 */
void SDL_set_color(Screen_sdl *screen_sdl, int color_num)
{
    // variable decalration
    SDL_Renderer *ren;

    // safety
    while(color_num >= NB_COLORS_TOT_SDL)
    {
        color_num -= NB_COLORS_TOT_SDL;
    }
    while(color_num < 0)
    {
        color_num += NB_COLORS_TOT_SDL;
    }
    
    // no change in the color since previous time
    if (screen_sdl->last_color == color_num)
    {
        return;
    }

    screen_sdl->last_color = color_num;

    ren = screen_sdl->ren;

    // select the good color
    switch (color_num)
    {
        case WHITE_SDL:
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
            break;

        case BLACK_SDL:
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 1);
            break;

        case LIGHT_GRAY_SDL:
            SDL_SetRenderDrawColor(ren, 220, 212, 212, 0);
            break;

        case BLUE_SDL:
            SDL_SetRenderDrawColor(ren, 0, 0, 255, 0);
            break;

        case RED_SDL:
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
            break;

        case DARK_GREEN_SDL:
            SDL_SetRenderDrawColor(ren, 33, 75, 31, 0);
            break;

        case PURPLE_SDL:
            SDL_SetRenderDrawColor(ren, 131, 15, 246, 0);
            break;

        case TURQUOISE_SDL:
            SDL_SetRenderDrawColor(ren, 15, 246, 238, 0);
            break;

        case ORANGE_SDL:
            SDL_SetRenderDrawColor(ren, 243, 134, 26, 0);
            break;

        case LIGHT_BLUE_SDL:
            SDL_SetRenderDrawColor(ren, 15, 146, 246, 0);
            break;

        case PINK_SDL:
            SDL_SetRenderDrawColor(ren, 246, 15, 215, 0);
            break;              

        case LIGHT_GREEN_SDL:
            SDL_SetRenderDrawColor(ren, 23, 246, 15, 0);
            break;

        case DARK_YELLOW_SDL:
            SDL_SetRenderDrawColor(ren, 175, 165, 27, 0);
            break;

        case YELLOW_SDL:
            SDL_SetRenderDrawColor(ren, 239, 225, 36, 0);
            break;

        case GREEN_SDL:
            SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
            break;
    
        default:
            break;
    }
}

/*! \brief refresh the screen with the white color
 * 
 * \param[in,out] screen_sdl SDL main data structure
 */
void refresh_screen(Screen_sdl *screen_sdl)
{
    SDL_set_color(screen_sdl, WHITE_SDL);
    SDL_RenderClear(screen_sdl->ren);
}

/*! \brief draw a pixel, coordinates: i, j
 * 
 * \param[in] ren SDL renderer
 * \param[in] i x coordinate
 * \param[in] j y coordinate
 *
 * For some mysterious reasons, using the 'SDL_RenderFillRect' 
 * function with 1x1 rectangles is 2 times faster than using 
 * the 'SDL_RenderDrawPoint' function
 */
void draw_pixel(SDL_Renderer *ren, int i, int j)
{
    SDL_Rect rectangle;
    
    rectangle.x = i;
    rectangle.y = j;
    rectangle.w = 1;
    rectangle.h = 1;
    SDL_RenderFillRect(ren, &rectangle);
}

/*! \brief draw a rectangle (x axis: [x1 ; x2] , y axis: [y1 ; y2])
 * 
 * \pre x1 <= x2
 * \pre y1 <= y2
 * \param[in] ren SDL renderer
 * \param[in] x1 min x coordinate
 * \param[in] x2 max x coordinate
 * \param[in] y1 min y coordinate
 * \param[in] y2 max y coordinate
 */
void draw_rectangle(SDL_Renderer *ren, int x1, int x2, int y1, int y2)
{
    SDL_Rect rect;

    rect.x = x1;
    rect.y = y1;
    rect.w = x2 - x1 + 1;
    rect.h = y2 - y1 + 1;
    SDL_RenderFillRect(ren, &rect);
}

/*! \brief draw a horizontal line (x axis: [x1 ; x2] , y axis: y)
 * 
 * \pre x1 <= x2
 * \param[in] ren SDL renderer
 * \param[in] x1 min x coordinate
 * \param[in] x2 max x coordinate
 * \param[in] y y coordinate
 */
void draw_horizontal_line(SDL_Renderer *ren, int x1, int x2, int y)
{
    SDL_RenderDrawLine(ren, x1, y, x2, y);
}

/*! \brief draw a vertical line (x axis: x , y axis: [y1 ; y2])
 * 
 * \pre y1 <= y2
 * \param[in] ren SDL renderer
 * \param[in] x x coordinate
 * \param[in] y1 min y coordinate
 * \param[in] y2 max y coordinate
 */
void draw_vertical_line(SDL_Renderer *ren, int x, int y1, int y2)
{
    SDL_RenderDrawLine(ren, x, y1, x, y2);
}

/*! \brief print a text
 * 
 * \param[in] str string to print
 * \param[in] font Font to use
 * \param[in] txt_col text color
 * \param[in] ren SDL renderer 
 * \param[in] x x position
 * \param[in] y y position
 * \param[in] pos_flag position flag
 */
void print_text_sdl(char *str, TTF_Font *font, SDL_Color txt_col, SDL_Renderer *ren, int x, int y, int pos_flag)
{
    SDL_Texture *text_texture;
    
    text_texture = renderText(font, str, txt_col, ren);
    renderTexture(text_texture, ren, x, y, pos_flag);
    SDL_DestroyTexture(text_texture);
}

/*! \brief apply a texture at a given corrdinate point
 * 
 * \param[in] tex SDL texture
 * \param[in] ren SDL renderer 
 * \param[in] x x position
 * \param[in] y y position
 * \param[in] pos_flag position flag
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int pos_flag)
{
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    SDL_Rect clip;

    //Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);  

    clip.x = 0;
    clip.y = 0;

    clip.w = dst.w;
    clip.h = dst.h;

    switch (pos_flag)
    {
        case 1: // position: right-bottom corner
            dst.x = x - dst.w;
            dst.y = y - dst.h;
            break;

        case 2: // position: left-mid height
            dst.x = x;
            dst.y = y - (dst.h / 2);
            break;
    
        default:
            dst.x = x;
            dst.y = y;
            break;
    }

    SDL_RenderCopy(ren, tex,  &clip, &dst);
}

/*! \brief create a texture for a message
 * 
 * \param[in] font requested Font
 * \param[in] message requested message
 * \param[in] text_color text color
 * \param[in] ren SDL renderer
 * \return requested message texture
 */
SDL_Texture* renderText(TTF_Font *font, char *message, SDL_Color text_color, SDL_Renderer *ren)
{
    SDL_Surface *text_surf;
    SDL_Texture *text_texture;

    text_surf = TTF_RenderText_Solid(font, message, text_color);
    if (text_surf == NULL)
    {
        log_SDL_error("TTF_RenderText_Solid");
    }

    text_texture = SDL_CreateTextureFromSurface(ren, text_surf);

    if (text_texture == NULL)
    {
        log_SDL_error("SDL_CreateTextureFromSurface");
    }

    SDL_FreeSurface(text_surf);

    return text_texture;
}

/*! \brief update the vectors and tabs used for the plots
 * 
 * \param[in,out] realtime real-time structure
 * \param[in] t curent simulation time [s]
 *
 * the min-max vectors used for the auto-scaling are also updated
 */
void update_plot_vectors(Simu_realtime *realtime, double t)
{
    Screen_sdl *screen_sdl;

    screen_sdl = realtime->ext->sdl->screen_sdl;

    screen_sdl->index_vec++;

    if (screen_sdl->index_vec >= screen_sdl->screen_plot_width)
    {
        screen_sdl->index_vec -= screen_sdl->screen_plot_width;
    }

    screen_sdl->t_vec[screen_sdl->index_vec] = t;
    
    update_scale_signals(screen_sdl, realtime, 0);
}

/*! \brief update 'y_tab' and 't_vec' for plot data durinf the breaks
 * 
 * \param[in,out] screen_sdl SDL main data structure
 */
void update_y_tab(Screen_sdl *screen_sdl)
{
    // -- Variables declaration -- //

    int i, j;
    int nb_curves;
    int prev_index, next_index;
    int buffer_size;
    int min_tsim_index, max_tsim_index;
    int flag_buffer_round;

    double weight_prev;
    double x_min, x_max, diff_x, step_x;
    double request_tsim;
    double tsim_prev, tsim_next;
    double min_tsim, max_tsim;
    double delta_tsim;

    double *tsim_save;
    double **y_save;
    double **y_tab;
    double *t_vec;

    AutoPlot *auto_plot;

    // -- Variables initialization -- //

    auto_plot = screen_sdl->auto_plot;

    nb_curves  = auto_plot->nb;

    min_tsim = screen_sdl->min_tsim;
    max_tsim = screen_sdl->max_tsim;

    t_vec = screen_sdl->t_vec;

    // no curves: just fill t_vec with time from origin
    if (!nb_curves)
    {
        delta_tsim = (max_tsim - min_tsim) / (screen_sdl->screen_plot_width - 1);

        // safety
        if (delta_tsim <= 0.0) { return; }

        for(i=0; i<screen_sdl->screen_plot_width; i++)
        {
            t_vec[i] = min_tsim + i * delta_tsim;
        }

        screen_sdl->index_vec = screen_sdl->screen_plot_width-1;
        return;
    }

    tsim_save  = screen_sdl->tsim_save;
    y_save = screen_sdl->y_save;

    min_tsim_index = screen_sdl->min_tsim_index;
    max_tsim_index = screen_sdl->max_tsim_index;

    buffer_size = screen_sdl->buffer_size;

    x_min = screen_sdl->x_min;
    x_max = screen_sdl->x_max;

    y_tab = screen_sdl->y_tab;

    flag_buffer_round = screen_sdl->flag_buffer_round;

    if (!max_tsim)
    {
        return;
    }

    // safety if not yet updated
    if (!screen_sdl->flag_started)
    {
        for(i=0; i<screen_sdl->screen_plot_width; i++)
        {
            t_vec[i] = 0.0;

            for(j=0; j<nb_curves; j++)
            {
                y_tab[j][i] = 0.0;
            }
        }

        // update the index for SDL
        screen_sdl->index_vec = screen_sdl->screen_plot_width - 1;

        return;
    }

    // step in the x axis for each pixel
    diff_x = x_max - x_min;
    step_x = diff_x / screen_sdl->screen_plot_width;

    // tsim value expected
    request_tsim = x_min + (step_x / 2.0);

    // loop on all the pixels
    for(i=0; i<screen_sdl->screen_plot_width; i++)
    {
        guess_tsim_index(request_tsim, tsim_save, min_tsim_index, max_tsim_index, min_tsim, max_tsim, buffer_size, flag_buffer_round, &prev_index, &next_index);

        if (prev_index == next_index)
        {
            // loop on all the curves
            for(j=0; j<nb_curves; j++)
            {
                y_tab[j][i] = y_save[j][prev_index];
            }

            // time vector
            t_vec[i] = tsim_save[prev_index];
        }
        else
        {
            // time
            tsim_prev = tsim_save[prev_index];
            tsim_next = tsim_save[next_index];

            // weight for the linear interpolation
            if (tsim_next > tsim_prev)
            {
                weight_prev = (tsim_next - request_tsim) / (tsim_next - tsim_prev);
            }
            else
            {
                weight_prev = 0.0;
            }

            // loop on all the curves
            for(j=0; j<nb_curves; j++)
            {
                y_tab[j][i] = weight_prev * y_save[j][prev_index] + (1 - weight_prev) * y_save[j][next_index];
            }

            // time vector
            t_vec[i] = weight_prev * tsim_prev + (1 - weight_prev) * tsim_next;
        }

        // new time value
        request_tsim += step_x;
    }

    // update the index for SDL
    screen_sdl->index_vec = screen_sdl->screen_plot_width - 1;
}

/*! \brief update legends for new number of curves
 * 
 * \param[in,out] realtime real-time structure
 * \param[in] new_nb_legends new number of curves
 */
void update_sdl_legend(Simu_realtime *realtime, int new_nb_legends)
{
    int i;
    int nb_legends;
    int space_up_label;

    Screen_sdl *screen_sdl;

    screen_sdl = realtime->ext->sdl->screen_sdl;

    // safety
    if (new_nb_legends > screen_sdl->max_nb_legends)
    {
        new_nb_legends = screen_sdl->max_nb_legends;
    }
    else if (new_nb_legends < 0)
    {
        new_nb_legends = 0;
    }

    nb_legends = screen_sdl->nb_legends;

    // no update needed
    if (new_nb_legends == nb_legends)
    {
        return;
    }

    if (nb_legends)
    {
        free(screen_sdl->pixel_pos_label);
    }

    // position of the labels
    if (new_nb_legends)
    {
        screen_sdl->pixel_pos_label = (int*) malloc(new_nb_legends * sizeof(int));

        space_up_label = (screen_sdl->screen_width - UP_COLOR_WIDTH_LOSS_BEGIN - UP_COLOR_WIDTH_LOSS_END) / new_nb_legends;

        for (i=0; i<new_nb_legends; i++)
        {
            screen_sdl->pixel_pos_label[i] = UP_COLOR_WIDTH_LOSS_BEGIN + i* space_up_label;
        }
    }

    screen_sdl->nb_legends = new_nb_legends;
    
    plot_screen_sdl(realtime, realtime->tsim, 3);
}

/*! \brief bound the screen motion to stay in the x bounds
 * 
 * \param[in] min_thres minimal threshold
 * \param[in] max_thres maximal threshold
 * \param[in] cur_min current minimum
 * \param[in] cur_max current maximum
 * \param[in] cur_move current move
 * \param[in] sign_flag flag for sign
 * \return move bounded
 */
double bound_diff_min_max(double min_thres, double max_thres, double cur_min, double cur_max, double cur_move, int sign_flag)
{
    // variables declaration

    double new_min, new_max;

    // bound: minimal   

    if (sign_flag <= 0)
    {
        new_min = cur_min - cur_move;
    }
    else 
    {
        new_min = cur_min + cur_move;
    }

    if (new_min <= min_thres)
    {
        if (sign_flag <= 0)
        {
            cur_move = cur_min - min_thres;
        }
        else 
        {
            cur_move = min_thres - cur_min;
        }
    }

    // bound: maximal

    if (sign_flag >= 0)
    {
        new_max = cur_max + cur_move;
    }
    else 
    {
        new_max = cur_max - cur_move;
    }

    if (new_max >= max_thres)
    {
        if (sign_flag >= 0)
        {
            cur_move = max_thres - cur_max;
        }
        else
        {
            cur_move = cur_max - max_thres;
        }
    }

    return cur_move;
}

/*! \brief update the plot ranges according to the mouse inputs
 * 
 * \param[in,out] screen_sdl SDL main data structure
 * \param[in,out] realtime real-time structure
 * \param[in] tsim current simulation time [s]
 * \param[in] mouse_flag flag for the mouse
 */
void mouse_sdl(Screen_sdl *screen_sdl, Simu_realtime *realtime, double tsim, int mouse_flag)
{
    // variables declaration

    double x_min_plot, x_max_plot, y_min_plot, y_max_plot;
    double diff_x_plot, diff_y_plot;
    double new_diff_x_plot, new_diff_y_plot;
    double move_x_min_max, move_y_min_max;
    double ratio_min_x, ratio_min_y;
    double x_star, y_star;

    Realtime_sdl *realtime_sdl;

    realtime_sdl = realtime->ext->sdl;

    // y axis

    if ( (!realtime_sdl->mouse_wheel_flag) || (realtime_sdl->mouse_wheel_flag == 1) )
    {
        y_min_plot  = screen_sdl->y_min;
        y_max_plot  = screen_sdl->y_max;
        diff_y_plot = y_max_plot - y_min_plot;

        if (!mouse_flag)
        {
            move_y_min_max = (realtime_sdl->mouse_delta_y * diff_y_plot) / screen_sdl->screen_plot_height;
        }
        else
        {
            ratio_min_y = 1.0 - ( ((double) realtime_sdl->mouse_cur_y - PLOT_Y_START) / screen_sdl->screen_plot_height );
            y_star      = y_min_plot + ratio_min_y * diff_y_plot;
        }

        if (!mouse_flag)
        {
            screen_sdl->y_min = y_min_plot + move_y_min_max;
            screen_sdl->y_max = y_max_plot + move_y_min_max;
        }
        else 
        {
            if (mouse_flag < 0)
            {
                new_diff_y_plot = diff_y_plot / WHEEL_MOUSE_FACTOR;
            }
            else
            {
                new_diff_y_plot = diff_y_plot * WHEEL_MOUSE_FACTOR;
            }

            screen_sdl->y_min = y_star - ratio_min_y * new_diff_y_plot;
            screen_sdl->y_max = screen_sdl->y_min + new_diff_y_plot;
        }
    }


    // x axis

    if (realtime->simu_break)
    {
        if ( (!realtime_sdl->mouse_wheel_flag) || (realtime_sdl->mouse_wheel_flag == 2) )
        {
            x_min_plot  = screen_sdl->x_min;
            x_max_plot  = screen_sdl->x_max;
            diff_x_plot = x_max_plot - x_min_plot;

            if (!mouse_flag)
            {
                move_x_min_max = (realtime_sdl->mouse_delta_x * diff_x_plot) / screen_sdl->screen_plot_width;
            }
            else
            {
                ratio_min_x = ((double) realtime_sdl->mouse_cur_x) / screen_sdl->screen_plot_width;
                x_star      = x_min_plot + ratio_min_x * diff_x_plot;
            }

            if (!mouse_flag)
            {
                // safety for the bounds
                move_x_min_max = bound_diff_min_max(screen_sdl->min_tsim, screen_sdl->max_tsim, x_min_plot, x_max_plot, move_x_min_max, -1);

                screen_sdl->x_min = x_min_plot - move_x_min_max;
                screen_sdl->x_max = x_max_plot - move_x_min_max;
            }
            else
            {
                if (mouse_flag < 0)
                {
                    new_diff_x_plot = diff_x_plot / WHEEL_MOUSE_FACTOR;
                }
                else
                {
                    new_diff_x_plot = diff_x_plot * WHEEL_MOUSE_FACTOR;
                }

                screen_sdl->x_min = x_star - ratio_min_x * new_diff_x_plot;
                screen_sdl->x_max = screen_sdl->x_min + new_diff_x_plot;

                if (screen_sdl->x_min <= realtime->t0)
                {
                    screen_sdl->x_min = realtime->t0;
                }

                if (screen_sdl->x_max >= tsim)
                {
                    screen_sdl->x_max = tsim;
                }
            }
        }       
    }
    else
    {
        screen_sdl->scaling_flag = 1;
    }

    limit_x_limits(screen_sdl);

    // update the plots
    plot_screen_sdl(realtime, tsim, realtime->simu_break);
}

/*! \brief set 'bottom_flag' to 1
 * 
 * \param[in,out] realtime real-time structure
 */
void set_bottom_flag(Simu_realtime *realtime)
{
    Screen_sdl *screen_sdl;

    screen_sdl = (Screen_sdl*) realtime->ext->sdl->screen_sdl;

    screen_sdl->bottom_flag = 1;
}

/*! \brief compute the minimal and maximal times at the beginning and at the end of a break
 * 
 * \param[in,out] screen_sdl SDL main data structure
 * \param[in,out] realtime real-time structure
 */
void update_x_min_max(Screen_sdl *screen_sdl, Simu_realtime *realtime)
{
    double cur_tsim;

    cur_tsim = screen_sdl->tsim_save[screen_sdl->max_tsim_index];

    screen_sdl->x_max = cur_tsim;
    screen_sdl->x_min = cur_tsim - ( screen_sdl->tsim_pixel * screen_sdl->screen_plot_width * get_simu_speed_factor(realtime->simu_speed_flag) );

    limit_x_limits(screen_sdl);
}

/*! \brief update the minimal and maximal value of each signal
 * 
 * \param[in,out] realtime real-time structure
 */
void update_y_min_max(Simu_realtime *realtime)
{
    // variables declaration
    int i;
    int nb_curves;

    double *y_tab_min;
    double *y_tab_max;
    double *y_curves;

    Screen_sdl *screen_sdl;
    AutoPlot *auto_plot;

    screen_sdl = realtime->ext->sdl->screen_sdl;
    auto_plot  = screen_sdl->auto_plot;
    y_curves   = auto_plot->y_curves;

    nb_curves = auto_plot->nb;

    if (!nb_curves)
    {
        return;
    }

    // tabs
    y_tab_min = screen_sdl->y_tab_min;
    y_tab_max = screen_sdl->y_tab_max;

    // loop on the different curves
    for(i=0; i<nb_curves; i++)
    {
        if ((y_curves[i] < y_tab_min[i]) || (y_tab_min[i] == START_VECTOR))
        {
            y_tab_min[i] = y_curves[i];
        }

        if ((y_curves[i] > y_tab_max[i]) || (y_tab_max[i] == START_VECTOR))
        {
            y_tab_max[i] = y_curves[i];
        }
    }
}

/*! \brief update the vectors with the full information (for each time step of the simulation)
 *
 * \param[in,out] realtime real-time structure 
 * \param[in] tsim current simulation time [s]
 */
void update_full_vectors(Simu_realtime *realtime, double tsim)
{
    int i;
    int min_tsim_index;
    int buffer_size;
    int nb_curves;

    double *tsim_save;
    double **y_save;
    double *y_curves;

    Screen_sdl *screen_sdl;
    AutoPlot *auto_plot;

    screen_sdl = realtime->ext->sdl->screen_sdl;
    auto_plot  = screen_sdl->auto_plot;
    y_curves   = auto_plot->y_curves;

    nb_curves = auto_plot->nb;

    if (!nb_curves)
    {
        screen_sdl->max_tsim = tsim;
        return;
    }

    if (auto_plot->nb_new_curves > 0)
    {
        new_curves_save(screen_sdl, auto_plot->nb_new_curves);

        auto_plot->nb_new_curves = 0;

        update_sdl_legend(realtime, auto_plot->nb);
    }

    screen_sdl->flag_started = 1;

    min_tsim_index = screen_sdl->min_tsim_index;

    buffer_size = screen_sdl->buffer_size;

    tsim_save = screen_sdl->tsim_save;
    y_save    = screen_sdl->y_save;

    // update the time
    tsim_save[min_tsim_index] = tsim;

    // update the vectors
    for (i=0; i<nb_curves; i++)
    {
        y_save[i][min_tsim_index] = y_curves[i];
    }

    // update indexes
    screen_sdl->max_tsim_index = min_tsim_index;

    min_tsim_index++;

    if (min_tsim_index >= buffer_size)
    {
        min_tsim_index = 0;
        screen_sdl->flag_buffer_round = 1;
    }

    screen_sdl->min_tsim_index = min_tsim_index;

    // update min-max
    screen_sdl->min_tsim = tsim_save[min_tsim_index];
    screen_sdl->max_tsim = tsim;
}

/*! \brief scaling of the signals
 * 
 * \param[in,out] screen_sdl SDL main data structure
 * \param[in,out] realtime real-time structure
 * \param[in] screen_flag 0:running simulation ; 1:break ; 2:initialization ; 3:update legend
 */
void update_scale_signals(Screen_sdl *screen_sdl, Simu_realtime *realtime, int screen_flag)
{
    // -- Variables declaration -- //

    int i;
    int nb_curves;
    int cur_scaling_expo_mult;

    double y_min_A, y_max_A;
    double cur_y_min, cur_y_max;
    double mean_A, semi_diff_A;
    double cur_mean, cur_semi_diff;
    double min_plot_A, max_plot_A;
    double cur_fact_semi_diff, cur_scale_fact_semi_diff;
    double cur_scale_y_min, cur_scale_y_max, cur_scale_semi_diff;
    double y_scale_super_min, y_scale_super_max;
    double y_min_plot, y_max_plot, diff_y_plot;
    double x_min_plot, x_max_plot, diff_x_plot;
    double diff_y_scale_super;    

    int *scaling_expo_mult;

    double *y_tab_min;
    double *y_tab_max;
    double *scaling_offset;

    AutoPlot *auto_plot;


    // -- Variables initialization -- //

    auto_plot  = screen_sdl->auto_plot;

    nb_curves = auto_plot->nb;

    if (!nb_curves)
    {
        return;
    }

    y_tab_min = screen_sdl->y_tab_min;
    y_tab_max = screen_sdl->y_tab_max;

    scaling_expo_mult = screen_sdl->scaling_expo_mult;
    scaling_offset    = screen_sdl->scaling_offset;

    // A: first signal
    y_min_A = y_tab_min[0];
    y_max_A = y_tab_max[0];

    mean_A      = 0.5 * (y_min_A + y_max_A);
    semi_diff_A = 0.5 * (y_max_A - y_min_A);

    // safety
    if (!semi_diff_A)
    {
        semi_diff_A = 0.5;
    }

    // limits for the signal auto-scaling
    min_plot_A = mean_A - PLOT_SCALE_MAX * semi_diff_A;
    max_plot_A = mean_A + PLOT_SCALE_MAX * semi_diff_A;

    // reset during the break
    if (realtime->simu_break)
    {
        for(i=0; i<nb_curves; i++)
        {
            scaling_expo_mult[i] = 0;
            scaling_offset[i]    = 0.0;
        }
    }


    // -- Scaling curves -- //

    if (screen_sdl->signal_auto_scaling)
    {
        // loop on the curves
        for(i=1; i<nb_curves; i++)
        {
            cur_y_min = y_tab_min[i];
            cur_y_max = y_tab_max[i];

            // current scaled signal
            cur_scale_y_min = scale_signal(cur_y_min, scaling_expo_mult[i], scaling_offset[i]);
            cur_scale_y_max = scale_signal(cur_y_max, scaling_expo_mult[i], scaling_offset[i]);

            cur_semi_diff      = 0.5 * (cur_y_max - cur_y_min);
            cur_fact_semi_diff = fabs(cur_semi_diff / semi_diff_A);

            cur_scale_semi_diff      = 0.5 * (cur_scale_y_max - cur_scale_y_min);
            cur_scale_fact_semi_diff = fabs(cur_scale_semi_diff / semi_diff_A);

            // Scaling factor

            if ( (cur_scale_fact_semi_diff < (1.0/SCALING_MULT_THRESHOLD)) || (cur_scale_fact_semi_diff > SCALING_MULT_THRESHOLD) )
            {
                cur_scaling_expo_mult = 0;

                if (cur_fact_semi_diff)
                {
                    while (cur_fact_semi_diff > SCALING_MULT_THRESHOLD)
                    {
                        cur_scaling_expo_mult++;
                        cur_fact_semi_diff *= 0.1;
                    }

                    while (cur_fact_semi_diff < (1.0 / SCALING_MULT_THRESHOLD) )
                    {
                        cur_scaling_expo_mult--;
                        cur_fact_semi_diff *= 10.0;
                    }
                }

                scaling_expo_mult[i] = cur_scaling_expo_mult;

                scaling_offset[i] = 0.0;

                cur_scale_y_min = scale_signal(cur_y_min, cur_scaling_expo_mult, scaling_offset[i]);
                cur_scale_y_max = scale_signal(cur_y_max, cur_scaling_expo_mult, scaling_offset[i]);

                screen_sdl->scaling_flag = 1;
            }
            else
            {
                cur_scaling_expo_mult = scaling_expo_mult[i];
            }

            // Scaling offset

            if ( (cur_scale_y_min < min_plot_A) || (cur_scale_y_max > max_plot_A))
            {
                cur_mean = 0.5 * (cur_y_min + cur_y_max);

                if ( (cur_scale_y_min < min_plot_A) || (cur_scale_y_max > max_plot_A))
                {
                    scaling_offset[i] = cur_mean - pow_int(10.0, cur_scaling_expo_mult) * mean_A;
                }
                screen_sdl->scaling_flag = 1;
            }
        }
    }
    // no auto-scaling
    else
    {
        for(i=0; i<nb_curves; i++)
        {
            if (scaling_expo_mult[i] != 0)
            {
                scaling_expo_mult[i]     = 0;
                screen_sdl->scaling_flag = 1;
            }
            if (scaling_offset[i] != 0.0)
            {
                scaling_offset[i]        = 0.0;
                screen_sdl->scaling_flag = 1;
            }
        }
    }

    
    // -- Scaling Plot limits -- //

    // y axis
    y_min_plot  = screen_sdl->y_min;
    y_max_plot  = screen_sdl->y_max;
    diff_y_plot = y_max_plot - y_min_plot;

    // x axis
    x_min_plot  = screen_sdl->x_min;
    x_max_plot  = screen_sdl->x_max;
    diff_x_plot = x_max_plot - x_min_plot;

    // auto scaling: y axis
    if (screen_sdl->plot_auto_scaling)
    {
        y_scale_super_min = y_min_A;
        y_scale_super_max = y_max_A;

        for(i=1; i<nb_curves; i++)
        {
            cur_scale_y_min = scale_signal(y_tab_min[i], scaling_expo_mult[i], scaling_offset[i]);
            cur_scale_y_max = scale_signal(y_tab_max[i], scaling_expo_mult[i], scaling_offset[i]);

            if (cur_scale_y_min < y_scale_super_min)
            {
                y_scale_super_min = cur_scale_y_min;
            }

            if (cur_scale_y_max > y_scale_super_max)
            {
                y_scale_super_max = cur_scale_y_max;
            }
        }

        if (screen_flag || (y_scale_super_min < y_min_plot + SCALING_PLOT_TOO_SMALL * diff_y_plot) ||
             (y_scale_super_max > y_max_plot - SCALING_PLOT_TOO_SMALL * diff_y_plot))
        {
            diff_y_scale_super = y_scale_super_max - y_scale_super_min;

            if (!diff_y_scale_super)
            {
                diff_y_scale_super = SCALING_NUL_DIFF;
            }

            screen_sdl->y_min = y_scale_super_min - SCALING_PLOT_OPTIMAL_INCREASE * diff_y_scale_super;
            screen_sdl->y_max = y_scale_super_max + SCALING_PLOT_OPTIMAL_INCREASE * diff_y_scale_super;

            screen_sdl->scaling_flag = 1;
        }
        else if ((y_scale_super_min > y_min_plot + SCALING_PLOT_TOO_HIGH  * diff_y_plot) ||
                 (y_scale_super_max < y_max_plot - SCALING_PLOT_TOO_HIGH  * diff_y_plot) )
        {
            diff_y_scale_super = y_scale_super_max - y_scale_super_min;

            if (!diff_y_scale_super)
            {
                diff_y_scale_super = SCALING_NUL_DIFF;
            }

            screen_sdl->y_min = y_scale_super_min - SCALING_PLOT_OPTIMAL_DECREASE * diff_y_scale_super;
            screen_sdl->y_max = y_scale_super_max + SCALING_PLOT_OPTIMAL_DECREASE * diff_y_scale_super;

            screen_sdl->scaling_flag = 1;       
        }
    }

    // no auto-scaling: y axis
    else if (screen_sdl->increase_plot_y_diff_flag)
    {
        screen_sdl->increase_plot_y_diff_flag = 0;

        screen_sdl->y_min = y_min_plot - MANUAL_SCALING_PLOT * diff_y_plot;
        screen_sdl->y_max = y_max_plot + MANUAL_SCALING_PLOT * diff_y_plot;

        screen_sdl->scaling_flag = 1;
    }
    else if (screen_sdl->decrease_plot_y_diff_flag)
    {
        screen_sdl->decrease_plot_y_diff_flag = 0;

        screen_sdl->y_min = y_min_plot + 0.5 * MANUAL_SCALING_PLOT * diff_y_plot;
        screen_sdl->y_max = y_max_plot - 0.5 * MANUAL_SCALING_PLOT * diff_y_plot;

        screen_sdl->scaling_flag = 1;
    }
    else if (screen_sdl->up_plot_y_flag)
    {
        screen_sdl->up_plot_y_flag = 0;

        screen_sdl->y_min = y_min_plot + MANUAL_OFFSET_PLOT * diff_y_plot;
        screen_sdl->y_max = y_max_plot + MANUAL_OFFSET_PLOT * diff_y_plot;

        screen_sdl->scaling_flag = 1;
    }
    else if (screen_sdl->bottom_plot_y_flag)
    {
        screen_sdl->bottom_plot_y_flag = 0;

        screen_sdl->y_min = y_min_plot - MANUAL_OFFSET_PLOT * diff_y_plot;
        screen_sdl->y_max = y_max_plot - MANUAL_OFFSET_PLOT * diff_y_plot;

        screen_sdl->scaling_flag = 1;
    }

    // x axis
    if (screen_sdl->increase_plot_x_diff_flag)
    {
        screen_sdl->increase_plot_x_diff_flag = 0;

        screen_sdl->x_min = x_min_plot - MANUAL_SCALING_PLOT * diff_x_plot;
        screen_sdl->x_max = x_max_plot + MANUAL_SCALING_PLOT * diff_x_plot;

        limit_x_limits(screen_sdl);

    }
    else if (screen_sdl->decrease_plot_x_diff_flag)
    {
        screen_sdl->decrease_plot_x_diff_flag = 0;

        screen_sdl->x_min = x_min_plot + 0.5 * MANUAL_SCALING_PLOT * diff_x_plot;
        screen_sdl->x_max = x_max_plot - 0.5 * MANUAL_SCALING_PLOT * diff_x_plot;

        limit_x_limits(screen_sdl);
    }
    else if (screen_sdl->right_plot_y_flag)
    {
        screen_sdl->right_plot_y_flag = 0;

        screen_sdl->x_min = x_min_plot + MANUAL_OFFSET_PLOT * diff_x_plot;
        screen_sdl->x_max = x_max_plot + MANUAL_OFFSET_PLOT * diff_x_plot;

        limit_x_limits(screen_sdl);
    }
    else if (screen_sdl->left_plot_y_flag)
    {
        screen_sdl->left_plot_y_flag = 0;

        screen_sdl->x_min = x_min_plot - MANUAL_OFFSET_PLOT * diff_x_plot;
        screen_sdl->x_max = x_max_plot - MANUAL_OFFSET_PLOT * diff_x_plot;

        limit_x_limits(screen_sdl);
    }
}

/*! \brief limit the x axis minimal and maximal values
 * 
 * \param[in,out] screen_sdl SDL main data structure
 */
void limit_x_limits(Screen_sdl *screen_sdl)
{
    double min_tsim, max_tsim;

    min_tsim = screen_sdl->min_tsim;
    max_tsim = screen_sdl->max_tsim;

    if (screen_sdl->x_min < min_tsim)
    {
        screen_sdl->x_min = min_tsim;
    }

    if (screen_sdl->x_max > max_tsim)
    {
        screen_sdl->x_max = max_tsim;
    }

    if (screen_sdl->x_max <= screen_sdl->x_min)
    {
        screen_sdl->x_max = screen_sdl->x_min + SAFETY_TIME;
    }
}

/*! \brief return the value 'no_scale_nb' after scaling
 * 
 * \param[in] no_scale_nb value to scale
 * \param[in] scaling_expo_mult exponent multiplication for the curves scaling
 * \param[in] scaling_offset offset for the curves scaling
 * \return 'no_scale_nb' after scaling
 */
double scale_signal(double no_scale_nb, int scaling_expo_mult, double scaling_offset)
{
    double scale_nb;

    scale_nb = no_scale_nb - scaling_offset;

    if (scaling_expo_mult)
    {
        scale_nb *= ( 1.0 / pow_int(10.0, scaling_expo_mult) );
    }

    return scale_nb;
}

/*! \brief return the value 'scale_nb' before scaling
 * 
 * \param[in] scale_nb value to get before scaling
 * \param[in] scaling_expo_mult exponent multiplication for the curves scaling
 * \param[in] scaling_offset offset for the curves scaling
 * \return 'scale_nb' before scaling
 */
double unscale_signal(double scale_nb, int scaling_expo_mult, double scaling_offset)
{
    return (scale_nb * pow_int(10.0, scaling_expo_mult)) + scaling_offset;
}

/*! \brief create a string to represent the value of the real signal (no scaled)
 * 
 * \param[out] str string to create
 * \param[in] scaling_expo_mult exponent multiplication for the curves scaling
 * \param[in] scaling_offset offset for the curves scaling
 */
void str_scaling(char *str, int scaling_expo_mult, double scaling_offset)
{
    // variables decalration
    int exp_inf;
    char str_offset[10];

    // offset present
    if (scaling_offset)
    {
        exp_inf = expo_ten_inf(scaling_offset);

        if ((exp_inf > 2) || (exp_inf < -1))
        {
            sprintf(str_offset,"%.1fe%d", fabs(scaling_offset / pow_int(10.0, exp_inf)) , exp_inf );
        }
        else
        {
            if (exp_inf < 0)
            {
                sprintf(str_offset,"%.3f", scaling_offset);
            }
            else
            {
                sprintf(str_offset,"%.2f", scaling_offset);
            }
        }
    }
    
    // scaling with multiplication
    if (scaling_expo_mult)
    {
        if (scaling_offset)
        {
            if (scaling_offset > 0.0)
            {
                sprintf(str, "(1e%d x + %s)", scaling_expo_mult, str_offset);
            }
            else
            {
                sprintf(str, "(1e%d x - %s)", scaling_expo_mult, str_offset);
            }
        }
        else
        {
            sprintf(str, "(1e%d x)", scaling_expo_mult);
        }
    }
    // no scaling with multiplication
    else
    {
        if (scaling_offset)
        {
            
            if (scaling_offset > 0.0)
            {
                sprintf(str, "(+ %s)", str_offset);
            }
            else
            {
                sprintf(str, "(- %s)", str_offset);
            }
        }
        else
        {
            sprintf(str, " ");
        }
    }
}

/*! \brief create a string representing bounds
 * 
 * \param[out] str string to create 
 * \param[in] low_nb low number
 * \param[in] high_nb high number
 * \return the exponent (base 10) of these bounds used for the string
 */
int str_label(char *str, double low_nb, double high_nb)
{
    // variables declaration
    int exp_inf;
    double diff_nb;
    double div_exp;

    // string computation
    diff_nb = fabs(high_nb - low_nb);

    // numerical safety
    if (fabs(diff_nb) < STR_LABEL_MIN)
    {
        sprintf(str,"[%.2f ; %.2f]", low_nb, high_nb );

        return 0;
    }

    exp_inf  = expo_ten_inf(diff_nb);

    div_exp = pow_int(10.0, exp_inf);

    if ((exp_inf > 2) || (exp_inf < -1))
    {
        sprintf(str,"[%.2f ; %.2f]e%d", low_nb / div_exp , high_nb / div_exp, exp_inf);
    }
    else
    {
        sprintf(str,"[%.2f ; %.2f]", low_nb, high_nb );
    }

    return exp_inf;
}

/*! \brief create a string representing a number
 * 
 * \param[out] str string to create
 * \param[in] nb number
 * \param[in] expo exponent
 */
void str_axis(char *str, double nb, int expo)
{
    double div_exp;

    div_exp = pow_int(10.0, expo);

    if ((expo > 2) || (expo < -1))
    {
        sprintf(str,"%.2f", nb / div_exp);
    }
    else
    {
        sprintf(str,"%.2f", nb);
    }
}

/*! \brief main function used to plot signals along with the corresponding indications
 * 
 * \param[in,out] realtime real-time structure
 * \param[in] tsim current simulation time [s]
 * \param[in] screen_flag 0:running simulation ; 1:break ; 2:initialization ; 3:update legend
 */
void plot_screen_sdl(Simu_realtime *realtime, double tsim, int screen_flag)
{
    //  -- Variables declaration -- //

    int i, j, k;
    int found_y_value;
    int nb_curves_sdl;
    int max_tsim_index;
    int this_y_pixel;
    int init_x_index, end_x_index;
    int y_pixel_nul_axis;
    int index_vec, index_sdl, next_index_sdl;
    int second_next_index_sdl, erase_index_sdl;
    int y_pixel_label;
    int x_end_label_plot;
    int x_pixel_label;
    int x_pixel_past;
    int ind_vec;
    int pixel_pos_label;
    int nb_legends;
    int curve_width;
    int request_color;
    
    double pixel_y_min, pixel_y_max;
    double pixel_height;
    double y_min_sdl, y_max_sdl;
    double prev_diff_y_sdl;
    double prev_y_diff_label;
    double prev_y_value;
    double this_y, prev_y;
    double this_y0, this_y1;
    double diff_y_sdl;
    double alpha_nul_axis;
    double y_value;
    double y_diff_label;
    double tsim_end, tsim_init;
    double tsim_x_label;
    double legend_x_min, legend_x_max;
    double tsim_x_label_minus_1, tsim_x_label_minus_2;
    double speed_factor_plot;

    char str[80];

    int *color_tab;
    int *scaling_expo_mult;
    int *last_scaling_expo_mult;

    double *previous_y_vec;
    double **y_save;
    double **y_tab;
    double *t_vec;
    double *scaling_offset;
    double *last_scaling_offset;

    SDL_Color text_color;

    SDL_Renderer *ren;

    TTF_Font *font_label;
    TTF_Font *font_small_label;
    TTF_Font *font_axis;
    TTF_Font *font_scaling;
    TTF_Font *font_arrows;
    TTF_Font *font_play;
    TTF_Font *font_hor_mouse;
    TTF_Font *font_mouse;   
    TTF_Font *font_break;
    TTF_Font *font_speed;

    Screen_sdl *screen_sdl;
    Realtime_sdl *realtime_sdl;
    AutoPlot *auto_plot;

    #ifdef JAVA
    Realtime_java *java;
    #endif


    // -- Variables initialization -- //

    realtime_sdl = realtime->ext->sdl;
    screen_sdl   = realtime_sdl->screen_sdl;
    auto_plot    = screen_sdl->auto_plot;

    if ((screen_flag == 1) && (!realtime->first_break))
    {
        update_y_tab(screen_sdl);
    }

    t_vec             = screen_sdl->t_vec;
    y_tab             = screen_sdl->y_tab;
    ren               = screen_sdl->ren;
    color_tab         = screen_sdl->color_tab;
    font_label        = screen_sdl->font_label;
    font_small_label  = screen_sdl->font_small_label;   
    font_axis         = screen_sdl->font_axis;
    font_scaling      = screen_sdl->font_scaling;
    font_arrows       = screen_sdl->font_arrows;
    font_play         = screen_sdl->font_play;
    font_hor_mouse    = screen_sdl->font_hor_mouse; 
    font_mouse        = screen_sdl->font_mouse; 
    font_break        = screen_sdl->font_break;
    font_speed        = screen_sdl->font_speed;
    text_color        = screen_sdl->text_color;
    scaling_expo_mult = screen_sdl->scaling_expo_mult;
    scaling_offset    = screen_sdl->scaling_offset;
    max_tsim_index    = screen_sdl->max_tsim_index;
    previous_y_vec    = screen_sdl->previous_y_vec;
    y_save            = screen_sdl->y_save;
    index_vec         = screen_sdl->index_vec;
    index_sdl         = screen_sdl->index_sdl;
    y_min_sdl         = screen_sdl->y_min;
    y_max_sdl         = screen_sdl->y_max;
    curve_width       = screen_sdl->curve_width;

    nb_curves_sdl     = auto_plot->nb;

    // next index (after the current one)
    next_index_sdl = index_sdl + 1;
    if (next_index_sdl >= screen_sdl->screen_width)
    {
        next_index_sdl -= screen_sdl->screen_width;
    }

    // index after the next one
    second_next_index_sdl = next_index_sdl + 1;
    if (second_next_index_sdl >= screen_sdl->screen_width)
    {
        second_next_index_sdl -= screen_sdl->screen_width;
    }

    // index to erase with white color
    erase_index_sdl = next_index_sdl + X_ERASE_WIDTH;
    if (erase_index_sdl >= screen_sdl->screen_width)
    {
        erase_index_sdl -= screen_sdl->screen_width;
    }

    // difference in y axis
    diff_y_sdl      = y_max_sdl - y_min_sdl;
    prev_diff_y_sdl = screen_sdl->last_y_max - screen_sdl->last_y_min;

    // difference in y axis for the axis labels
    y_diff_label      = diff_y_sdl / Y_LABEL_NB;
    prev_y_diff_label = prev_diff_y_sdl / Y_LABEL_NB;

    // height of a pixel
    pixel_height = diff_y_sdl / screen_sdl->screen_plot_height;
    
    // height corresponding to 0
    alpha_nul_axis = y_max_sdl / diff_y_sdl;
    #ifdef UNIX
    y_pixel_nul_axis = round(alpha_nul_axis * screen_sdl->plot_y_end + (1-alpha_nul_axis) * PLOT_Y_START);
    #else
    y_pixel_nul_axis = (alpha_nul_axis * screen_sdl->plot_y_end + (1-alpha_nul_axis) * PLOT_Y_START);
    #endif


    // -- New curves safety -- //

    if (screen_sdl->auto_plot->nb_new_curves > 0)
    {
        new_curves_save(screen_sdl, screen_sdl->auto_plot->nb_new_curves);

        screen_sdl->auto_plot->nb_new_curves = 0;

        update_sdl_legend(realtime, screen_sdl->auto_plot->nb);
    }


    // -- Refresh screen -- //

    if (screen_flag && (screen_flag != 3))
    {
        refresh_screen(screen_sdl);
        screen_sdl->last_scaling_flag = 0;
    }


    // -- Light gray for up and bottom -- //

    SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);

    if (screen_flag)
    {
        // up
        draw_rectangle(ren, 0, screen_sdl->screen_width-1, 0, PLOT_Y_START-2);

        // bottom
        draw_rectangle(ren, 0, screen_sdl->screen_width-1, screen_sdl->plot_y_end+1, screen_sdl->screen_height-1);
    }

    
    // -- Horizontal black lines -- //

    SDL_set_color(screen_sdl, BLACK_SDL);

    if (screen_flag)
    {
        draw_horizontal_line(ren, 0, screen_sdl->screen_width-1, PLOT_Y_START-1);
        draw_horizontal_line(ren, 0, screen_sdl->screen_width-1, screen_sdl->plot_y_end);
    }


    // -- Vertical black line -- //

    SDL_set_color(screen_sdl, BLACK_SDL);

    if (screen_flag == 1)
    {
        draw_vertical_line(ren, screen_sdl->plot_x_end-1, PLOT_Y_START, screen_sdl->plot_y_end-1);
    }
    else if(!screen_flag)
    {
        draw_vertical_line(ren, next_index_sdl, PLOT_Y_START, screen_sdl->plot_y_end-1);
    }


    // -- Erase with white rectangle -- //

    if(!screen_flag)
    {
        SDL_set_color(screen_sdl, WHITE_SDL);

        if (erase_index_sdl < second_next_index_sdl)
        {
            draw_rectangle(ren, second_next_index_sdl, screen_sdl->screen_width-1, PLOT_Y_START, screen_sdl->plot_y_end-1);
            draw_rectangle(ren, 0, erase_index_sdl, PLOT_Y_START, screen_sdl->plot_y_end-1);
        }
        else
        {
            draw_rectangle(ren, second_next_index_sdl, erase_index_sdl, PLOT_Y_START, screen_sdl->plot_y_end-1);
        }
    }


    // -- Curves -- //

    if (screen_flag != 2) // not for the first initialization
    {
        // indexes to work with
        if (screen_flag == 1)
        {
            init_x_index = 0;
            end_x_index = screen_sdl->plot_x_end-2;
        }
        else
        {
            init_x_index = index_sdl;
            end_x_index  = index_sdl;
        }
        
        // loop on all the pixels in the width (x axis)
        for (i = init_x_index; i <= end_x_index; i++)
        {
            // black line for auto scaling
            if ((!screen_flag) && (screen_sdl->scaling_flag) && (!screen_sdl->last_scaling_flag))
            {
                SDL_set_color(screen_sdl, BLACK_SDL);
                draw_vertical_line(ren, i, PLOT_Y_START, screen_sdl->plot_y_end-1);
            }
            else 
            {
                // erase with a white line
                if (!screen_flag)
                {
                    SDL_set_color(screen_sdl, WHITE_SDL);
                    draw_vertical_line(ren, i, PLOT_Y_START, screen_sdl->plot_y_end-1);
                }

                // loop on the pixels in the height (y axis)
                for (j = 0; j < screen_sdl->screen_plot_height; j++)
                {
                    found_y_value = 0; // remains white pixel

                    // y pixel position
                    this_y_pixel = PLOT_Y_START + j;

                    pixel_y_max = y_max_sdl   - (j - (curve_width/2)) * pixel_height;
                    pixel_y_min = pixel_y_max - pixel_height * curve_width;

                    // x pixel position
                    if (!screen_flag)
                    {
                        ind_vec = index_vec;
                    }
                    else
                    {
                        ind_vec = index_vec + 2 + i;
                    }

                    // limit
                    if (ind_vec >= screen_sdl->screen_plot_width)
                    {
                        ind_vec -= screen_sdl->screen_plot_width;
                    }

                    // loop on all the curves
                    for (k=0; k<nb_curves_sdl; k++)
                    {
                        // current y value and previous one
                        if (!screen_flag)
                        {
                            this_y = y_save[k][max_tsim_index];
                            prev_y = previous_y_vec[k];                 
                        }
                        else
                        {
                            this_y = y_tab[k][ind_vec];                 

                            if (ind_vec <= 0)
                            {
                                prev_y = y_tab[k][screen_sdl->screen_plot_width-1];
                            }
                            else
                            {
                                prev_y = y_tab[k][ind_vec-1];
                            }
                        }

                        // scaling of the signals
                        if (this_y != START_VECTOR)
                        {
                            this_y = scale_signal(this_y, scaling_expo_mult[k], scaling_offset[k]);
                        }

                        if (prev_y != START_VECTOR)
                        {
                            prev_y = scale_signal(prev_y, scaling_expo_mult[k], scaling_offset[k]);
                        }   

                        
                        // smaller signal
                        if (prev_y < this_y)
                        {
                            this_y0 = prev_y;
                            this_y1 = this_y;
                        }
                        else
                        {
                            this_y0 = this_y;
                            this_y1 = prev_y;
                        }


                        // safety
                        if (this_y0 == START_VECTOR)
                        {
                            this_y0 = this_y1;
                        }
                        else if (this_y1 == START_VECTOR)
                        {
                            this_y1 = this_y0;
                        }

                        // pixel no more white (correponding curve found)
                        if (((pixel_y_min <= prev_y)  && (prev_y <= pixel_y_max)) || 
                            ((pixel_y_min <= this_y)  && (this_y <= pixel_y_max)) || 
                            ((this_y0 <= pixel_y_min) && (pixel_y_max <= this_y1)))
                        {
                            // colors range
                            request_color = color_tab[k];

                            while(request_color >= NB_COLORS_TOT_SDL)
                            {
                                request_color -= NB_COLORS_SDL;
                            }
                            while(request_color < NB_COLORS_INIT_SDL)
                            {
                                request_color += NB_COLORS_SDL;
                            }

                            SDL_set_color(screen_sdl, request_color);
                            found_y_value = 1;
                            break;
                        }
                    }
                    
                    // no pixel to color (corresponding curve not found)
                    if (!found_y_value)
                    {
                        // dashed line for the nul axis
                        if (this_y_pixel == y_pixel_nul_axis)
                        {
                            if (i%(2*NUL_AXIS_DASH_WIDTH) > NUL_AXIS_DASH_WIDTH)
                            {
                                SDL_set_color(screen_sdl, BLACK_SDL);
                                found_y_value = 1;
                            }
                            
                        }
                    }
                
                    // pixel to color (corresponding curve found)
                    if (found_y_value)
                    {
                        draw_pixel(ren, i, this_y_pixel);
                    }
                }

                // store the current value for the next iteration
                for (j=0; j<nb_curves_sdl; j++)
                {
                    previous_y_vec[j] = y_save[j][max_tsim_index];
                }
            }

            // scaling flag
            if (!screen_flag)
            {
                screen_sdl->last_scaling_flag = screen_sdl->scaling_flag;
            }

            screen_sdl->scaling_flag = 0;
        }
    }


    // -- Bottom legend -- //

    if (screen_flag || (screen_sdl->bottom_flag == 1))
    {
        // flag
        screen_sdl->bottom_flag = 0;

        // no changing labels
        if (screen_flag)
        {
            // auto-scaling
            sprintf(str, "k: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_SIGNAL_SCALING_X_POS, screen_sdl->bottom_mid_y_indications, 2);

            sprintf(str, "i: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_PLOT_SCALING_X_POS, screen_sdl->bottom_mid_y_indications, 2);

            // graph navigation
            sprintf(str, "u: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_PLOT_MOVE_X_POS_1, screen_sdl->scaling_up_indications, 2);

            sprintf(str, "j: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_PLOT_MOVE_X_POS_1, screen_sdl->scaling_bottom_indications, 2);

            sprintf(str, "y: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_PLOT_MOVE_X_POS_2, screen_sdl->scaling_up_indications, 2);

            sprintf(str, "h: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_PLOT_MOVE_X_POS_2, screen_sdl->scaling_bottom_indications, 2);

            // plot limits
            sprintf(str, "x axis: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_XY_LABEL_X_POS, screen_sdl->x_axis_indication, 2);

            sprintf(str, "y axis: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_XY_LABEL_X_POS, screen_sdl->y_axis_indication, 2);

            // simulation speed and time
            sprintf(str, "speed: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_TIME_SPEED_X_POS, screen_sdl->x_axis_indication, 2);

            sprintf(str, "time: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_TIME_SPEED_X_POS, screen_sdl->y_axis_indication, 2);

            // break - quit
            sprintf(str, "p: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_QUIT_X_POS, screen_sdl->legend_speed_indication_1, 2);

            sprintf(str, "m: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_QUIT_X_POS, screen_sdl->legend_speed_indication_2, 2);

            sprintf(str, "F");
            print_text_sdl(str, font_break, text_color, ren, QUIT_X_POS, screen_sdl->y_axis_indication, 2);

            sprintf(str, "o: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_SPEED_X_POS, screen_sdl->legend_speed_indication_1, 2);

            sprintf(str, "l: ");
            print_text_sdl(str, font_small_label, text_color, ren, LEGEND_SPEED_X_POS, screen_sdl->legend_speed_indication_2, 2);

            sprintf(str, "a");
            print_text_sdl(str, font_speed, text_color, ren, SPEED_X_POS, screen_sdl->speed_indication_1, 2);

            sprintf(str, "b");
            print_text_sdl(str, font_speed, text_color, ren, SPEED_X_POS, screen_sdl->speed_indication_2, 2);

            // x axis scaling
            if (screen_sdl->hor_plot_scaling)
            {
                sprintf(str, "A");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_up_indications, 2);
                sprintf(str, "B");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_up_indications, 2);

                sprintf(str, "Q");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_bottom_arrow_indications_1, 2);
                sprintf(str, "R");
                print_text_sdl(str, font_arrows, text_color, ren, LEGEND_PLOT_MOVE_X_POS_1_BIS, screen_sdl->scaling_bottom_arrow_indications_1, 2);

                sprintf(str, "A");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_2, screen_sdl->scaling_up_indications_2, 2);

                sprintf(str, "B");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_2, screen_sdl->scaling_bottom_arrow_indications_1, 2);
            }
            // y axis scaling
            else
            {
                sprintf(str, "C");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_up_indications, 2);
                sprintf(str, "D");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_up_indications, 2);

                sprintf(str, "T");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_bottom_arrow_indications_2, 2);
                sprintf(str, "S");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_1, screen_sdl->scaling_bottom_arrow_indications_3, 2);

                sprintf(str, "C");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_2, screen_sdl->scaling_up_indications, 2);

                sprintf(str, "D");
                print_text_sdl(str, font_arrows, text_color, ren, PLOT_MOVE_X_POS_2, screen_sdl->scaling_bottom_arrow_indications_1, 2);
            }
            
            // break
            if (realtime->simu_break)
            {
                sprintf(str, "(n)");
                print_text_sdl(str, font_small_label, text_color, ren, LEGEND_SWITCH_HOR_VERT_X_POS, screen_sdl->bottom_mid_y_indications, 2);

                sprintf(str, ")");
                print_text_sdl(str, font_break, text_color, ren, QUIT_X_POS, screen_sdl->x_axis_indication, 2);

                #ifdef JAVA
                if (realtime->flag_visu)
                {
                    sprintf(str, "t:");
                    print_text_sdl(str, font_small_label, text_color, ren, VISU_PAST_X_POS_1, screen_sdl->legend_speed_indication_1, 2);

                    sprintf(str, "g:");
                    print_text_sdl(str, font_small_label, text_color, ren, VISU_PAST_X_POS_1, screen_sdl->legend_speed_indication_2, 2);

                    sprintf(str, "Q:");
                    print_text_sdl(str, font_speed, text_color, ren, VISU_PAST_X_POS_2, screen_sdl->speed_indication_1, 2);

                    sprintf(str, "Q:");
                    print_text_sdl(str, font_speed, text_color, ren, VISU_PAST_X_POS_3, screen_sdl->speed_indication_1, 2);

                    sprintf(str, "R:");
                    print_text_sdl(str, font_speed, text_color, ren, VISU_PAST_X_POS_2, screen_sdl->speed_indication_2, 2);

                    sprintf(str, "R:");
                    print_text_sdl(str, font_speed, text_color, ren, VISU_PAST_X_POS_3, screen_sdl->speed_indication_2, 2);
                }   
                #endif      
            }
            // simulation running
            else
            {
                sprintf(str, "I");
                print_text_sdl(str, font_play, text_color, ren, QUIT_X_POS, screen_sdl->x_axis_indication, 2);
            }
        }


        // signal au scaling: yes
        if ((screen_sdl->signal_auto_scaling) && ((!screen_sdl->last_signal_auto_scaling) || screen_flag) )
        {
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, SIGNAL_SCALING_X_POS, LEGEND_PLOT_SCALING_X_POS-1, screen_sdl->plot_y_end + 1, screen_sdl->screen_height-1);

            sprintf(str, "o");
            print_text_sdl(str, font_scaling, text_color, ren, SIGNAL_SCALING_X_POS, screen_sdl->scaling_indications, 2);
        }

        // signal auto-scaling: no
        else if ((!screen_sdl->signal_auto_scaling) && ((screen_sdl->last_signal_auto_scaling) || screen_flag) )
        {
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, SIGNAL_SCALING_X_POS, LEGEND_PLOT_SCALING_X_POS-1, screen_sdl->plot_y_end + 1, screen_sdl->screen_height-1);

            sprintf(str, "p");
            print_text_sdl(str, font_scaling, text_color, ren, SIGNAL_SCALING_X_POS, screen_sdl->scaling_indications, 2);
        }

        // y axis auto-sclaing: yes
        if ((screen_sdl->plot_auto_scaling) && ((!screen_sdl->last_plot_auto_scaling) || screen_flag) )
        {
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, PLOT_SCALING_X_POS, LEGEND_PLOT_MOVE_X_POS_1-1, screen_sdl->plot_y_end + 1, screen_sdl->screen_height-1);

            sprintf(str, "x"); 
            print_text_sdl(str, font_scaling, text_color, ren, PLOT_SCALING_X_POS, screen_sdl->scaling_indications, 2);
        }

        // y axis auto-scaling: no
        else if ((!screen_sdl->plot_auto_scaling) && ((screen_sdl->last_plot_auto_scaling) || screen_flag) )
        {
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, PLOT_SCALING_X_POS, LEGEND_PLOT_MOVE_X_POS_1-1, screen_sdl->plot_y_end + 1, screen_sdl->screen_height-1);

            sprintf(str, ",");
            print_text_sdl(str, font_scaling, text_color, ren, PLOT_SCALING_X_POS, screen_sdl->scaling_indications, 2);
        }

        // store current values of auto-scaling for the next iteration
        screen_sdl->last_signal_auto_scaling = screen_sdl->signal_auto_scaling;
        screen_sdl->last_plot_auto_scaling   = screen_sdl->plot_auto_scaling;


        // x label range

        // corresponding index
        ind_vec = index_vec+1;
        if (ind_vec >= screen_sdl->screen_plot_width)
        {
            ind_vec -= screen_sdl->screen_plot_width;
        }

        // minimal value
        legend_x_min = t_vec[ind_vec];

        if (!ind_vec && ((2*legend_x_min - t_vec[1]) < screen_sdl->min_tsim))
        {
            legend_x_min = screen_sdl->min_tsim;
        }

        // maximal value
        tsim_x_label_minus_1 = t_vec[index_vec];

        ind_vec = index_vec - 1;

        if (ind_vec < 0)
        {               
            ind_vec += screen_sdl->plot_x_end;
        }

        tsim_x_label_minus_2 = t_vec[ind_vec];

        legend_x_max = tsim_x_label_minus_1 + (tsim_x_label_minus_1 - tsim_x_label_minus_2);

        // change the x axis bounds indication
        if (screen_flag || (screen_sdl->last_legend_x_min != legend_x_min) || (screen_sdl->last_legend_x_max != legend_x_max))
        {
            // erase previous value
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, XY_LABEL_X_POS, LEGEND_TIME_SPEED_X_POS-1, screen_sdl->plot_y_end + 1, screen_sdl->bottom_mid_y_indications);

            screen_sdl->expo_x = str_label(str, legend_x_min, legend_x_max);

            print_text_sdl(str, font_label, text_color, ren, XY_LABEL_X_POS, screen_sdl->x_axis_indication, 2);

            // store current values of auto-scaling for the next iteration
            screen_sdl->last_legend_x_min = legend_x_min;
            screen_sdl->last_legend_x_max = legend_x_max;
        }

        // black vertical line for past visu
        #ifdef JAVA
        if (screen_flag == 1 && realtime->flag_visu)
        {
            java = realtime->ext->java;

            if (legend_x_min < java->t_visu_past && java->t_visu_past < legend_x_max)
            {
                x_pixel_past = round(((java->t_visu_past - legend_x_min) / (legend_x_max - legend_x_min)) * (screen_sdl->plot_x_end-1));

                if (1 < x_pixel_past && x_pixel_past < screen_sdl->plot_x_end-2) // safety: no vertical line next to the border
                {
                    SDL_set_color(screen_sdl, BLACK_SDL);

                    draw_vertical_line(ren, x_pixel_past, PLOT_Y_START, screen_sdl->plot_y_end-1);
                }
            }
        }
        #endif

        // y label range

        // change the y axis bounds indication
        if (screen_flag || (screen_sdl->last_legend_y_min != screen_sdl->y_min) || (screen_sdl->last_legend_y_max != screen_sdl->y_max))
        {
            // erase previous value
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, XY_LABEL_X_POS, LEGEND_TIME_SPEED_X_POS-1, screen_sdl->bottom_mid_y_indications + 1, screen_sdl->screen_height - 1);

            screen_sdl->expo_y = str_label(str, screen_sdl->y_min, screen_sdl->y_max);
    
            print_text_sdl(str, font_label, text_color, ren, XY_LABEL_X_POS, screen_sdl->y_axis_indication, 2);

            // store current values of auto-scaling for the next iteration
            screen_sdl->last_y_min = screen_sdl->y_min;
            screen_sdl->last_y_max = screen_sdl->y_max;

            screen_sdl->last_legend_y_min = screen_sdl->y_min;
            screen_sdl->last_legend_y_max = screen_sdl->y_max;
        }



        // simulation speed

        // change the simulation speed indication
        if (screen_flag || (realtime->last_real_simu_speed_factor != realtime->real_simu_speed_factor))
        {
            // erase previous value
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, TIME_SPEED_X_POS, VISU_PAST_X_POS_1-1, screen_sdl->plot_y_end + 1, screen_sdl->bottom_mid_y_indications);

            if (screen_flag)
            {
                speed_factor_plot = get_simu_speed_factor(realtime->simu_speed_flag);
            }
            else
            {
                speed_factor_plot = realtime->real_simu_speed_factor;
            }           

            if (speed_factor_plot < 0.1)
            {
                sprintf(str, "%.3fx", speed_factor_plot);
            }
            else
            {
                sprintf(str, "%.2fx", speed_factor_plot);
            }

            print_text_sdl(str, font_label, text_color, ren, TIME_SPEED_X_POS, screen_sdl->x_axis_indication, 2);

            // store current values of auto-scaling for the next iteration
            realtime->last_real_simu_speed_factor = speed_factor_plot;
        }

        // simulation time

        // change the simulation time indication
        if (screen_flag || (tsim != realtime->last_tsim))
        {
            // erase previous value
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
            draw_rectangle(ren, TIME_SPEED_X_POS, VISU_PAST_X_POS_1-1, screen_sdl->bottom_mid_y_indications + 1, screen_sdl->screen_height - 1);

            tsim_init = realtime->t0;
            tsim_end  = realtime->tf;

            sprintf(str, "%.0f%% (%.1f/%.0f)", ((tsim - tsim_init) / (tsim_end - tsim_init)) * 100.0, tsim, tsim_end);
            print_text_sdl(str, font_label, text_color, ren, TIME_SPEED_X_POS, screen_sdl->y_axis_indication, 2);

            // store current values of auto-scaling for the next iteration
            realtime->last_tsim = tsim;
        }
    }


    // -- Mouse wheel -- //

    if (screen_sdl->change_mouse_wheel_flag || screen_flag)
    {
        screen_sdl->change_mouse_wheel_flag = 0;

        // erase previous data
        SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);
        draw_rectangle(ren, SWITCH_HOR_VERT_X_POS, SWITCH_HOR_VERT_X_POS+20, screen_sdl->scaling_indications-15, screen_sdl->scaling_indications+10);

        switch(realtime_sdl->mouse_wheel_flag)
        {
            case 0:

                sprintf(str, "r");
                print_text_sdl(str, font_mouse, text_color, ren, SWITCH_HOR_VERT_X_POS, screen_sdl->scaling_indications, 2);
                break;

            case 1:
                sprintf(str, "7");
                print_text_sdl(str, font_scaling, text_color, ren, SWITCH_HOR_VERT_X_POS, screen_sdl->scaling_indications, 2);
                break;

            case 2:
                sprintf(str, "C");
                print_text_sdl(str, font_hor_mouse, text_color, ren, SWITCH_HOR_VERT_X_POS, screen_sdl->scaling_indications, 2);                
                break;
        
            default:
                sprintf(str, "&");
                break;
        }
    }

    
    // -- X label -- //

    x_pixel_label = screen_sdl->x_label_width; // initial value for x
    SDL_set_color(screen_sdl, BLACK_SDL);

    // break screen
    if (screen_flag == 1)
    {   
        // loop on the x values
        while (x_pixel_label <= screen_sdl->plot_x_end)
        {
            // vertical black line
            draw_vertical_line(ren, x_pixel_label - 1, screen_sdl->plot_y_end - X_LABEL_HEIGHT - 2, screen_sdl->plot_y_end - 1);

            // first values
            if (x_pixel_label < screen_sdl->plot_x_end)
            {
                ind_vec = index_vec + x_pixel_label + 1;
                if (ind_vec >= screen_sdl->plot_x_end)
                {               
                    ind_vec -= screen_sdl->plot_x_end;
                }

                tsim_x_label = t_vec[ind_vec];
            }
            // last value
            else
            {
                ind_vec = index_vec + x_pixel_label - 1;
                if (ind_vec >= screen_sdl->plot_x_end)
                {               
                    ind_vec -= screen_sdl->plot_x_end;
                }

                tsim_x_label_minus_2 = t_vec[ind_vec];

                ind_vec = index_vec + x_pixel_label;
                if (ind_vec >= screen_sdl->plot_x_end)
                {               
                    ind_vec -= screen_sdl->plot_x_end;
                }

                tsim_x_label_minus_1 = t_vec[ind_vec];

                tsim_x_label = tsim_x_label_minus_1 + (tsim_x_label_minus_1 - tsim_x_label_minus_2);
            }

            // string
            str_axis(str, tsim_x_label, screen_sdl->expo_x);

            // print
            print_text_sdl(str, font_axis, text_color, ren, x_pixel_label - X_LABEL_SHIFT_X - 1, screen_sdl->plot_y_end - X_LABEL_SHIFT_Y, 1);
            
            // next value
            x_pixel_label += screen_sdl->x_label_width;
        }

        // restore counter and flag
        screen_sdl->x_label_print_count = -1;
        screen_sdl->last_x_label_flag   = 1;
        
    }
    // running simulation (time to plot a new x label)
    else if((!screen_flag) && (screen_sdl->x_label_print_count >= screen_sdl->x_label_width-1))
    {
        screen_sdl->x_label_print_count = 0;

        // vertical black line
        draw_vertical_line(ren, index_sdl, screen_sdl->plot_y_end - X_LABEL_HEIGHT - 2, screen_sdl->plot_y_end - 1);

        tsim_x_label = tsim;

        sprintf(str, "%.2f", tsim_x_label);

        print_text_sdl(str, font_axis, text_color, ren, index_sdl - X_LABEL_SHIFT_X, screen_sdl->plot_y_end - X_LABEL_SHIFT_Y, 1);
    }
    // running simulation (no time to plot a new x label)
    else if(!screen_flag)
    {
        // increase the counter
        screen_sdl->x_label_print_count++;

        if (screen_sdl->last_x_label_flag)
        {
            screen_sdl->last_x_label_flag = 0;

            draw_vertical_line(ren, screen_sdl->plot_x_end-1, screen_sdl->plot_y_end - X_LABEL_HEIGHT - 2, screen_sdl->plot_y_end - 1);
        }
    }
    

    // -- Y label -- //

    SDL_set_color(screen_sdl, BLACK_SDL);

    y_pixel_label = PLOT_Y_START;
    y_value       = y_max_sdl;
    prev_y_value  = screen_sdl->last_y_max;

    // store current values of auto-scaling for the next iteration
    screen_sdl->last_y_min = screen_sdl->y_min;
    screen_sdl->last_y_max = screen_sdl->y_max;

    screen_sdl->expo_y = str_label(str, screen_sdl->y_min, screen_sdl->y_max);

    if (screen_flag != 2)
    {
        // break screen
        if (screen_flag == 1)
        {
            x_end_label_plot = screen_sdl->plot_x_end;
        }
        // running simulation
        else
        {
            x_end_label_plot = next_index_sdl + 1;
        }

        // loop on the y values
        while (y_pixel_label <= screen_sdl->plot_y_end)
        {
            str_axis(str, y_value, screen_sdl->expo_y);

            // first value
            if (y_pixel_label == PLOT_Y_START)
            {
                print_text_sdl(str, font_axis, text_color, ren, x_end_label_plot + Y_LABEL_SHIFT_X, y_pixel_label + Y_LABEL_SHIFT_Y_START, 2);      
            }
            // last value
            else if (y_pixel_label == screen_sdl->plot_y_end)
            {
                print_text_sdl(str, font_axis, text_color, ren, x_end_label_plot + Y_LABEL_SHIFT_X, y_pixel_label + Y_LABEL_SHIFT_Y_END, 2);
            }
            // other values
            else
            {
                draw_horizontal_line(ren, x_end_label_plot, x_end_label_plot + Y_LABEL_WIDTH - 1, y_pixel_label);

                print_text_sdl(str, font_axis, text_color, ren, x_end_label_plot + Y_LABEL_WIDTH + Y_LABEL_SHIFT_X, y_pixel_label + Y_LABEL_SHIFT_Y, 2);
            }       

            // iterate
            y_value      -= y_diff_label;
            prev_y_value -= prev_y_diff_label;
            
            y_pixel_label += screen_sdl->y_label_height;
        }
    }


    // -- Up legend -- /
    
    scaling_expo_mult = screen_sdl->scaling_expo_mult;
    scaling_offset    = screen_sdl->scaling_offset;

    nb_legends = screen_sdl->nb_legends;

    // break of first plot: write curves labels
    if (screen_flag)
    {
        for (i=0; i<nb_legends; i++)
        {
            pixel_pos_label = screen_sdl->pixel_pos_label[i];

            // colors range
            request_color = screen_sdl->color_tab[i];

            while(request_color >= NB_COLORS_TOT_SDL)
            {
                request_color -= NB_COLORS_SDL;
            }
            while(request_color < NB_COLORS_INIT_SDL)
            {
                request_color += NB_COLORS_SDL;
            }

            SDL_set_color(screen_sdl, request_color);

            draw_rectangle(ren, pixel_pos_label- UP_COLOR_SPACE_LABEL - UP_COLOR_WIDTH_LABEL, pixel_pos_label- UP_COLOR_SPACE_LABEL - 1,
                                UP_FIRST_Y_INDICATIONS - 1, UP_FIRST_Y_INDICATIONS + 1);
            
            sprintf(str, "%s", screen_sdl->auto_plot->label_curves[i]);
            print_text_sdl(str, font_label, text_color, ren, pixel_pos_label, UP_FIRST_Y_INDICATIONS, 2);

            str_scaling(str, scaling_expo_mult[i], scaling_offset[i]);
            print_text_sdl(str, font_small_label, text_color, ren, pixel_pos_label, UP_SECOND_Y_INDICATIONS, 2);
        }
    }

    // write scaling indications
    last_scaling_expo_mult = screen_sdl->last_scaling_expo_mult;
    last_scaling_offset    = screen_sdl->last_scaling_offset;

    for (i=0; i<nb_legends; i++)
    {
        pixel_pos_label = screen_sdl->pixel_pos_label[i];

        if ( (last_scaling_offset[i] != scaling_offset[i]) || (last_scaling_expo_mult[i] != scaling_expo_mult[i]))
        {
            // erase previous data
            SDL_set_color(screen_sdl, LIGHT_GRAY_SDL);

            if (i == nb_legends - 1)
            {
                draw_rectangle(ren, pixel_pos_label, screen_sdl->screen_width - 1, UP_SEPARATION_INDICATION, PLOT_Y_START-2);
            }
            else
            {
                draw_rectangle(ren, pixel_pos_label, screen_sdl->pixel_pos_label[i+1] - 1, UP_SEPARATION_INDICATION, PLOT_Y_START-2);
            }

            // new data
            str_scaling(str, scaling_expo_mult[i], scaling_offset[i]);
            print_text_sdl(str, font_small_label, text_color, ren, pixel_pos_label, UP_SECOND_Y_INDICATIONS, 2);
        }

        // store current values of auto-scaling for the next iteration
        last_scaling_expo_mult[i] = scaling_expo_mult[i];
        last_scaling_offset[i]    = scaling_offset[i];
    }

    // iterate SDL index
    if (!screen_flag)
    {
        index_sdl++;

        if (index_sdl >= screen_sdl->screen_width)
        {
            index_sdl -= screen_sdl->screen_width;
        }

        screen_sdl->index_sdl = index_sdl;
    }
    // reset SDL index
    else if (screen_flag == 1)
    {
        screen_sdl->index_sdl = screen_sdl->plot_x_end-1;
    }

    // -- Apply the plot -- //
    SDL_RenderPresent(ren);
}

/*! \brief wait time before next user key
 * 
 * \param[in,out] realtime real-time structure
 * \param[in] cur_t_usec current real time [us]
 * \param[in] tsim current simulation time [s]
 */
void wait_key(Simu_realtime *realtime, int cur_t_usec, double tsim)
{
    Realtime_sdl *realtime_sdl;

    realtime_sdl = realtime->ext->sdl;

    realtime_sdl->next_user_keyboard_event_usec = cur_t_usec + ((int) (tsim * 1e6));
    realtime_sdl->last_action_break_usec = cur_t_usec;
}

/*! \brief wait time before next generic key
 * 
 * \param[in,out] realtime real-time structure
 * \param[in] cur_t_usec current real time [us]
 * \param[in] tsim current simulation time [s]
 */
void wait_key_generic(Simu_realtime *realtime, int cur_t_usec, double tsim)
{
    Realtime_sdl *realtime_sdl;

    realtime_sdl = realtime->ext->sdl;

    realtime_sdl->next_generic_keyboard_event_usec = cur_t_usec + ((int) (tsim * 1e6));
    realtime_sdl->last_action_break_usec = cur_t_usec;
}

/*! \brief real-time and plot SDL gestion during simulation break
 * 
 * \param[in,out] realtime real-time structure
 * \param[in] tsim current simulation time [s]
 */
void break_gestion(Simu_realtime *realtime, double tsim)
{
    // variables declaration
    int start_break_t_usec;
    int delta_break_u_sec;

    int cur_t_usec;

    #ifdef JAVA
    Realtime_java *java;
    #endif

    int init_t_sec, init_t_usec;

    Screen_sdl *screen_sdl;
    Realtime_sdl *realtime_sdl;

    realtime_sdl = realtime->ext->sdl;
    screen_sdl   = realtime_sdl->screen_sdl;

    init_t_sec  = realtime->init_t_sec;
    init_t_usec = realtime->init_t_usec;

    #ifdef JAVA
    if (realtime->flag_visu)
    {
        java = realtime->ext->java;
    }
    #endif

    // start of the break instant
    start_break_t_usec = t_usec(init_t_sec, init_t_usec);

    // first break plot
    update_x_min_max(screen_sdl, realtime);
    update_scale_signals(screen_sdl, realtime, 1);
    plot_screen_sdl(realtime, tsim, 1);

    while (realtime->simu_break == 1)
    {
        // handle events
        cur_t_usec = t_usec(init_t_sec, init_t_usec);

        events_sdl(realtime, cur_t_usec);

        // break plot
        if (screen_sdl->break_plot_flag)
        {
            screen_sdl->break_plot_flag = 0;

            update_scale_signals(screen_sdl, realtime, 1);
            plot_screen_sdl(realtime, tsim, 1);
        }

        #ifdef JAVA
        if (realtime->flag_visu)
        {
            if (java->change_viewpoint || java->visu_past_flag)
            {
                update_java(realtime);
            }
        }
        #endif

        // program is sleeping during 25 ms, decrease CPU usage during break if user is not interacting
        if (t_usec(init_t_sec, init_t_usec) - realtime_sdl->last_action_break_usec > TIME_NO_INTERACTION_BREAK)
        {
            SDL_Delay(TIME_SDL_DELAY);
        }
    }

    realtime->first_break = 0;

    // update variables after the break
    delta_break_u_sec = t_usec(init_t_sec, init_t_usec) - start_break_t_usec;
    realtime->speed_last_t_usec += delta_break_u_sec;
    update_realtime_constraints_break(realtime, delta_break_u_sec);

    screen_sdl->hor_plot_scaling = 0;

    // last_break_plot
    update_x_min_max(screen_sdl, realtime);
    plot_screen_sdl(realtime, tsim, 1);
}

#endif
