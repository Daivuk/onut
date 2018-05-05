cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(EffectsSample)

include_directories(
    ./src
)
    
add_executable(EffectsSample WIN32
    src/EffectsSample.cpp
)

target_link_libraries(EffectsSample 
    libonut
)
