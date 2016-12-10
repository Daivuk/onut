cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(EntitiesSample)

include_directories(
    ./src
)
    
add_executable(EntitiesSample
    src/EntitiesSample.cpp
)

target_link_libraries(EntitiesSample 
    onut
)
