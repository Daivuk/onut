cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(HttpSample)

include_directories(
    ./src
)
    
add_executable(HttpSample WIN32
    src/HttpSample.cpp
)

target_link_libraries(HttpSample 
    libonut
)
