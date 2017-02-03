#if (defined OPEN_GL) && (!defined SEPARATE_OPEN_GL) && (!defined PROCESS_OPEN_GL)

#include "open_gl_c_int.h"
#include "MbsWorld3D.hh"
#include "user_realtime.h"
#include "cmake_open_gl_config.h"

#include <iostream>
#include <string>
#include <sstream>

#if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
    #include <unistd.h>
    #include <pthread.h>
    #include <errno.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    #ifdef APPLE_VISU
        #include <CoreFoundation/CoreFoundation.h>
    #endif
#endif

#ifdef OPEN_GL_PROCESS_WIN
    #include <windows.h> 
    #include <tchar.h>
    #include <stdio.h> 
    #include <strsafe.h>
    #include <iomanip>
#endif

#define START_VIEWPOINT 0 ///< default initial viewpoint

// states of the window
enum {OPEN_GL_RUN, OPEN_GL_BREAK, OPEN_GL_STOP};

bool child_living = false; ///< true if child process living, false otherwise

#if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

    /*! \brief input of the 3D window
     */
    typedef struct OpenGlIn
    {
        int nb_models; ///< number of models

        int start_viewpoint; ///< initial viewpoint

        int *nb_q; ///< number of joints for each model

        double **q_vec; ///< initial joint poisitions

        char **mbs_file; ///< mbs files to load
        
        Realtime_visu *visu; ///< pointer to the "visu" structure

    } OpenGlIn;

    int pid_father; ///< father exact PID (converted to integer)

    pid_t pid_fork = -1; ///< PID of the fork

#endif

#if (defined OPEN_GL_THREAD_UNIX)

    int pipe_state[2];        ///< descriptor pipe: flag to get the state
    int pipe_continue[2];     ///< descriptor pipe: flag to tell if child continue or not
    int pipe_q_vec[2];        ///< descriptor pipe: position of the joints
    int pipe_viewpoint[2];    ///< descriptor pipe: viewpoint flag
    int pipe_nb_viewpoint[2]; ///< descriptor pipe: number of viewpoints

#elif (defined OPEN_GL_PROCESS_UNIX)

    int in_pipe_state;         ///<  input of the named pipe 'name_pipe_state'
    int in_pipe_q_vec;         ///<  input of the named pipe 'name_pipe_q_vec'
    int in_pipe_viewpoint;     ///<  input of the named pipe 'name_pipe_viewpoint'
    int out_pipe_continue;     ///< output of the named pipe 'name_pipe_continue'
    int out_pipe_nb_viewpoint; ///< output of the named pipe 'name_pipe_nb_viewpoint'

    char *name_pipe_state;        ///< name of the named pipe 'name_pipe_state'
    char *name_pipe_q_vec;        ///< name of the named pipe 'name_pipe_q_vec'
    char *name_pipe_viewpoint;    ///< name of the named pipe 'name_pipe_viewpoint'
    char *name_pipe_continue;     ///< name of the named pipe 'name_pipe_continue'
    char *name_pipe_nb_viewpoint; ///< name of the named pipe 'name_pipe_nb_viewpoint'

#elif (defined OPEN_GL_PROCESS_WIN)

    #define BUFSIZE 4096 ///< size of the buffer to send messages

    char pipe_msg[BUFSIZE]; ///< buffer for pipe messages

    // handles
    HANDLE hdl_in_rd  = NULL;
    HANDLE hdl_in_wr  = NULL;
    HANDLE hdl_out_rd = NULL;
    HANDLE hdl_out_wr = NULL;
#endif

// function prototypes
void joints_view_open_gl(Realtime_visu *visu, int nb_models, int *nb_q, double **q_vec);
bool check_file_exist(const std::string& name);

std::string int_to_string(int int_val);
std::string double_to_string(double double_val);

#if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
    pid_t unix_create_process();
    bool unix_check_child_living();
#endif

#if (defined OPEN_GL_THREAD_UNIX)

    void opengl_thread_unix_init_pipes();
    void opengl_thread_unix_start_fork(MbsData *mbs_data, Realtime_visu *visu, OpenGlIn *open_gl_in);
    void opengl_thread_unix_finish_son(int nb_models, int *nb_q, double **q_vec, OpenGLMbs::MbsWorld3D *world_3d);
    void opengl_thread_unix_run_visu(OpenGlIn* open_gl_in);

#elif (defined OPEN_GL_PROCESS_UNIX)

    void opengl_proc_unix_init_pipes();
    void opengl_proc_unix_start_fork(MbsData *mbs_data, Realtime_visu *visu, OpenGlIn *open_gl_in);
    void opengl_proc_unix_close_pipes();

#elif (defined OPEN_GL_PROCESS_WIN)

    int opengl_win_pipe_to_int();
    bool opengl_win_continue_pipe_answer();

    void opengl_win_send_ok_pipe();
    void opengl_win_send_pipe_msg(std::string msg);
    void opengl_win_print_pipe_string(std::string pipe_str);
    void opengl_win_create_pipes();
    void opengl_win_create_child(MbsData *mbs_data);

    std::string opengl_win_create_pipe_string(int flag_state, int flag_viewpoint, int nb_models, int *nb_q, double **q_vec);
#endif

