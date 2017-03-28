cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# Output directories
if (UNIX AND NOT APPLE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux)
elseif (APPLE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/osx)
elseif (WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win)
endif()

project(onut)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
if (NOT ONUT_SHOW_FPS)
    set(ONUT_SHOW_FPS 1)
endif()

# Some compiler flags
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=3")
endif()

# Pre processors
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
endif()
if (ONUT_SHOW_FPS EQUAL 1)
    add_definitions(-DONUT_SHOW_FPS)
endif()
if (RPI)
    add_definitions(-DHAVE_LIBOPENMAX=2)
    add_definitions(-DOMX)
    add_definitions(-DOMX_SKIP64BIT)
    add_definitions(-DUSE_EXTERNAL_OMX)
    add_definitions(-DUSE_VCHIQ_ARM)
endif()

# Add RaspberryPI specific source files
if (RPI)
    list(APPEND src_files
        src/AudioEngineLinux.cpp
        src/audioplay/audioplay.c
        src/GamePadLinux.cpp
        src/InputDeviceLinux.cpp
        src/VideoPlayerLinux.cpp
        src/IndexBufferGLES2.cpp 
        src/RendererGLES2.cpp 
        src/ShaderGLES2.cpp 
        src/TextureGLES2.cpp 
        src/VertexBufferGLES2.cpp 
        src/WindowX11.cpp 
    )
endif()

# Add OSX specific source files
if (APPLE)
    list(APPEND src_files
        src/AudioEngineCoreAudio.cpp
        src/GamePadOSX.cpp
        src/InputDeviceOSX.cpp
        src/VideoPlayerOSX.cpp
        src/IndexBufferGL.cpp 
        src/RendererGL.cpp 
        src/ShaderGL.cpp 
        src/TextureGL.cpp 
        src/VertexBufferGL.cpp 
        src/WindowOSX.cpp 
    )
endif()

