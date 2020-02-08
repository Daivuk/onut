cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(DeferredSample)

include_directories(
    ./src
)
    
add_executable(DeferredSample WIN32
    src/DeferredSample.cpp
)

target_link_libraries(DeferredSample 
    libonut
)
