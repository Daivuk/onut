cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(TexturesSample)

include_directories(
    ./src
)
    
add_executable(TexturesSample WIN32
    src/TexturesSample.cpp
)

target_link_libraries(TexturesSample 
    libonut
)
