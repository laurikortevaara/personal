//
//  Utils.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#include "Utils.h"
#include <stdexcept>

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
}
