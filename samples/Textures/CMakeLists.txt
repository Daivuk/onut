cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(TexturesSample)

include_directories(
    ./src
)
    
add_executable(TexturesSample
    src/TexturesSample.cpp
)

target_link_libraries(TexturesSample 
    onut
)
