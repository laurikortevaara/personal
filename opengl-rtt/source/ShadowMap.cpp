//
//  FrameBuffetObject.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 12/04/15.
//
//

#include "ShadowMap.h"
#include <assert.h>
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



ShadowMap::ShadowMap(GLuint width, GLuint height, GLenum colorMode ) :
    m_object(0),
    m_width(width),
    m_height(height),
    m_colorMode(colorMode),
    m_renderedTexture(0)
{    
    glGenFramebuffers(1, &m_object);
    Utils::checkGLError();
    Utils::assert_throw(isValid(), "Unable to create ShadowMap");
    
    glBindFramebuffer(GL_FRAMEBUFFER,m_object);
    
    createTexture();
    
    bindTexture();
    
    // Always check that our framebuffer is ok
    Utils::assert_throw(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "ShadowMap Frame buffer is not complete!");

    unuse();
}

ShadowMap::~ShadowMap()
{
    assert(isValid());
    assert(!isInUse());
    glDeleteFramebuffers(1, &m_object);
    Utils::checkGLError();
}

void ShadowMap::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    Utils::checkGLError();
}

void ShadowMap::createTexture()
{
    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glGenTextures(1, &m_renderedTexture);
    Utils::checkGLError();
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    Utils::checkGLError();
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, m_width, m_height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_width, m_height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
    Utils::checkGLError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Utils::checkGLError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Utils::checkGLError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    Utils::checkGLError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Utils::checkGLError();

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    Utils::checkGLError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    Utils::checkGLError();
     */
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_renderedTexture, 0);
    
    /*glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    */
    
    Utils::checkGLError();
    
    // These are essential for depth buffer rendering
    glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    glReadBuffer(GL_NONE); // No read buffer
    Utils::checkGLError();
}

GLuint ShadowMap::object()
{
    return m_object;
}

GLuint ShadowMap::texture()
{
    return m_renderedTexture;
}

GLboolean ShadowMap::isInUse()
{
    GLint currentBuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentBuffer);
    Utils::checkGLError();
    return m_object == currentBuffer;
}

GLboolean ShadowMap::isValid()
{
    return m_object > 0;
}

void ShadowMap::setShaderMVP(Program* p)
{
    glm::vec3 lightInvDir = glm::vec3(0,1,0);
    
    // Compute the MVP matrix from the light's point of view
    GLfloat size = 0;
    glm::mat4 depthProjectionMatrix = glm::ortho<float>( -size, size, -size, size, -10.0, 20.0f);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,0,1));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    m_depthMVP = glm::mat4(1); //depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    GLint depthMatrixID = p->uniform("mvp");
    //glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &m_depthMVP[0][0]);
    Utils::checkGLError();

}
static glm::mat4 lookAtOrtho(glm::vec3 pos, glm::vec3 target, glm::vec3 up, GLfloat near, GLfloat far,
                             GLfloat left, GLfloat top, GLfloat right, GLfloat bottom)
{
    //set the "projection" uniform in the vertex shader, because it's not going to change
    glm::mat4 projection = glm::ortho(left,right,bottom,top, near, far);
    
    //set the "camera" uniform in the vertex shader, because it's also not going to change
    glm::mat4 view = glm::lookAt(pos, target, up);
    
    return projection * view;
}
glm::mat4 ShadowMap::getMVP()
{
    GLfloat s = 10.0f;
    GLfloat left =  -s;
    GLfloat right =  s;
    GLfloat top =   -s;
    GLfloat bottom = s;
    glm::mat4 viewProjOrtho = lookAtOrtho({0,10,0}, {0,0,0}, {0,0,1}, -5.0f, 50.0f,left,top,right,bottom);

    return viewProjOrtho;
}

void ShadowMap::use()
{
    glGetIntegerv( GL_VIEWPORT, &m_viewportCache[0] );
    Utils::checkGLError();
    
    assert(!isInUse());
    glBindFramebuffer(GL_FRAMEBUFFER, m_object);
    Utils::checkGLError();
    
    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_object);
    Utils::checkGLError();
    glViewport(0,0,m_width,m_height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    Utils::checkGLError();
    
    glEnable(GL_MULTISAMPLE_ARB);
    Utils::checkGLError();
}

void ShadowMap::unuse()
{
    assert(isInUse());
    glBindFramebuffer(GL_FRAMEBUFFER, 0 );
    Utils::checkGLError();
    
    // Restore the viewport
    glViewport(m_viewportCache[0], m_viewportCache[1], m_viewportCache[2], m_viewportCache[3]);
    Utils::checkGLError();
}