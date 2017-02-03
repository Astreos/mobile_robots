/*! 
 * \author Nicolas Van der Noot
 * \file MbsViewPoint.hh
 * \brief MbsViewPoint class
 */
#ifdef OPEN_GL

#ifndef _MBS_VIEW_POINT_HH_
#define _MBS_VIEW_POINT_HH_

#ifndef degreesToRadians
    #define degreesToRadians(x) x*(3.141592f/180.0f)
#endif

#ifndef GLM_FORCE_RADIANS
    #define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>

// forward declaration
class GLFWwindow;

namespace OpenGLMbs{

// forward declaration
class MbsWorld3D;

/*! \brief viewpoint element
 */
class MbsViewPoint
{
    public:
        MbsViewPoint(MbsWorld3D *world_3d, bool pers_flag=true, glm::mat4 const& parent_mat = glm::mat4(1.0f));
        virtual ~MbsViewPoint();

        virtual void Update() = 0;

        /// get the view matrix
        glm::mat4 GetViewMatrix() const { return view_mat; }

        /// get the projection matrix
        glm::mat4 GetProjectionMatrix() const { return proj_mat; }

        /// get the viewpoint position
        glm::vec3 GetPosition() const { return position; }

        /// update the parent position
        void UpdateParentPos(glm::mat4 const& parent_mat) { this->parent_mat = parent_mat; }

        /// set the windows size
        void SetWinSize(int x_width, int y_width);

    protected:
        MbsWorld3D *world_3d; ///< 3D world
        GLFWwindow *win;      ///< window

        glm::mat4 view_mat; ///< view matrix
        glm::mat4 proj_mat; ///< projection matrix

        glm::vec3 position; ///< viewpoint position
        glm::vec3 look_at;  ///< look-at position
        glm::vec3 z_up;     ///< +z axis

        glm::mat4 rotMat; ///< rotation matrix
        glm::mat4 posMat; ///< position matrix

        bool pers_flag; ///< true for perspective, false for parallel
        
        // window size
        int x_width; ///< along the X axis [pixels]
        int y_width; ///< along the Y axis [pixels]

        float x_width_f; ///< width (pixels) as a float
        float y_width_f; ///< height (pixels) as a float
        float semi_x_width_f; ///< half of 'x_width_f'
        float semi_y_width_f; ///< half of 'y_width_f'

        float par_fact; ///< parallel factor

        float near;    ///< near (close) region
        float far;     ///< far region
        float FoV_rad; ///< field of view [rad] (perspective view)
        float FoV_deg; ///< field of view [deg] (perspective view)
        float scale;   ///< scale (parallel view)

        glm::mat4 parent_mat; ///< parent position

        // function prototype
        void UpdateMatrices();
        void UpdateProjection();
        void UpdateScale(float scale);
        void ScaleFactor(float factor);

        virtual void UpdateView() = 0;
};

}
#endif
#endif
