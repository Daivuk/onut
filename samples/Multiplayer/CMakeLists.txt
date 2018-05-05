cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(MultiplayerSample)

include_directories(
    ./src
)
    
add_executable(MultiplayerSample WIN32
    src/MultiplayerSample.cpp
)

target_link_libraries(MultiplayerSample 
    libonut
)
