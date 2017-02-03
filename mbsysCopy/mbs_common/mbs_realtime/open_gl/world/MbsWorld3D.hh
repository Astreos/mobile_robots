/*! 
 * \author Nicolas Van der Noot
 * \file MbsWorld3D.hh
 * \brief MbsWorld3D class
 */
#ifdef OPEN_GL

#ifndef _MBS_WORLD_3D_HH_
#define _MBS_WORLD_3D_HH_

#include "MbsLight.hh"
#include "MbsViewPoint.hh"
#include "MbsShape3D.hh"
#include "Base3D.hh"
#include "MbsRead.hh"

#include <vector>

// forward declaration
class GLFWwindow;

namespace OpenGLMbs{

// forward declaration
class MbsBox;
class MbsCylinder;
class MbsCone;
class MbsSphere;
class MbsInline;

class Joint3D;

class MbsPointLight;
class MbsSpotLight;
class MbsDirLight;

class MbsViewPointMouse;
class MbsViewPointFPS;

/*! \brief OpenGL main visualization
 */
class MbsWorld3D
{
    public:
        MbsWorld3D(int x_width, int y_width,
            glm::vec3 const& background_color = glm::vec3(0.8, 0.8, 0.8));
        ~MbsWorld3D();

        /// get the window
        GLFWwindow* getWin() { return win; }

        /// get the width along the X axis (in pixels)
        int GetXWidth() const { return x_width; }

        /// get the width along the X axis (in pixels)
        int GetYWidth() const { return y_width; }

        /// get the current time
        double GetT() const { return t; }

        /// get the number of nodes
        int GetNbModels() const { return model_list.size(); }

        /// get the number of lights
        int GetNbLights() const { return light_list.size(); }

        /// get the number of viewpoints
        int GetNbViews() const { return view_list.size();  }

        /// get the number of joints
        int GetNbJoints(int i)  const { return joint_tab[i].size(); }

        /// get a requested model
        MbsShape3D* GetModel(int i) { return model_list[i]; }

        /// get a requested light
        MbsLight* GetLight(int i) { return light_list[i]; }

        /// get a requested view
        MbsViewPoint* GetView(int i) { return view_list[i]; }

        /// get the current view
        MbsViewPoint* GetCurView() { return view_list[cur_view]; }

        // add a joint list corresponding to a single .mbs file
        void AddJointList(std::vector<Joint3D*> const& joint_list)
        {
            joint_tab.push_back(joint_list);
        }

        void Update();
        void UpdateTime();
        int CheckClose();

        void NextViewPoint();
        void SetViewPoint(int view_id);

        void AddMbs(const char* mbs_filename);

        void UpdateJoints(int model_id, int nb_q, double *q_vec);

        void SetBackgroundColor(glm::vec3 const& background_color);

        MbsBox* AddBox(double d, double w, double h, glm::vec3 color,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");

        MbsCylinder* AddCylinder(double r, double h, glm::vec3 color,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");

        MbsCone* AddCone(double r, double h, glm::vec3 color,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");

        MbsSphere* AddSphere(double r, glm::vec3 color,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");

        MbsInline* AddInline(std::string cad_file,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");

        MbsPointLight* AddPointLight(glm::vec3 const& rel_pos = glm::vec3(0.0f),
            glm::vec3 const& attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
            glm::mat4 const& parent_mat = glm::mat4(1.0f),
            glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f,
            bool active = true);

        MbsSpotLight* AddSpotLight(glm::vec3 const& rel_pos = glm::vec3(0.0f),
            glm::vec3 const& attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3 const& rel_dir = glm::vec3(0.0f, 0.0f, -1.0f),
            glm::mat4 const& parent_mat = glm::mat4(1.0f),
            glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f,
            float spread_angle = 3.1415927f,
            float concentration = 0.0f,
            bool active = true);

        MbsDirLight* AddDirLight(glm::vec3 const& rel_dir = glm::vec3(0.0f, 0.0f, -1.0f),
            glm::mat4 const& parent_mat = glm::mat4(1.0f),
            glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f,
            bool active = true);

        MbsViewPointMouse* AddViewPointMouse(glm::vec3 const&rel_pos, glm::vec3 const&rel_look_at, glm::vec3 const&z_up,
            bool pers_flag=true,
            float FoV_scale=0.785f, float near=0.1f, float far=100.0f,
            glm::mat4 const& parent_mat = glm::mat4(1.0f));

        MbsViewPointFPS* AddViewPointFPS(glm::vec3 const& position, glm::vec3 const& look_at,
            glm::vec3 const& z_up, float FoV_rad);

    private:
        MbsRead mbs_read; ///< .mbs reader

        GLFWwindow *win; ///< main visualizatin window

        std::vector<MbsViewPoint*> view_list; ///< list of viewpoints

        std::vector<MbsShape3D*> model_list; ///< list of models

        std::vector<MbsLight*> light_list; ///< list of lights

        std::vector< std::vector<Joint3D*> > joint_tab; ///< tabular of joints

        std::vector<Base3D*> mbs_list; ///< list of .mbs models

        std::string main_vert_sh; ///< main vertex shader path
        std::string main_frag_sh; ///< main fragment shader path

        int cur_view; ///< ID of the current view

        bool flag_first; ///< flag to know if it is the first call

        double t;      ///< current time [s]
        double init_t; ///< initial time [s]

        int x_width; ///< screen width along the X axis (in pixels)
        int y_width; ///< screen width along the Y axis (in pixels)
};

}
#endif
#endif
