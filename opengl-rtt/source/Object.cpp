//
//  Object.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 26/04/15.
//
//

#include "Object.h"
#include "Utils.h"

#include <GL/glew.h>

Object::Object()
{
    m_ModelMatrix = glm::mat4(1);
}

Object::~Object()
{
    
}

void Object::setMesh( std::vector<Vertex> vertices )
{
    m_vao.bind();
    m_vbo.bind();
    
    m_vbo.setVertices(vertices);
    m_numVertices = vertices.size();
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
    
    // conenct the uv of texture coords to the "texture_coords" or the vertex shader
    // Last parameter 3*sizeof(GLfloat) is the (stride or) start address of texture coordinates
    // (x,y,z)*sizeof(GLfloat==4 bytes)

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), reinterpret_cast<const GLvoid*>(3*sizeof(GLfloat)));
    Utils::checkGLError();

    m_vao.bind();
    m_vbo.unbind();
    m_vao.unbind();
    
    m_vao.bind();
    m_vao.unbind();
}

void Object::render()
{
    m_vao.bind();
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    Utils::checkGLError();
    
    m_vao.unbind();
}