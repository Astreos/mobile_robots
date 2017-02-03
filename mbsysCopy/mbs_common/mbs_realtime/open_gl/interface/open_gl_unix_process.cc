/*! 
 * \author Nicolas Van der Noot
 * \file open_gl_process.cc
 * \brief main function to run the separate process for Unix
 */
#if (defined OPEN_GL) && (defined PROCESS_OPEN_GL) && (defined UNIX)

#include "MbsWorld3D.hh"
#include "cmake_config.h"
#include "cmake_open_gl_config.h"
#include "open_gl_c_int.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <sstream>

// states of the window
enum {OPEN_GL_RUN, OPEN_GL_BREAK, OPEN_GL_STOP};

// function prototypes
int string_to_int(std::string str);
double string_to_double(std::string str);

/*! \brief main function, used for 3D visualization in a separate process
 * 
 * \param[in] argc number of arguments
 * \param[in,out] args strucure with all the needed inputs
 */
int main(int argc, char const *argv[])
{
    // variables declaration
    size_t res;
    int nb_models, in_count, nb_viewpoint;
    int flag_state, flag_viewpoint, flag_continue;
    bool run_open_gl;

    std::string pipe_start_name;

    int *nb_q;
    double **q_vec;

    // pipes IO
    int out_pipe_state;       ///< output of the named pipe 'name_pipe_state'
    int out_pipe_q_vec;       ///< output of the named pipe 'name_pipe_q_vec'
    int out_pipe_viewpoint;   ///< output of the named pipe 'name_pipe_viewpoint'
    int in_pipe_continue;     ///<  input of the named pipe 'name_pipe_continue'
    int in_pipe_nb_viewpoint; ///<  input of the named pipe 'name_pipe_nb_viewpoint'

    // pipe names
    char *name_pipe_state;        ///< name of the named pipe 'name_pipe_state'
    char *name_pipe_q_vec;        ///< name of the named pipe 'name_pipe_q_vec'
    char *name_pipe_viewpoint;    ///< name of the named pipe 'name_pipe_viewpoint'
    char *name_pipe_continue;     ///< name of the named pipe 'name_pipe_continue'
    char *name_pipe_nb_viewpoint; ///< name of the named pipe 'name_pipe_nb_viewpoint'

    OpenGLMbs::MbsWorld3D *world_3d;

    // create world
    world_3d = new OpenGLMbs::MbsWorld3D(1024, 768);

    // named pipes: get names
    pipe_start_name = argv[1];

    name_pipe_state        = strdup( (char*) (pipe_start_name + "_state.fifo").c_str()        );
    name_pipe_q_vec        = strdup( (char*) (pipe_start_name + "_q_vec.fifo").c_str()        );
    name_pipe_viewpoint    = strdup( (char*) (pipe_start_name + "_viewpoint.fifo").c_str()    );
    name_pipe_continue     = strdup( (char*) (pipe_start_name + "_continue.fifo").c_str()     );
    name_pipe_nb_viewpoint = strdup( (char*) (pipe_start_name + "_nb_viewpoint.fifo").c_str() );

    // create named pipes
    if ( mkfifo((name_pipe_continue)    , 0666) == -1 ||
         mkfifo((name_pipe_nb_viewpoint), 0666) == -1 )
    {
        std::cout << "open_gl_unix_process error while creating the pipes" << std::endl;
        exit(EXIT_FAILURE);
    }    

    if( (out_pipe_state     = open(name_pipe_state    , O_RDONLY)) == -1 ||
        (out_pipe_q_vec     = open(name_pipe_q_vec    , O_RDONLY)) == -1 ||
        (out_pipe_viewpoint = open(name_pipe_viewpoint, O_RDONLY)) == -1 ) 
    {
        std::cout << "open_gl_unix_process error while opening the output pipes" << std::endl;
        exit(EXIT_FAILURE);
    }

    // initialize input
    if( (in_pipe_continue     = open(name_pipe_continue    , O_WRONLY)) == -1 ||
        (in_pipe_nb_viewpoint = open(name_pipe_nb_viewpoint, O_WRONLY)) == -1 )
    {
        std::cout << "open_gl_unix_process error while opening the input pipes" << std::endl;
        exit(EXIT_FAILURE);
    }

    // number of .mbs models to load
    nb_models = string_to_int(argv[2]);

    // load each model
    in_count = 4; ///< first possible index

    for(int i=0; i<nb_models; i++)
    {
        world_3d->AddMbs(argv[in_count]);
        in_count++;
    }

    // save number of joints for each model
    nb_q = new int[nb_models];

    for(int i=0; i<nb_models; i++)
    {
        nb_q[i] = string_to_int(argv[in_count]);
        in_count++;
    }

    // vectors to get joint position
    q_vec = new double*[nb_models];

    for(int i=0; i<nb_models; i++)
    {
        q_vec[i] = new double[nb_q[i]];

        for(int j=0; j<nb_q[i]; j++)
        {
            // set the initial values of each joint
            q_vec[i][j] = string_to_double(argv[in_count]);
            in_count++;
        }
    }

    for(int i=0; i<nb_models; i++)
    {
        world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);       
    }

    flag_viewpoint = string_to_int(argv[3]);

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
    nb_viewpoint = world_3d->GetNbViews();
    res = write(in_pipe_nb_viewpoint, &nb_viewpoint, sizeof(int));

    // flag to know when to run the 3D window
    run_open_gl = true;

    // loop until command is sent to close
    while (run_open_gl)
    {
        // update world 3D timing
        world_3d->UpdateTime();             

        // get state from father process
        res = read(out_pipe_state, &flag_state, sizeof(int));

        switch (flag_state)
        {
            // model running
            case OPEN_GL_RUN:

                // update joint positions
                for(int i=0; i<nb_models; i++)
                {
                    res = read(out_pipe_q_vec, q_vec[i], nb_q[i]*sizeof(double));

                    // update the joints
                    world_3d->UpdateJoints(i, nb_q[i], q_vec[i]);
                }

                // update viewpoint
                res = read(out_pipe_viewpoint, &flag_viewpoint, sizeof(int));
                
                if (flag_viewpoint >= 0)
                {
                    world_3d->SetViewPoint(flag_viewpoint);
                }

                // check 3D window
                flag_continue = world_3d->CheckClose() ? 1 : 0;               

                // tell father that update command is received
                res = write(in_pipe_continue, &flag_continue, sizeof(int));

                // update the window
                world_3d->Update();

                if (!flag_continue)
                {
                    run_open_gl = false;
                }
                break;

            // break
            case OPEN_GL_BREAK:

                // check 3D window
                flag_continue = world_3d->CheckClose() ? 1 : 0;

                // tell father that break command is received
                res = write(in_pipe_continue, &flag_continue, sizeof(int));

                // update the window
                world_3d->Update();

                if (!flag_continue)
                {
                    run_open_gl = false;
                }
                break;

            // stop the simulation
            case OPEN_GL_STOP:

                run_open_gl = false;

                // tell father that stop command is received
                flag_continue = 0;
                res = write(in_pipe_continue, &flag_continue, sizeof(int));
                break;
        
            default:
                std::cout << "open_gl_unix_process error: unknown state flag (" << flag_state << ") !" << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    // close pipes
    close(out_pipe_state);
    close(out_pipe_q_vec);
    close(out_pipe_viewpoint);
    close(in_pipe_continue);
    close(in_pipe_nb_viewpoint);

    // release fifo
    unlink(name_pipe_nb_viewpoint);

    // char* memory release
    free(name_pipe_state);
    free(name_pipe_continue);
    free(name_pipe_q_vec);
    free(name_pipe_viewpoint);
    free(name_pipe_nb_viewpoint);
    
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
            
    // stop this process
    exit(EXIT_SUCCESS);
}

/*! \brief convert string to int
 *
 * \return requested integer
 */
int string_to_int(std::string str)
{
    int int_val;
    std::stringstream str_val;

    str_val << str;
    str_val >> int_val;

    return int_val;
}

/*! \brief convert string to double
 *
 * \return requested double
 */
double string_to_double(std::string str)
{
    double double_val;
    std::stringstream str_val;

    str_val << str;
    str_val >> double_val;

    return double_val;
}

#endif
