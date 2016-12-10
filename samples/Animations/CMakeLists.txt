cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(AnimationsSample)

include_directories(
    ./src
)
    
add_executable(AnimationsSample
    src/AnimationsSample.cpp
)

target_link_libraries(AnimationsSample 
    onut
)
