/*! 
 * \author Nicolas Van der Noot
 * \file Shader.hh
 * \brief Shader class
 */
#ifdef OPEN_GL

#ifndef _SHADER_HH_
#define _SHADER_HH_

#include <string>
#include <glew.h>

namespace OpenGLMbs{

/*! \brief shader: compile and link shader files
 */
class Shader
{
    public:
        Shader(std::string vertex_file, std::string fragment_file);
        Shader(Shader const &copy_shader);
        ~Shader();

        /// get the program ID
        GLuint ProgramID() const { return program_id; }

        /// get the vertex filw
        std::string VertexFile()   const { return vertex_file;   }

        /// get the fragment file
        std::string FragmentFile() const { return fragment_file; }

    private:
        GLuint vertex_id;   ///< vertex ID
        GLuint fragment_id; ///< fragment ID
        GLuint program_id;  ///< program ID

        std::string vertex_file;   ///< vertex shader file to process
        std::string fragment_file; ///< fragment shader file to process

        // function prototype
        void CompileLink();
        void Compile(GLuint &shader, GLenum type, std::string const &fichierSource);
};

}
#endif
#endif
