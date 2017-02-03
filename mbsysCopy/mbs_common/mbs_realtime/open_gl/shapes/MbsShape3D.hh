/*! 
 * \author Nicolas Van der Noot
 * \file MbsShape3D.hh
 * \brief MbsShape3D class
 */
#ifdef OPEN_GL

#ifndef _MBS_SHAPE_3D_HH_
#define _MBS_SHAPE_3D_HH_

#ifndef degreesToRadians
    #define degreesToRadians(x) x*(3.141592f/180.0f)
#endif

#ifndef GLM_FORCE_RADIANS
    #define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>
#include <string>
#include <vector>

#include "Shader.hh"

namespace OpenGLMbs{

// forward declaration
class MbsWorld3D;
class MbsLight;

/*! \brief shader light class
 */
class ShaderLight
{
    public:
        ShaderLight()
        {
            Clear();
        }

        glm::vec4 pos;         ///< light position
        glm::vec3 color;       ///< light color
        glm::vec3 attenuation; ///< attenuation
        float ambientCoef;     ///< ambient coefficient

        // specific to cones
        float coneCosAngle; ///< cone opening angle
        glm::vec3 coneDir;  ///< cone direction

        MbsLight *light; ///< pointer to the related 'MbsLight' component

        /// clear the current light shader
        void Clear()
        {
            pos         = glm::vec4(0.0f);
            color   = glm::vec3(0.0f);
            coneDir     = glm::vec3(0.0f);
            attenuation = glm::vec3(0.0f);

            ambientCoef  = 0.0f;
            coneCosAngle = 0.0f;

            light = NULL;
        }
};

/*! \brief 3D shape element
 */
class MbsShape3D
{
    public:
        MbsShape3D(std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");
        virtual ~MbsShape3D();

        /// set the model matrix
        void SetModelMat(glm::mat4 const& model_mat)
        {
            this->model_mat = model_mat;

            M_inv_trans = glm::transpose(glm::inverse(glm::mat3(model_mat)));
        }

        void Update();

        /// set the shiny material value
        void SetShinyMat(float val) { shiny_mat = val; }

        /// set the specular material value
        void SetSpecularMat(glm::vec3 const& vec) { specular_mat = vec; }

    protected:
        char* name; ///< name of the shape

        Shader shader; ///< shader

        std::vector<glm::vec3> temp_vertices; ///< temporary vertices
        std::vector<glm::vec3> temp_colors;   ///< temporary colors
        std::vector<glm::vec3> temp_normals;  ///< temporary normals

        std::vector<glm::vec3> vertices; ///< vertices
        std::vector<glm::vec3> colors;   ///< colors
        std::vector<glm::vec3> normals;  ///< normals

        std::vector<unsigned short> indexes; ///< indexes (for VBO indexing)

        glm::mat4 proj_mat;    ///< projection matrix
        glm::mat4 view_mat;    ///< view matrix
        glm::mat4 model_mat;   ///< model matrix
        glm::mat4 MVP;         ///< model-view-projection matrix
        glm::mat3 M_inv_trans; ///< transpose and inverse of the model matrix

        glm::vec3 trans_pos;   ///< transformation: position
        glm::vec3 trans_scale; ///< transformation: scaling
        glm::vec3 trans_rot;   ///< transformation: rotation

        MbsWorld3D *world_3d; ///< 3D world

        float transparency; ///< transparancy

        float shiny_mat;        ///< shiny material
        glm::vec3 specular_mat; ///< specular material

        std::vector<ShaderLight> sh_lights; ///< shaders for lights

        bool light_init; ///< flag to know if the ligth is initialized

        int nb_triangles; ///< number of triangles
        int nb_vertices;  ///< number of vertices

        GLuint VAO_id; ///< VAO
        GLuint VBO_vertex_id;  ///< VBO for vertices
        GLuint VBO_color_id;   ///< VBO for colors
        GLuint VBO_normal_id;  ///< VBO for normals
        GLuint VBO_indexes_id; ///< VBO for indexes

        GLuint transID;     ///< ID of the transparency
        GLuint nbLightsID;  ///< ID of the number of lights
        GLuint cameraID;    ///< ID of the camera
        GLuint shininessID; ///< ID of the shininess (shape material)
        GLuint specularID;  ///< ID of the specular value (shape material)

        std::vector<GLuint> lightPosID;    ///< ID of the light position
        std::vector<GLuint> lightColID;    ///< ID of the light color
        std::vector<GLuint> lightAttID;    ///< ID of the light attenuation
        std::vector<GLuint> lightAmbID;    ///< ID of the light ambiance
        std::vector<GLuint> lightConAngID; ///< ID of the light cone angle (for spots)
        std::vector<GLuint> lightConDirID; ///< ID of the light cone direction (for spots)

        GLuint mvpID;       ///< ID of the MVP matrix
        GLuint modelMatID;  ///< ID of the model matrix
        GLuint MInvTransID; ///< ID of transpose and inverse of the model matrix

        // function prototypes
        void Init();
        void SetNbTriangles(int nb_triangles);
        void NormalsCompute();
        void ApplyTransform();
        void VBOIndexing();
        void BasicLoad(GLfloat *tmp_vert, GLfloat *tmp_coord, int nb_tri, int nb_vert, glm::vec3 const& color);

        void LightsInit();
        void LightsUpdate();
        std::string LightStringLoc(const char* propertyName, size_t lightIndex);

        void ColorHeight(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> &colors,
            int axis, bool inv_color, bool bound_flag, float min_range, float max_range, const char* name);

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
