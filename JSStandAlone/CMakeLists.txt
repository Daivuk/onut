cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(JSStandAlone)
    
add_executable(JSStandAlone WIN32
    src/main.cpp
)

target_link_libraries(JSStandAlone 
    onut
)
