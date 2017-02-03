#ifdef OPEN_GL

#include "Shader.hh"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <glfw3.h>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] vertex_file vertex file name
 * \param[in] fragment_file fragment file name
 */
Shader::Shader(std::string vertex_file, std::string fragment_file)
{
    vertex_id   = 0;
    fragment_id = 0;
    program_id  = 0;

    this->vertex_file   = vertex_file;
    this->fragment_file = fragment_file;

    // compile and link
    CompileLink();
}

/*! \brief constructor
 * 
 * \param[in] copy_shader shader to copy
 */
Shader::Shader(Shader const &copy_shader)
{
    vertex_id   = 0;
    fragment_id = 0;
    program_id  = 0;

    vertex_file   = copy_shader.VertexFile();
    fragment_file = copy_shader.FragmentFile();

    // compile and link
    CompileLink();
}

/*! \brief destructor
 */
Shader::~Shader()
{
    glDeleteProgram(program_id);
}

/*! \brief compile the shaders and link to the graphic card
 */
void Shader::CompileLink()
{
    // variables declaration
    char *er_char;
    GLint er_link;
    GLint er_size;

    // safety (avoid memory loss)
    if(glIsShader(vertex_id) == GL_TRUE)
    {
        glDeleteShader(vertex_id);
    }

    if(glIsShader(fragment_id) == GL_TRUE)
    {
        glDeleteShader(fragment_id);
    }

    if(glIsProgram(program_id) == GL_TRUE)
    {
        glDeleteProgram(program_id);
    }

    // create program
    program_id = glCreateProgram();

    // compile shaders
    Compile(vertex_id  , GL_VERTEX_SHADER  , vertex_file  );
    Compile(fragment_id, GL_FRAGMENT_SHADER, fragment_file);

    // attach shaders
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);

    // lock inputs
    glBindAttribLocation(program_id, 0, "in_Vertex");
    glBindAttribLocation(program_id, 1, "in_Color");
    glBindAttribLocation(program_id, 2, "in_Normal");

    // program linking
    glLinkProgram(program_id);

    // linking check
    glGetProgramiv(program_id, GL_LINK_STATUS, &er_link);

    if(er_link != GL_TRUE)
    {
        // error size
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &er_size);

        // get error
        er_char = new char[er_size + 1];

        glGetShaderInfoLog(program_id, er_size, &er_size, er_char);

        er_char[er_size] = '\0';

        // print error
        std::cout << "------- Shader linking error -------\n" << er_char << std::endl;

        // release memory
        delete[] er_char;
        glDeleteProgram(program_id);

        exit(EXIT_FAILURE);
    }

    glDetachShader(program_id, vertex_id);
    glDetachShader(program_id, fragment_id);
    
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}

/*! \brief shader compilation
 * 
 * \param[out] shader_id shader ID
 * \param[in] type shader type
 * \param[in] file_in input file to read
 */
void Shader::Compile(GLuint &shader_id, GLenum type, std::string const &file_in)
{
    // variables declaration
    char *er_char;
    GLint compile_er;
    GLint er_size;

    std::string cur_line;
    std::string tot_file;

    // shader creation
    shader_id = glCreateShader(type);

    if(shader_id == 0)
    {
        std::cout << "Shader error: " << type << " is not a correct type !" << std::endl;
        exit(EXIT_FAILURE);
    }


    // file reading
    std::ifstream f_in(file_in.c_str());
    
    if(!f_in)
    {
        std::cout << "Shader error: " << file_in << " cannot be open !" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(getline(f_in, cur_line))
    {
        tot_file += cur_line + "\n";
    }

    f_in.close();

    // send input to shader
    const GLchar* gl_char = tot_file.c_str();

    glShaderSource(shader_id, 1, &gl_char, 0);

    // shader compilation
    glCompileShader(shader_id);

    // compilation check
    compile_er = 30;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_er);

    if(compile_er != GL_TRUE)
    {
        // error size
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &er_size);

        // get error
        er_char = new char[er_size+1];

        glGetShaderInfoLog(shader_id, er_size, &er_size, er_char);
        er_char[er_size] = '\0';

        // print error
        std::cout << "------- Shader compilation error -------\n" << er_char << std::endl;

        // release memory
        delete[] er_char;
        glDeleteShader(shader_id);

        exit(EXIT_FAILURE);
    }
}

}
#endif
