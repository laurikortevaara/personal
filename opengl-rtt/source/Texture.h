//
//  Texture.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#ifndef __Texture__
#define __Texture__

#include <glfw/glfw3.h>

class Texture {
private:
    GLuint m_object;
    
    void genTexture( const GLubyte* buf, GLuint width, GLuint height, GLuint format);
public:
    enum Format {
        Format_Grayscale = 1, /**< one channel: grayscale */
        Format_GrayscaleAlpha = 2, /**< two channels: grayscale and alpha */
        Format_RGB = 3, /**< three channels: red, green, blue */
        Format_RGBA = 4 /**< four channels: red, green, blue, alpha */
    };

    Texture( const GLchar* texturePath );
    Texture( const GLubyte* buf, GLuint width, GLuint height, GLuint format );
    Texture(){};
    
    virtual ~Texture();
    
    GLuint object() { return m_object; }
    void setObject(GLuint obj) { m_object = obj; }
};
#endif /* defined(__opengl_rtt__Texture__) */