/*! \brief initialize the OpenGL class
 *
 * \param[in] mbs_data Robotran main structure
 * \param[out] visu realtime 3D structure
 * \param[in] nb_models number of models
 * \param[in] nb_q number of joints in the .mbs file
 * \param[in] q_vec vector of size nb_q with the initial values for q
 * \param[in] mbs_file path and name of the .mbs file used for visualization
 * \param[in] start_viewpoint initial viewpoint
 */
void init_open_gl(Realtime_visu *visu, MbsData *mbs_data, int nb_models, int *nb_q, double **q_vec, char **mbs_file, int start_viewpoint)
{
    // variables declaration
    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
        OpenGlIn *open_gl_in;
    #elif (!defined OPEN_GL_PROCESS_WIN)
        OpenGLMbs::MbsWorld3D *world_3d;
    #endif

    // safety
    if (nb_models < 1)
    {
        std::cout << "open_gl_c_int error: nb_models for OpenGL anim should be positive (not " << nb_models << ") !" << std::endl;
        exit(EXIT_FAILURE);
    }

    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

        // exact PID of the father
        pid_father = (int) getpid();

        // inputs as argument structure
        open_gl_in = new OpenGlIn;

        open_gl_in->nb_models       = nb_models;
        open_gl_in->start_viewpoint = start_viewpoint;
        open_gl_in->nb_q            = nb_q;
        open_gl_in->q_vec           = q_vec;
        open_gl_in->mbs_file        = mbs_file;
        open_gl_in->visu            = visu;

        // initialization of the pipes and the fork
        #if (defined OPEN_GL_THREAD_UNIX)
            opengl_thread_unix_init_pipes();
            opengl_thread_unix_start_fork(mbs_data, visu, open_gl_in);
        #elif (defined OPEN_GL_PROCESS_UNIX)
            opengl_proc_unix_init_pipes();
            opengl_proc_unix_start_fork(mbs_data, visu, open_gl_in);
        #endif

    #elif (defined OPEN_GL_PROCESS_WIN)

        // initialization of the pipes and launch of the child process
        opengl_win_create_pipes();
        opengl_win_create_child(mbs_data);

        // send number of models to child
        opengl_win_send_pipe_msg(std::to_string(nb_models));
        opengl_win_continue_pipe_answer();

        // send .mbs files to child
        for(int i=0; i<nb_models; i++)
        {
            opengl_win_send_pipe_msg(mbs_file[i]);
            opengl_win_continue_pipe_answer();
        }

        // send number of joins to child
        for(int i=0; i<nb_models; i++)
        {
            opengl_win_send_pipe_msg(std::to_string(nb_q[i]));
            opengl_win_continue_pipe_answer();
        }

        // send initial position and viewpoint to child
        opengl_win_send_pipe_msg(opengl_win_create_pipe_string(OPEN_GL_RUN, start_viewpoint, nb_models, nb_q, q_vec));
        opengl_win_continue_pipe_answer();

        // get number of viewpoints from child
        opengl_win_continue_pipe_answer();
        visu->nb_viewpoint = opengl_win_pipe_to_int();
        opengl_win_send_ok_pipe();

    #else
        // create world
        world_3d = new OpenGLMbs::MbsWorld3D(1024, 768);

        // load each model
        for(int i=0; i<nb_models; i++)
        {
            world_3d->AddMbs(mbs_file[i]);
        }

        // set the initial values of each joint
        for(int i=0; i<nb_models; i++)
        {
            world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);
        }

        // set the initial viewpoint
        world_3d->SetViewPoint(start_viewpoint);

        // set viewpoint
        visu->nb_viewpoint = world_3d->GetNbViews();

        // save world adress
        visu->visu_class = world_3d;
    #endif
}

/*! \brief Release memory
 *
 * \param[out] visu_class 3D world
 */
void free_open_gl(void *visu_class)
{
    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

        size_t res;
        int flag_state, flag_continue;

        if(!pid_fork) // son, in charge of the visualization
        {
            return; // safety, should never happen
        }

        // child is already dead
        if (!unix_check_child_living())
        {
            // release pipe names
            #ifdef OPEN_GL_PROCESS_UNIX
                opengl_proc_unix_close_pipes();
            #endif

            return;
        }

        // send order to stop
        flag_state = OPEN_GL_STOP;

        #if (defined OPEN_GL_THREAD_UNIX)
            res = write(pipe_state[1], &flag_state, sizeof(int));
        #elif (defined OPEN_GL_PROCESS_UNIX)
            res = write(in_pipe_state, &flag_state, sizeof(int));
        #endif

        // wait for order to be executed
        #if (defined OPEN_GL_THREAD_UNIX)
            res = read(pipe_continue[0], &flag_continue, sizeof(int));
        #elif (defined OPEN_GL_PROCESS_UNIX)
            res = read(out_pipe_continue, &flag_continue, sizeof(int));
        #endif

        // wait for child to be finished
        if (wait(NULL) == -1)
        {
            perror("wait :");
            exit(EXIT_FAILURE);
        }

        // release pipe names
        #ifdef OPEN_GL_PROCESS_UNIX
            opengl_proc_unix_close_pipes();
        #endif

    #elif (defined OPEN_GL_PROCESS_WIN)

        // child is already dead
        if (!child_living)
        {
            return;
        }

        // send message to stop the process
        opengl_win_send_pipe_msg(opengl_win_create_pipe_string(OPEN_GL_STOP, 0, 0, NULL, NULL));
        opengl_win_continue_pipe_answer();
    #else

        OpenGLMbs::MbsWorld3D *world_3d;

        world_3d = static_cast<OpenGLMbs::MbsWorld3D*>(visu_class);

        // release 'world_3d' memory
        if (world_3d != NULL)
        {
            delete world_3d;
        }
    #endif
}

