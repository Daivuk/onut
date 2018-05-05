cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(PrimitivesSample)

include_directories(
    ./src
)
    
add_executable(PrimitivesSample WIN32
    src/PrimitivesSample.cpp
)

target_link_libraries(PrimitivesSample 
    libonut
)
