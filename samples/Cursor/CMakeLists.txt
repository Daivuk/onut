cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(CursorSample)

include_directories(
    ./src
)
    
add_executable(CursorSample WIN32
    src/CursorSample.cpp
)

target_link_libraries(CursorSample 
    libonut
)
