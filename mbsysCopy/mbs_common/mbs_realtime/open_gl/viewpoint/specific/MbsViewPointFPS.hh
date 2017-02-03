/*! 
 * \author Nicolas Van der Noot
 * \file MbsViewPointFPS.hh
 * \brief MbsViewPointFPS class
 */
#ifdef OPEN_GL

#ifndef _MBS_VIEW_POINT_FPS_HH_
#define _MBS_VIEW_POINT_FPS_HH_

#include "MbsViewPoint.hh"

namespace OpenGLMbs{

/*! \brief viewpoint with a FPS motion style
 */
class MbsViewPointFPS: public MbsViewPoint
{
    public:
        MbsViewPointFPS(glm::vec3 const& position, glm::vec3 const& look_at, glm::vec3 const& z_up,
            float FoV_rad, MbsWorld3D *world_3d);
        virtual ~MbsViewPointFPS();

        virtual void Update();

    private:
        float hor_angle;  ///< horizontal angle [rad]
        float vert_angle; ///< vertical angle [rad]

        float key_speed;   ///< speed for keybord keys
        float mouse_speed; ///< speed for mouse

        double lastTime; ///< last time camera was updated

        virtual void UpdateView();
};

}
#endif
#endif
