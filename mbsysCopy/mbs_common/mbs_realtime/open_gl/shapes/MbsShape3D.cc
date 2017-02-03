#ifdef OPEN_GL

#include "MbsShape3D.hh"
#include "MbsWorld3D.hh"

#include "MbsLight.hh"
#include "MbsPointLight.hh"
#include "MbsSpotLight.hh"
#include "MbsDirLight.hh"

#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/color_space.hpp>

#include <iostream>
#include <sstream>

#include <map>

namespace OpenGLMbs{

#define MAX_NB_LIGHTS 10 ///< maximum number of lights

/// check if integer is in range
inline bool int_in_range(int x, int min, int max) { return (x < min) ? false : (x > max ? false : true); }

/// limit x in the [min;max] range
inline double limit_range(double x, double min, double max) { return (x < min) ? min : (x > max ? max : x); }

// vertex attributes
struct VertexAttr
{
    glm::vec3 position; ///< position
    glm::vec3 color;    ///< color
    glm::vec3 normal;   ///< normal

    bool operator<(const VertexAttr that) const
    {
        return memcmp((void*)this, (void*)&that, sizeof(VertexAttr))>0;
    };
};

/*! \brief check if two vertices are similar enough
 * 
 * \param[in] vertex_attr new vertex with attribute
 * \param[in] map_vertex_attr map with the already known vertices (with attributes)
 * \param[out] result result where ID of the already known vertex is stored if similar
 * \return true if similar, false otherwise
 */
bool SimilarVertexID(VertexAttr const& vertex_attr, std::map<VertexAttr, unsigned short> & map_vertex_attr,
    unsigned short &result)
{
    std::map<VertexAttr, unsigned short>::iterator it = map_vertex_attr.find(vertex_attr);

    if ( it == map_vertex_attr.end() )
    {
        return false;
    }
    else
    {
        result = it->second;
        return true;
    }
}

/*! \brief constructor
 *
 * \param[in] vertex_file vertex shader file to load (with path)
 * \param[in] fragment_file fragment shader file to load (with path)
 * \param[in] world_3d 3D world
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 */
MbsShape3D::MbsShape3D(std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
    float transparency, glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot,
    const char* name):
    shader(vertex_file, fragment_file)
{
    this->transparency = transparency;

    this->trans_pos   = trans_pos;
    this->trans_scale = trans_scale;
    this->trans_rot   = trans_rot;

    this->world_3d = world_3d;

    this->name = strdup(name);

    VAO_id = 0;
    VBO_vertex_id  = 0;
    VBO_color_id   = 0;
    VBO_normal_id  = 0;
    VBO_indexes_id = 0;

    proj_mat  = glm::mat4(1.0f);
    view_mat  = glm::mat4(1.0f);
    model_mat = glm::mat4(1.0f);
    MVP = glm::mat4(1.0f);
    M_inv_trans = glm::mat3(1.0f);

    light_init = false;
}

/*! \brief destructor
 */
MbsShape3D::~MbsShape3D()
{
    if (name != NULL)
    {
        free(name);
    }

    // clean VBOs
    glDeleteBuffers(1, &VBO_indexes_id);
    glDeleteBuffers(1, &VBO_normal_id);
    glDeleteBuffers(1, &VBO_color_id);
    glDeleteBuffers(1, &VBO_vertex_id);

    // clean VAO
    glDeleteVertexArrays(1, &VAO_id);

    // clean shader
    glDeleteProgram(shader.ProgramID());
}

/*! \brief basic loading with temporary vertices and coordinates
 * 
 * \param[in] tmp_vert temporary vertices
 * \param[in] tmp_coord temporary coordinates
 * \param[in] nb_tri number of triangles
 * \param[in] nb_vert number of vertices
 * \param[in] color color to apply
 */
void MbsShape3D::BasicLoad(GLfloat *tmp_vert, GLfloat *tmp_coord, int nb_tri, int nb_vert,
    glm::vec3 const& color)
{
    for(int i=0; i<nb_tri; i++)
    {
        int i0, i1, i2;

        i0 = tmp_coord[3*i]-1;
        i1 = tmp_coord[3*i+1]-1;
        i2 = tmp_coord[3*i+2]-1;

        if (!int_in_range(i0, 0, nb_vert-1) ||
            !int_in_range(i1, 0, nb_vert-1) ||
            !int_in_range(i2, 0, nb_vert-1))
        {
            std::cout << "Basic shape loading error: coordinate vertex error." << std::endl;
            exit(EXIT_FAILURE);
        }

        temp_vertices.push_back(glm::vec3(tmp_vert[3*i0], tmp_vert[3*i0+1], tmp_vert[3*i0+2]));
        temp_vertices.push_back(glm::vec3(tmp_vert[3*i1], tmp_vert[3*i1+1], tmp_vert[3*i1+2]));
        temp_vertices.push_back(glm::vec3(tmp_vert[3*i2], tmp_vert[3*i2+1], tmp_vert[3*i2+2]));

        for(int i=0; i<3; i++)
        {
            temp_colors.push_back(color);
        }
    }
}

/*! \brief set the number of vertices and triangles
 * 
 * \param[in] nb_triangles number of triangles
 */
void MbsShape3D::SetNbTriangles(int nb_triangles)
{
    this->nb_triangles = nb_triangles;

    nb_vertices = 3 * nb_triangles;
}

/*! \brief apply the transformations (position scaling, rotation) on the shape
 */
void MbsShape3D::ApplyTransform()
{
    glm::vec4 cur_vertex;
    
    glm::mat4 modif_mat =
        TransMat(trans_pos[0], trans_pos[1], trans_pos[2])
        * ScaleMat(trans_scale[0], trans_scale[1], trans_scale[2])
        * RotMatX(trans_rot[0])
        * RotMatY(trans_rot[1])
        * RotMatZ(trans_rot[2]);

    for(unsigned int i=0; i<temp_vertices.size(); i++)
    {
        cur_vertex = modif_mat * glm::vec4(temp_vertices[i], 1.0);

        temp_vertices[i] = glm::vec3(cur_vertex);
    }
}

/*! \brief compute the normals
 */
void MbsShape3D::NormalsCompute()
{
    glm::vec3 cur_normal;

    for(int i=0; i<nb_triangles; i++)
    {
        cur_normal = glm::normalize(glm::cross(temp_vertices[3*i]-temp_vertices[3*i+1], temp_vertices[3*i]-temp_vertices[3*i+2]));
        
        for(int i=0; i<3; i++)
        {
            temp_normals.push_back(cur_normal);
        }
    }
}

/*! \brief advanced user function to change the color of a shape, according to the vertices positions along an axis
 * 
 * \param[in] vertices vertices positions
 * \param[out] colors updated colors
 * \param[in] axis axis along which the grading is computed (0:X ; 1:Y ; 2:Z)
 * \param[in] inv_color true to invert the color grading direction, false otherwise
 * \param[in] bound_flag true to bound in the [min_range;max_range] range, false otherwise
 * \param[in] min_range minimal range value (only used with 'bound_flag' set to true)
 * \param[in] max_range maximal range value (only used with 'bound_flag' set to true)
 * \param[in] name name of the shape to apply this modification
 *
 * This function is normally not called, except for an advanced user, who wants to temporarily modify
 * the generic Robotran OpenGL source files (in fact, this file).
 * This function can, for instance, be called in the 'VBOIndexing()' function, after the first
 * safety check.
 */
void MbsShape3D::ColorHeight(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> &colors,
    int axis, bool inv_color, bool bound_flag, float min_range, float max_range, const char* name)
{
    int vert_size;
    float cur_val, min_val, max_val, diff_val;

    // check if correct name
    if (strcmp(this->name, name))
    {
        return;
    }

    // number of vertices
    vert_size = vertices.size();

    // safety
    if (vert_size != colors.size())
    {
        std::cout << "MbsShape3D error in sizes: vertices (" << vert_size
            << ") ; colors (" << colors.size() << ")." << std::endl;
            exit(EXIT_FAILURE);
    }

    // get the minimal and maximal values along the specified axis
    min_val = bound_flag ? limit_range(vertices[0][axis], min_range, max_range) : vertices[0][axis];
    max_val = min_val;

    for(int i=1; i<vert_size; i++)
    {
        cur_val = bound_flag ? limit_range(vertices[i][axis], min_range, max_range) : vertices[i][axis];

        if (cur_val < min_val)
        {
            min_val = cur_val;
        }

        if (cur_val > max_val)
        {
            max_val = cur_val;
        }
    }

    // range between maximal and minimal values
    diff_val = max_val - min_val;

    if (diff_val <= 0.0)
    {
        return;
    }

    // apply colors depending on the values along the axis
    for(int i=0; i<vert_size; i++)
    {
        cur_val = bound_flag ? limit_range(vertices[i][axis], min_range, max_range) : vertices[i][axis];

        cur_val = (cur_val - min_val) / diff_val;

        cur_val = inv_color ? (1.0 - cur_val) * 230.0f : cur_val * 230.0f;

        colors[i] = glm::rgbColor(glm::vec3(cur_val, 1.0f, 1.0f));
    }
}

/*! \brief apply VBO indexing to reduce the number of vertices, normals, colors
 */
void MbsShape3D::VBOIndexing()
{
    bool found;
    int vert_size;
    unsigned short index, newindex;
    std::map<VertexAttr, unsigned short> map_vertex_attr;

    vert_size = temp_vertices.size();

    // safety
    if (vert_size != temp_colors.size() || vert_size != temp_normals.size())
    {
        std::cout << "MbsShape3D error in sizes: temp_vertices (" << vert_size
            << ") ; temp_colors (" << temp_colors.size()
            << ") ; temp_normals (" << temp_normals.size() << ")." << std::endl;
            exit(EXIT_FAILURE);
    }

    // loop on each input vertex
    for (unsigned int i=0; i<temp_vertices.size(); i++)
    {
        // fill the current vertex attributes
        VertexAttr vertex_attr = {temp_vertices[i], temp_colors[i], temp_normals[i]};
        
        // try to find a similar vertex
        found = SimilarVertexID( vertex_attr, map_vertex_attr, index);

        if ( found ) // similar vertex found
        { 
            indexes.push_back( index );
        }
        else // new vertex
        {
            vertices.push_back( temp_vertices[i]);
            colors.push_back( temp_colors[i]);
            normals.push_back( temp_normals[i]);

            newindex = (unsigned short) vertices.size() - 1;

            indexes.push_back(newindex);
            map_vertex_attr[vertex_attr] = newindex;
        }
    }
}

/*! \brief initialize the shape
 */
void MbsShape3D::Init()
{
    // apply transformations
    ApplyTransform();

    // normals computation
    NormalsCompute();

    // VBO indexing
    VBOIndexing();

    glUseProgram(shader.ProgramID());

        // get a handle for our "MVP" uniform
        mvpID       = glGetUniformLocation(shader.ProgramID(), "MVP");
        modelMatID  = glGetUniformLocation(shader.ProgramID(), "M");
        MInvTransID = glGetUniformLocation(shader.ProgramID(), "M_inv_trans");

        // generate one buffer for VBO_vertex_id
        if(glIsBuffer(VBO_vertex_id) == GL_TRUE)
        {
            glDeleteBuffers(1, &VBO_vertex_id);
        }

        glGenBuffers(1, &VBO_vertex_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW); // give vertices to OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate one buffer for VBO_color_id
        if(glIsBuffer(VBO_color_id) == GL_TRUE)
        {
            glDeleteBuffers(1, &VBO_color_id);
        }

        glGenBuffers(1, &VBO_color_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_color_id);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate one buffer for VBO_normal_id
        if(glIsBuffer(VBO_normal_id) == GL_TRUE)
        {
            glDeleteBuffers(1, &VBO_normal_id);
        }

        glGenBuffers(1, &VBO_normal_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_id);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate one buffer for VBO_indexes_id
        if(glIsBuffer(VBO_indexes_id) == GL_TRUE)
        {
            glDeleteBuffers(1, &VBO_indexes_id);
        }

        glGenBuffers(1, &VBO_indexes_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indexes_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned short), &indexes[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     
        // VAO creation
        if(glIsVertexArray(VAO_id) == GL_TRUE)
        {
            glDeleteVertexArrays(1, &VAO_id);
        }

        glGenVertexArrays(1, &VAO_id);
        glBindVertexArray(VAO_id);

            // vertices
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_id);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
                glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // colors
            glBindBuffer(GL_ARRAY_BUFFER, VBO_color_id);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
                glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // normals
            glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_id);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
                glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

