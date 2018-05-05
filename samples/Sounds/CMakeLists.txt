cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(SoundsSample)

include_directories(
    ./src
)
    
add_executable(SoundsSample WIN32
    src/SoundsSample.cpp
)

target_link_libraries(SoundsSample 
    libonut
)
