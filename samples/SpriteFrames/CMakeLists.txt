cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

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
