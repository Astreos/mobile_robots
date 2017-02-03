#ifdef OPEN_GL

#include "MbsWorld3D.hh"
#include "cmake_open_gl_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include "MbsPointLight.hh"
#include "MbsSpotLight.hh"
#include "MbsDirLight.hh"

#include "MbsViewPointFPS.hh"
#include "MbsViewPointMouse.hh"

#include "MbsBox.hh"
#include "MbsCylinder.hh"
#include "MbsCone.hh"
#include "MbsSphere.hh"
#include "MbsInline.hh"

#include <gtx/transform.hpp>

namespace OpenGLMbs{

/// limit an integer x in the [min;max] range
inline int limit_int_range(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }

/*! \brief constructor
 *
 * \param[in] x_width screen width along the X axis (in pixels)
 * \param[in] y_width screen width along the Y axis (in pixels)
 * \param[in] background_color color of the background
 */
MbsWorld3D::MbsWorld3D(int x_width, int y_width, glm::vec3 const& background_color): mbs_read(this)
{
    // GLFW initialization (window for visualization)
    if( !glfwInit() )
    {
        std::cout << "Error: failed to initialize GLFW !" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL *.3
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MacOS compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // prevent old OpenGL versions

    // new window
    this->x_width = x_width;
    this->y_width = y_width;

    win = glfwCreateWindow(x_width, y_width, "OpenGL visualization", NULL, NULL);

    if( win == NULL )
    {
        std::cout << "Error: failed to open GLFW window !" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);

    // GLEW initialization
    glewExperimental = true; // needed in core profile

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: failed to initialize GLEW !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // being able to capture the escape key being pressed below
    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

    // background
    glClearColor(background_color[0], background_color[1], background_color[2], 0.0);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    // performance improvement
    glEnable(GL_CULL_FACE);

    // accept fragment if it closer to the view than the former one
    glDepthFunc(GL_LESS);

    // remove triangles whose normal is not towards the view
    glEnable(GL_CULL_FACE);

    // enable blending (alpha transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    t = 0.0;
    init_t = glfwGetTime();

    cur_view = 0;

    // clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // swap buffers
    glfwSwapBuffers(win);

    flag_first = false;

    // shaders
    main_vert_sh = OPEN_GL_FILES_PATH"shaders/frag_vert/shader.vert";
    main_frag_sh = OPEN_GL_FILES_PATH"shaders/frag_vert/shader.frag";
}

/*! \brief destructor
 */
MbsWorld3D::~MbsWorld3D()
{
    for(unsigned int i=0; i<mbs_list.size(); i++)
    {
        mbs_list[i]->ReleaseChildList();
        delete(mbs_list[i]);
    }

    for(unsigned int i=0; i<model_list.size(); i++)
    {
        delete model_list[i];
    }

    for(unsigned int i=0; i<light_list.size(); i++)
    {
        delete light_list[i];
    }

    for(unsigned int i=0; i<view_list.size(); i++)
    {
        delete view_list[i];
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

/*! \brief update the real timing
 */
void MbsWorld3D::UpdateTime()
{
    // time since start
    if (!flag_first)
    {
        flag_first = true;

        t = 0.0;
        init_t = glfwGetTime();
    }
    else
    {
        t = glfwGetTime() - init_t;
    }
}

/*! \brief set the background color
 * 
 * \param[in] background_color background color
 */
void MbsWorld3D::SetBackgroundColor(glm::vec3 const& background_color)
{
    glClearColor(background_color[0], background_color[1], background_color[2], 0.0);
}

/*! \brief update the joints position
 * 
 * \param[in] model_id ID of the model to update
 * \param[in] nb_q number of joints of the current model (for check)
 * \param[in] q_vec vector (of size 'nb_q') with the new joint positions
 */
void MbsWorld3D::UpdateJoints(int model_id, int nb_q, double *q_vec)
{
    int nb_q_check = joint_tab[model_id].size();

    // safety
    if (nb_q != nb_q_check)
    {
        std::cout << "MbsWorld3D error: nb_q (" << nb_q << ") different from number of joints (" << nb_q_check << ") !" << std::endl;
        exit(EXIT_FAILURE);
    }

    for(unsigned int i=0; i<nb_q; i++)
    {
        joint_tab[model_id][i]->UpdateRelMat(q_vec[i]);
    }
}

/*! \brief check if user is closing the window (escape key or close button)
 */
int MbsWorld3D::CheckClose()
{
    return glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(win) == 0;
}

/*! \brief set to next viewpoint
 */
void MbsWorld3D::NextViewPoint()
{
    cur_view++;

    if (cur_view >= view_list.size())
    {
        cur_view = 0;
    }
}

/*! \brief set to a given viewpoint number
 * 
 * \param[in] view_id ID of the requested viewpoint
 */
void MbsWorld3D::SetViewPoint(int view_id)
{
    cur_view = limit_int_range(view_id, 0, view_list.size()-1);
}

/*! \brief main 3D world update
 */
void MbsWorld3D::Update()
{
    int new_x_width, new_y_width;

    // safety
    if (view_list.size() <= 0)
    {
        std::cout << "Error: at least one MbsViewPoint must be defined !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // window size update
    glfwGetFramebufferSize(win, &new_x_width, &new_y_width);

    if (x_width != new_x_width || y_width != new_y_width)
    {
        x_width = new_x_width;
        y_width = new_y_width;

        for(unsigned int i=0; i<view_list.size(); i++)
        {
            view_list[i]->SetWinSize(x_width, y_width);
        }
    }

    // update absolute matrices positions for all .mbs models
    for(unsigned int i=0; i<mbs_list.size(); i++)
    {
        mbs_list[i]->UpdateAbsMatTree();
    }

    // clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update current view
    view_list[cur_view]->Update();

    // update all lights
    for(unsigned int i=0; i<light_list.size(); i++)
    {
        light_list[i]->Update();
    }

    // update all models (3D shapes)
    for(unsigned int i=0; i<model_list.size(); i++)
    {
        model_list[i]->Update();
    }

    // make context current
    glfwMakeContextCurrent(win);

    // swap buffers
    glfwSwapBuffers(win);

    // check evevnts
    glfwPollEvents();
}

/*! \brief add a viewpoint of type 'MbsViewPointMouse'
 *
 * \param[in] rel_pos relative position (with regard to the parent joint)
 * \param[in] rel_look_at relative look direction (with regard to the parent joint)
 * \param[in] z_up up vector
 * \param[in] pers_flag true for perspective, false for parallel view
 * \param[in] FoV_scale field of view (perspective [rad]) or scale (parallel [-])
 * \param[in] near near value for screen projection
 * \param[in] far far value for screen projection
 * \param[in] parent_mat parent matrix
 * \return pointer to the new viewpoint
 */
MbsViewPointMouse* MbsWorld3D::AddViewPointMouse(glm::vec3 const&rel_pos, glm::vec3 const&rel_look_at,glm::vec3 const&z_up,
    bool pers_flag, float FoV_scale, float near, float far,
    glm::mat4 const& parent_mat)
{
    MbsViewPointMouse *new_view_mouse = new MbsViewPointMouse(rel_pos, rel_look_at, z_up, this,
            pers_flag, FoV_scale, near, far, parent_mat);

    view_list.push_back(new_view_mouse);

    return new_view_mouse;
}

/*! \brief add a viewpoint of type 'MbsViewPointFPS'
 *
 * \param[in] position position
 * \param[in] look_at look direction
 * \param[in] z_up up vector
 * \param[in] FoV_rad field of view (perspective [rad])
 * \return pointer to the new viewpoint
 */
MbsViewPointFPS* MbsWorld3D::AddViewPointFPS(glm::vec3 const& position, glm::vec3 const& look_at,
    glm::vec3 const& z_up, float FoV_rad)
{
    MbsViewPointFPS *new_view_fps = new MbsViewPointFPS(position, look_at, z_up, FoV_rad, this);

    view_list.push_back(new_view_fps);

    return new_view_fps;
}

/*! \brief add a light of type 'MbsPointLight'
 *
 * \param[in] rel_pos relative (with regard to parent joint) position of the light
 * \param[in] attenuation attenuation of the light
 * \param[in] parent_mat parent matrix
 * \param[in] active true if light active, 0 otherwise
 * \return pointer to the new light
 */
MbsPointLight* MbsWorld3D::AddPointLight(glm::vec3 const& rel_pos,
    glm::vec3 const& attenuation, glm::mat4 const& parent_mat, glm::vec3 const& color, float ambient, bool active)
{
    MbsPointLight *new_light;

    // maximum of 4 lights allowed
    if (light_list.size() >= 10)
    {
        std::cout << "Warning: only 10 lights can be added !" << std::endl;
        return NULL;
    }

    new_light = new MbsPointLight(rel_pos, attenuation, parent_mat, color, ambient, active);

    light_list.push_back(new_light);

    return new_light;
}

/*! \brief add a light of type 'MbsSpotLight'
 *
 * \param[in] rel_pos relative (with regard to parent joint) position of the light
 * \param[in] attenuation attenuation of the light
 * \param[in] rel_dir relative (with regard to parent joint) direction of the light
 * \param[in] parent_mat parent matrix
 * \param[in] spread_angle sprend angle of the light [rad]
 * \param[in] concentration concentration value
 * \param[in] active true if light active, 0 otherwise
 * \return pointer to the new light
 */
MbsSpotLight* MbsWorld3D::AddSpotLight(glm::vec3 const& rel_pos, glm::vec3 const& attenuation,
    glm::vec3 const& rel_dir, glm::mat4 const& parent_mat, glm::vec3 const& color,
    float ambient, float spread_angle, float concentration, bool active)
{
    MbsSpotLight *new_light;

    // maximum of 4 lights allowed
    if (light_list.size() >= 10)
    {
        std::cout << "Warning: only 10 lights can be added !" << std::endl;
        return NULL;
    }

    new_light = new MbsSpotLight(rel_pos, attenuation, rel_dir, parent_mat, color, ambient,
        spread_angle, concentration, active);

    light_list.push_back(new_light);

    return new_light;
}

/*! \brief add a light of type 'MbsDirLight'
 *
 * \param[in] rel_dir relative (with regard to parent joint) direction of the light
 * \param[in] parent_mat parent matrix
 * \param[in] active true if light active, 0 otherwise
 * \return pointer to the new light
 */
MbsDirLight* MbsWorld3D::AddDirLight(glm::vec3 const& rel_dir, glm::mat4 const& parent_mat, glm::vec3 const& color, float ambient, bool active)
{
    MbsDirLight *new_light;

    // maximum of 4 lights allowed
    if (light_list.size() >= 10)
    {
        std::cout << "Warning: only 10 lights can be added !" << std::endl;
        return NULL;
    }

    new_light = new MbsDirLight(rel_dir, parent_mat, color, ambient, active);

    light_list.push_back(new_light);

    return new_light;
}

/*! \brief add a box
 *
 * \param[in] d depth (along X) [m]
 * \param[in] w width (along Y) [m]
 * \param[in] h height (along Z) [m]
 * \param[in] color color of the shape
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 * \return pointer to the new box
 */
MbsBox* MbsWorld3D::AddBox(double d, double w, double h, glm::vec3 color, float transparency,
    glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot, const char* name)
{
    MbsBox *new_box = new MbsBox(d, w, h, color,
        main_vert_sh.c_str(), main_frag_sh.c_str(),
        this, transparency, trans_pos, trans_scale, trans_rot, name);

    model_list.push_back(new_box);

    return new_box;
}

/*! \brief add a cylinder
 *
 * \param[in] r radius [m]
 * \param[in] h height [m]
 * \param[in] color color of the shape
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 * \return pointer to the new cylinder
 */
MbsCylinder* MbsWorld3D::AddCylinder(double r, double h, glm::vec3 color, float transparency,
    glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot, const char* name)
{
    MbsCylinder *new_cylinder = new MbsCylinder(r, h, color,
        main_vert_sh.c_str(), main_frag_sh.c_str(),
        this, transparency, trans_pos, trans_scale, trans_rot, name);

    model_list.push_back(new_cylinder);

    return new_cylinder;
}

/*! \brief add a cone
 *
 * \param[in] r radius [m]
 * \param[in] h height [m]
 * \param[in] color color of the shape
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 * \return pointer to the new cone
 */
MbsCone* MbsWorld3D::AddCone(double r, double h, glm::vec3 color, float transparency,
    glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot, const char* name)
{
    MbsCone *new_cone = new MbsCone(r, h, color,
        main_vert_sh.c_str(), main_frag_sh.c_str(),
        this, transparency, trans_pos, trans_scale, trans_rot, name);

    model_list.push_back(new_cone);

    return new_cone;
}

/*! \brief add a sphere
 *
 * \param[in] r radius [m]
 * \param[in] color color of the shape
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 * \return pointer to the new sphere
 */
MbsSphere* MbsWorld3D::AddSphere(double r, glm::vec3 color, float transparency,
    glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot, const char* name)
{
    MbsSphere *new_sphere = new MbsSphere(r, color,
        main_vert_sh.c_str(), main_frag_sh.c_str(),
        this, transparency, trans_pos, trans_scale, trans_rot, name);

    model_list.push_back(new_sphere);

    return new_sphere;
}

/*! \brief add an inline shape
 *
 * \param[in] cad_file CAD file to read (with path)
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 * \return pointer to the new inline shape
 */
MbsInline* MbsWorld3D::AddInline(std::string cad_file, float transparency,
    glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot, const char* name)
{
    MbsInline *new_inline = new MbsInline(cad_file,
        main_vert_sh.c_str(), main_frag_sh.c_str(),
        this, transparency, trans_pos, trans_scale, trans_rot, name);

    model_list.push_back(new_inline);

    return new_inline;
}

/*! \brief add a .mbs file to the 3D world
 * 
 * \param[in] mbs_filename name of the .mbs file (with path)
 */
void MbsWorld3D::AddMbs(const char* mbs_filename)
{
    Base3D *new_mbs = NULL;

    new_mbs = mbs_read.AddSingleMbs(mbs_filename);

    if (new_mbs != NULL)
    {
        new_mbs->UpdateAbsMatTree();

        mbs_list.push_back(new_mbs);
    }
}

}
#endif
