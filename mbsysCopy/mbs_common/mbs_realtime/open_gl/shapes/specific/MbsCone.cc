#include "MbsCone.hh"

namespace OpenGLMbs{

#define NB_VERTICES_INIT 33  ///< initial number of vertices
#define NB_TRIANGLES_INIT 62 ///< initial number of triangles

// temporary vertices
GLfloat tmp_vert_cone[3*NB_VERTICES_INIT] = {
     0.000000, -1.0, -1.000000,
     0.000000,  1.0,  0.000000,
     0.195090, -1.0, -0.980785,
     0.382683, -1.0, -0.923880,
     0.555570, -1.0, -0.831470,
     0.707107, -1.0, -0.707107,
     0.831470, -1.0, -0.555570,
     0.923880, -1.0, -0.382683,
     0.980785, -1.0, -0.195090,
     1.000000, -1.0, -0.000000,
     0.980785, -1.0,  0.195090,
     0.923880, -1.0,  0.382683,
     0.831470, -1.0,  0.555570,
     0.707107, -1.0,  0.707107,
     0.555570, -1.0,  0.831470,
     0.382683, -1.0,  0.923880,
     0.195090, -1.0,  0.980785,
    -0.000000, -1.0,  1.000000,
    -0.195091, -1.0,  0.980785,
    -0.382684, -1.0,  0.923879,
    -0.555571, -1.0,  0.831469,
    -0.707107, -1.0,  0.707106,
    -0.831470, -1.0,  0.555570,
    -0.923880, -1.0,  0.382683,
    -0.980785, -1.0,  0.195089,
    -1.000000, -1.0, -0.000001,
    -0.980785, -1.0, -0.195091,
    -0.923879, -1.0, -0.382684,
    -0.831469, -1.0, -0.555571,
    -0.707106, -1.0, -0.707108,
    -0.555569, -1.0, -0.831470,
    -0.382682, -1.0, -0.923880,
    -0.195089, -1.0, -0.980786
};

// temporary coordinates
GLfloat tmp_coord_cone[3*NB_TRIANGLES_INIT] = {
    32, 2, 33,
    1, 2, 3,
    31, 2, 32,
    30, 2, 31,
    29, 2, 30,
    28, 2, 29,
    27, 2, 28,
    26, 2, 27,
    25, 2, 26,
    24, 2, 25,
    23, 2, 24,
    22, 2, 23,
    21, 2, 22,
    20, 2, 21,
    19, 2, 20,
    18, 2, 19,
    17, 2, 18,
    16, 2, 17,
    15, 2, 16,
    14, 2, 15,
    13, 2, 14,
    12, 2, 13,
    11, 2, 12,
    10, 2, 11,
    9, 2, 10,
    8, 2, 9,
    7, 2, 8,
    6, 2, 7,
    5, 2, 6,
    4, 2, 5,
    33, 2, 1,
    3, 2, 4,
    1, 3, 33,
    3, 4, 33,
    4, 5, 33,
    5, 6, 33,
    6, 7, 33,
    7, 8, 33,
    8, 9, 33,
    9, 10, 33,
    10, 11, 33,
    11, 12, 33,
    12, 13, 33,
    13, 14, 33,
    28, 26, 27,
    14, 15, 16,
    33, 14, 16,
    19, 17, 18,
    19, 16, 17,
    33, 16, 19,
    22, 20, 21,
    22, 19, 20,
    33, 19, 22,
    25, 23, 24,
    32, 33, 22,
    28, 25, 26,
    28, 23, 25,
    31, 32, 22,
    30, 31, 22,
    29, 30, 22,
    28, 29, 23,
    29, 22, 23
};

/*! \brief constructor
 *
 * \param[in] r radius [m]
 * \param[in] h height [m]
 * \param[in] color color of the shape
 * \param[in] vertex_file vertex shader file to load (with path)
 * \param[in] fragment_file fragment shader file to load (with path)
 * \param[in] world_3d 3D world
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 */
MbsCone::MbsCone(double r, double h, glm::vec3 const& color,
    std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
    float transparency, glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot,
    const char* name):
    MbsShape3D(vertex_file, fragment_file, world_3d, transparency, trans_pos, trans_scale, trans_rot, name)
{
    double semi_h;

    this->r = r;
    this->h = h;

    semi_h = h / 2.0;

    BasicLoad(tmp_vert_cone, tmp_coord_cone, NB_TRIANGLES_INIT, NB_VERTICES_INIT, color);

    // apply radius, height
    for(unsigned int i=0; i<temp_vertices.size(); i++)
    {
        temp_vertices[i] = glm::vec3(r * temp_vertices[i][0], semi_h * temp_vertices[i][1], r * temp_vertices[i][2]);
    }

    // set number of triangles
    SetNbTriangles(NB_TRIANGLES_INIT);

    // initialize the shape
    Init();
}

/*! \brief destructor
 */
MbsCone::~MbsCone()
{

}

}
