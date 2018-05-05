cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(AnimationsSample)

include_directories(
    ./src
)
    
add_executable(AnimationsSample WIN32
    src/AnimationsSample.cpp
)

target_link_libraries(AnimationsSample 
    libonut
)
