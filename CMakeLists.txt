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
    src/Color.cpp 
    src/ContentManager.cpp
    src/Curve.cpp 
    src/Dispatcher.cpp 
    src/Files.cpp 
    src/IndexBuffer.cpp 
    src/IndexBufferGLES2.cpp 
    src/lodepng/LodePNG.cpp 
    src/Log.cpp 
    src/Maths.cpp 
    src/Matrix.cpp 
    src/onut.cpp 
    src/PrimitiveBatch.cpp 
    src/Random.cpp 
    src/Renderer.cpp 
    src/RendererGLES2.cpp 
    src/Resource.cpp 
    src/Settings.cpp 
    src/Shader.cpp 
    src/ShaderGLES2.cpp 
    src/SpriteBatch.cpp 
    src/Strings.cpp 
    src/Texture.cpp 
    src/TextureGLES2.cpp 
    src/ThreadPool.cpp 
    src/Timing.cpp 
    src/Tween.cpp
    src/Updater.cpp 
    src/Vector2.cpp 
    src/Vector3.cpp 
    src/Vector4.cpp 
    src/VertexBuffer.cpp 
    src/VertexBufferGLES2.cpp 
    src/Window.cpp 
    src/WindowX11.cpp 
    )
