cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ParticlesSample)

include_directories(
    ./src
)
    
add_executable(ParticlesSample WIN32
    src/ParticlesSample.cpp
)

target_link_libraries(ParticlesSample 
    libonut
)
