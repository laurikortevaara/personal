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
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#include <string>
#include <sstream>
#include <iostream>

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
    m_filePath = texturePath;
    int width, height, channels;
    unsigned char* pixels = stbi_load(texturePath, &width, &height, &channels, 0);

    if(!pixels)
        throw std::runtime_error(stbi_failure_reason());

    genTexture(pixels, width, height, TextureFormatForBitmapFormat((Format)channels));
    stbi_image_free(pixels);
    
    m_width = width;
    m_height = height;
    m_format = TextureFormatForBitmapFormat((Format)channels);
}

Texture::Texture( const GLubyte* buf, GLuint width, GLuint height, GLuint format )
{
    genTexture(buf, width, height, format);
}

Texture::~Texture()
{
    
}

GLenum Texture::getTextureTarget()
{
    glBindTexture(GL_TEXTURE_1D, m_object);
    if (glGetError() != GL_INVALID_OPERATION) {
        glBindTexture(GL_TEXTURE_1D, 0);
        return GL_TEXTURE_1D;
    }
    glBindTexture(GL_TEXTURE_2D, m_object);
    if (glGetError() != GL_INVALID_OPERATION) {
        glBindTexture(GL_TEXTURE_2D, 0);
        return GL_TEXTURE_2D;
    }
    glBindTexture(GL_TEXTURE_3D, m_object);
    if (glGetError() != GL_INVALID_OPERATION) {
        glBindTexture(GL_TEXTURE_3D, 0);
        return GL_TEXTURE_3D;
    }
    glBindTexture(GL_PROXY_TEXTURE_1D, m_object);
    if (glGetError() != GL_INVALID_OPERATION) {
        glBindTexture(GL_PROXY_TEXTURE_1D, 0);
        return GL_PROXY_TEXTURE_1D;
    }
    glBindTexture(GL_PROXY_TEXTURE_2D, m_object);
    if (glGetError() != GL_INVALID_OPERATION) {
        glBindTexture(GL_PROXY_TEXTURE_2D, 0);
        return GL_PROXY_TEXTURE_3D;
    }
    glBindTexture(GL_PROXY_TEXTURE_3D, m_object);
    if (glGetError() != GL_INVALID_OPERATION) {
        glBindTexture(GL_PROXY_TEXTURE_3D, 0);
        return GL_PROXY_TEXTURE_3D;
    }
}

std::string Texture::getTextureTargetStr(GLenum textureTarget)
{
    switch (textureTarget) {
        case GL_TEXTURE_1D:
            return "GL_TEXTURE_1D";
            break;
        case GL_TEXTURE_2D:
            return "GL_TEXTURE_2D";
            break;
        case GL_TEXTURE_3D:
            return "GL_TEXTURE_3D";
            break;
        case GL_PROXY_TEXTURE_1D:
            return "GL_PROXY_TEXTURE_1D";
            break;
        case GL_PROXY_TEXTURE_2D:
            return "GL_PROXY_TEXTURE_2D";
            break;
        case GL_PROXY_TEXTURE_3D:
            return "GL_PROXY_TEXTURE_3D";
            break;
            
        default:
            return "Unknown texture target";
            break;
    }
}

std::string Texture::getTextureFormstStr(GLint textureFormat)
{
    switch(textureFormat)
    {
        case GL_ALPHA4:
            return "GL_ALPHA4";
        case GL_ALPHA8:
            return "GL_ALPHA8";
        case GL_ALPHA12:
            return "GL_ALPHA12";
        case GL_ALPHA16:
            return "GL_ALPHA16";
        case GL_LUMINANCE4:
            return "GL_LUMINANCE4";
        case GL_LUMINANCE8:
            return "GL_LUMINANCE8";
        case GL_LUMINANCE12:
            return "GL_LUMINANCE12";
        case GL_LUMINANCE16:
            return "GL_LUMINANCE16";
        case GL_LUMINANCE4_ALPHA4:
            return "GL_LUMINANCE4_ALPHA4";
        case GL_LUMINANCE6_ALPHA2:
            return "GL_LUMINANCE6_ALPHA2";
        case GL_LUMINANCE8_ALPHA8:
            return "GL_LUMINANCE8_ALPHA8";
        case GL_LUMINANCE12_ALPHA4:
            return "GL_LUMINANCE12_ALPHA4";
        case GL_LUMINANCE12_ALPHA12:
            return "GL_LUMINANCE12_ALPHA12";
        case GL_LUMINANCE16_ALPHA16:
            return "GL_LUMINANCE16_ALPHA16";
        case GL_INTENSITY:
            return "GL_INTENSITY";
        case GL_INTENSITY4:
            return "GL_INTENSITY4";
        case GL_INTENSITY8:
            return "GL_INTENSITY8";
        case GL_INTENSITY12:
            return "GL_INTENSITY12";
        case GL_INTENSITY16:
            return "GL_INTENSITY16";
        case GL_R3_G3_B2:
            return "GL_R3_G3_B2";
        case GL_RGB4:
            return "GL_RGB4";
        case GL_RGB5:
            return "GL_RGB5";
        case GL_RGB8:
            return "GL_RGB8";
        case GL_RGB10:
            return "GL_RGB10";
        case GL_RGB12:
            return "GL_RGB12";
        case GL_RGB16:
            return "GL_RGB16";
        case GL_RGBA2:
            return "GL_RGBA2";
        case GL_RGBA4:
            return "GL_RGBA4";
        case GL_RGB5_A1:
            return "GL_RGB5_A1";
        case GL_RGBA8:
            return "GL_RGBA8";
        case GL_RGB10_A2:
            return "GL_RGB10_A2";
        case GL_RGBA12:
            return "GL_RGBA12";
        case GL_RGBA16:
            return "GL_RGBA16";
        default:
            return "Unknown texture format";
    }
}