/*! \brief send commands to make a break while updating the camera
 */
void break_open_gl(Simu_realtime *realtime)
{
    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

        // variables declaration
        size_t res;
        int flag_state, flag_continue;

        if(!pid_fork) // son, in charge of the visualization
        {
            return; // safety, should never happen
        }

        // child is already dead
        if (!unix_check_child_living())
        {
            return;
        }

        // send command to the son to make a break
        flag_state = OPEN_GL_BREAK;

        #if (defined OPEN_GL_THREAD_UNIX)
            res = write(pipe_state[1], &flag_state, sizeof(int));
        #elif (defined OPEN_GL_PROCESS_UNIX)
            res = write(in_pipe_state, &flag_state, sizeof(int));
        #endif

        // wait for answer
        #if (defined OPEN_GL_THREAD_UNIX)
            res = read(pipe_continue[0], &flag_continue, sizeof(int));
        #elif (defined OPEN_GL_PROCESS_UNIX)
            res = read(out_pipe_continue, &flag_continue, sizeof(int));
        #endif

        // child stopped
        if (!flag_continue)
        {
            child_living = false;
        }

    #elif (defined OPEN_GL_PROCESS_WIN)

        // child is already dead
        if (!child_living)
        {
            return;
        }

        // send command for a break
        opengl_win_send_pipe_msg(opengl_win_create_pipe_string(OPEN_GL_BREAK, 0, 0, NULL, NULL));

        // check if child wants to continue
        if (!opengl_win_continue_pipe_answer())
        {
            child_living = false;
        }

    #else
        OpenGLMbs::MbsWorld3D *world_3d;

        world_3d = static_cast<OpenGLMbs::MbsWorld3D*>(realtime->ext->visu->visu_class);

        // update the window
        if (world_3d != NULL)
        {
            world_3d->Update();
        }
    #endif
}

/*! \brief update the OpenGL visualization
 *
 * \param[in,out] realtime real-time structure
 */
void update_open_gl(Simu_realtime *realtime)
{
    // variables declaration
    Realtime_visu *visu;
    MbsData* mbs_data;

    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
        size_t res;
        int flag_state;
    #endif    

    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

        if(!pid_fork) // son, in charge of the visualization
        {
            return; // safety, should never happen
        }

        // child is already dead
        if (!unix_check_child_living())
        {
            return;
        }        
    #endif

    // get fields
    visu     = realtime->ext->visu;
    mbs_data = realtime->ext->mbs_data;

    // send order to run
    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
        flag_state = OPEN_GL_RUN;
    #endif

    #if (defined OPEN_GL_THREAD_UNIX)
        res = write(pipe_state[1], &flag_state, sizeof(int));
    #elif (defined OPEN_GL_PROCESS_UNIX)
        res = write(in_pipe_state, &flag_state, sizeof(int));
    #endif

    // update joint positions
    user_realtime_visu(mbs_data, visu->nb_models, visu->nb_q, visu->cur_q);

    // joint values to display
    if (visu->visu_past_flag)
    {
        visu->visu_past_flag   = 0;
        visu->last_past_q_flag = 1;

        joints_view_open_gl(visu, visu->nb_models, visu->nb_q, visu->past_q);
    }
    else if (visu->change_viewpoint)
    {
        if (visu->last_past_q_flag)
        {
            joints_view_open_gl(visu, visu->nb_models, visu->nb_q, visu->past_q);
        }
        else
        {
            joints_view_open_gl(visu, visu->nb_models, visu->nb_q, visu->cur_q);
        }
    }
    else
    {
        visu->last_past_q_flag = 0;

        joints_view_open_gl(visu, visu->nb_models, visu->nb_q, visu->cur_q);
    }
}

/*! \brief send commands to update the 3D view with OpenGL
 *
 * \param[in] visu 3D real-time structure
 * \param[in] nb_models number of models
 * \param[in] nb_q number of joints to update for the visualization
 * \param[in] q_vec vector of size nb_q with the current values to plot
 */
