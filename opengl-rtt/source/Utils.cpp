//
//  Utils.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#include "Utils.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <GL/glew.h>


namespace FileUtils {
size_t fileLen( const std::string filePath ) {
    std::ifstream file;
    file.open( filePath, std::ios::in | std::ios::ate | std::ios::binary );
    
    if(!file.good())
        return 0;
    
    size_t len = file.tellg();
    file.seekg(std::ios::beg);

    file.close();
    return len;
}
}

namespace StringUtils {
    std::string hexStr(unsigned long iValue)
    {
        std::stringstream ss;
        
        if(iValue <= 0xf)
            ss << "0x" << std::setfill('0') << std::setw(1) << iValue << std::dec << "h";
        else if(iValue <= 0xff)
            ss << "0x" << std::setfill('0') << std::setw(2) << iValue << std::dec << "h";
        else if(iValue <= 0xffff)
            ss << "0x" << std::setfill('0') << std::setw(4) << iValue << std::dec << "h";
        else if(iValue <= 0xffffffff)
            ss << "0x" << std::setfill('0') << std::setw(8) << iValue << std::dec << "h";
        
        return ss.str();
    }
    

    std::string formatStr(const std::string formatStr, ...)
    {
        int final_n, n = ((int)formatStr.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
        std::string str;
        std::unique_ptr<char[]> formatted;
        va_list ap;
        while(1) {
            formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
            strcpy(&formatted[0], formatStr.c_str());
            va_start(ap, formatStr);
            final_n = vsnprintf(&formatted[0], n, formatStr.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }
        return std::string(formatted.get());
    }

}

namespace Utils {
void assert_throw( bool bAssertion, const std::string& error )
{
    if(!bAssertion)
        throw std::runtime_error(error);
}
    
    void checkGLError(bool showError, const char* functionName)
    {
        // check for errors
        GLenum error = glGetError();
        do {
            if(showError)
            {
                switch(error)
                {
                    case GL_NO_ERROR:
                        break;
                        //GL_INVALID_ENUM
                    case 0x0500:
                        std::cerr << functionName << ":  OpenGL Error: GL_INVALID_ENUM (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_INVALID_VALUE
                    case 0x0501:
                        std::cerr << functionName << ":  OpenGL Error: GL_INVALID_VALUE (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_INVALID_OPERATION:
                    case 0x0502:
                        std::cerr << functionName << ":  OpenGL Error: GL_INVALID_OPERATION (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_STACK_OVERFLOW​:
                    case 0x0503:
                        std::cerr << functionName << ":  OpenGL Error: GL_STACK_OVERFLOW (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_STACK_UNDERFLOW​:
                    case 0x0504:
                        std::cerr << functionName << ":  OpenGL Error: GL_STACK_UNDERFLOW (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_OUT_OF_MEMORY​:
                    case 0x0505:
                        std::cerr << functionName << ":  OpenGL Error: GL_OUT_OF_MEMORY (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_INVALID_FRAMEBUFFER_OPERATION
                    case 0x0506:
                        std::cerr << functionName << ":  OpenGL Error: GL_INVALID_FRAMEBUFFER_OPERATION (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_CONTEXT_LOST
                    case 0x0507:
                        std::cerr << functionName << ":  OpenGL Error: GL_CONTEXT_LOST (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        //GL_TABLE_TOO_LARGE
                    case 0x8031:
                        std::cerr << functionName << ":  OpenGL Error: GL_TABLE_TOO_LARGE (" << StringUtils::hexStr(error) << ")" << std::endl;
                        break;
                        
                    default:
                        std::cerr << functionName << ":  Unknown OpenGL Error " << StringUtils::hexStr(error) << ")" << std::endl;
                }
            }
        }
        while( (error=glGetError())!=GL_NO_ERROR);
    }
}
