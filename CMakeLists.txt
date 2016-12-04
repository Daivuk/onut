cmake_minimum_required(VERSION 3.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/linux)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/linux)
set(CMAKE_BUILD_TYPE Debug)

project(onut)

add_definitions(-std=gnu++11)
add_definitions(-fmax-errors=3)

include_directories(
    /opt/vc/include/
    /opt/vc/include/interface/vcos/pthreads
    /opt/vc/include/interface/vmcs_host/linux 
    ./include 
    ./src
    )

add_library(onut STATIC
    src/onut.cpp 
    src/Random.cpp 
    src/Vector2.cpp 
    src/Vector3.cpp 
    src/Vector4.cpp 
    src/Matrix.cpp 
    src/Color.cpp 
    src/ThreadPool.cpp 
    src/Dispatcher.cpp 
    src/Timing.cpp 
    src/Settings.cpp 
    src/Updater.cpp 
    src/Window.cpp 
    src/WindowX11.cpp 
    src/Maths.cpp 
    src/Renderer.cpp 
    src/RendererGLES2.cpp 
    src/Shader.cpp 
    src/Texture.cpp 
    src/VertexBuffer.cpp 
    src/IndexBuffer.cpp 
    src/ContentManager.cpp 
    src/Resource.cpp 
    src/Files.cpp 
    src/Strings.cpp 
    src/Curve.cpp 
    src/ShaderGLES2.cpp 
    src/TextureGLES2.cpp 
    src/VertexBufferGLES2.cpp 
    src/IndexBufferGLES2.cpp 
    src/Log.cpp 
    src/lodepng/LodePNG.cpp 
    src/SpriteBatch.cpp 
    src/PrimitiveBatch.cpp 
    src/Tween.cpp
    )
