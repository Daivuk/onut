cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(setup)
    
add_executable(setup WIN32
    src/main.cpp
)

target_link_libraries(setup 
    onut
)
