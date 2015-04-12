//
//  Texture.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//
#include <stdexcept>

#include "Texture.h"
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

static GLenum TextureFormatForBitmapFormat(Texture::Format format)
{
    switch (format) {
        case Texture::Format_Grayscale: return GL_LUMINANCE;
        case Texture::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
        case Texture::Format_RGB: return GL_RGB;
        case Texture::Format_RGBA: return GL_RGBA;
        default: throw std::runtime_error("Unrecognised Bitmap::Format");
    }
}

void Texture::genTexture( const GLubyte* buf, GLuint width, GLuint height, GLuint format)
{
    glGenTextures(1, &m_object);
    glBindTexture(GL_TEXTURE_2D, m_object);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 (GLsizei)width,
                 (GLsizei)height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 buf);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const GLchar* texturePath)
{
    int width, height, channels;
    unsigned char* pixels = stbi_load(texturePath, &width, &height, &channels, 0);

    if(!pixels)
        throw std::runtime_error(stbi_failure_reason());

    genTexture(pixels, width, height, TextureFormatForBitmapFormat((Format)channels));
    stbi_image_free(pixels);
}

Texture::Texture( const GLubyte* buf, GLuint width, GLuint height, GLuint format )
{
    genTexture(buf, width, height, format);
}

Texture::~Texture()
{
    
}
