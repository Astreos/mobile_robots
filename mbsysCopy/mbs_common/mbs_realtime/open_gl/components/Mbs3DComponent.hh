/*! 
 * \author Nicolas Van der Noot
 * \file Mbs3DComponent.hh
 * \brief Mbs3DComponent class
 */
#ifdef OPEN_GL

#ifndef _MBS_3D_COMPONENT_HH_
#define _MBS_3D_COMPONENT_HH_

#ifndef degreesToRadians
    #define degreesToRadians(x) x*(3.141592f/180.0f)
#endif

#ifndef GLM_FORCE_RADIANS
    #define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>
#include <vector>

namespace OpenGLMbs{

// forward declaration
class MbsWorld3D;
class Joint3D;
class Base3D;

// mbs component types
enum {COMP_TYPE_COMPONENT, COMP_TYPE_BODY, COMP_TYPE_ABSTRACT, COMP_TYPE_ANCHOR, COMP_TYPE_JOINT, COMP_TYPE_BASE};

/*! \brief mother class of all 3D components
 */
class Mbs3DComponent
{
    public:
        Mbs3DComponent(MbsWorld3D *world_3d, const char* name, glm::mat4 const& abs_mat = glm::mat4(1.0f));
        virtual ~Mbs3DComponent();

        /// set the absolute matrix
        void SetAbsMat(glm::mat4 const& abs_mat) { this->abs_mat = abs_mat; }

        /// get the absolute matrix
        glm::mat4 GetAbsMat() const { return abs_mat; }

        /// get the component type
        int GetCompType() const { return comp_type; }

        void ReleaseChildList();

        Joint3D* AddT1(const char* name, float joint_value = 0.0f);
        Joint3D* AddT2(const char* name, float joint_value = 0.0f);
        Joint3D* AddT3(const char* name, float joint_value = 0.0f);
        Joint3D* AddR1(const char* name, float joint_value = 0.0f);
        Joint3D* AddR2(const char* name, float joint_value = 0.0f);
        Joint3D* AddR3(const char* name, float joint_value = 0.0f);

        Base3D* AddBaseJoint();

        bool IsName(const char* name);

        Mbs3DComponent* Get3DCompName(int comp_type, const char* name);

        virtual void UpdateAbsMatTree();

        virtual void PrintTree(int level);

    protected:
        MbsWorld3D *world_3d; ///< 3D world

        glm::mat4 abs_mat; ///< absolute position matrix

        std::vector<Joint3D*> joint_child_list; ///< list of child joints

        std::vector<Mbs3DComponent*> mbs_3d_child_list; ///< list of all 3D components (joints included)

        char* name; ///< name of the component

        int comp_type; ///< component type

        // function prototypes
        Joint3D* AddJoint(const char* name, int joint_type, float joint_value);
        void PrintName(int level);
        void PrintChildList(int level);

        /// get the translational matrix to move of a vector (x;y;z)
        glm::mat4 TransMat(float x, float y, float z)
        {
            return glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                   x,    y,    z, 1.0f
                );
        }

        /// get the scaling matrix to scale of a vector (x;y;z)
        glm::mat4 ScaleMat(float x, float y, float z)
        {
            return glm::mat4(
                 x  , 0.0f, 0.0f, 0.0f,
                0.0f,  y  , 0.0f, 0.0f,
                0.0f, 0.0f,  z  , 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
                );
        }

        /// get the rotating matrix to rotate along the X value of an angle 'angle_rad' [rad]
        glm::mat4 RotMatX(float angle_rad)
        {
            float cos_a, sin_a;

            cos_a = cos(angle_rad);
            sin_a = sin(angle_rad);

            return glm::mat4(
                1.0f,  0.0f , 0.0f , 0.0f,
                0.0f,  cos_a, sin_a, 0.0f,
                0.0f, -sin_a, cos_a, 0.0f,
                0.0f,  0.0f , 0.0f , 1.0f
                );
        }

        /// get the rotating matrix to rotate along the Y value of an angle 'angle_rad' [rad]
        glm::mat4 RotMatY(float angle_rad)
        {
            float cos_a, sin_a;

            cos_a = cos(angle_rad);
            sin_a = sin(angle_rad);

            return glm::mat4(
                cos_a, 0.0f, -sin_a, 0.0f,
                0.0f , 1.0f,  0.0f , 0.0f,
                sin_a, 0.0f,  cos_a, 0.0f,
                0.0f , 0.0f,  0.0f , 1.0f
                );
        }

        /// get the rotating matrix to rotate along the Z value of an angle 'angle_rad' [rad]
        glm::mat4 RotMatZ(float angle_rad)
        {
            float cos_a, sin_a;

            cos_a = cos(angle_rad);
            sin_a = sin(angle_rad);

            return glm::mat4(
                 cos_a, sin_a, 0.0f, 0.0f,
                -sin_a, cos_a, 0.0f, 0.0f,
                 0.0f , 0.0f , 1.0f, 0.0f,
                 0.0f , 0.0f , 0.0f, 1.0f
                );
        }
};

}

#endif
#endif
