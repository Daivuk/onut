cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(MultiplayerSample)

include_directories(
    ./src
)
    
add_executable(MultiplayerSample
    src/MultiplayerSample.cpp
)

target_link_libraries(MultiplayerSample 
    onut
)
