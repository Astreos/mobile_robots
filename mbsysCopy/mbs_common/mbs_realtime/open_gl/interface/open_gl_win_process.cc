/*! 
 * \author Nicolas Van der Noot
 * \file open_gl_process.cc
 * \brief main function to run the separate process for Windows
 */
#if (defined OPEN_GL) && (defined PROCESS_OPEN_GL) && (defined WIN32)

#include "MbsWorld3D.hh"
#include "cmake_config.h"
#include "cmake_open_gl_config.h"
#include "open_gl_c_int.h"

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <windows.h>

// size of the buffer to send messages
#define BUFSIZE 4096

// states of the window
enum {OPEN_GL_RUN, OPEN_GL_BREAK, OPEN_GL_STOP};

// gloabal variable
char pipe_msg[BUFSIZE]; ///< buffer for pipe messages
HANDLE hdl_in, hdl_out; ///< handles for pipes inputs and outputs

// function prototypes
int opengl_win_pipe_to_int();
void opengl_win_send_continue_pipe();
void opengl_win_send_stop_pipe();
void opengl_win_wait_pipe();
void opengl_win_send_int_pipe_msg(int val);
void opengl_win_extract_pipe_string(std::string pipe_str, int &flag_state, int &flag_viewpoint, int nb_models, int *nb_q, double **q_vec);

/*! \brief main function, used for 3D visualization in a separate process
 * 
 * \param[in] argc number of arguments
 * \param[in,out] args strucure with all the needed inputs
 */
int main(int argc, char const *argv[])
{
    // variables declaration
    int nb_models;
    int flag_state, flag_viewpoint;
    bool run_open_gl;

    int *nb_q;
    double **q_vec;

    OpenGLMbs::MbsWorld3D *world_3d;

    // get handles
    hdl_out = GetStdHandle(STD_OUTPUT_HANDLE); 
    hdl_in  = GetStdHandle(STD_INPUT_HANDLE); 

    if ( (hdl_out == INVALID_HANDLE_VALUE) || (hdl_in == INVALID_HANDLE_VALUE) )
    {
        std::cout << "open_gl_process error when getting the handles." << std::endl;
        exit(EXIT_FAILURE);
    }

    // create world
    world_3d = new OpenGLMbs::MbsWorld3D(1024, 768);

    // number of .mbs models to load
    opengl_win_wait_pipe();
    nb_models = opengl_win_pipe_to_int();
    opengl_win_send_continue_pipe();

    // load each model
    for(int i=0; i<nb_models; i++)
    {
        opengl_win_wait_pipe();
        world_3d->AddMbs(pipe_msg);
        opengl_win_send_continue_pipe();
    }

    // save number of joints for each model
    nb_q = new int[nb_models];

    for(int i=0; i<nb_models; i++)
    {
        opengl_win_wait_pipe();
        nb_q[i] = opengl_win_pipe_to_int();
        opengl_win_send_continue_pipe();
    }

    // vectors to get joint position
    q_vec = new double*[nb_models];

    for(int i=0; i<nb_models; i++)
    {
        q_vec[i] = new double[nb_q[i]];

        for(int j=0; j<nb_q[i]; j++)
        {
            q_vec[i][j] = 0.0;
        }
    }

    // set the initial values of each joint
    opengl_win_wait_pipe();
    opengl_win_extract_pipe_string(pipe_msg, flag_state, flag_viewpoint, nb_models, nb_q, q_vec);
    opengl_win_send_continue_pipe();

    for(int i=0; i<nb_models; i++)
    {
        world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);       
    }

    // set the initial viewpoint
    if (flag_viewpoint >= 0)
    {
        world_3d->SetViewPoint(flag_viewpoint);
    }
    else
    {
        world_3d->SetViewPoint(0);
    }

    // send the number of viewpoints
    opengl_win_send_int_pipe_msg(world_3d->GetNbViews());
    opengl_win_wait_pipe();

    // flag to know when to run the 3D window
    run_open_gl = true;

    // loop until command is sent to close
    while (run_open_gl)
    {
        // update world 3D timing
        world_3d->UpdateTime();             

        // get state, view and joints from father process
        opengl_win_wait_pipe();
        opengl_win_extract_pipe_string(pipe_msg, flag_state, flag_viewpoint, nb_models, nb_q, q_vec);

        // answer to father, depending on 3D window state
        if (!world_3d->CheckClose() || flag_state == OPEN_GL_STOP)
        {
            run_open_gl = false;
            opengl_win_send_stop_pipe();
        }
        else
        {
            opengl_win_send_continue_pipe();
        }

        switch (flag_state)
        {
            // model running
            case OPEN_GL_RUN:

                // update joint positions
                for(int i=0; i<nb_models; i++)
                {
                    // update the joints
                    world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);
                }

                // update viewpoint
                if (flag_viewpoint >= 0)
                {
                    world_3d->SetViewPoint(flag_viewpoint);
                }

                // update the window
                world_3d->Update();
                break;

            // break
            case OPEN_GL_BREAK:

                // update the window
                world_3d->Update();
                break;

            // stop the simulation
            case OPEN_GL_STOP:

                run_open_gl = false; // safety: already done normally
                break;
        
            default:
                std::cout << "open_gl_win_process error: unknown state flag (" << flag_state << ") !" << std::endl;
                exit(EXIT_FAILURE);
        }
    }

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

    exit(EXIT_SUCCESS);
}

