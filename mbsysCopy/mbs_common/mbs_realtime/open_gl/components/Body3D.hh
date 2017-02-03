/*! 
 * \author Nicolas Van der Noot
 * \file Body3D.hh
 * \brief Body3D class
 */
#ifdef OPEN_GL

#ifndef _BODY_3D_HH_
#define _BODY_3D_HH_

#include "Mbs3DComponent.hh"
#include "MbsShape3D.hh"

namespace OpenGLMbs{

// forward declaration
class AnchorPoint3D;

/*! \brief body 3D component
 */
class Body3D: public Mbs3DComponent
{
    public:
        Body3D(MbsWorld3D *world_3d, const char* name, glm::mat4 const& abs_mat = glm::mat4(1.0f));
        virtual ~Body3D();

        /// add a 3D shape
        void AddMbsShape3D(MbsShape3D *new_shape_3d) { shape_3d_list.push_back(new_shape_3d); }

        void UpdateShapes3DMat();

        AnchorPoint3D* AddAnchor(const char* name, glm::vec3 const& anchor_pos = glm::vec3(0.0f));

        virtual void UpdateAbsMatTree();

        virtual void PrintTree(int level);

    private:
        std::vector<AnchorPoint3D*> anchor_child_list; ///< list of anchor point childs

        std::vector<MbsShape3D*> shape_3d_list; ///< list of 3D shapes childs
};

}
#endif
#endif
