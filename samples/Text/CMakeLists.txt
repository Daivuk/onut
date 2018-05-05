cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(TextSample)

include_directories(
    ./src
)

add_executable(TextSample WIN32
    src/TestSample.cpp
)

target_link_libraries(TextSample 
    libonut
)
