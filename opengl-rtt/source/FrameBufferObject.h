//
//  FrameBuffetObject.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 12/04/15.
//
//

#ifndef __opengl_rtt__FrameBuffetObject__
#define __opengl_rtt__FrameBuffetObject__

#include <GL/glew.h>
#include <glfw/glfw3.h>

class FrameBufferObject
{
private:
    GLuint m_object;
    GLuint m_renderedTexture;
    
    GLuint m_width;
    GLuint m_height;
    GLenum m_colorMode;
    
    GLint m_viewportCache[4];
    
    void bindTexture();
public:
    FrameBufferObject(GLuint width, GLuint height, GLenum colorMode );
    virtual ~FrameBufferObject();

    GLuint object();
    GLuint texture();

    GLboolean isInUse();
    GLboolean isValid();
    void use();
    void unuse();
    
    void createTexture();
};

#endif /* defined(__opengl_rtt__FrameBuffetObject__) */
