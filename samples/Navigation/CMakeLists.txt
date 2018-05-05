cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(NavigationSample)

include_directories(
    ./src
)
    
add_executable(NavigationSample WIN32
    src/NavigationSample.cpp
)

target_link_libraries(NavigationSample 
    libonut
)
