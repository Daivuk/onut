cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ShaderSample)

include_directories(
    ./src
)
    
add_executable(ShaderSample WIN32
    src/ShaderSample.cpp
)

target_link_libraries(ShaderSample 
    libonut
)
