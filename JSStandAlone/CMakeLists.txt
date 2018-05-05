cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(onut)
    
add_executable(onut WIN32
    src/main.cpp
)

target_link_libraries(onut 
    libonut
)
