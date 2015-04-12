//
//  Shader.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#include "Shader.h"
#include "Utils.h"
#include <fstream>

bool Shader::readShaderFile( const std::string& shaderPath )
{
    std::ifstream input;
    input.open( shaderPath, std::ios::in );
    if(!input)
        return false;
    
    size_t len = FileUtils::fileLen(shaderPath);
    m_shaderSource = std::vector<GLchar>(len+1024);
    input.read(m_shaderSource.data(), len);
    
    const GLchar* source = m_shaderSource.data();
    glShaderSource(m_object, 1, &source, (GLint*)&len);
    
    GLenum error = glGetError();
    if(error != GL_NO_ERROR && !getShaderInfo().empty()) {
        return false;
    }
    
    return true;
}

Shader::Shader( GLenum shaderType, const std::string& shaderPath )
{
    m_object = glCreateShader(shaderType);
    Utils::assert_throw(readShaderFile(shaderPath),StringUtils::formatStr("Unable to read %s-shader from file %s",shaderType==GL_VERTEX_SHADER?"Vertex":"Fragment",shaderPath.c_str()));
    Utils::assert_throw(compile(),StringUtils::formatStr("Unable to compile %s-shader from file %s. Error: %s",shaderType==GL_VERTEX_SHADER?"Vertex":"Fragment",shaderPath.c_str(),m_infoLog.data()));
}

bool Shader::compile()
{
    glCompileShader(m_object);
    getShaderInfo();
    return isCompiled();
}

bool Shader::isCompiled() const
{
    GLint isCompiled;
    glGetShaderiv(m_object, GL_COMPILE_STATUS, &isCompiled);
    
    if(glGetError()!=GL_NO_ERROR)
        return false;
    
    return isCompiled == GL_TRUE;
}

std::vector<GLchar> Shader::getShaderInfo()
{
    GLint infoLogLength;
    
    glGetShaderiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
    m_infoLog = std::vector<char>(infoLogLength);
    glGetShaderInfoLog(m_object, infoLogLength, NULL, &m_infoLog[0]);;
    
    return m_infoLog;
}
