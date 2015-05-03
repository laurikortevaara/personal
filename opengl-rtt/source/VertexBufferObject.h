//
//  VertexBufferObject.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 25/04/15.
//
//

#ifndef __opengl_rtt__VertexBufferObject__
#define __opengl_rtt__VertexBufferObject__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Vertex;

class VertexBufferObject
{
private:
    GLuint m_object;
protected:
public:
    VertexBufferObject();
    virtual ~VertexBufferObject();
    
    void setVertices(std::vector<Vertex>& vertices);
    
    void bind();
    void unbind();
    
    GLuint object();
};

#endif /* defined(__opengl_rtt__VertexBufferObject__) */
