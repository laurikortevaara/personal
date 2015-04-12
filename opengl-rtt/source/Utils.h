//
//  Utils.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 06/04/15.
//
//

#ifndef __opengl_rtt__Utils__
#define __opengl_rtt__Utils__

#include <fstream>

namespace FileUtils {
size_t fileLen( const std::string filePath );
}

namespace StringUtils {
    std::string formatStr(const std::string format_str, ...);
}

namespace Utils {
void assert_throw( bool bAssertion, const std::string& error );
}

#endif /* defined(__opengl_rtt__Utils__) */
