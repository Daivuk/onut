cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(SpriteFramesSample)

include_directories(
    ./src
)

add_executable(SpriteFramesSample WIN32
    src/SpriteFramesSample.cpp
)

target_link_libraries(SpriteFramesSample 
    libonut
)
