cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if (UNIX AND NOT APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/osx)
elseif (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/win)
endif()

project(SpriteFramesSample)

include_directories(
    ./src
)

add_executable(SpriteFramesSample
    src/SpriteFramesSample.cpp
)

target_link_libraries(SpriteFramesSample 
    onut
)
