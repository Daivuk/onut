cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(CustomMeshSample)

include_directories(
    ./src
)
    
add_executable(CustomMeshSample
    src/CustomMeshSample.cpp
)

target_link_libraries(CustomMeshSample 
    onut
)
