cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if (UNIX AND NOT APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/osx)
elseif (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/win)
endif()

project(EmptySample)

include_directories(
    ./src
)
    
add_executable(EmptySample
    src/EmptySample.cpp
)

target_link_libraries(EmptySample 
    onut
)
