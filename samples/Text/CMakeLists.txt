cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(TextSample)

include_directories(
    ./src
)

add_executable(TextSample
    src/TestSample.cpp
)

target_link_libraries(TextSample 
    onut
)
