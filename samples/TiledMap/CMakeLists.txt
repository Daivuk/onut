cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(TiledMapSample)

include_directories(
    ./src
)
    
add_executable(TiledMapSample
    src/TiledMapSample.cpp
)

target_link_libraries(TiledMapSample 
    onut
)
