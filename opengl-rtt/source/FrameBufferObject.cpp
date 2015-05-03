//
//  FrameBuffetObject.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 12/04/15.
//
//

#include "FrameBufferObject.h"
#include <assert.h>
#include "Utils.h"


FrameBufferObject::FrameBufferObject(GLuint width, GLuint height, GLenum colorMode ) :
    m_object(0),
    m_width(width),
    m_height(height),
    m_colorMode(colorMode)
{    
    glGenFramebuffers(1, &m_object);
    Utils::assert_throw(isValid(), "Unable to create FrameBufferObject");
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_object);
    Utils::checkGLError();
    
    createTexture();
    Utils::checkGLError();
    
    bindTexture();
    Utils::checkGLError();
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);
    Utils::checkGLError();
    
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    Utils::checkGLError();
    
    // Always check that our framebuffer is ok
    Utils::assert_throw(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is not complete!");

    unuse();
}

FrameBufferObject::~FrameBufferObject()
{
    assert(isValid());
    assert(!isInUse());
    glDeleteFramebuffers(1, &m_object);
}

void FrameBufferObject::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
    Utils::checkGLError();
}

void FrameBufferObject::createTexture()
{
    glGenTextures(1, &m_renderedTexture);
    Utils::assert_throw(m_renderedTexture>0, "Could not create texture for FBO");
    
    bindTexture();
    
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, m_colorMode, m_width, m_height, 0, m_colorMode, GL_UNSIGNED_BYTE, 0);
    
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

GLuint FrameBufferObject::object()
{
    return m_object;
}

GLuint FrameBufferObject::texture()
{
    return m_renderedTexture;
}

GLboolean FrameBufferObject::isInUse()
{
    GLint currentBuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentBuffer);
    return m_object == currentBuffer;
}

GLboolean FrameBufferObject::isValid()
{
    return m_object > 0;
}

void FrameBufferObject::use()
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

void FrameBufferObject::unuse()
{
    assert(isInUse());
    glBindFramebuffer(GL_FRAMEBUFFER, 0 );
    
    // Restore the viewport
    glViewport(m_viewportCache[0], m_viewportCache[1], m_viewportCache[2], m_viewportCache[3]);
}