cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(ComponentsSample)

include_directories(
    ./src
)
    
add_executable(ComponentsSample
    src/ComponentsSample.cpp
)

target_link_libraries(ComponentsSample 
    onut
)
