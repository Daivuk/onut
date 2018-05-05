cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(RandomSample)

include_directories(
    ./src
)
    
add_executable(RandomSample WIN32
    src/RandomSample.cpp
)

target_link_libraries(RandomSample 
    libonut
)