void joints_view_open_gl(Realtime_visu *visu, int nb_models, int *nb_q, double **q_vec)
{
    // variables declaration
    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
        size_t res;
        int flag_continue;
    #elif (!defined OPEN_GL_PROCESS_WIN)
        OpenGLMbs::MbsWorld3D *world_3d;
    #endif

    int flag_viewpoint;

    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

        // send joints position to visualization
        for(int i=0; i<nb_models; i++)
        {
            #if (defined OPEN_GL_THREAD_UNIX)
                res = write(pipe_q_vec[1], q_vec[i], nb_q[i]*sizeof(double));
            #elif (defined OPEN_GL_PROCESS_UNIX)
                res = write(in_pipe_q_vec, q_vec[i], nb_q[i]*sizeof(double));
            #endif
        }

    #elif (!defined OPEN_GL_PROCESS_WIN)

        world_3d = static_cast<OpenGLMbs::MbsWorld3D*>(visu->visu_class);

        // 3D window closed
        if (world_3d == NULL)
        {
            return;
        }

        // check 3D window to close it
        if (!world_3d->CheckClose())
        {
            delete world_3d;
            visu->visu_class = NULL;
            return;
        }

        // update world 3D timing
        world_3d->UpdateTime();

        // update joint positions
        for(int i=0; i<nb_models; i++)
        {
            // update the joints
            world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);
        }
    #endif

    // adapt the viewpoint
    if (visu->change_viewpoint)
    {
        visu->change_viewpoint = 0;

        visu->cur_viewpoint++;

        if (visu->cur_viewpoint >= visu->nb_viewpoint)
        {
            visu->cur_viewpoint = START_VIEWPOINT;
        }

        flag_viewpoint = visu->cur_viewpoint;
    }
    else
    {
        flag_viewpoint = -1;
    }

    #if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)
        
        #if (defined OPEN_GL_THREAD_UNIX)
            // send viewpoint through pipe
            res = write(pipe_viewpoint[1], &flag_viewpoint, sizeof(int));

            // wait for answer
            res = read(pipe_continue[0], &flag_continue, sizeof(int));

        #elif (defined OPEN_GL_PROCESS_UNIX)
            // send viewpoint through named pipe
            res = write(in_pipe_viewpoint, &flag_viewpoint, sizeof(int));

            // wait for answer
            res = read(out_pipe_continue, &flag_continue, sizeof(int));
        #endif

        // child stopped
        if (!flag_continue)
        {
            child_living = false;
        }

    #elif (defined OPEN_GL_PROCESS_WIN)

        // child is already dead
        if (!child_living)
        {
            return;
        }

        // send command to change the viewpoint, the joints and to do normal run
        opengl_win_send_pipe_msg(opengl_win_create_pipe_string(OPEN_GL_RUN, flag_viewpoint, nb_models, nb_q, q_vec));

        // check if child wants to continue
        if (!opengl_win_continue_pipe_answer())
        {
            child_living = false;
        }
    #else
        // update viewpoint
        if (flag_viewpoint >= 0)
        {
            world_3d->SetViewPoint(flag_viewpoint);
        }

        // update the window
        world_3d->Update();
    #endif
}

/*! \brief check if a file exists
 *
 * \param[in] name name (with path) of the file to check
 * \return true if file exists, 0 otherwise
 */