# Add common source files
list(APPEND src_files
    src/ActionManager.cpp
    src/AudioEngine.cpp
    src/Box2D/Collision/Shapes/b2ChainShape.cpp
    src/Box2D/Collision/Shapes/b2CircleShape.cpp
    src/Box2D/Collision/Shapes/b2EdgeShape.cpp
    src/Box2D/Collision/Shapes/b2PolygonShape.cpp
    src/Box2D/Collision/b2BroadPhase.cpp
    src/Box2D/Collision/b2CollideCircle.cpp
    src/Box2D/Collision/b2CollideEdge.cpp
    src/Box2D/Collision/b2CollidePolygon.cpp
    src/Box2D/Collision/b2Collision.cpp
    src/Box2D/Collision/b2Distance.cpp
    src/Box2D/Collision/b2DynamicTree.cpp
    src/Box2D/Collision/b2TimeOfImpact.cpp
    src/Box2D/Common/b2BlockAllocator.cpp
    src/Box2D/Common/b2Draw.cpp
    src/Box2D/Common/b2Math.cpp
    src/Box2D/Common/b2Settings.cpp
    src/Box2D/Common/b2StackAllocator.cpp
    src/Box2D/Common/b2Timer.cpp
    src/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
    src/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
    src/Box2D/Dynamics/Contacts/b2CircleContact.cpp
    src/Box2D/Dynamics/Contacts/b2Contact.cpp
    src/Box2D/Dynamics/Contacts/b2ContactSolver.cpp
    src/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
    src/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
    src/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
    src/Box2D/Dynamics/Contacts/b2PolygonContact.cpp
    src/Box2D/Dynamics/Joints/b2DistanceJoint.cpp
    src/Box2D/Dynamics/Joints/b2FrictionJoint.cpp
    src/Box2D/Dynamics/Joints/b2GearJoint.cpp
    src/Box2D/Dynamics/Joints/b2Joint.cpp
    src/Box2D/Dynamics/Joints/b2MotorJoint.cpp
    src/Box2D/Dynamics/Joints/b2MouseJoint.cpp
    src/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
    src/Box2D/Dynamics/Joints/b2PulleyJoint.cpp
    src/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
    src/Box2D/Dynamics/Joints/b2RopeJoint.cpp
    src/Box2D/Dynamics/Joints/b2WeldJoint.cpp
    src/Box2D/Dynamics/Joints/b2WheelJoint.cpp
    src/Box2D/Dynamics/b2Body.cpp
    src/Box2D/Dynamics/b2ContactManager.cpp
    src/Box2D/Dynamics/b2Fixture.cpp
    src/Box2D/Dynamics/b2Island.cpp
    src/Box2D/Dynamics/b2World.cpp
    src/Box2D/Dynamics/b2WorldCallbacks.cpp
    src/Box2D/Rope/b2Rope.cpp
    src/Camera2DComponent.cpp
    src/Collider2DComponent.cpp
    src/Color.cpp 
    src/Component.cpp
    src/ComponentFactory.cpp
    src/ContentManager.cpp
    src/Crypto.cpp
    src/CSV.cpp
    src/Curve.cpp 
    src/Dialogs.cpp
    src/Dispatcher.cpp 
    src/duktape/duktape.c
    src/Entity.cpp
    src/EntityFactory.cpp
    src/Files.cpp 
    src/Font.cpp
    src/GamePad.cpp
    src/Http.cpp
    src/Images.cpp
    src/IndexBuffer.cpp 
    src/Input.cpp
    src/InputDevice.cpp
    src/JSBindings.cpp
    src/json/json_reader.cpp
    src/json/json_value.cpp
    src/json/json_writer.cpp
    src/lodepng/LodePNG.cpp 
    src/Log.cpp 
    src/Maths.cpp 
    src/Matrix.cpp 
    src/micropather.cpp
    src/Multiplayer.cpp
    src/Music.cpp
    src/onut.cpp 
    src/Particle.cpp
    src/ParticleEmitter.cpp
    src/ParticleSystem.cpp
    src/ParticleSystemManager.cpp
    src/Plane.cpp
    src/Point.cpp
    src/Pool.cpp
    src/PrimitiveBatch.cpp 
    src/Random.cpp 
    src/Ray.cpp
    src/Renderer.cpp 
    src/Resource.cpp 
    src/SceneManager.cpp
    src/Settings.cpp 
    src/Shader.cpp 
    src/SocketTCP.cpp
    src/SocketTCP_Unix.cpp
    src/Sound.cpp
    src/SoundComponent.cpp
    src/SpriteAnim.cpp
    src/SpriteAnimComponent.cpp
    src/SpriteBatch.cpp 
    src/SpriteComponent.cpp
    src/Strings.cpp 
    src/TextComponent.cpp
    src/Texture.cpp 
    src/ThreadPool.cpp 
    src/TiledMap.cpp
    src/TiledMapComponent.cpp
    src/Timer.cpp
    src/Timing.cpp 
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
    src/VideoPlayer.cpp
    src/Window.cpp 
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

add_library(onut STATIC ${src_files})

# Libraries
if (RPI)
    list(APPEND includes
        PUBLIC
            /opt/vc/include
            /opt/vc/include/interface/vmcs_host/linux
    )
endif()
list(APPEND includes
    PUBLIC
        ./include 
    PRIVATE
        ./src
)

if (UNIX AND NOT APPLE) # Only required on linux if we use std::thread
    find_package(Threads)
    list(APPEND libs PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()
if (UNIX AND NOT RPI)
    find_package(OpenGL REQUIRED)
    list(APPEND includes PUBLIC ${OPENGL_INCLUDE_DIR})
    list(APPEND libs PUBLIC ${OPENGL_LIBRARIES})
endif()

if (RPI)  
    list(APPEND libs
        PUBLIC
            -L/opt/vc/lib/
            -L${CMAKE_SOURCE_DIR}/lib/linux/
            -lbcm_host 
            -lvcos 
            -lvchiq_arm
            -lEGL
            -lGLESv2 
            -lEGL 
            -lGLESv2 
            -lrt 
            -ldl
            -lilclient
            -lopenmaxil
    )
endif()

# Include libs and includes
target_include_directories(onut ${includes})
target_link_libraries(onut ${libs})

add_subdirectory(JSStandAlone) # JSStandAlone

add_subdirectory(samples/Animations) # AnimationsSample
add_subdirectory(samples/Components) # ComponentsSample
add_subdirectory(samples/Crypto) # CryptoSample
add_subdirectory(samples/CustomMesh) # CustomMeshSample
add_subdirectory(samples/Dungeon) # DungeonSample
add_subdirectory(samples/Empty) # EmptySample
add_subdirectory(samples/Entities) # EntitiesSample
add_subdirectory(samples/Multiplayer) # MultiplayerSample
add_subdirectory(samples/Navigation) # NavigationSample
add_subdirectory(samples/Particles) # ParticlesSample
add_subdirectory(samples/Primitives) # PrimitivesSample
add_subdirectory(samples/Sounds) # SoundsSample
add_subdirectory(samples/Sprites) # SpritesSample
add_subdirectory(samples/SpriteFrames) # SpriteFramesSample
add_subdirectory(samples/Text) # TextSample
add_subdirectory(samples/Textures) # TexturesSample
add_subdirectory(samples/TiledMap) # TiledMapSample
