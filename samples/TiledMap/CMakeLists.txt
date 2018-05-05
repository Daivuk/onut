cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(TiledMapSample)

include_directories(
    ./src
)
    
add_executable(TiledMapSample WIN32
    src/TiledMapSample.cpp
)

target_link_libraries(TiledMapSample 
    libonut
)
