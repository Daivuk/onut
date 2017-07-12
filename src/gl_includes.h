#ifndef GL_INCLUDES_H_INCLUDED
#define GL_INCLUDES_H_INCLUDED

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
    #include <SDL2/SDL.h>
#elif defined(WIN32)
    #define GLEW_STATIC
    #include <onut_gl/glew.h>
    #include <gl/GL.h>
    #include <gl/GLU.h>
    #include <onut_gl/glext.h>
#else
    #include <GL/glew.h>
    #include <SDL2/SDL.h>
#endif

#endif
