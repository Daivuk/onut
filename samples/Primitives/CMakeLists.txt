cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(PrimitivesSample)

include_directories(
    ./src
)
    
add_executable(PrimitivesSample
    src/PrimitivesSample.cpp
)

target_link_libraries(PrimitivesSample 
    onut
)
