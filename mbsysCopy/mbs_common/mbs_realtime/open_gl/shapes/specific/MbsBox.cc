#include "MbsBox.hh"

namespace OpenGLMbs{

#define NB_VERTICES_INIT 8   ///< initial number of vertices
#define NB_TRIANGLES_INIT 12 ///< initial number of triangles

// temporary vertices
GLfloat tmp_vert_box[3*NB_VERTICES_INIT] = {
    -1.0, -1.0,  1.0,
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0,  1.0
};

// temporary coordinates
GLfloat tmp_coord_box[3*NB_TRIANGLES_INIT] = {
    5, 6, 1,
    6, 7, 2,
    7, 8, 3,
    8, 5, 4,
    1, 2, 4,
    8, 7, 5,
    6, 2, 1,
    7, 3, 2,
    8, 4, 3,
    5, 1, 4,
    2, 3, 4,
    7, 6, 5
};

/*! \brief constructor
 *
 * \param[in] d depth (along X) [m]
 * \param[in] w width (along Y) [m]
 * \param[in] h height (along Z) [m]
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
MbsBox::MbsBox(double d, double w, double h, glm::vec3 const& color,
    std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
    float transparency, glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot,
    const char* name):
    MbsShape3D(vertex_file, fragment_file, world_3d, transparency, trans_pos, trans_scale, trans_rot, name)
{
    double semi_d, semi_h, semi_w;

    this->d = d;
    this->w = w;
    this->h = h;

    semi_d = d / 2.0;
    semi_w = w / 2.0;
    semi_h = h / 2.0;

    BasicLoad(tmp_vert_box, tmp_coord_box, NB_TRIANGLES_INIT, NB_VERTICES_INIT, color);

    // apply depth, width, height
    for(unsigned int i=0; i<temp_vertices.size(); i++)
    {
        temp_vertices[i] = glm::vec3(semi_d * temp_vertices[i][0], semi_w * temp_vertices[i][1], semi_h * temp_vertices[i][2]);
    }

    // set number of triangles
    SetNbTriangles(NB_TRIANGLES_INIT);

    // initialize the shape
    Init();
}

/*! \brief destructor
 */
MbsBox::~MbsBox()
{

}

}
