/*! 
 * \author Nicolas Van der Noot
 * \file Joint3D.hh
 * \brief Joint3D class
 */
#ifdef OPEN_GL

#ifndef _JOINT_3D_HH_
#define _JOINT_3D_HH_

#include "AbstractPoint3D.hh"
#include "MbsLight.hh"
#include "MbsViewPoint.hh"

namespace OpenGLMbs{

enum {JOINT_T1, JOINT_T2, JOINT_T3, JOINT_R1, JOINT_R2, JOINT_R3, NO_JOINT};

// forward declaration
class Body3D;

/*! \brief joint in 3D component
 */
class Joint3D: public AbstractPoint3D
{
    public:
        Joint3D(MbsWorld3D *world_3d, const char* name, int joint_type, float joint_value = 0.0f);
        virtual ~Joint3D();

        void UpdateRelMat(float joint_value);

        Body3D* AddBody(const char* name);

        /// add a light
        void AddLight(MbsLight *new_light) { light_list.push_back(new_light); }

        /// add a viewpoint
        void AddViewPoint(MbsViewPoint *new_view) { view_list.push_back(new_view); }

        void UpdateLights();
        void UpdateViewPoints();

        virtual void UpdateAbsMatTree();

        virtual void PrintTree(int level);

    protected:
        int joint_type;    ///< joint type
        float joint_value; ///< joint value [m] or [rad]

        Body3D *body_3d_child; ///< Body3D child (max 1)

        std::vector<MbsLight*> light_list;    ///< list of light childs
        std::vector<MbsViewPoint*> view_list; ///< list of viewpoint childs
};

}
#endif
#endif
