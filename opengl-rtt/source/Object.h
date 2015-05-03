//
//  Object.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 26/04/15.
//
//

#ifndef __opengl_rtt__Object__
#define __opengl_rtt__Object__

#include <glm/glm.hpp>
#include <vector>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Texture.h"

class Vertex
{
public:
    GLfloat x,y,z;
    GLfloat u,v;
    Vertex(GLfloat x,GLfloat y,GLfloat z,GLfloat u,GLfloat v):x(x),y(y),z(z),u(u),v(v){};
};

class Object
{
private:
    glm::mat4           m_ModelMatrix;
    VertexArrayObject   m_vao;
    VertexBufferObject  m_vbo;
    unsigned int        m_numVertices;
    Texture*            m_texture;
    
protected:
public:
    Object();
    virtual ~Object();
    
    void setMesh( std::vector<Vertex> vertices );
    glm::mat4& modelMatrix() { return m_ModelMatrix; }
    void setModelMatrix(glm::mat4 mat) { m_ModelMatrix = mat; }
    void setTexture(Texture* tex) { m_texture = tex; }
    Texture* texture() { return m_texture; }
    
    void render();
    
};

#endif /* defined(__opengl_rtt__Object__) */
