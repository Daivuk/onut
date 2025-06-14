cmake_minimum_required(VERSION 3.5)

option(ONUT_BUILD_SAMPLES "Build the samples" OFF)
option(ONUT_BUILD_STANDALONE "Build the Javascript Stand Alone" OFF)
option(ONUT_BUILD_UI_EDITOR "Build the UI Editor" OFF)
option(ONUT_USE_SDL "Use SDL+OpenGL on Windows instead of Win32/D3D11/DI8" OFF)
option(ONUT_USE_OPENGL "Use OpenGL on Windows instead of DirectX11" OFF)
option(ONUT_STATIC_SDL "Statically link SDL instead of using system libraries" ON)
option(ONUT_BUILD_JSONCPP "If using another API that requires json disable this" ON)
option(ONUT_DUKTAPE_DEBUGGER "Allows processes to attach to the duktape debugger" OFF)
option(ONUT_BUILD_ZLIB "Build zlib as part of onut source" ON)

if (NOT WIN32)
    set(ONUT_USE_SDL ON)
endif()

if (ONUT_USE_SDL)
    set(ONUT_USE_OPENGL ON)
endif()

# Easier defines
if (UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

# Setup modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# project name
project(libonut)

# Some compiler flags
set(CMAKE_CXX_STANDARD 17)
if (MSVC) # Multi core in VS
    SET(PUBLIC CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "/MP /FS")
endif()

# Pre processors
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
endif()
if (ONUT_USE_OPENGL)
    add_definitions(-DONUT_USE_OPENGL)
endif()
if (ONUT_USE_SDL)
    add_definitions(-DONUT_USE_SDL)
endif()
if (WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
if (ONUT_DUKTAPE_DEBUGGER)
    add_definitions(-DDUK_USE_DEBUGGER_SUPPORT -DDUK_USE_INTERRUPT_COUNTER -DDUK_USE_DEBUGGER_INSPECT -DDUK_USE_DEBUGGER_THROW_NOTIFY -DDUK_USE_DEBUGGER_PAUSE_UNCAUGHT -DDUK_USE_DEBUGGER_DUMPHEAP)
endif()
if (APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION -DONUT_APPLE -DONUT_USE_OPENGL)
endif()

# Add OSX specific source files
if (APPLE)
    list(APPEND src_files
        src/AudioEngineSDL2.cpp
        src/GamePadSDL2.cpp
        src/InputDeviceSDL2.cpp
        src/IndexBufferGL.cpp
        src/JoystickSDL2.cpp
        src/HttpCURL.cpp
        src/RendererGL.cpp 
        src/ShaderGL.cpp 
        src/TextureGL.cpp 
        src/VertexBufferGL.cpp 
        src/WindowSDL2.cpp 
    )
endif()

# Add Linux specific source files
if (LINUX)
    list(APPEND src_files
        src/AudioEngineSDL2.cpp
        src/GamePadSDL2.cpp
        src/InputDeviceSDL2.cpp
        src/IndexBufferGL.cpp
        src/JoystickSDL2.cpp
        src/HttpCURL.cpp
        src/RendererGL.cpp 
        src/ShaderGL.cpp 
        src/TextureGL.cpp 
        src/VertexBufferGL.cpp 
        src/WindowSDL2.cpp 
        thirdparty/gl3w/src/gl3w.c
    )
    list(APPEND includes PUBLIC ./thirdparty/gl3w/include/)
endif()

if (ONUT_DUKTAPE_DEBUGGER)
    if (UNIX)
        list(APPEND src_files
            src/duktape/duk_trans_socket.h
            src/duktape/duk_trans_socket_unix.c
        )
    elseif (WIN32)
        list(APPEND src_files
            src/duktape/duk_trans_socket.h
            src/duktape/duk_trans_socket_windows.c
        )
    endif()
endif()

# Add Win32 specific source files
if (WIN32)
    if (ONUT_USE_SDL)
        list(APPEND src_files
            src/AudioEngineSDL2.cpp
            src/GamePadSDL2.cpp
            src/InputDeviceSDL2.cpp
            src/JoystickSDL2.cpp
            src/WindowSDL2.cpp 
            thirdparty/gl3w/src/gl3w.c
        )
    else()
        list(APPEND src_files
            src/AudioEngineWASAPI.cpp
            src/GamePadXInput.cpp
            src/InputDeviceDI8.cpp
            src/JoystickDI8.cpp
            src/HttpXMLHTTPRequest.cpp
            src/WindowWIN32.cpp 
        )
    endif()
    list(APPEND src_files
        src/HttpXMLHTTPRequest.cpp
    )
    if (ONUT_USE_OPENGL)
        list(APPEND src_files
            src/IndexBufferGL.cpp 
            src/RendererGL.cpp 
            src/ShaderGL.cpp 
            src/TextureGL.cpp 
            src/VertexBufferGL.cpp 
            thirdparty/gl3w/src/gl3w.c
        )
        list(APPEND includes PUBLIC ./thirdparty/gl3w/include/)
    else()
        list(APPEND src_files
            src/IndexBufferD3D11.cpp 
            src/RendererD3D11.cpp 
            src/ShaderD3D11.cpp 
            src/TextureD3D11.cpp 
            src/VertexBufferD3D11.cpp
        )
    endif()
endif()

if (ONUT_BUILD_JSONCPP)
    list(APPEND src_files
        src/json/json_reader.cpp
        src/json/json_value.cpp
        src/json/json_writer.cpp
    )
endif()

# Add common source files
list(APPEND src_files
    src/ActionManager.cpp
    src/AudioEngine.cpp
    src/Color.cpp 
    src/ContentManager.cpp
    src/Crypto.cpp
    src/CSV.cpp
    src/Curve.cpp 
    src/Deferred.cpp
    src/Dialogs.cpp
    src/Dispatcher.cpp 
    src/duktape/duktape.c
    src/Files.cpp 
    src/Font.cpp
    src/GamePad.cpp
    src/Http.cpp
    src/Images.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/IndexBuffer.cpp 
    src/Input.cpp
    src/InputDevice.cpp
    src/Json.cpp
    src/Joystick.cpp
    src/JSBindings.cpp
    src/JSBindings_Imgui.cpp
    src/lodepng/LodePNG.cpp 
    src/Log.cpp 
    src/Matrix.cpp 
    src/micropather.cpp
    src/Model.cpp 
    src/Music.cpp
    src/MusicOGG.cpp
    src/onut.cpp 
    src/Particle.cpp
    src/ParticleEmitter.cpp
    src/ParticleSystem.cpp
    src/ParticleSystemManager.cpp
    src/Plane.cpp
    src/Point.cpp
    src/Pool.cpp
    src/PrimitiveBatch.cpp 
    src/PrimitiveBatch3D.cpp 
    src/Random.cpp 
    src/Ray.cpp
    src/Renderer.cpp 
    src/Resource.cpp 
    src/Settings.cpp 
    src/Shader.cpp 
    src/Sound.cpp
    src/SpriteAnim.cpp
    src/SpriteBatch.cpp 
    src/Strings.cpp 
    src/Texture.cpp 
    src/ThreadPool.cpp 
    src/TiledMap.cpp
    src/Timer.cpp
    src/Timing.cpp 
    src/tinyfiledialogs/tinyfiledialogs.c
    src/tinyxml2/tinyxml2.cpp
    src/Tween.cpp
    src/UIButton.cpp
    src/UICheckBox.cpp
    src/UIContext.cpp
    src/UIControl.cpp
    src/UIImage.cpp
    src/UIJson.cpp
    src/UILabel.cpp
    src/UIPanel.cpp
    src/UIScrollView.cpp
    src/UITextBox.cpp
    src/UITreeView.cpp
    src/Updater.cpp 
    src/Vector2.cpp 
    src/Vector3.cpp 
    src/Vector4.cpp 
    src/VertexBuffer.cpp 
    src/Window.cpp
)

if (ONUT_BUILD_ZLIB)
    list(APPEND src_files
        src/zlib/adler32.c
        src/zlib/compress.c
        src/zlib/crc32.c
        src/zlib/deflate.c
        src/zlib/gzclose.c
        src/zlib/gzlib.c
        src/zlib/gzread.c
        src/zlib/gzwrite.c
        src/zlib/infback.c
        src/zlib/inffast.c
        src/zlib/inflate.c
        src/zlib/inftrees.c
        src/zlib/trees.c
        src/zlib/uncompr.c
        src/zlib/zutil.c
    )
endif()

file(GLOB inc_files ./include/onut/*.h)

add_library(libonut STATIC ${src_files} ${inc_files})

# Libraries
list(APPEND includes
    PUBLIC
        ./include 
    PRIVATE
        ./src
)

if (LINUX) # Only required on linux if we use std::thread
    find_package(Threads)
    list(APPEND libs PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()
if (UNIX)
    find_package(OpenGL REQUIRED)
    list(APPEND includes PUBLIC ${OPENGL_INCLUDE_DIR})
    list(APPEND libs PUBLIC ${OPENGL_LIBRARIES})

    find_package(SDL2 REQUIRED)
    list(APPEND includes PUBLIC ${SDL2_INCLUDE_DIR})
    list(APPEND includes PUBLIC ${SDL2_INCLUDE_DIR}/..)
    list(APPEND libs PUBLIC ${SDL2_LIBRARY})

    find_package(CURL REQUIRED)
    list(APPEND includes PUBLIC ${CURL_INCLUDE_DIRS})
    list(APPEND libs PUBLIC ${CURL_LIBRARIES})
endif()
if (WIN32)
    if (ONUT_USE_OPENGL)
        find_package(OpenGL REQUIRED)
        list(APPEND includes PUBLIC ${OPENGL_INCLUDE_DIR})
        list(APPEND libs PUBLIC ${OPENGL_LIBRARIES})
    endif()
    if (NOT ONUT_USE_SDL)
        find_package(DirectX11 REQUIRED)
        list(APPEND libs PUBLIC ${DirectX11_LIBRARIES})
    endif()
endif()

if (ONUT_USE_SDL)
    set(HAVE_LIBC ON)
    set(SDL_MAIN_HANDLED ON)

    if (ONUT_STATIC_SDL)
        add_subdirectory(./thirdparty/SDL ./thirdparties/SDL)
        list(APPEND libs SDL3-static)
    else()
        list(APPEND libs SDL2)
    endif()

    list(APPEND includes PUBLIC ./thirdparty/SDL/include/)
endif()

if (WIN32)
    list(APPEND libs
        PUBLIC
            Mfplat
            msxml6
            comsuppwd
            Winhttp
    )
endif()

# assimp
set(ZLIB_USE_STATIC_LIBS ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(./thirdparty/assimp/)
list(APPEND libs assimp)

# Include libs and includes
target_include_directories(libonut ${includes})
target_link_libraries(libonut PUBLIC ${libs})

if (ONUT_BUILD_STANDALONE)
    add_subdirectory(JSStandAlone) # JSStandAlone
endif()

if (ONUT_BUILD_UI_EDITOR)
    add_subdirectory(uieditor) # uieditor
endif()

if (ONUT_BUILD_SAMPLES)
    add_subdirectory(samples/Animations) # AnimationsSample
    add_subdirectory(samples/Crypto) # CryptoSample
    add_subdirectory(samples/Cursor) # CursorSample
    add_subdirectory(samples/CustomMesh) # CustomMeshSample
    add_subdirectory(samples/Deferred) # DeferredSample
    # add_subdirectory(samples/Dungeon) # DungeonSample // EnTT example
    add_subdirectory(samples/Effects) # EffectsSample
    add_subdirectory(samples/Empty) # EmptySample
    add_subdirectory(samples/GamePads) # GamePadsSample
    add_subdirectory(samples/HOTAS) # HOTASSample
    add_subdirectory(samples/Http) # HttpSample
    add_subdirectory(samples/Imgui) # ImguiSample
    add_subdirectory(samples/ModelBatch) # ModelBatchSample
    add_subdirectory(samples/Models) # ModelsSample
    add_subdirectory(samples/Navigation) # NavigationSample
    add_subdirectory(samples/Particles) # ParticlesSample
    add_subdirectory(samples/Primitives) # PrimitivesSample
    add_subdirectory(samples/Random) # RandomSample
    add_subdirectory(samples/Shader) # ShaderSample
    add_subdirectory(samples/Sounds) # SoundsSample
    add_subdirectory(samples/Sprites) # SpritesSample
    add_subdirectory(samples/SpriteFrames) # SpriteFramesSample
    add_subdirectory(samples/Text) # TextSample
    add_subdirectory(samples/Textures) # TexturesSample
    add_subdirectory(samples/TiledMap) # TiledMapSample
    add_subdirectory(samples/UI) # UISample
    add_subdirectory(samples/UIFromCode) # UIFromCodeSample
    add_subdirectory(samples/UndoRedo) # UndoRedoSample
endif()
