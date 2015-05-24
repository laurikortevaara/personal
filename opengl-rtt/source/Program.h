//
//  Shader.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#ifndef __opengl_rtt__Program__
#define __opengl_rtt__Program__

#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>


class Program {

private:
    GLuint m_object;
    
    Program(const Program&);
    const Program& operator=(const Program&);

public:
    Program(const std::vector<Shader>& shaders);
    ~Program();
    
    
    GLuint object() const;
    
    void use() const;
    void unuse() const;
    bool isInUse() const;
    
    GLint attrib(const GLchar* attribName) const;
    GLint uniform(const GLchar* uniformName) const;
    
// Uniform setter macros begin
#define UNIFORM_SETTERS(TYPE) \
void setAttrib(const GLchar* attribName, TYPE v0); \
void setAttrib(const GLchar* attribName, TYPE v0, TYPE v1); \
void setAttrib(const GLchar* attribName, TYPE v0, TYPE v1, TYPE v2); \
void setAttrib(const GLchar* attribName, TYPE v0, TYPE v1, TYPE v2, TYPE v3); \
\
void setAttrib1v(const GLchar* attribName, const TYPE* v); \
void setAttrib2v(const GLchar* attribName, const TYPE* v); \
void setAttrib3v(const GLchar* attribName, const TYPE* v); \
void setAttrib4v(const GLchar* attribName, const TYPE* v); \
\
void setUniform(const GLchar* uniformName, TYPE v0); \
void setUniform(const GLchar* uniformName, TYPE v0, TYPE v1); \
void setUniform(const GLchar* uniformName, TYPE v0, TYPE v1, TYPE v2); \
void setUniform(const GLchar* uniformName, TYPE v0, TYPE v1, TYPE v2, TYPE v3); \
\
void setUniform1v(const GLchar* uniformName, const TYPE* v, GLsizei count=1); \
void setUniform2v(const GLchar* uniformName, const TYPE* v, GLsizei count=1); \
void setUniform3v(const GLchar* uniformName, const TYPE* v, GLsizei count=1); \
void setUniform4v(const GLchar* uniformName, const TYPE* v, GLsizei count=1); \

UNIFORM_SETTERS(GLfloat)
UNIFORM_SETTERS(GLdouble)
UNIFORM_SETTERS(GLint)
UNIFORM_SETTERS(GLuint)
// Uniform setter macros end
    
    void setUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::mat3& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::mat4& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::vec3& v);
    void setUniform(const GLchar* uniformName, const glm::vec4& v);
    
    GLboolean hasUniform( const GLchar* uniformName );
};
#endif