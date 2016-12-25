/*! 
 * \author Nicolas Van der Noot
 * \file MbsViewPointMouse.hh
 * \brief MbsViewPointMouse class
 */
#ifdef OPEN_GL

#ifndef _MBS_VIEW_POINT_MOUSE_HH_
#define _MBS_VIEW_POINT_MOUSE_HH_

#include "MbsViewPoint.hh"

namespace OpenGLMbs{

/*! \brief viewpoint to move with the mouse
 */
class MbsViewPointMouse: public MbsViewPoint
{
    public:
        MbsViewPointMouse(glm::vec3 const&rel_pos, glm::vec3 const&rel_look_at, glm::vec3 const&z_up, MbsWorld3D *world_3d,
            bool pers_flag=true, float FoV_scale=0.785f, float near=0.1f, float far=100.0f,
            glm::mat4 const& parent_mat = glm::mat4(1.0f));
        virtual ~MbsViewPointMouse();

        virtual void Update();

    private:
        int state; ///< mouse state

        double init_x; ///< initial mouse position (X axis)
        double init_y; ///< initial mouse position (Y axis)

        float move_fact; ///< move factor [-]
        float zoom_fact; ///< zoom factor [-]
        float scroll_fact; ///< scrolling factor [-]

        double scroll_time; ///< scrolling reset time

        double last_y_scroll;   ///< last y value for mouse scroll
        double t_rise_y_scroll; ///< time when scrolling started

        glm::vec3 look_vec; ///< vector for camera looking direction

        float look_len; ///< length of the 'look_vec' vector

        // function prototypes
        virtual void UpdateView();
        glm::mat4 ParentApply(glm::mat4 const& child_look_at, glm::mat4 const& parent_joint);
};

}
#endif
#endif
