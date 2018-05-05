cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(EmptySample)

include_directories(
    ./src
)
    
add_executable(EmptySample WIN32
    src/EmptySample.cpp
)

target_link_libraries(EmptySample 
    libonut
)
