//
//  messagebox.cpp
//  opengl_rtt
//
//  Created by Lauri Kortevaara(personal) on 05/04/15.
//
//

#include "messagebox.h"

int MessageBox( const char* header, const char* message, unsigned long message_type )
{
    //convert the strings from char* to CFStringRef
    CFStringRef header_ref      = CFStringCreateWithCString( NULL, header,     strlen(header)    );
    CFStringRef message_ref  = CFStringCreateWithCString( NULL, message,  strlen(message) );
    
    CFOptionFlags result;  //result code from the message box
    
    //launch the message box
    CFUserNotificationDisplayAlert(
                                   0, // no timeout
                                   kCFUserNotificationNoteAlertLevel, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
                                   NULL, //icon url, use default, you can change it depending message_type flags
                                   NULL, //not used
                                   NULL, //localization of strings
                                   header_ref, //header text
                                   message_ref, //message text
                                   NULL, //default "ok" text in button
                                   CFSTR("Cancel"), //alternate button title
                                   NULL, //other button title, null--> no other button
                                   &result //response flags
                                   );
    
    //Clean up the strings
    CFRelease( header_ref );
    CFRelease( message_ref );
    
    //Convert the result
    if( result == kCFUserNotificationDefaultResponse )
        return IDOK;
    else
        return IDCANCEL;
    
}
