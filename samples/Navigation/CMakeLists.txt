cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(NavigationSample)

include_directories(
    ./src
)
    
add_executable(NavigationSample
    src/NavigationSample.cpp
)

target_link_libraries(NavigationSample 
    onut
)