/*! \brief convert pipe to int
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

/*! \brief wait to receive pipe message from father process
 */
void opengl_win_wait_pipe()
{
    bool bSuccess;
    DWORD dwRead;

    // read child answer
    bSuccess = ReadFile(hdl_in, pipe_msg, BUFSIZE, &dwRead, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem receiving answer from child process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief send integer message to father process
 */
void opengl_win_send_int_pipe_msg(int val)
{
    bool bSuccess;
    DWORD dwWritten;

    strcpy(pipe_msg, std::to_string(val).c_str());

    // write to standard output
    bSuccess = WriteFile(hdl_out, pipe_msg, BUFSIZE, &dwWritten, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem sending int value (" << val << ") answer to father process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief send continue message to father process
 */
void opengl_win_send_continue_pipe()
{
    bool bSuccess;
    DWORD dwWritten;

    strcpy(pipe_msg, "continue");

    // write to standard output
    bSuccess = WriteFile(hdl_out, pipe_msg, BUFSIZE, &dwWritten, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem sending ok answer to father process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief send stop message to father process
 */
void opengl_win_send_stop_pipe()
{
    bool bSuccess;
    DWORD dwWritten;

    strcpy(pipe_msg, "stop");

    // write to standard output
    bSuccess = WriteFile(hdl_out, pipe_msg, BUFSIZE, &dwWritten, NULL);

    if (!bSuccess)
    {
        std::cout << "open_gl_process error: problem sending ok answer to father process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief extract pipe message from father (string message)
 *
 * \param[in] pipe_str string input message
 * \param[out] flag_state state of the visualization
 * \param[out] flagviewpoint flag to change the viewpoint
 * \param[in] nb_models number of models (to perform checking)
 * \param[in] nb_q number of joints per model (to perform checking)
 * \param[out] q_vec joints position
 */
void opengl_win_extract_pipe_string(std::string pipe_str, int &flag_state, int &flag_viewpoint, int nb_models, int *nb_q, double **q_vec)
{
    size_t pos;
    int id_count, model_count;

    model_count = -1;
    id_count = -2;
    pos = pipe_str.find_first_of("|");

    // loop on the whole string
    while (pos != std::string::npos || flag_state != OPEN_GL_RUN)
    {
        switch (id_count)
        {
            // state
            case -2 :
                flag_state = std::stoi(pipe_str.substr(0, pos));
                break;

            // viewpoint
            case -1 :
                flag_viewpoint = std::stoi(pipe_str.substr(0, pos));

                // no need to extract the other info
                if (flag_state != OPEN_GL_RUN)
                {
                    return;
                }
                break;

            // joint positions
            default :
                // new model
                if (pos == 0) 
                {
                    id_count = 0;
                    model_count++;

                    // safety
                    if (model_count >= nb_models)
                    {
                        std::cout << "open_gl_process error: at least" << model_count + 1 << " models from pipe instead of " << nb_models << "." << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    pipe_str = pipe_str.substr(1);
                    pos = pipe_str.find_first_of("|");
                }

                // safety
                if (id_count >= nb_q[model_count])
                {
                    std::cout << "open_gl_process error: index" << id_count << " from pipe is to high (nb_q[" << model_count << "] = " << nb_q[model_count] << ")." << std::endl;
                    exit(EXIT_FAILURE);
                }
                
                // update joint position
                q_vec[model_count][id_count] = std::stof(pipe_str.substr(0, pos));
                break;
        }

        // next part of the string
        pipe_str = pipe_str.substr(pos+1);
        pos = pipe_str.find_first_of("|");
        id_count++; 
    }

    // safety
    if (id_count >= nb_q[model_count])
    {
        std::cout << "open_gl_process error: index" << id_count << " from pipe is to high (nb_q[" << model_count << "] = " << nb_q[model_count] << ")." << std::endl;
        exit(EXIT_FAILURE);
    }

    // last value
    q_vec[model_count][id_count] = std::stof(pipe_str.substr(0, pos));
}

#endif
