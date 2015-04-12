//
//  Shader.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#ifndef __opengl_rtt__Shader__
#define __opengl_rtt__Shader__

#include <glfw/glfw3.h>
#include <string>
#include <vector>

class Shader {
private:
    GLuint m_object;
    std::vector<char> m_infoLog;
    std::vector<GLchar> m_shaderSource;
    
    bool readShaderFile( const std::string& shaderPath );
public:
    Shader( GLenum shaderType, const std::string& shaderPath );
    
    GLuint object() const { return m_object; }
    bool compile();
    bool isCompiled() const;
    
    std::vector<GLchar> getShaderInfo();
};

#endif /* defined(__opengl_rtt__Shader__) */
