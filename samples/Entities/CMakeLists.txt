cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(EntitiesSample)

include_directories(
    ./src
)
    
add_executable(EntitiesSample WIN32
    src/EntitiesSample.cpp
)

target_link_libraries(EntitiesSample 
    libonut
)