bool check_file_exist(const std::string &name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

/*! \brief convert integer to string
 *
 * \param[in] int_val value of the integer
 * \return requested string
 */
std::string int_to_string(int int_val)
{
    std::stringstream ss_val;

    ss_val << int_val;

    return ss_val.str();
}

/*! \brief convert double to string
 *
 * \param[in] double_val value of the double
 * \return requested string
 */
std::string double_to_string(double double_val)
{
    std::stringstream ss_val;

    ss_val << double_val;

    return ss_val.str();
}

#if (defined OPEN_GL_THREAD_UNIX) || (defined OPEN_GL_PROCESS_UNIX)

/*! \brief create process for Unix
 * 
 * \return PID of the the child process created
 */
pid_t unix_create_process()
{
    pid_t pid;

    // child born
    child_living = true;

    // fork while error is EAGAIN
    do
    {
        pid = fork();

    } while ((pid == -1) && (errno == EAGAIN));

    return pid;
}

/*! \brief check if the child is still living (Unix)
 * 
 * \return true if the child is still living, false otherwise
 */
bool unix_check_child_living()
{
    pid_t test_living;
    int status;

    // child is already dead
    if (!child_living)
    {
        return false;
    }

    // check if child is living
    test_living = waitpid(0, &status, WNOHANG);

    if (test_living == -1) // error
    {
        std::cout << "open_gl_c_int error when checking child existance." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (test_living != 0) // child dead
    {
        child_living = false;
        return false;
    }
    else // child is still living
    {
        return true;
    }
}

#endif

#if (defined OPEN_GL_THREAD_UNIX)

/*! \brief initialize the pipes (Unix thread)
 */
void opengl_thread_unix_init_pipes()
{
    if ((pipe(pipe_state) != 0) || (pipe(pipe_continue) != 0)
        || (pipe(pipe_q_vec) != 0) || (pipe(pipe_viewpoint) != 0) || (pipe(pipe_nb_viewpoint) != 0))
    {
        std::cout << "open_gl_c_int error while creating the pipes." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief start the fork (Unix thread)
 *
 * \param[in] mbs_data Robotran main structure
 * \param[out] visu real-time structure for 3D visualization
 * \param[in] open_gl_in inputs for the OpenGL window and fields creation
 */
void opengl_thread_unix_start_fork(MbsData *mbs_data, Realtime_visu *visu, OpenGlIn *open_gl_in)
{
    // variables declaration
    size_t res;
    int nb_viewpoint;

    // fork: create two processes
    pid_fork = unix_create_process();

    if(pid_fork == -1)    
    {
        fprintf(stderr, "open_gl_c_int error while creating the OpenGL process (fork).\n");
        exit(EXIT_FAILURE);
    }

    if (!pid_fork) // son, in charge of the visualization
    {
        // pipes safety
        close(pipe_continue[0]);
        close(pipe_nb_viewpoint[0]);
        close(pipe_state[1]);
        close(pipe_q_vec[1]);
        close(pipe_viewpoint[1]);

        // OpenGL run
        opengl_thread_unix_run_visu(open_gl_in);

        // stop this thread
        exit(EXIT_SUCCESS);
    }
    else // father, in charge of all simulation, except visualization
    {
        // pipes safety
        close(pipe_continue[1]);
        close(pipe_nb_viewpoint[1]);
        close(pipe_state[0]);
        close(pipe_q_vec[0]);
        close(pipe_viewpoint[0]);

        // get the number of viewpoints
        res = read(pipe_nb_viewpoint[0], &nb_viewpoint, sizeof(int));
        visu->nb_viewpoint = nb_viewpoint;
    }
}

/*! \brief close the son (visualization) for Unix thread
 *
 * \param[in] nb_models number of models
 * \param[out] nb_q number of joints for each model
 * \param[out] q_vec joints positions
 * \param[out] world_3d main 3D world (Open GL)
 */
void opengl_thread_unix_finish_son(int nb_models, int *nb_q, double **q_vec, OpenGLMbs::MbsWorld3D *world_3d)
{
    // release 'q_vec' memory
    for(int i=0; i<nb_models; i++)
    {
        delete[] q_vec[i];
    }
    delete[] q_vec;

    // release 'nb_q' memory
    delete[] nb_q;

    // release 'world_3d' memory
    delete world_3d;
}

/*! \brief 3D visualization main function (Unix thread)
 * 
 * \param[in,out] open_gl_in strucure with all the needed inputs
 */
void opengl_thread_unix_run_visu(OpenGlIn* open_gl_in)
{
    // variables declaration
    size_t res;
    bool run_open_gl_son;
    int nb_models;
    int flag_state, flag_continue, flag_viewpoint;
    int nb_viewpoint;

    int *nb_q;
    double **q_vec;

    OpenGLMbs::MbsWorld3D *world_3d;

    if(pid_fork) // father, in charge of everything, except visualization
    {
        return; // safety, should never happen
    }

    // create world
    world_3d = new OpenGLMbs::MbsWorld3D(1024, 768);

    // save world adress
    open_gl_in->visu->visu_class = world_3d;

    // number of .mbs models to load
    nb_models = open_gl_in->nb_models;

    // load each model
    for(int i=0; i<nb_models; i++)
    {
        world_3d->AddMbs(open_gl_in->mbs_file[i]);
    }

    // set the initial values of each joint
    for(int i=0; i<nb_models; i++)
    {
        world_3d->UpdateJoints(i, open_gl_in->nb_q[i], open_gl_in->q_vec[i]);
    }

    // set the initial viewpoint
    world_3d->SetViewPoint(open_gl_in->start_viewpoint);
    
    // save number of joints for each model
    nb_q = new int[nb_models];

    for(int i=0; i<nb_models; i++)
    {
        nb_q[i] = open_gl_in->nb_q[i];
    }

    // vectors to get joint position
    q_vec = new double*[nb_models];

    for(int i=0; i<nb_models; i++)
    {
        q_vec[i] = new double[nb_q[i]]; 
    }

    // release input structure
    delete open_gl_in;

    // tell to father the number of loaded viewpoints
    nb_viewpoint = world_3d->GetNbViews();
    res = write(pipe_nb_viewpoint[1], &nb_viewpoint, sizeof(int));

    // flag to know when to run the 3D window
    run_open_gl_son = true;

    // loop until command is sent to close
    while (run_open_gl_son)
    {
        // update world 3D timing
        world_3d->UpdateTime();

        // get state from father
        res = read(pipe_state[0], &flag_state, sizeof(int));

        switch (flag_state)
        {
            // model running
            case OPEN_GL_RUN:

                // update joint positions
                for(int i=0; i<nb_models; i++)
                {
                    res = read(pipe_q_vec[0], q_vec[i], nb_q[i]*sizeof(double));

                    // update the joints
                    world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);
                }

                // update viewpoint
                res = read(pipe_viewpoint[0], &flag_viewpoint, sizeof(int));

                if (flag_viewpoint >= 0)
                {
                    world_3d->SetViewPoint(flag_viewpoint);
                }

                // check 3D window
                flag_continue = world_3d->CheckClose() ? 1 : 0;               

                // tell father that update command is received
                res = write(pipe_continue[1], &flag_continue, sizeof(int));

                // update the window
                world_3d->Update();

                if (!flag_continue)
                {
                    opengl_thread_unix_finish_son(nb_models, nb_q, q_vec, world_3d);
                    run_open_gl_son = false;
                }
                break;

            // break
            case OPEN_GL_BREAK:

                // check 3D window
                flag_continue = world_3d->CheckClose() ? 1 : 0;

                // tell father that break command is received
                res = write(pipe_continue[1], &flag_continue, sizeof(int));

                // update the window
                world_3d->Update();

                if (!flag_continue)
                {
                    opengl_thread_unix_finish_son(nb_models, nb_q, q_vec, world_3d);
                    run_open_gl_son = false;
                }
                break;

            // stop the simulation
            case OPEN_GL_STOP:

                opengl_thread_unix_finish_son(nb_models, nb_q, q_vec, world_3d);
                run_open_gl_son = false;

                // tell father that stop command is received
                flag_continue = 0;
                res = write(pipe_continue[1], &flag_continue, sizeof(int));
                break;
        
            default:
                std::cout << "open_gl_c_int error: unknown state flag (" << flag_state << ") !" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
}

#elif (defined OPEN_GL_PROCESS_UNIX)

/*! \brief initialize the pipes (Unix process)
 */
void opengl_proc_unix_init_pipes()
{
    // named pipes: give name
    name_pipe_state        = strdup((char*) ( "/tmp/mbs_visu_" + int_to_string(pid_father) + "_0" + "_state.fifo").c_str()        );
    name_pipe_q_vec        = strdup((char*) ( "/tmp/mbs_visu_" + int_to_string(pid_father) + "_0" + "_q_vec.fifo").c_str()        );
    name_pipe_viewpoint    = strdup((char*) ( "/tmp/mbs_visu_" + int_to_string(pid_father) + "_0" + "_viewpoint.fifo").c_str()    );
    name_pipe_continue     = strdup((char*) ( "/tmp/mbs_visu_" + int_to_string(pid_father) + "_0" + "_continue.fifo").c_str()     );
    name_pipe_nb_viewpoint = strdup((char*) ( "/tmp/mbs_visu_" + int_to_string(pid_father) + "_0" + "_nb_viewpoint.fifo").c_str() );

    // create named pipes
    if ( mkfifo((name_pipe_state)    , 0666) == -1 ||
         mkfifo((name_pipe_q_vec)    , 0666) == -1 ||
         mkfifo((name_pipe_viewpoint), 0666) == -1 )
    {
        std::cout << "open_gl_c_int error while creating the pipes" << std::endl;
        exit(EXIT_FAILURE);
    } 
}

/*! \brief start the fork (Unix process)
 *
 * \param[in] mbs_data Robotran main structure
 * \param[out] visu real-time structure for 3D visualization
 * \param[in] open_gl_in inputs for the OpenGL window and fields creation
 */
void opengl_proc_unix_start_fork(MbsData *mbs_data, Realtime_visu *visu, OpenGlIn *open_gl_in)
{
    // variables declaration
    size_t res;
    int nb_viewpoint;

    std::vector<char*> argv;
    std::string exe_proc_ind, exe_proc_sep;

    // fork: create two processes
    pid_fork = unix_create_process();

    if(pid_fork == -1)    
    {
        fprintf(stderr, "open_gl_c_int error while creating the OpenGL process (fork).\n");
        exit(EXIT_FAILURE);
    }

    if (!pid_fork) // son, in charge of the visualization
    {
        // name of the process
        argv.push_back(strdup((char*) "open_gl_process"));

        // pipe start name: "mbs_visu_" +  PID of the father followed + index of the child (always 0 because only one child)
        argv.push_back(strdup((char*) ("/tmp/mbs_visu_" + int_to_string(pid_father) + "_0").c_str()));

        // inputs of the 3D visualization
        argv.push_back(strdup((char*) int_to_string(open_gl_in->nb_models).c_str()));
        argv.push_back(strdup((char*) int_to_string(open_gl_in->start_viewpoint).c_str()));

        for(int i=0; i<open_gl_in->nb_models; i++)
        {
             argv.push_back(strdup((char*) open_gl_in->mbs_file[i]));
        }

        for(int i=0; i<open_gl_in->nb_models; i++)
        {
            argv.push_back(strdup((char*) int_to_string(open_gl_in->nb_q[i]).c_str()));
        }

        for(int i=0; i<open_gl_in->nb_models; i++)
        {
            for(int j=0; j<open_gl_in->nb_q[i]; j++)
            {
                double cur_val = open_gl_in->q_vec[i][j];
                argv.push_back(strdup((char*) double_to_string(cur_val).c_str()));
            }
        }

        // end of 'argv'
        argv.push_back(NULL);

        // individual build
        exe_proc_ind = mbs_data->build_name;
        exe_proc_ind += "/open_gl_process";

        // separate build
        exe_proc_sep = OPEN_GL_FILES_PATH;
        exe_proc_sep += "/../../../build/open_gl_process";

        // find path of the OpenGL executable
        if (check_file_exist(exe_proc_ind))
        {
            if (execv(exe_proc_ind.c_str(), &argv[0]) == -1)
            {
                std::cout << "open_gl_c_int error with \"execv\": " << exe_proc_ind << " not found !" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (check_file_exist(exe_proc_sep))
        {
            if (execv(exe_proc_sep.c_str(), &argv[0]) == -1)
            {
                std::cout << "open_gl_c_int error with \"execv\": " << exe_proc_sep << " not found !" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            std::cout << "open_gl_c_int error: \"open_gl_process\" executable not found !" << std::endl;
            exit(EXIT_FAILURE);
        }

        // release 'argv'
        for(unsigned int i=0; i<argv.size(); i++)
        {
            free(argv[i]);
        }

        // child done
        exit(EXIT_SUCCESS);
    }
    else // father, in charge of all simulation, except visualization
    {
        // initialize inputs
        if( (in_pipe_state     = open(name_pipe_state    , O_WRONLY)) == -1 ||
            (in_pipe_q_vec     = open(name_pipe_q_vec    , O_WRONLY)) == -1 ||
            (in_pipe_viewpoint = open(name_pipe_viewpoint, O_WRONLY)) == -1 )
        {
            std::cout << "open_gl_c_int error while opening the input pipes" << std::endl;
            exit(EXIT_FAILURE);
        }

        // initialize output
        if( (out_pipe_continue     = open(name_pipe_continue    , O_RDONLY)) == -1 ||
            (out_pipe_nb_viewpoint = open(name_pipe_nb_viewpoint, O_RDONLY)) == -1 ) 
        {
            std::cout << "open_gl_c_int error while opening the output pipes" << std::endl;
            exit(EXIT_FAILURE);
        }

        // get the number of viewpoints
        res = read(out_pipe_nb_viewpoint, &nb_viewpoint, sizeof(int));
        visu->nb_viewpoint = nb_viewpoint;
    }
}

/*! \brief close named pipes (Unix process)
 */
void opengl_proc_unix_close_pipes()
{
    // close pipes
    close(in_pipe_state);
    close(in_pipe_q_vec);
    close(in_pipe_viewpoint);
    close(out_pipe_continue);
    close(out_pipe_nb_viewpoint);

    // release fifo
    unlink(name_pipe_state);
    unlink(name_pipe_q_vec);
    unlink(name_pipe_viewpoint);

    // char* memory release
    free(name_pipe_state);
    free(name_pipe_q_vec);
    free(name_pipe_viewpoint);
    free(name_pipe_continue);
    free(name_pipe_nb_viewpoint);
}

#elif (defined OPEN_GL_PROCESS_WIN)

/*! \brief transfrom pipe message to integer
 *
 * \return requested integer
 */
int opengl_win_pipe_to_int()
{
    int int_val;
    std::stringstream str_val;

    str_val << pipe_msg;
    str_val >> int_val;

    return int_val;
}

/*! \brief send of message to child process
 */
void opengl_win_send_ok_pipe()
{
    bool bSuccess;
    DWORD dwWritten;

    strcpy(pipe_msg, "ok");

    // write to standard output
    bSuccess = WriteFile(hdl_in_wr, pipe_msg, BUFSIZE, &dwWritten, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem sending ok answer to child process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief send pipe message to child process
 */
void opengl_win_send_pipe_msg(std::string msg)
{
    bool bSuccess;
    DWORD dwWritten;

    strcpy(pipe_msg, msg.c_str());

    // write to standard output
    bSuccess = WriteFile(hdl_in_wr, pipe_msg, BUFSIZE, &dwWritten, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem sending pipe message to child process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief wait for message from child and check if message is "stop"
 *
 * \return false if message is "stop", true otherwise
 */
bool opengl_win_continue_pipe_answer()
{
    bool bSuccess;
    DWORD dwRead;
    std::string pipe_str;

    // read child answer
    bSuccess = ReadFile( hdl_out_rd, pipe_msg, BUFSIZE, &dwRead, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem receiving answer from child process." << std::endl;
        exit(EXIT_FAILURE);
    }

    pipe_str = pipe_msg;

    // stop message received
    if (!pipe_str.compare("stop"))
    {
        return false;
    }
    else // no stop
    {
        return true;
    }
}

/*! \brief create a string with information about the visualization update
 *
 * \param[in] flag_state state of the visualization
 * \param[in] flag_viewpoint viewpoint info
 * \param[in] nb_models number of models
 * \param[in] nb_q number of joints for each model
 * \param[in] q_vec joint values for each model
 * \return requested string
 */
std::string opengl_win_create_pipe_string(int flag_state, int flag_viewpoint, int nb_models, int *nb_q, double **q_vec)
{
    std::string new_str;
    std::ostringstream out_float;

    new_str = std::to_string(flag_state) + "|" + std::to_string(flag_viewpoint);

    for(int i=0; i<nb_models; i++)
    {
        new_str += "|";
        for(int j=0; j<nb_q[i]; j++)
        {
            out_float.str("");
            out_float << std::setprecision(6) << q_vec[i][j];

            new_str += "|" + out_float.str();
        }
    }

    return new_str;
}

/*! \brief print the values from the string created with 'opengl_win_create_pipe_string' (used for debug)
 *
 * \param[in] pipe_str input string
 */
void opengl_win_print_pipe_string(std::string pipe_str)
{
    size_t pos;
    int id_count, model_count, flag_state;

    std::cout << "------------" << std::endl;
    std::cout << pipe_str << std::endl;

    model_count = -1;
    id_count = -2;
    pos = pipe_str.find_first_of("|");

    while (pos != std::string::npos || flag_state != OPEN_GL_RUN)
    {
        switch (id_count)
        {
            case -2 :
                flag_state = std::stoi(pipe_str.substr(0, pos));
                std::cout << "flad_state: " << flag_state << std::endl;
                break;

            case -1 :
                std::cout << "flag_viewpoint: " << std::stoi(pipe_str.substr(0, pos)) << std::endl;

                // no need to extract the other info
                if (flag_state != OPEN_GL_RUN)
                {
                    std::cout << "------------" << std::endl;
                    return;
                }
                break;

            default :

                // new model
                if (pos == 0) 
                {
                    id_count = 0;
                    model_count++;

                    pipe_str = pipe_str.substr(1);
                    pos = pipe_str.find_first_of("|");
                }
                
                std::cout << "model " << model_count << ", q[" << id_count << "]: " << std::stof(pipe_str.substr(0, pos)) << std::endl;
                break;
        }

        pipe_str = pipe_str.substr(pos+1);
        pos = pipe_str.find_first_of("|");
        id_count++;
    }

    std::cout << "model " << model_count << ", q[" << id_count << "]: " << std::stof(pipe_str.substr(0, pos)) << std::endl;
    std::cout << "------------" << std::endl;
}

/*! \brief create the pipes
 */
void opengl_win_create_pipes()
{
    SECURITY_ATTRIBUTES s_attr;

    // set the bInheritHandle flag so pipe handles are inherited
    s_attr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    s_attr.bInheritHandle = TRUE; 
    s_attr.lpSecurityDescriptor = NULL; 

    // create a pipe for the child process's STDOUT
    if ( ! CreatePipe(&hdl_out_rd, &hdl_out_wr, &s_attr, 0) )
    {
        std::cout << "open_gl_c_int error: StdoutRd CreatePipe !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ensure the read handle to the pipe for STDOUT is not inherited
    if ( ! SetHandleInformation(hdl_out_rd, HANDLE_FLAG_INHERIT, 0) )
    {
        std::cout << "open_gl_c_int error: Stdout SetHandleInformation !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // create a pipe for the child process's STDIN
    if (! CreatePipe(&hdl_in_rd, &hdl_in_wr, &s_attr, 0))
    {
        std::cout << "open_gl_c_int error: Stdin CreatePipe !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ensure the write handle to the pipe for STDIN is not inherited
    if ( ! SetHandleInformation(hdl_in_wr, HANDLE_FLAG_INHERIT, 0) )
    {
        std::cout << "open_gl_c_int error: Stdin SetHandleInformation !" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief create and launch the child process
 *
 * \param[in] Robotran main structure
 */
void opengl_win_create_child(MbsData *mbs_data)
{
    std::string exe_proc, exe_proc_ind, exe_proc_sep;
    TCHAR *exe_tchar;
    PROCESS_INFORMATION pi_proc_info; 
    STARTUPINFO si_start_info;
    BOOL bSuccess;

    // find executable name with path
    exe_proc_ind = mbs_data->build_name;
    exe_proc_sep = OPEN_GL_FILES_PATH;

    if (check_file_exist(exe_proc_ind + "/open_gl_process.exe"))
    {
        exe_proc = exe_proc_ind + "/open_gl_process.exe";
    }
    else if (check_file_exist(exe_proc_ind + "/Release/open_gl_process.exe"))
    {
        exe_proc = exe_proc_ind + "/Release/open_gl_process.exe";
    }
    else if (check_file_exist(exe_proc_ind + "/Debug/open_gl_process.exe"))
    {
        exe_proc = exe_proc_ind + "/Debug/open_gl_process.exe";
    }
    else if (check_file_exist(exe_proc_sep + "/../../../build/open_gl_process.exe"))
    {
        exe_proc = exe_proc_sep + "/../../../build/open_gl_process.exe";
    }
    else if (check_file_exist(exe_proc_sep + "/../../../build/Release/open_gl_process.exe"))
    {
        exe_proc = exe_proc_sep + "/../../../build/Release/open_gl_process.exe";
    }
    else if (check_file_exist(exe_proc_sep + "/../../../build/Debug/open_gl_process.exe"))
    {
        exe_proc = exe_proc_sep + "/../../../build/Debug/open_gl_process.exe";
    }
    else
    {
        std::cout << "open_gl_c_int error: open_gl_process.exe not found !" << std::endl;
        exit(EXIT_FAILURE);
    }

    exe_tchar = (TCHAR*) exe_proc.c_str();
    
    // set up members of the PROCESS_INFORMATION structure. 
    ZeroMemory( &pi_proc_info, sizeof(PROCESS_INFORMATION) );

    // set up members of the STARTUPINFO structure, specifying the STDIN and STDOUT handles for redirection
    ZeroMemory( &si_start_info, sizeof(STARTUPINFO) );
    si_start_info.cb = sizeof(STARTUPINFO); 
    si_start_info.hStdError  = hdl_out_wr;
    si_start_info.hStdOutput = hdl_out_wr;
    si_start_info.hStdInput  = hdl_in_rd;
    si_start_info.dwFlags    = STARTF_USESTDHANDLES;

    // child born
    child_living = true;

    // create and launch the child process. 
    bSuccess = CreateProcess(NULL, 
        exe_tchar,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &si_start_info,  // STARTUPINFO pointer 
        &pi_proc_info);  // receives PROCESS_INFORMATION 

    // safety
    if ( ! bSuccess )
    {
        std::cout << "open_gl_c_int error: process creation failed (" << exe_tchar << ") !" << std::endl;
        exit(EXIT_FAILURE);
    }
    else 
    {
        // executable name displayed
        std::cout << "Executable " << exe_tchar << " launched" << std::endl;

        // close handles to the child process and its primary thread
        CloseHandle(pi_proc_info.hProcess);
        CloseHandle(pi_proc_info.hThread);
    }
}
#endif

#endif
