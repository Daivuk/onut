cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ComponentsSample)

include_directories(
    ./src
)
    
add_executable(ComponentsSample WIN32
    src/ComponentsSample.cpp
)

target_link_libraries(ComponentsSample 
    libonut
)
