//
//  VertexBufferObject.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 25/04/15.
//
//

#include "VertexBufferObject.h"
#include "Utils.h"
#include "Object.h"


VertexBufferObject::VertexBufferObject()
{
    m_object = 0;
    glGenBuffers(1, &m_object);
    Utils::checkGLError();
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &m_object);
    Utils::checkGLError();
}

void VertexBufferObject::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER,m_object);
    Utils::checkGLError();
}

void VertexBufferObject::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
    Utils::checkGLError();
}

GLuint VertexBufferObject::object()
{
    return m_object;
}

void VertexBufferObject::setVertices(std::vector<Vertex>& verts)
{
    std::vector<GLfloat> vertices;
    for(auto v : verts){
        vertices.push_back(v.x);vertices.push_back(v.y);vertices.push_back(v.z);
        vertices.push_back(v.u);vertices.push_back(v.v);
    }
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    Utils::checkGLError();
}