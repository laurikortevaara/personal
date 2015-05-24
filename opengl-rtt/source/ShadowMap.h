//
//  ShadowMap.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 12/04/15.
//
//

#ifndef __opengl_rtt__ShadowMap__
#define __opengl_rtt__ShadowMap__

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "Program.h"

class ShadowMap
{
private:
    GLuint m_object;
    GLuint m_renderedTexture;
    
    GLuint m_width;
    GLuint m_height;
    GLenum m_colorMode;
    
    GLint m_viewportCache[4];
    
    glm::mat4 m_depthMVP;
    glm::vec3 m_lightPos;
    
    void bindTexture();
public:
    ShadowMap(GLuint width, GLuint height, GLenum colorMode );
    virtual ~ShadowMap();

    GLuint object();
    GLuint texture();

    GLboolean isInUse();
    GLboolean isValid();
    void use();
    void unuse();
    
    glm::vec3 getLightPos() const { return m_lightPos; }
    void setLightPos(const glm::vec3& pos){m_lightPos = pos;}
    
    void setShaderMVP(Program* p);
    glm::mat4 getMVP();
    
    void createTexture();
};

#endif /* defined(__opengl_rtt__ShadowMap__) */
