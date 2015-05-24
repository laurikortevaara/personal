//
//  messagebox.h
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 05/04/15.
//
//

#ifndef opengl_rtt__messagebox
#define opengl_rtt__messagebox

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>

#define IDOK       1
#define IDCANCEL   2
int MessageBox( const char* header, const char* message, unsigned long message_type );
#endif /* defined(__opengl_rtt__messagebox__) */