void Texture::printInfo()
{
    GLint width = 0;
    GLint height = 0;
    GLint depth = 0;
    GLint internalFormat = 0;
    GLint isCompressed = 0;
    
    GLenum textureTarget = getTextureTarget();
    glBindTexture(textureTarget,m_object);
    
    glGetTexLevelParameteriv(textureTarget, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(textureTarget, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(textureTarget, 0, GL_TEXTURE_DEPTH, &depth);
    glGetTexLevelParameteriv(textureTarget, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
    glGetTexLevelParameteriv(textureTarget, 0, GL_TEXTURE_COMPRESSED, &isCompressed);

    std::stringstream ss;
    
    std::string strTextureFormat;
    ss << "\tTexture Format: " << getTextureFormstStr(internalFormat) << std::endl;
    strTextureFormat = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();

    std::string strTextureTarget;
    ss << "\tTexture Target: " << getTextureTargetStr(getTextureTarget()) << std::endl;
    strTextureTarget = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();

    std::string strTextureWidth;
    ss << "\tTexture Width: " << width << std::endl;
    strTextureWidth = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();

    std::string strTextureHeight;
    ss << "\tTexture Height: " << height << std::endl;
    strTextureHeight = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();
    
    std::string strTextureDepth;
    ss << "\tTexture Depth: " << depth << std::endl;
    strTextureDepth = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();
    
    GLint size = width * height * depth;
    std::string strTextureSize;
    ss << "\tTexture size: " << size << " bytes (" << size / 1024 << " kb) (" << size / 1024 / 1024 << " Mb)" << std::endl;
    strTextureSize = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();

    
    std::string strIsCompressed;
    ss << "\tIs Compressed: " << (isCompressed == GL_TRUE ? "true" : "false") << std::endl;
    strIsCompressed = ss.str();
    ss.str(std::string());
    ss.flush();
    ss.clear();
    
    std::cout << "TextureObject: " << m_object << "[" << std::endl;
    if(!m_filePath.empty())
        std::cout << "\tTexture file: " << m_filePath << std::endl;
    else
        std::cout << "\tMemory texture:" << std::endl;
    std::cout << strTextureFormat;
    std::cout << strTextureTarget;
    std::cout << strTextureWidth;
    std::cout << strTextureHeight;
    std::cout << strTextureDepth;
    std::cout << strTextureSize;
    std::cout << strIsCompressed;
    std::cout << "]" << std::endl << std::endl;
}

void Texture::writeToFile(const std::string &filename, bool flipY)
{
    int stride = 0;
    int comp = 3;
    GLint mipmap_level = 0;
    
    GLint texWidth;
    GLint texHeight;
    GLint texFormat;
    GLuint texPixelFormat;
    GLuint texSize;
    GLint texDepth;
    GLint texComponents;
    
    glBindTexture(GL_TEXTURE_2D, m_object);
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level, GL_TEXTURE_HEIGHT, &texHeight);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level, GL_TEXTURE_INTERNAL_FORMAT, &texFormat);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level, GL_TEXTURE_COMPONENTS, &texComponents);

    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level, GL_TEXTURE_DEPTH, &texDepth);
    
    texSize = texWidth * texHeight * 3;//(texComponents == GL_RGB8 ? 3 : 4);
    unsigned char* pixels;
    pixels = (unsigned char*)malloc(texSize); //new unsigned char[texSize];
    memset(pixels, 0, texSize);

    unsigned char* pixels2;
    pixels2 = (unsigned char*)malloc(texSize); //new unsigned char[texSize];
    memset(pixels2, 0, texSize);

    
    glGetTexImage(GL_TEXTURE_2D, mipmap_level, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    GLenum error = glGetError();
    
    // Flip the image as it's upside down
    if(flipY) {
        for(int y = 0; y < texHeight; ++y)
            for(int x = 0; x < texWidth*3; ++x){
                pixels2[y*texWidth*3+x] = pixels[(texHeight-y)*texWidth*3+x];
            }
    }
    else
        memcpy(pixels2, pixels, texSize);
    
    glBindTexture(GL_TEXTURE_2D, 0);
   
    stbi_write_png(filename.c_str(), texWidth, texHeight, comp, pixels2, stride);
    free(pixels);
    free(pixels2);
}