cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ModelsSample)

include_directories(
    ./src
)
    
add_executable(ModelsSample WIN32
    src/ModelsSample.cpp
)

target_link_libraries(ModelsSample 
    libonut
)
