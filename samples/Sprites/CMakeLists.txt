cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(SpritesSample)

include_directories(
    ./src
)
    
add_executable(SpritesSample WIN32
    src/SpritesSample.cpp
)

target_link_libraries(SpritesSample 
    libonut
)
