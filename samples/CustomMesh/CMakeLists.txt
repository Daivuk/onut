cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(CustomMeshSample)

include_directories(
    ./src
)
    
add_executable(CustomMeshSample WIN32
    src/CustomMeshSample.cpp
)

target_link_libraries(CustomMeshSample 
    libonut
)
