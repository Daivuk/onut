cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux)
set(CMAKE_BUILD_TYPE Debug)

project(SpriteFramesSample)

add_definitions(-std=gnu++11)

include_directories(
    /opt/vc/include/
    /opt/vc/include/interface/vcos/pthreads
    /opt/vc/include/interface/vmcs_host/linux 
    ../../include 
    ./src
    )
link_directories(
    /opt/vc/lib/
    ../../lib/linux/
    )
    
add_executable(SpriteFramesSample
    src/SpriteFramesSample.cpp
    )
target_link_libraries(SpriteFramesSample 
    -lbcm_host 
    -lvcos 
    -lvchiq_arm
    -lonut
    -lEGL
    -lGLESv2 
    -lEGL 
    -lGLESv2 
    -lpthread 
    -lrt 
    -ldl)
