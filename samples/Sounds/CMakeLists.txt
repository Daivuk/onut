cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(SoundsSample)

include_directories(
    ./src
)
    
add_executable(SoundsSample
    src/SoundsSample.cpp
)

target_link_libraries(SoundsSample 
    onut
)