    glUseProgram(0);
}

/*! \brief generate a string to get the location of a light value (to communicate with fragment shader)
 * 
 * \param[in] propertyName name of the property
 * \param[in] lightIndex light ID
 * \return requested string
 */
std::string MbsShape3D::LightStringLoc(const char* propertyName, size_t lightIndex)
{
    std::ostringstream ss;
    ss << "sh_lights[" << lightIndex << "]." << propertyName;

    return ss.str();
}

/*! \brief initalize the lights
 */
void MbsShape3D::LightsInit()
{
    // variables declaration
    MbsLight *cur_ligt;
    MbsPointLight *cur_point_light;
    MbsSpotLight *cur_spot_light;
    ShaderLight cur_sh_light;

    // specular material properies
    shiny_mat    = 250.f;
    specular_mat = glm::vec3(1.0f, 1.0f, 1.0f);

    // safety
    if (world_3d->GetNbLights() > MAX_NB_LIGHTS)
    {
        std::cout << "MbsWorld3D error: only " << MAX_NB_LIGHTS << " lights should be added (" << world_3d->GetNbLights() << " received)." << std::endl;
        exit(EXIT_FAILURE);
    }

    // loop on all the lights
    for(unsigned int i=0; i<world_3d->GetNbLights(); i++)
    {
        cur_sh_light.Clear();

        cur_ligt = world_3d->GetLight(i);

        // go to next light if current light is not active
        if (!cur_ligt->IsActive())
        {
            continue;
        }

        // check the type of light
        switch (cur_ligt->GetType())
        {
            // point light
            case POINT_LIGHT :
                cur_point_light = static_cast<MbsPointLight*>(cur_ligt);

                cur_sh_light.ambientCoef = cur_ligt->GetAmbient();
                cur_sh_light.color       = cur_ligt->GetColor();

                cur_sh_light.pos         = glm::vec4(cur_point_light->GetPos(), 1.0);
                cur_sh_light.attenuation = cur_point_light->GetAttenuation();

                break;

            // spot light
            case SPOT_LIGHT :
                cur_spot_light = static_cast<MbsSpotLight*>(cur_ligt);

                cur_sh_light.ambientCoef = cur_ligt->GetAmbient();
                cur_sh_light.color       = cur_ligt->GetColor();

                cur_sh_light.pos          = glm::vec4(cur_spot_light->GetPos(), 1.0);
                cur_sh_light.attenuation  = cur_spot_light->GetAttenuation();
                cur_sh_light.coneCosAngle = cos(cur_spot_light->GetSpreadAngle());
                cur_sh_light.coneDir      = cur_spot_light->GetNormDir();
                break;

            // directional light
            case DIR_LIGHT :
                cur_sh_light.ambientCoef = cur_ligt->GetAmbient();
                cur_sh_light.color       = cur_ligt->GetColor();

                cur_sh_light.pos = glm::vec4(-static_cast<MbsDirLight*>(cur_ligt)->GetNormDir(), 0.0);
                break;
        
            default:
                std::cout << "MbsShape3D error: unknown light type (" << cur_ligt->GetType() << ") !" << std::endl;
                exit(EXIT_FAILURE);
        }

        // add new light
        sh_lights.push_back(cur_sh_light);
    }

    // safety
    if (sh_lights.size() > MAX_NB_LIGHTS)
    {
        std::cout << "MbsWorld3D error: only " << MAX_NB_LIGHTS << " shader lights should be added (" << sh_lights.size() << " received)." << std::endl;
        exit(EXIT_FAILURE);
    }

    // get gl locations
    glUseProgram(shader.ProgramID());

        // get attributes locations
        transID     = glGetUniformLocation(shader.ProgramID(), "transparency" );
        nbLightsID  = glGetUniformLocation(shader.ProgramID(), "nbLights" );
        cameraID    = glGetUniformLocation(shader.ProgramID(), "camPos"   );
        shininessID = glGetUniformLocation(shader.ProgramID(), "shiny"    );
        specularID  = glGetUniformLocation(shader.ProgramID(), "specColor");

        // loop on all the lights
        for(unsigned int i=0; i<sh_lights.size(); i++)
        {
            // get attributes locations
            lightPosID.push_back(glGetUniformLocation(shader.ProgramID(),    LightStringLoc("pos",          i).c_str()));
            lightColID.push_back(glGetUniformLocation(shader.ProgramID(),    LightStringLoc("color",    i).c_str()));
            lightAttID.push_back(glGetUniformLocation(shader.ProgramID(),    LightStringLoc("attenuation",  i).c_str()));
            lightAmbID.push_back(glGetUniformLocation(shader.ProgramID(),    LightStringLoc("ambientCoef",  i).c_str()));
            lightConAngID.push_back(glGetUniformLocation(shader.ProgramID(), LightStringLoc("coneCosAngle", i).c_str()));
            lightConDirID.push_back(glGetUniformLocation(shader.ProgramID(), LightStringLoc("coneDir",      i).c_str()));
        }

        glBindVertexArray(VAO_id);

            // transparency
            glUniform1f( transID , transparency );

            // lights
            glUniform1i(  nbLightsID  , sh_lights.size() );
            glUniform1f(  shininessID , shiny_mat );
            glUniform3fv( specularID  , 1, &(specular_mat[0]) );

            for(unsigned int i=0; i<sh_lights.size(); i++)
            {           
                glUniform3fv( lightColID[i]    , 1, &(sh_lights[i].color[0])   );
                glUniform3fv( lightAttID[i]    , 1, &(sh_lights[i].attenuation[0]) );
                glUniform1f(  lightAmbID[i]    ,      sh_lights[i].ambientCoef     );
                glUniform1f(  lightConAngID[i] ,      sh_lights[i].coneCosAngle    );
            }

            // draw the triangles
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indexes_id);

                glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, (void*) 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

    glUseProgram(0);
}

