//
//  VertexArrayObject.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 25/04/15.
//
//

#include "VertexArrayObject.h"
#include "Utils.h"

VertexArrayObject::VertexArrayObject()
{
    m_object = 0;
    glGenVertexArrays(1,&m_object);
    Utils::checkGLError();
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1,&m_object);
    Utils::checkGLError();
}

void VertexArrayObject::bind()
{
    Utils::checkGLError();
    glBindVertexArray(m_object);
    Utils::checkGLError();
}

void VertexArrayObject::unbind()
{
    glBindVertexArray(0);
    Utils::checkGLError();
}
