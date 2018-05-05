cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(UISample)

include_directories(
    ./src
)
    
add_executable(UISample WIN32
    src/UISample.cpp
)

target_link_libraries(UISample 
    libonut
)
