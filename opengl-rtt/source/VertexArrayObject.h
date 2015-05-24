//
//  VertexArrayObject.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 25/04/15.
//
//

#ifndef __opengl_rtt__VertexArrayObject__
#define __opengl_rtt__VertexArrayObject__

#include <GL/glew.h>

class VertexArrayObject
{
private:
    GLuint m_object;
protected:
public:
    VertexArrayObject();
    virtual ~VertexArrayObject();
    void bind();
    void unbind();
};

#endif /* defined(__opengl_rtt__VertexArrayObject__) */
