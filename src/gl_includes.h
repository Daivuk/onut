#ifndef GL_INCLUDES_H_INCLUDED
#define GL_INCLUDES_H_INCLUDED

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
    #include <SDL2/SDL.h>
#elif defined(WIN32)
    #include <GL/gl3w.h>
#else
    #include <GL/gl3w.h>
    #include <SDL2/SDL.h>
#endif

#endif
