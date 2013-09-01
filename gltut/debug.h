//
//  debug.h
//  gltut-glfw
//
//  Copyright (c) 2013 Ricardo Sánchez-Sáez.
//
// This file is licensed under the MIT License.
//

#ifndef gltut_glfw_debug_h
#define gltut_glfw_debug_h

#ifdef DEBUG

#include "stdlib.h"

const int ExitFailure = 1;
static bool printOpenGLError(const char* file,
                      const int line,
                      const char* extraInformation,
                      bool exitOnError)
{
    
    GLenum errorCode;
    bool errorHappened = false;
    
    errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
    {
        printf("glError in file %s:%d: OpenGL Error 0x%x\n",
               file, line, errorCode);
        if (extraInformation != NULL)
        {
            printf("  %s\n", extraInformation);
        }
        errorHappened = true;
        
        if ( exitOnError )
        {
            exit(ExitFailure);
        }
        
    }
    
    return errorHappened;
}

#define printOpenGLError() printOpenGLError(__FILE__, __LINE__, NULL, false)

#else

#define printOpenGLError() ;

#endif

#endif