/*! \brief update the lights
 */
void MbsShape3D::LightsUpdate()
{
    // variables declaration
    int nb_lights, count;
    MbsLight *cur_ligt;
    MbsSpotLight *cur_spot_light;

    nb_lights = world_3d->GetNbLights();

    // count to get the current light
    count = -1;

    // safety
    for(unsigned int i=0; i<nb_lights; i++)
    {
        cur_ligt = world_3d->GetLight(i);

        // go to next light if current light is not active
        if (!cur_ligt->IsActive())
        {
            continue;
        }
        else
        {
            count++;
        }

        // check the type of light
        switch (cur_ligt->GetType())
        {
            // point light
            case POINT_LIGHT :
                sh_lights[count].pos = glm::vec4(static_cast<MbsPointLight*>(cur_ligt)->GetPos(), 1.0);
                break;

            // spot light
            case SPOT_LIGHT :
                cur_spot_light = static_cast<MbsSpotLight*>(cur_ligt);

                sh_lights[count].pos     = glm::vec4(cur_spot_light->GetPos(), 1.0);
                sh_lights[count].coneDir = cur_spot_light->GetNormDir();
                break;

            // directional light
            case DIR_LIGHT :
                sh_lights[count].pos = glm::vec4(-static_cast<MbsDirLight*>(cur_ligt)->GetNormDir(), 0.0);
                break;
        
            default:
                std::cout << "MbsShape3D error: unknown light type (" << cur_ligt->GetType() << ") !" << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    // safety
    if (sh_lights.size() != count + 1)
    {
        std::cout << "MbsShape3D error: mismatch in the number of lights: "
            << sh_lights.size() << " >< " << count + 1 << std::endl;
            exit(EXIT_FAILURE);
    }
}

/*! \brief update the shape visualization
 */
void MbsShape3D::Update()
{
    MbsViewPoint *cur_view;

    // first iteration: initialize the lights
    if (!light_init)
    {
        light_init = true;

        LightsInit();
    }

    // lights update
    LightsUpdate();

    // get the current viewpoint
    cur_view = world_3d->GetCurView();

    // MVP matrix
    proj_mat = cur_view->GetProjectionMatrix();
    view_mat = cur_view->GetViewMatrix();
    MVP = proj_mat * view_mat * model_mat;

    // screen resolution
    glViewport(0, 0, world_3d->GetXWidth(), world_3d->GetYWidth());

    // use shader
    glUseProgram(shader.ProgramID());

        glBindVertexArray(VAO_id);

            // send transformation to the currently bound shader
            glUniformMatrix4fv(mvpID      , 1, GL_FALSE, &MVP[0][0]        );
            glUniformMatrix4fv(modelMatID , 1, GL_FALSE, &model_mat[0][0]  );
            glUniformMatrix3fv(MInvTransID, 1, GL_FALSE, &M_inv_trans[0][0]);

            // lights
            glUniform3fv( cameraID, 1, &(cur_view->GetPosition()[0]) );

            for(unsigned int i=0; i<sh_lights.size(); i++)
            {           
                glUniform4fv( lightPosID[i]    , 1, &(sh_lights[i].pos[0])     );
                glUniform3fv( lightConDirID[i] , 1, &(sh_lights[i].coneDir[0]) );
            }

            // draw the triangles
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indexes_id);

                glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_SHORT, (void*) 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

    glUseProgram(0);
}

}
#endif
